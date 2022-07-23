<h3 align="center">Alarm System Project</h3>

  <p align="center">
    The purpose of this exercise is to create an API and an implementation to play different alarm audio sequences. 
    <br />
    <a href="https://github.com/RomainSever/AlarmSys/blob/master/alarm_sys.pdf"><strong>Complete project proposition document »</strong></a>
    
  </p>
</div>

<!-- ABOUT THE PROJECT -->
## About The Project
This project has been developed in C++ 20 on Windows. 


<!-- GETTING STARTED -->
## Getting Started

### Prerequisites
A way to make this project work from scratch is to observe the following instructions. 
### Installation

1. Download and Install compilers from Visual Studio Community edition with Desktop C++ workload at [https://visualstudio.microsoft.com/fr/vs/community/](https://visualstudio.microsoft.com/fr/vs/community/).
2. Download CMake binaries for Windows at [https://cmake.org/download/](https://cmake.org/download/).
3. Install it and add it to the Path variable.
4. Clone this project and build it.

<!-- USAGE EXAMPLES -->
## Usage

You can execute `alarm_sys.exe`. It will start a silent alarm that will write "sounds" to the `alarm_output.txt` file in the build folder. "X" represent a beep and "_" a silence. 

To read this file on windows you can use : 
```sh
Get-Content .\alarm_output.txt –Wait
```

By default three alarms are created. 
- LOW: one beep every 30s, with a beep duration of 1s. Repeat continuously
- MEDIUM: one beep every second, with a beep duration of 250ms. Repeat continuously
- HIGH: five beeps every 500ms with duration of 250ms each, then wait for 2s with no beep. Repeat continuously

You can toggle this alarms respectively whit keys "l", "m" and "h" and pressing enter.

Alarm respects the following rules :
- There can be only one alarm sequence at a time being played.
- Only the highest priority active alarm is played at a time.
- They can be started and stopped independently.

Toggling an alarm add it to the active alarm list. If an alarm with the same priority is already activated, deactivate it. The activated with the highest priority is executed.

<!-- TESTS -->
## Tests
Test files have been written for each classes and can be executed using `alarm_sys_test.exe`. These tests ensure :
- Alarm creation is robust and permit to create alarms of different sound patterns and with arbitrary keys for their activation/deactivation.
- Alarm sound patterns are correct for the three default alarms.
- Executor is robust and start with a "silent" alarm.
- Alarm toggling has the correct behavior.
- Alarm execution produce the correct sound pattern.
- The whole API works on different basic scenarios.

