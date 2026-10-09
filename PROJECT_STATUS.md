git switch # Snake Embedded Lab — Project Status

## Project purpose

Small embedded learning project: Snake game on 8x8 LED matrix controlled by joystick.

The goal is not only to make the game work, but to organize it as a maintainable embedded project using PlatformIO, Git, GitHub, README, CHANGELOG, and incremental refactoring.

## Current hardware

- Board: Arduino Uno
- MCU: ATmega328P
- LED matrix: 8x8 (Current active game field: 5x5)
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

- PlatformIO build: successful
- Upload to Arduino Uno: successful
- LED matrix output: works
- Game start state: works
- Absolute joystick direction input: works
- Neutral joystick dead zone: works
- Opposite-direction input filtering: works
- Snake movement: works
- Snake body growth: works
- Food generation: works
- Food placement avoids the snake body: works
- Self-collision detection: works
- Movement into the current tail position without growth: supported
- Lose state and animation: works
- Win state and animation: works
- Error state and animation: works
- Restart handling: works
- Score: not implemented
- LCD 1602A support: planned

## Release verification

- PlatformIO build: successful
- Upload to Arduino Uno: successful
- Hardware smoke test: successful
- Release candidate: v0.2.0

## Known limitations and technical debt

- Automated tests are not implemented yet.
- Score display is not implemented.
- LCD 1602A support is not implemented.

## Block 0 outcome

- VS Code and PlatformIO environment configured
- Git and GitHub workflow established
- README and CHANGELOG added
- Project status documented
- Build and upload verified
- Hardware smoke testing completed
- Initial firmware refactoring completed
- Semantic versioning introduced
- Release `v0.2.0` published

## Roadmap

- Release and tag version `v0.2.0`
- Implement an alternative snake representation using a two-dimensional matrix
- Compare linked-list and matrix-based implementations
- Split `src/main.cpp` into focused modules
- Reduce or remove remaining dynamic memory allocation
- Replace blocking restart animation with non-blocking logic
- Add score tracking
- Add LCD 1602A support
- Add hardware-independent tests for game logic