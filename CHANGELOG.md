# Changelog

All notable changes to this project will be documented in this file.

The format is based on Keep a Changelog, and this project uses Semantic Versioning.

## [Unreleased]



## [0.2.0] - 2026-10-09

### Added

- Added project status documentation.
- Added root README.
- Added private learning-note exclusions to `.gitignore`.
- Added explicit game states: waiting, running, win, lose, and error.
- Added absolute joystick direction handling with a neutral dead zone.
- Added direction validation that prevents 180-degree turns.
- Added restart handling.
- Added Win, Lose, and Error screen animations.

### Changed

- Refactored snake movement to reuse the tail node during normal movement.
- Separated next-head coordinate calculation from linked-list mutation.
- Reduced dynamic allocation during normal gameplay.
- Consolidated game-state transition initialization.
- Combined Win and Lose animation handling through a drawing callback.
- Replaced separate Win, Lose, and Error timers with one game-state timer.
- Refactored joystick input into a dedicated direction-reading function.
- Refactored snake movement into calculation, validation, and mutation stages.

### Fixed

- Fixed immediate game over caused by comparing the new head with itself.
- Fixed incorrect linked-list updates that reduced the snake body to one segment.
- Fixed neutral joystick input being interpreted as movement.
- Fixed food consumption and food replacement logic.
- Fixed snake growth after food consumption.
- Allowed movement into the current tail position when the snake is not growing.
- Prevented collision checks from being repeated inside list-mutation functions.
- Prevented new food from being placed on the snake body.

### Verified

- PlatformIO build succeeds for Arduino Uno.
- Firmware upload and flash verification succeed.
- Joystick input works in all four absolute directions.
- Snake movement, growth, food consumption, and collision handling work on hardware.

## [0.1.0] - 2026-06-05

### Added

- Initial public repository structure.
- Initial PlatformIO project for Arduino Uno.
- Initial Snake firmware implementation in `src/main.cpp`.