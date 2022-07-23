#include <chrono>
using namespace std::chrono_literals;

// Sound pattern is a sequence of bool in which True represent a beep sound and
// False a silence.
using SoundPattern = std::vector<bool>;

/**
 * Simple class that define an Alarm.
 * Alarms posses certain priority, a specific key and sound pattern. The sound
 * pattern is a sequence of bool in which True represent a beep sound with a
 * duration equal to the execution_period_ and False a silence with a duration
 * equal to the execution_period_
 */
class Alarm {
public:
  /**
   * @brief Construct a new Alarm object
   *
   * @param key The string key that triggers the Alarm.
   * @param priority  The priority level of the Alarm.
   * @param execution_period  The unity of time of one sound of the alarm. Must
   * be > 0.
   * @throws std::invalid_argument if the @a execution_period is <= 0;
   */
  Alarm(const std::string &key, unsigned int priority,
        const std::chrono::milliseconds &execution_period);
  /**
   * Create a sound pattern from different duration information. Created
   * pattern start with @a beep_number times the beep pattern. Beep pattern is a
   * combination of a certain amount of beeps of 250ms defined by @a
   * beep_duration and certain amount of silence defined by @a beep_pause. After
   * the beep patterns, we add multiple silences of 250ms defined by @a
   * silence_duration.
   *
   * @param beep_duration Duration of one beep sound. Used with beep_pause to
   * create one beep pattern.
   * @param silence_duration Duration of the silence between beep patterns.
   * @param beep_number Number of time to repeat the beep patterns.
   * @param beep_pause Duration of the silence after a beep. Used with
   * beep_duration to create one beep pattern.
   *
   * @throws std::invalid_argument If the desired durations are not multiples of
   * execution_period_.
   */
  void setPattern(std::chrono::milliseconds beep_duration,
                  std::chrono::milliseconds silence_duration,
                  unsigned int beep_number = 1,
                  std::chrono::milliseconds beep_pause = 0s);

  // Getters
  const std::string &getKey() const;
  unsigned int getPriority() const;
  const SoundPattern &getPattern() const;
  const std::chrono::milliseconds &getExecutionPeriod() const;

private:
  /**
   * If @a variable_to_check != 0, check if @a variable_to_check is >= and a
   * multiple of execution_period_. If not, throws a std::invalid_argument with
   * a message that contains @a variable_name.
   *
   * @param variable_to_check Variable to compare with execution_period_.
   * @param variable_name Name to display in the error message.
   * @throws std::invalid_argument if @a variable_to_check doesn't respect
   * criterions.
   */
  void checkEntryValidity(const std::chrono::milliseconds &variable_to_check,
                          const std::string &variable_name);
  // Key used to toggle the alarm.
  std::string key_;
  // Priority to define which alarm to execute.
  unsigned int priority_;
  // Sound pattern to execute.
  SoundPattern pattern_;
  // Sound pattern unit of time.
  std::chrono::milliseconds execution_period_;
};

// Specific Alarms.
struct LowPriorityAlarm : Alarm {
  LowPriorityAlarm();
};
struct MediumPriorityAlarm : Alarm {

  MediumPriorityAlarm();
};
struct HighPriorityAlarm : Alarm {
  HighPriorityAlarm();
};