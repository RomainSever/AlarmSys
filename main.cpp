#include <fstream>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

struct Alarm {
  Alarm(){};
  Alarm(const std::string &k, unsigned int prio) : key(k), priority(prio) {}
  void setPattern(std::chrono::milliseconds beep_dur,
                  std::chrono::milliseconds period, int beep_num = 1,
                  std::chrono::milliseconds beep_pause = 0s) {
    for (int num = 0; num < beep_num; num++) {
      for (int i = 0; i < beep_dur / 250ms; i++) {
        pattern.push_back({1});
      }
      for (int i = 0; i < beep_pause / 250ms; i++) {
        pattern.push_back({0});
      }
    }
    for (int i = 0; i < period / 250ms; i++) {
      pattern.push_back({0});
    }
  }
  std::string key;
  unsigned int priority;
  std::vector<bool> pattern;
};

struct Executor {

  Executor() {

    auto silent = Alarm("", 0);
    silent.pattern.push_back({0});
    activated_alarms = {silent};

    alarm_output.open("alarm_output.txt");
  };
  ~Executor() {
    executor_thread.request_stop();
    alarm_output.close();
  }
  void toggleAlarm(const auto &a) {
    if (!std::erase_if(activated_alarms, [a](const auto &active) {
          return active.priority == a.priority;
        })) {
      activated_alarms.push_back(a);
    }
  }
  void executePattern() {

    auto highest = std::ranges::max(activated_alarms, [](Alarm a, Alarm b) {
      return a.priority < b.priority;
    });
    if (highest.priority != current_alarm_prio) {
      current_alarm_prio = highest.priority;
      executor_thread.request_stop();
      alarm_output << "start  " << std::endl;
      executor_thread = std::jthread(
          [std::ref(alarm_output), highest](const std::stop_token &stop_token) {
            while (!stop_token.stop_requested()) {
              for (const auto e : highest.pattern) {
                e ? alarm_output << "beep" << std::endl
                  : alarm_output << "--- " << std::endl;
                std::this_thread::sleep_for(250ms);
              }
            }
          });
    }
  }
  std::jthread executor_thread;
  std::ofstream alarm_output;
  std::vector<Alarm> activated_alarms;

  int current_alarm_prio = 0;
};

int main(int, char *[]) {

  auto low = Alarm("l", 1);
  low.setPattern(1s, 30s);

  auto med = Alarm("m", 2);
  med.setPattern(250ms, 1s);

  auto high = Alarm("h", 3);
  high.setPattern(250ms, 2s, 5, 500ms);

  std::vector<Alarm> lib = {low, med, high};

  std::string user_input = "";

  Executor exe;
  exe.executePattern();

  while (user_input != "q" && user_input != "quit") {

    std::cout << "Please enter input :";
    std::cin >> user_input;
    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
      for (const auto &a : lib) {
        if (user_input == a.key) {
          exe.toggleAlarm(a);
          exe.executePattern();
        }
      }
    }

    std::this_thread::sleep_for(100ms);
  }
  return 0;
}
