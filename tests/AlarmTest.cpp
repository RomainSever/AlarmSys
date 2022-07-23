#include "include/Alarm.hpp"
#include "include/catch.hpp"

TEST_CASE("Alarm creation", "[unit_test]") {
  // Invalid execution period.
  REQUIRE_THROWS_AS(Alarm("", 5, 0s), std::invalid_argument);
  REQUIRE_THROWS_AS(Alarm("", 5, -10s), std::invalid_argument);
  // Valid constructor.
  REQUIRE_NOTHROW(Alarm("", 5, 10s));
}

TEST_CASE("Setting a pattern from durations", "[unit_test]") {
  auto alarm = Alarm("", 0, 250ms);
  // Check if variable are < to the execution period.
  REQUIRE_THROWS_AS(alarm.setPattern(1ms, 250ms), std::invalid_argument);
  REQUIRE_THROWS_AS(alarm.setPattern(250ms, 1ms), std::invalid_argument);
  REQUIRE_THROWS_AS(alarm.setPattern(250ms, 250ms, 0, 1ms),
                    std::invalid_argument);
  // Check if variable are multiple of the execution period.
  REQUIRE_THROWS_AS(alarm.setPattern(251ms, 250ms), std::invalid_argument);
  REQUIRE_THROWS_AS(alarm.setPattern(250ms, 251ms), std::invalid_argument);
  REQUIRE_THROWS_AS(alarm.setPattern(250ms, 250ms, 0, 251ms),
                    std::invalid_argument);
  // Valid constructor.
  REQUIRE_NOTHROW(alarm.setPattern(250ms, 250ms, 0, 250ms));
}
TEST_CASE("Low priority pattern", "[unit_test]") {
  // Create THE low priority pattern. 4 beeps then 120 silences.
  auto low = LowPriorityAlarm();
  auto pattern = low.getPattern();
  int i;
  // Check the order of the pattern.
  for (i = 0; i < low.getPattern().size(); i++) {
    if (i < 4) {
      REQUIRE(pattern.at(i) == true);
    } else {
      REQUIRE(pattern.at(i) == false);
    }
  }
  // Check the number of element.
  REQUIRE(i == low.getPattern().size());
}

TEST_CASE("Medium priority pattern", "[unit_test]") {
  // Create THE medium priority pattern. 1 beeps then 4 silences.
  auto medium = MediumPriorityAlarm();
  auto pattern = medium.getPattern();
  int i;
  // Check the order of the pattern.
  for (i = 0; i < medium.getPattern().size(); i++) {
    if (i < 1) {
      REQUIRE(pattern.at(i) == true);
    } else {
      REQUIRE(pattern.at(i) == false);
    }
  }
  // Check the number of element.
  REQUIRE(i == medium.getPattern().size());
}

TEST_CASE("High priority pattern", "[unit_test]") {
  // Create THE high priority pattern. 5 * (1 beeps, 2 silences) then 8
  // silences.
  auto high = HighPriorityAlarm();
  auto pattern = high.getPattern();
  int i;
  // Check the order of the pattern.
  for (i = 0; i < pattern.size();) {
    if (i < 13) {
      REQUIRE(pattern.at(i) == true);
      REQUIRE(pattern.at(i + 1) == false);
      REQUIRE(pattern.at(i + 2) == false);
      i += 3;
    } else {
      REQUIRE(pattern.at(i++) == false);
    }
  }
  // Check the number of element.
  REQUIRE(i == high.getPattern().size());
}
