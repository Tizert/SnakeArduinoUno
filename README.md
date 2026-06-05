# Snake Arduino Uno

Snake game for Arduino Uno with an 8x8 LED matrix and joystick control.

This is an early embedded learning project. The goal is to practice not only firmware development, but also project organization: PlatformIO, Git, GitHub, documentation, and incremental refactoring.

## Hardware

- Arduino Uno
- ATmega328P
- 8x8 LED matrix
- Joystick module
- LCD 1602A planned for later

## Software

- VS Code
- PlatformIO
- Arduino framework
- C / C++ style code

## Build

Open the repository folder in VS Code with PlatformIO installed.

Build:

```bash
pio run
```

## Current status

- Build works
- Upload to Arduino Uno works
- LED matrix output works
- Game currently does not start correctly on hardware
- Current behavior: firmware appears to enter the losing screen/state immediately after startup
- Joystick, food generation, and collision behavior need verification
- Most code is currently in `src/main.cpp`
- Refactoring plan is not written yet

## Next steps

- Check joystick and LED matrix behavior on real hardware
- Add wiring notes
- Update `CHANGELOG.md` as the project changes
- Prepare a small refactoring plan