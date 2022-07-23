#include "include/Executor.hpp"
#include "include/catch.hpp"
#include <iostream>

TEST_CASE("Executor creation", "[unit_test]") {
  // Invalid execution period.
  REQUIRE_THROWS_AS(Executor(0s), std::invalid_argument);

  // Valid constructor.
  auto period = 100ms;
  auto duration = 1s;
  Executor *executor;
  REQUIRE_NOTHROW(executor = new Executor(period));

  // Wait for some data to be written in the output file before destroying the
  // executor.
  std::cout << duration << " of silence" << std::endl;
  std::this_thread::sleep_for(duration);
  delete executor;

  // Read the output file, check that the correct amount of silence have been
  // written.
  std::ifstream output("alarm_output.txt");
  std::string line = "";
  int line_num = 0;
  while (getline(output, line)) {
    REQUIRE(line == "_");
    line_num++;
  }
  // As we are not executing this in real time, we consider that it's acceptable
  // to have +/- 1 line of difference between expectation and reality.
  REQUIRE(abs(line_num - static_cast<int>(duration / period)) <= 1);
  output.close();
}

TEST_CASE("Alarm toggling", "[unit_test]") {
  auto period = 100ms;
  Executor executor(period);
  // Periods of the alarm and the executor should match.
  REQUIRE_THROWS_AS(executor.toggleAlarm(Alarm("", 0, period * 2)),
                    std::invalid_argument);
  REQUIRE_NOTHROW(executor.toggleAlarm(Alarm("", 0, period)));
}

TEST_CASE("Medium priority alarm execution", "[unit_test]") {
  auto medium = MediumPriorityAlarm();
  auto executor = new Executor(medium.getExecutionPeriod());
  executor->toggleAlarm(medium);
  // Wait for a complete sound pattern to be written in the output file before
  // destroying the executor.
  auto pattern_duration =
      medium.getPattern().size() * medium.getExecutionPeriod();
  std::cout << "Executing MediumPriorityAlarm for " << pattern_duration
            << std::endl;
  std::this_thread::sleep_for(pattern_duration);
  delete executor;

  // Read the output file, check that the correct amount of beep/silence have
  // been written.
  std::ifstream output("alarm_output.txt");
  std::string line = "";
  int line_num = 0;
  while (getline(output, line)) {
    if (line_num == 0) {
      REQUIRE(line == "X");
    } else {
      REQUIRE(line == "_");
    }
    line_num++;
  }
  // WARNING : following assertion is not very robust but better than nothing.
  // As we are not executing this in real time, we consider that it's acceptable
  // to have +/- 1 line of difference between expectation and reality.
  REQUIRE(abs(line_num - static_cast<int>(medium.getPattern().size())) <= 1);
  output.close();
}

TEST_CASE("Multiple alarm execution", "[integration_test]") {
  // Toggle high priority alarm.
  auto high = HighPriorityAlarm();
  auto executor = new Executor(high.getExecutionPeriod());
  executor->toggleAlarm(high);

  // Toggle medium priority alarm.
  auto medium = MediumPriorityAlarm();
  executor->toggleAlarm(medium);

  // Wait for a complete high prio sound pattern to be written in the output.
  auto high_pattern_duration =
      high.getPattern().size() * high.getExecutionPeriod();
  std::cout << "Executing HighPriorityAlarm for " << high_pattern_duration
            << std::endl;
  std::this_thread::sleep_for(high_pattern_duration);
  // Then disable the high prio alarm.
  executor->toggleAlarm(high);

  // Wait for a complete medium prio sound pattern to be written in the output.
  auto med_pattern_duration =
      medium.getPattern().size() * medium.getExecutionPeriod();
  std::cout << "Executing MediumPriorityAlarm for " << med_pattern_duration
            << std::endl;
  std::this_thread::sleep_for(med_pattern_duration);
  // Then disable the medium prio alarm.
  executor->toggleAlarm(medium);

  // Wait for some silences before deleting the executor.
  int silences_number = 4;
  std::cout << silences_number * medium.getExecutionPeriod() << " of silence"
            << std::endl;
  std::this_thread::sleep_for(silences_number * medium.getExecutionPeriod());

  delete executor;

  // Read the output file, check that the correct amount of beeps and silences
  // have been written.
  std::ifstream output("alarm_output.txt");
  std::string line = "";
  int line_num = 0;
  int beep_num = 0;
  int silence_num = 0;
  while (getline(output, line)) {
    if (line == "X") {
      beep_num++;
    } else {
      silence_num++;
    }
    line_num++;
  }

  // WARNING : following assertions are not very robust but better than nothing.
  // As we are not executing this in real time, we consider that it's acceptable
  // to have +/- 1 line of difference between expectation and reality for each
  // activated alarm.
  REQUIRE(abs(line_num - static_cast<int>(medium.getPattern().size() +
                                          high.getPattern().size() +
                                          silences_number)) <= 2);
  REQUIRE(abs(beep_num - 6) <= 1);
  REQUIRE(abs(silence_num - static_cast<int>(medium.getPattern().size() +
                                             high.getPattern().size() +
                                             silences_number - beep_num)) <= 2);

  output.close();
}
