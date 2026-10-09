# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

### Added

- Added initial project status document.
- Added root README.
- Updated `.gitignore` to exclude private learning notes.

### Verified

- Confirmed that the project builds successfully with PlatformIO for Arduino Uno.
- Confirmed successful upload to Arduino Uno from the new laptop.
- Confirmed PlatformIO auto-detected the board on COM5.
- Confirmed flash write and verification completed successfully.

## [0.1.0] - 2026-06-05

### Added

- Initial public repository structure.
- PlatformIO project for Arduino Uno.
- Initial Snake firmware source in `src/main.cpp`

## [Unreleased]

### Changed

- Refactored snake movement to reuse the tail node during normal movement.
- Separated next-head coordinate calculation from linked-list mutation.
- Consolidated game-state transition initialization.
- Combined Win and Lose animation handling through a drawing callback.
- Replaced separate Win, Lose, and Error timers with one game-state timer.

### Fixed

- Allowed movement into the current tail position when the snake is not growing.
- Prevented collision validation from being repeated inside list-mutation functions.
- Fixed the error-screen coordinate that was outside the 5x5 board.