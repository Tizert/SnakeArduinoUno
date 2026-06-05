# Snake Embedded Lab — Project Status

## Project purpose

Small embedded learning project: Snake game on 8x8 LED matrix controlled by joystick.

The goal is not only to make the game work, but to organize it as a maintainable embedded project using PlatformIO, Git, GitHub, README, CHANGELOG, and incremental refactoring.

## Current hardware

- Board: Arduino Uno
- MCU: ATmega328P
- LED matrix: 8x8
- Input: joystick module
- Optional future module: LCD 1602A

## Current software stack

- Language: Arduino-style C/C++
- Editor: VS Code
- Build system: PlatformIO
- Platform: Atmel AVR
- Board target: uno
- Framework: Arduino

## Current repository structure

- `platformio.ini` — PlatformIO project configuration
- `src/main.cpp` — current firmware source code
- `include/` — reserved for project header files
- `lib/` — reserved for project-specific libraries
- `test/` — reserved for tests
- `.gitignore` — ignored generated and private files

## Current project state

- Project cloned from GitHub: yes
- VS Code opened at repository root: yes
- Git remote configured: yes
- Current branch: main
- PlatformIO build on new laptop: yes
- Game builds: yes
- Game uploads to board: yes
- LED matrix works: yes
- Snake starts: no
- Joystick input works: not verified
- Food generation works: not verified
- Collision detection works: not verified
- Score exists: not verified
- LCD support exists: planned

## Build result on new laptop

PlatformIO build completed successfully.

Memory usage:

- RAM: 216 bytes / 2048 bytes — 10.5%
- Flash: 2368 bytes / 32256 bytes — 7.3%

## Upload result on new laptop

PlatformIO upload completed successfully.

- Upload port: COM5
- Target MCU detected by avrdude: ATmega328P
- Flash written: 9002 bytes
- Flash verification: successful

## Known problems

- Project versioning is not formalized yet
- Most or all firmware code is currently located in `src/main.cpp`
- Repository structure needs documentation
- Minimal refactoring plan is not written yet
- `src/main.cpp`: `turnSnake()` does not explicitly handle enum value `None` in a `switch` statement.
- Current firmware builds and uploads, but the game does not start correctly on hardware.
- After startup, the game appears to enter the losing screen/state immediately.
- Several gameplay mechanisms exist in code, but their integration needs verification:
  - food generation
  - snake body tracking
  - direction handling
  - food eating check
  - win/lose screen rendering

## Block 0 goal

Prepare this project as a small but clean embedded portfolio lab:

- repository structure
- README
- CHANGELOG
- versioning
- initial GitHub release/tag
- minimal refactoring plan without rewriting from scratch