#include "include/Executor.hpp"
#include <iostream>

using namespace std::chrono_literals;

int main(int, char *[]) {
  auto execution_period = 250ms;
  // Create basic alarms and add them to the library.
  std::vector<Alarm> alarms_library = {
      LowPriorityAlarm(), MediumPriorityAlarm(), HighPriorityAlarm()};

  // Initialize the alarm executor (start the alarm execution thread with a
  // silent alarm).
  Executor exe(execution_period);

  // Start the user interface
  std::string user_input = "";
  while (user_input != "q" && user_input != "quit") {
    // Wait for user input
    std::cout << "Please enter key input: ";
    getline(std::cin, user_input);
    if (user_input == "h" || user_input == "help") {
      std::string keys = "Alarms keys are: ";
      for (const auto &alarm : alarms_library) {
        keys += alarm.getKey() + " ";
      }
      std::cout << keys << std::endl;
    }
    // Parse the library and toggle the corresponding alarm if found.
    else if (auto result = std::ranges::find_if(
                 alarms_library,
                 [&user_input](Alarm a) { return a.getKey() == user_input; });
             result != alarms_library.end()) {
      exe.toggleAlarm(*result);
      exe.executePattern();
    } else {
      std::cout << "No alarm bound to the key: " << user_input << std::endl;
    }
  }
  return 0;
}
