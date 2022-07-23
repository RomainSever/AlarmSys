#include "Alarm.hpp"
#include <fstream>
#include <thread>

/**
 * Class that keep the activated alarms and execute the sound pattern of the one
 * with the highest priority in a separated thread.
 *
 */
class Executor {
public:
  /**
   * Create a silent alarm that has no beep and a execution period equal to the
   * Executor execution period and add it to the active alarm list. Open the
   * alarm_output_ file "alarm_output.txt" and start the silent alarm
   * execution.
   *
   * @param execution_period  The unity of time of one sound of the alarm. Must
   * be > 0.
   * @throws std::invalid_argument if the @a execution_period is <= 0;
   */
  Executor(const std::chrono::milliseconds &execution_period);

  /**
   * Stop the execution thread properly and close the alarm_output_ file.
   */
  ~Executor();

  /**
   * Add @a alarm to the list of activated alarms. If an alarm of the same
   * priority already exist delete it. Then call
   * @c executeHighestPriorityPattern().
   *
   * @param alarm The alarm to activate / deactivate.
   *
   * @throws std::invalid_argument if the @a alarm execution period doesn't
   * match the executor execution_period_.
   */
  void toggleAlarm(const Alarm &alarm);

private:
  /**
   * Find the alarm with the highest priority in the list of active alarms. If
   * it's different from the currently playing alarm, stop the execution thread
   * and setup a new one with the new alarm.
   *
   * Alarm execution write alarm output in the alarm_output_ file. Alarm output
   * depends on the alarm SoundPattern. In the pattern a beep correspond to True
   * and is represented by a 'X' in the output file. Silence by a '_'. Sound
   * writing frequency depends on the execution_period_.
   */
  void executeHighestPriorityPattern();

  // Thread that execute the active alarm with the highest priority.
  std::jthread executor_thread_;
  // Output file where alarm sounds are written.
  std::ofstream alarm_output_;
  // List of active alarms.
  std::vector<Alarm> activated_alarms_;
  // Active alarm priority.
  unsigned int current_alarm_priority_ = -1;
  // Execution period for the execution thread.
  std::chrono::milliseconds execution_period_;
};