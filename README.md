# Snake Arduino Uno

Embedded learning project: Snake game on an 8x8 LED matrix controlled by a joystick.

## Purpose

This is an early embedded lab project focused on turning a small Arduino game into a maintainable firmware project.

The project is used to practice:

- VS Code
- PlatformIO
- Git and GitHub workflow
- embedded project structure
- documentation
- versioning and changelog discipline
- incremental refactoring without rewriting from scratch

## Hardware

- Arduino Uno
- ATmega328P microcontroller
- 8x8 LED matrix
- Joystick module
- LCD 1602A planned for future use

## Software stack

- Language: Arduino-style C/C++
- Editor: VS Code
- Build system: PlatformIO
- Platform: Atmel AVR
- Board target: `uno`
- Framework: Arduino

## Build

Open the project in VS Code as a folder and use PlatformIO.

Build command:

```bash
pio run