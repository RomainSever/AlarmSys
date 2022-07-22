#include "Executor.hpp"
#include <iostream>

using namespace std::chrono_literals;

int main(int, char *[]) {
  // Create basic alarms and add them to the library.
  auto low = Alarm("l", 1);
  low.setPattern(1s, 30s);
  auto med = Alarm("m", 2);
  med.setPattern(250ms, 1s);
  auto high = Alarm("h", 3);
  high.setPattern(250ms, 2s, 5, 500ms);
  std::vector<Alarm> alarms_library = {low, med, high};

  // Initialize the alarm executor (start the alarm execution thread with a
  // silent alarm).
  Executor exe;

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
