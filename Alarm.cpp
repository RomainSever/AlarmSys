#include "Alarm.hpp"

Alarm::Alarm(const std::string &key, unsigned int priority)
    : key_(key), priority_(priority) {}

void Alarm::setPattern(std::chrono::milliseconds beep_duration,
                       std::chrono::milliseconds silence_duration,
                       int beep_number, std::chrono::milliseconds beep_pause) {
  // Repeat beep_num times the beep pattern.
  for (int num = 0; num < beep_number; num++) {
    // Create one beep pattern.
    {
      for (int i = 0; i < beep_duration / 250ms; i++) {
        pattern_.push_back(true);
      }
      for (int i = 0; i < beep_pause / 250ms; i++) {
        pattern_.push_back(false);
      }
    }
  }
  // Create the silence between beep patterns.
  for (int i = 0; i < silence_duration / 250ms; i++) {
    pattern_.push_back(false);
  }
}

// Getters
unsigned int Alarm::getPriority() const { return priority_; };
const SoundPattern &Alarm::getPattern() const { return pattern_; };
const std::string &Alarm::getKey() const { return key_; };