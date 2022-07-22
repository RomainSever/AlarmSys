#include <chrono>
using namespace std::chrono_literals;

// Sound pattern is a sequence of bool in which True represent a beep sound of
// 250ms and False a silence of 250ms.
using SoundPattern = std::vector<bool>;

/**
 * Simple class that define an Alarm.
 * Alarms posses certain priority, a specific key and sound pattern. The sound
 * pattern is a sequence of bool in which True represent a beep sound of 250ms
 * and False a silence of 250ms.
 */
class Alarm {
public:
  Alarm(const std::string &key, unsigned int priority);
  /**
   * Create a sound pattern from different duration information. Created
   * pattern start with @a beep_number times the beep pattern. Beep pattern is a
   * combination of a certain amount of beeps of 250ms defined by @a
   * beep_duration and certain amount of silence defined by @a beep_pause. After
   * the beep patterns, we add multiple silences of 250ms defined by @a
   * silence_duration.
   *
   * @warning If the desired duration are not multiples of 250, we round up to
   * the nearest whole number.
   *
   * @param beep_duration Duration of one beep sound. Used with beep_pause to
   * create one beep pattern.
   * @param silence_duration Duration of the silence between beep patterns.
   * @param beep_number Number of time to repeat the beep patterns.
   * @param beep_pause Duration of the silence after a beep. Used with
   * beep_duration to create one beep pattern.
   */
  void setPattern(std::chrono::milliseconds beep_duration,
                  std::chrono::milliseconds silence_duration,
                  int beep_number = 1,
                  std::chrono::milliseconds beep_pause = 0s);

  // Getters
  unsigned int getPriority() const;
  const SoundPattern &getPattern() const;
  const std::string &getKey() const;

private:
  // Key used to toggle the alarm.
  std::string key_;
  // Priority to define which alarm to execute.
  unsigned int priority_;
  // Sound pattern to execute.
  SoundPattern pattern_;
};