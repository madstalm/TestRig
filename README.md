# TestRig
Codebase and documentation for the test rig.

## Contents
- [Getting Started](#getting-started)
  - [Getting the GUI Running on the Raspberry Pi](#getting-the-gui-running-on-the-raspberry-pi)
  - [Recommended Parameters for Testing the Axis](#recommended-parameters-for-testing-the-axis)
  - [Updating Code and Rebuilding Executables on the Raspberry Pi](#updating-code-and-rebuilding-executables-on-the-raspberry-pi)
- [Code Architecture](#code-architecture)
  - [Sending Programs Directly to the Galil](#sending-programs-directly-to-the-galil)
  - [Sending Commands Directly to the Galil from the Raspberry Pi](#sending-commands-directly-to-the-galil-from-the-raspberry-pi)

## Getting Started
### Getting the GUI Running on the Raspberry Pi
1. Open the terminal on the Raspberry Pi and enter:
```
cd ~/Documents/TestStation/build
```
3. From there run:
```
./TestRigControl
```

That will open the GUI.
  
### Recommended Parameters for Testing the Axis
We have found the following settings to work best for testing the motion of the axis:
- Speed: ___ mm/s
- Accel: ___ mm/s<sup>2</sup>
- Decel: ___ mm/s<sup>2</sup>
- Step size: ___ mm  
  
**Direction:**
- Negative/Reverse: stage moves towards stepper
- Positive/Forward: stage moves away from stepper


### Updating Code and Rebuilding Executables on the Raspberry Pi
This is pretty basic stuff, but of you don't know yet, you don't know and it's easier to find it here than elsewhere.
1. Open the parent directory for the `TestStation` code in the terminal and run the following command to pull the latest code from GitHub:
```
git pull
```
2. Open the '''TestStation/build''' directory in the terminal and run the following command to recompile the executable:
```
make
```

## Code Architecture
Right now the code follows standard C++ conventions and is set up with the following structure:
```
Parent Directory
├── src
│   ├── ui
│   │   └── mainwindow.ui
│   ├── main.cpp
│   ├── mainwindow
│   └── galilcontroller
├── CMakeLists.txt
├── program.dmc
├── pyUploadProgram.py
└── README.md
```  
**Source (src) code descriptions:**
- `main.cpp`: starts **Qt** application and then creates and runs an instance of `MainWindow`.
- *MainWindow*: code for the UI interface that calls the functions in `GalilController`
  - `mainwindow.h` (declarations)
    - Declares the class (inherits **Qt**'s `QMainWindow`), declares all slot signatures (`onConnectClicked`, `onEnableMotorClicked`, etc.) with no logic + holds member variables: `Ui::MainWindow *ui`, `GalilController m_galil`, etc.
  - `mainwindow.cpp` (implementations)
    - Constructor: builds the UI, connects each button's clicked signal to its slot, starts poll timer, sets initial disconnected state
    - Implements each slot's actual logic - e.g. `onMoveAbsClicked()` reads the spinbox and calls `m_galil.moveAbsolute()`
    - `pollStatus()` - 200ms tick reading position/limits/motion state and updating labels
    - Implements `setConnectedState()` and `log()`
    - **Contains the working instance of `GalilController`**
  - `mainwindow.ui`: **Qt** XML file defining the visual layout - buttons, spinboxes, labels, group boxes - of the UI and their object names (e.g. `connectBtn`, `speedSpin`)
    - Auto-processed at build time (via **CMake**'s `AUTOUIC`) into a generated `ui_mainwindow.h`, which creates the `Ui::MainWindow` class that `mainwindow.cpp` accesses through the `ui->` pointer
- *GalilController*: wraps **gclib** (Galil's official C library) via GOpen/GClose/GCommand
  - `galilcontroller.h` (declarations)
    - Declares public API + `MICROSTEPS_PER_MM` constant + private helper signatures + member variables (`m_handle`, `m_connected`, `m_lastError`)
  - `galilcontroller.cpp` (implementations)
    - Implements `sendCommand()`/`queryValue()` (actual `GCommand()` calls), and every public method's mm↔microstep math + raw DMC command strings
    - sends raw DMC command strings to the controller over the network
  
**Other files:**
- `CMakeLists.txt`: Compiler configuration that manages how the code compiled and built across different platforms to generate native build files tailored to specific systems (Win11 vs. RaspberryPiOS).
- `pyUploadProgram.py` and `program.dmc`: `pyUploadProgram.py` will upload the custom commands in `program.dmc` to the Galil motor controller for testing purposes.

### Sending Programs Directly to the Galil
- `program.dmc` is a DMC file (sequence of Galil commands) that can be stored on the controller itself and run independently, without the Qt GUI connected. *Talmage - Not sure what `program.dmc` does but it looks like right now  it sets up some kind of custom print routine? Maybe a holdover from the lab's custom BJ printer.*  
- `pyUploadProgram.py` is a small utility that opens telnet socket 192.168.42.100 (on port 23) to the controller and uploads `program.dmc` into the controller's program memory:

### Sending Commands Directly to the Galil from the Raspberry Pi
1. Open a terminal and enter:
```
telnet 192.168.42.100 23
```
3. Then type your command and press enter - e.g. `SHA`
4. To leave telnet press Ctrl+] then type `quit`