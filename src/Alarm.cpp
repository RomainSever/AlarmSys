#include "include/Alarm.hpp"

Alarm::Alarm(const std::string &key, unsigned int priority,
             const std::chrono::milliseconds &execution_period)
    : key_(key), priority_(priority) {
  if (execution_period <= 0s) {
    throw std::invalid_argument("Execution period should be >0 ms");
  }
  execution_period_ = execution_period;
}

void Alarm::setPattern(std::chrono::milliseconds beep_duration,
                       std::chrono::milliseconds silence_duration,
                       unsigned int beep_number,
                       std::chrono::milliseconds beep_pause) {
  // Check entries validity.
  auto unit_beep_number = ceil(beep_duration / execution_period_);
  if (unit_beep_number != beep_duration / execution_period_) {
    throw std::invalid_argument(
        "Beep duration: " + std::to_string(beep_duration.count()) +
        " is not a multiple of the execution period: " +
        std::to_string(execution_period_.count()));
  }

  auto unit_pause_number = ceil(beep_pause / execution_period_);
  if (unit_pause_number != beep_pause / execution_period_) {
    throw std::invalid_argument(
        "Beep pause: " + std::to_string(beep_pause.count()) +
        " is not a multiple of the execution period: " +
        std::to_string(execution_period_.count()));
  }

  auto unit_silence_number = ceil(silence_duration / execution_period_);
  if (unit_silence_number != silence_duration / execution_period_) {
    throw std::invalid_argument(
        "Silence duration: " + std::to_string(silence_duration.count()) +
        " is not a multiple of the execution period: " +
        std::to_string(execution_period_.count()));
  }

  // Repeat beep_num times the beep pattern.
  for (unsigned int num = 0; num < beep_number; num++) {
    // Create one beep pattern.
    {
      for (unsigned int beep = 0; beep < unit_beep_number; beep++) {
        pattern_.push_back(true);
      }
      for (unsigned int pause = 0; pause < unit_pause_number; pause++) {
        pattern_.push_back(false);
      }
    }
  }
  // Create the silence between beep patterns.
  for (unsigned int silence = 0; silence < unit_silence_number; silence++) {
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