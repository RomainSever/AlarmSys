#include "include/Executor.hpp"
#include <iostream>

Executor::Executor(const std::chrono::milliseconds &execution_period) {
  // Check parameters validity.
  if (execution_period <= 0s) {
    throw std::invalid_argument("Execution period should be >0 ms");
  }
  execution_period_ = execution_period;
  auto silent = Alarm("silent_alarm", 0, execution_period_);
  silent.setPattern(0s, execution_period_, 0, 0s);
  alarm_output_.open("alarm_output.txt");
  toggleAlarm(silent);
};

Executor::~Executor() {

  executor_thread_.request_stop();
  executor_thread_.join();
  alarm_output_.close();
};

void Executor::toggleAlarm(const Alarm &alarm) {
  // Check parameters validity.
  if (execution_period_ != alarm.getExecutionPeriod()) {
    throw std::invalid_argument(
        "Execution period of the alarm: " +
        std::to_string(alarm.getExecutionPeriod().count()) +
        " doesn't match Executor execution period: " +
        std::to_string(execution_period_.count()));
  }

  // If an alarm of the same priority already exist, deactivate it. Else
  // activate this alarm.
  if (!std::erase_if(activated_alarms_, [alarm](const auto &active) {
        return active.getPriority() == alarm.getPriority();
      })) {
    activated_alarms_.push_back(alarm);
  }
  executeHighestPriorityPattern();
}

void Executor::executeHighestPriorityPattern() {
  if (activated_alarms_.empty()) {
    std::cout << "No active alarm to execute" << std::endl;
    return;
  }
  // Parse the list of active alarms to find the one with the highest priority.
  auto highest = std::ranges::max(activated_alarms_, [](Alarm a, Alarm b) {
    return a.getPriority() < b.getPriority();
  });
  // If it's a different one that the current one execute it.
  if (highest.getPriority() != current_alarm_priority_) {
    // Stop the currently playing alarm and set the new one.
    executor_thread_.request_stop();
    current_alarm_priority_ = highest.getPriority();

    // Start the execution thread that write beep and silence to the
    // output_alarm file.
    executor_thread_ =
        std::jthread([this, highest](const std::stop_token &stop_token) {
          while (!stop_token.stop_requested()) {
            for (const auto sound : highest.getPattern()) {
              if (stop_token.stop_requested()) {
                break;
              }
              sound ? alarm_output_ << "X" << std::endl
                    : alarm_output_ << "_" << std::endl;
              std::this_thread::sleep_for(execution_period_);
            }
          }
        });
  }
};
