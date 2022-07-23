#include "include/Alarm.hpp"
#include <cmath>

Alarm::Alarm(const std::string &key, unsigned int priority,
             const std::chrono::milliseconds &execution_period)
    : key_(key), priority_(priority) {
  if (execution_period <= 0s) {
    throw std::invalid_argument("Execution period should be >0 ms");
  }
  execution_period_ = execution_period;
}

void Alarm::checkEntryValidity(
    const std::chrono::milliseconds &variable_to_check,
    const std::string &variable_name) {
  if (variable_to_check.count() == 0) {
    return; // Nothing to check.
  }

  std::string error_msg = "";
  // Check beep duration.
  if (variable_to_check < execution_period_) {
    error_msg = " is < to the execution period: ";
  }
  if (std::fmod(variable_to_check.count(), execution_period_.count()) != 0.0) {
    error_msg = " is not a multiple of the execution period: ";
  }
  if (!error_msg.empty()) {
    throw std::invalid_argument(
        variable_name + ": " + std::to_string(variable_to_check.count()) +
        error_msg + std::to_string(execution_period_.count()));
  }
}

void Alarm::setPattern(std::chrono::milliseconds beep_duration,
                       std::chrono::milliseconds silence_duration,
                       unsigned int beep_number,
                       std::chrono::milliseconds beep_pause) {

  // Check entries validity.
  checkEntryValidity(beep_duration, "Beep duration");
  checkEntryValidity(silence_duration, "Silence duration");
  checkEntryValidity(beep_pause, "Beep pause duration");

  // Repeat beep_num times the beep pattern.
  for (unsigned int num = 0; num < beep_number; num++) {
    // Create one beep pattern.
    {
      for (unsigned int beep = 0; beep < beep_duration / execution_period_;
           beep++) {
        pattern_.push_back(true);
      }
      for (unsigned int pause = 0; pause < beep_pause / execution_period_;
           pause++) {
        pattern_.push_back(false);
      }
    }
  }
  // Create the silence between beep patterns.
  for (unsigned int silence = 0; silence < silence_duration / execution_period_;
       silence++) {
    pattern_.push_back(false);
  }
}

// Getters
const std::string &Alarm::getKey() const { return key_; };
unsigned int Alarm::getPriority() const { return priority_; };
const SoundPattern &Alarm::getPattern() const { return pattern_; };
const std::chrono::milliseconds &Alarm::getExecutionPeriod() const {
  return execution_period_;
};

HighPriorityAlarm::HighPriorityAlarm() : Alarm("h", 3, 250ms) {
  setPattern(250ms, 2s, 5, 500ms);
};
MediumPriorityAlarm::MediumPriorityAlarm() : Alarm("m", 2, 250ms) {
  setPattern(250ms, 1s);
};
LowPriorityAlarm::LowPriorityAlarm() : Alarm("l", 1, 250ms) {
  setPattern(1s, 30s);
};