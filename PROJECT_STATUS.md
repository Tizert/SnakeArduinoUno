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
- Game uploads to board: not checked yet
- Snake moves: not checked yet
- Joystick input works: not checked yet
- Food generation works: not checked yet
- Collision detection works: not checked yet
- Score exists: not checked yet
- LCD support exists: planned

## Build result on new laptop

PlatformIO build completed successfully.

Memory usage:

- RAM: 216 bytes / 2048 bytes — 10.5%
- Flash: 2368 bytes / 32256 bytes — 7.3%

## Known problems

- `PROJECT_STATUS.md` was initially added as an empty file and then filled in a follow-up commit
- Root `README.md` is missing
- `CHANGELOG.md` is missing
- Project versioning is not formalized yet
- Most or all firmware code is currently located in `src/main.cpp`
- Repository structure needs documentation
- Minimal refactoring plan is not written yet

## Block 0 goal

Prepare this project as a small but clean embedded portfolio lab:

- repository structure
- README
- CHANGELOG
- versioning
- initial GitHub release/tag
- minimal refactoring plan without rewriting from scratch