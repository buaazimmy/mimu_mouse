# CLAUDE.md

## Project Overview

**MIMU Mouse** (Magnetic Inertial Measurement Unit Mouse) is a Windows-native C++ application that turns an IMU sensor device into a mouse controller. It reads accelerometer, gyroscope, and magnetometer data from a hardware IMU over a serial COM port, processes the data through filtering and orientation algorithms, and translates the result into mouse cursor movements on screen. A Qt-based system tray GUI provides start/stop controls, and an OpenGL window can visualize the motion path in 3D.

## Language and Standards

- **C++ (C++98/03 dialect)** — no modern C++ features (no `auto`, `nullptr`, range-for, etc.)
- One file (`rs232.c`) is plain C
- Windows-only: uses Win32 API throughout (`CreateThread`, `SetCursorPos`, `mouse_event`, `HANDLE`, `CRITICAL_SECTION`)

## Repository Layout

All source files live in the repository root (no `src/` directory):

```
/
├── main.cpp              # Entry point — Qt app + thread creation
├── MIMUGUI.cpp/h         # Qt GUI (system tray icon, start/stop)
├── algorithm.cpp/h       # IMU math: quaternions, orientation, offset correction
├── DataConvernt.cpp/h    # Serial data reception and parsing into IMU structs
├── kalman.cpp/h          # Kalman filter for sensor noise smoothing
├── camera.cpp/h          # FPS-style camera state for 3D visualization
├── COM.cpp/h             # COM port initialization (BT and CDC modes)
├── CSerialPort.cpp/h     # Windows serial port handling with threading
├── rs232.c/h             # Cross-platform RS232 library (C)
├── SetCursorTest.cpp/h   # Mouse cursor control (gyro, accel, angle modes)
├── AppAPITest.cpp/h      # VirTouch API initialization
├── VirTouchAppAPI.h      # Virtual touch API header
├── GL.cpp                # OpenGL rendering and GLUT callbacks
├── draw.cpp/h            # 2D drawing and key-input handling
├── gl.h                  # OpenGL headers
├── vector.h              # GLM vector utilities
├── stdafx.h              # Precompiled header
├── targetver.h           # Windows version targeting
├── RESOURCE.H            # Resource constants
├── MIMUGUI.moc           # Qt MOC-generated file (checked in)
├── MIMU PC.cbp           # Code::Blocks project (primary build config)
├── Makefile              # Alternative GNU Make build
├── VirTouchAPI.dll/lib   # Precompiled VirTouch library
├── ps.ico                # Application icon
├── bin/                  # Build output (Debug/ and Release/)
└── obj/                  # Object files (Debug/ and Release/)
```

## Architecture and Data Flow

```
IMU Hardware (serial)
  │
  ▼
COM.cpp / rs232.c         ← Serial port init (BT or CDC mode, 38400 baud)
  │
  ▼
DataConvernt.cpp          ← Parse raw bytes → IMU struct {acc, gyro, hmc, pressure, T}
  │
  ▼
kalman.cpp                ← Kalman filter smoothing
  │
  ▼
algorithm.cpp             ← Quaternion orientation, acceleration correction, euler angles
  │
  ▼
SetCursorTest.cpp         ← Map to screen coordinates → SetCursorPos / mouse_event
  │
  ▼
GL.cpp                    ← Optional 3D visualization (OpenGL/GLUT)
MIMUGUI.cpp               ← Qt system tray GUI
```

### Core Data Structures (`algorithm.h`)

- `IMU` — raw sensor reading (accelerometer, gyroscope, magnetometer vectors + pressure + timestamp)
- `IMUPOS` — computed position/orientation (acceleration, velocity, position, euler angles, quaternion)
- `IMUoffset` — calibration offsets for accelerometer and gyroscope

### Threading Model (`main.cpp`)

Uses Win32 `CreateThread`:
1. **Main thread** — Qt event loop (`QApplication::exec()`)
2. **InitMIMU** — Serial init, IMU startup, spawns the next two threads
3. **MainProcess** — Continuous sensor data processing loop
4. **KeyAction** — Mouse cursor control updates

Global data (`dataVec`, `PosVec`) is shared between threads.

## Build System

### Code::Blocks (primary)

Project file: `MIMU PC.cbp`

- **Compiler:** MinGW GCC (g++)
- **Targets:** `Debug` (no optimization) and `Release` (`-O2`, `-s` strip)
- **Flags:** `-fexceptions`, `-Wall` (release only)
- **Hardcoded paths:** `D:/Qt/4.7.3/`, `C:/MinGW/dll/` (Windows dev machine)
- **Output:** `bin/Debug/MIMU PC` or `bin/Release/MIMU PC`

### Makefile (alternative)

```bash
make              # Build (target: ShapeHuman)
make clean        # Remove build artifacts
```

Compiles a subset of files: `algorithm.o`, `kalman.o`, `main.o`, `SetCursorTest.o`. Links against `libuser32.a`.

## External Dependencies

| Library | Purpose |
|---------|---------|
| **Qt 4.7.3** | GUI framework (QtCore, QtGui) |
| **OpenGL + GLU** | 3D rendering |
| **GLEW** | OpenGL extension loading |
| **FreeGLUT** | OpenGL windowing/context |
| **GLM** | Header-only math (vectors, quaternions, matrices) |
| **VirTouchAPI** | Virtual touch/mouse control (proprietary DLL) |
| **Win32 API** | Threading, cursor control, serial ports |

## Testing

There is **no automated test suite**. Files named `*Test.*` (e.g., `SetCursorTest.cpp`, `AppAPITest.cpp`) contain application logic for manual hardware testing, not unit tests.

## Linting and Formatting

No linting or formatting tools are configured. No `.clang-format`, `.editorconfig`, or similar files exist.

## CI/CD

No CI/CD configuration exists. Builds are done manually.

## Key Conventions

- **Flat structure:** All sources in root directory — no subdirectories for source code
- **Mixed languages in comments:** English and Chinese
- **Windows-only APIs:** All threading, cursor control, and serial communication use Win32 directly
- **Global shared state:** IMU data vectors are shared across threads via `extern` declarations
- **Serial modes:** Two modes supported — `BTmode` (Bluetooth) and `CDCmode` (USB CDC), selected in `main.cpp`
- **No namespaces:** All code is in the global namespace
- **Header guards:** Traditional `#ifndef`/`#define`/`#endif` style

## Notes for AI Assistants

- This is an embedded-systems / hardware-interfacing project. Changes to the serial communication or IMU processing code affect real hardware interaction.
- The codebase targets Windows exclusively. Do not introduce POSIX-only APIs.
- Qt 4.7.3 is legacy (Qt 4 API). Do not use Qt 5/6 APIs without a migration plan.
- GLM is used for all vector/matrix math. Use `glm::vec3`, `glm::quat`, etc. rather than custom math.
- The Makefile only builds a subset of files. For a full build, use the Code::Blocks project.
- There are no tests to run. Validation requires the physical IMU hardware.
- The checked-in `MIMUGUI.moc` file is a Qt MOC output — it would need regeneration if `MIMUGUI.h` changes.
