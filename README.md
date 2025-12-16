# BreakerEngine Documentation

## Table of Contents
1. [Introduction](#introduction)
2. [Overview](#overview)
3. [Project Structure](#project-structure)
4. [Installation](#installation)
5. [Getting Started](#getting-started)
6. [Core Architecture](#core-architecture)
7. [Building the Project](#building-the-project)
8. [Contributing](#contributing)
9. [License](#license)

---

## Introduction

**BreakerEngine** is an advanced game engine built on top of CSFML (C bindings for SFML - Simple and Fast Multimedia Library). This engine provides a high-level framework for developing 2D games and multimedia applications in C, abstracting away common boilerplate and providing powerful tools for game development.
This engine is designed to be ready-to-go for the first year students of my school, that's why the lib/dll/resources are included in this repo and cmake is not used, please don't give attention to this. 

### Key Features
- Built on CSFML for robust multimedia support
- Windows graphics, audio, and input handling
- Modular architecture for easy extension
- Resource management system
- Cross-platform compatibility (Windows focus with x64 support)

### Author
Copyright (c) 2025 Yann Grallan

---

## Overview

BreakerEngine simplifies game development by providing a structured framework that handles:
- **Window Management**: Create and manage rendering windows
- **Graphics**: Sprite rendering, texture management, and visual effects
- **Audio**: Sound and music playback
- **Input Handling**: Keyboard, mouse, and joystick support
- **Resource Loading**: Asset management for textures, sounds, and fonts
- **Scene Management**: Organize game states and transitions
- **Animation management**: Animations and key anim with .anim file
- **Particles management**: Emitters and particles with .part file
- **UI module**: Ui button with state

---

## Project Structure

```
BreakerEngine/
├── BreakerEngine/          # Main engine source code
│   ├── *.c                 # C implementation files
│   ├── *.h                 # Header files
│   └── BreakerEngine.vcxproj  # Visual Studio project file
├── Doc/                    # Documentation files
├── Ressources/             # Game resources (textures, sounds, etc.)
├── include/                # External library headers (CSFML)
├── lib/                    # External library binaries
├── x64/                    # 64-bit build outputs
├── .gitattributes          # Git configuration
├── .gitignore              # Git ignore rules
├── BreakerEngine.sln       # Visual Studio solution file
└── LICENSE                 # MIT License
```

### Key Directories

- **BreakerEngine/**: Contains all the engine's source code and implementation
- **include/**: CSFML header files for compilation
- **lib/**: Compiled CSFML library files
- **Doc/**: Contains the Doxygen doc
- **Ressources/**: Store your game assets (images, sounds, fonts, etc.)
- **x64/**: Compiled binaries and intermediate build files
- **Doc/**: Additional documentation and reference materials

---

## Installation

### Prerequisites

1. **C Compiler**: Visual Studio 2019 or later (for Windows)
2. **CSFML**: The engine requires CSFML 2.5 or later
3. **Operating System**: Windows 10/11 (x64)

### Steps

1. **Clone the Repository**
   ```bash
   git clone https://github.com/Xanhos/BreakerEngine.git
   cd BreakerEngine
   ```

3. **Open the Solution**
   - Open `BreakerEngine.sln` in Visual Studio
   - Ensure the platform is set to x64
   - Build the solution 

---

## Getting Started

### Basic Application Structure

Check the test project in the solution.

## Core Architecture

### Engine Core

The engine is built around several core systems:

#### 1. **Window Management**
- Creates and manages the rendering window
- Handles window events (close, resize, focus)
- Provides framerate control

#### 2. **Resource Manager**
- Loads and caches game resources
- Manages memory for textures, sounds, and fonts

#### 3. **Scene System**
- Organizes game into manageable states
- Handles scene transitions
- Provides scene lifecycle methods (init, update, render, destroy)

#### 4. **Input Handler**
- Processes keyboard input
- Handles mouse events
- Supports joystick/gamepad input

#### 5. **Audio System**
- Plays sound effects
- Streams music
- Controls volume and playback

### Design Patterns

BreakerEngine follows these design patterns:

- **Resource Management**: Centralized loading and caching of assets
- **State Pattern**: Scene management using state machine principles

---

### Build Configuration

The project uses the following configurations:

- **Debug x64**: For development with debugging symbols
- **Release x64**: Optimized build for production


## Contributing

Contributions are welcome! Here's how you can help:

### Getting Involved

1. **Fork the Repository**

3. **Create a Feature Branch**

4. **Make Your Changes**
   
6. **Test Thoroughly**
   
8. **Commit with Clear Messages**

9. **Push to Your Fork**

10. **Open a Pull Request**

### Code Style Guidelines

- Comment complex logic
- Keep functions focused and modular
- Write descriptive variable names

### Testing

- Test on Windows x64
- Verify no memory leaks
- Ensure backwards compatibility
- Document new features

---

## License

BreakerEngine is licensed under the **MIT License**.

---

## Additional Resources

### CSFML Documentation
- [Official CSFML Repository](https://github.com/SFML/CSFML)
- [SFML Tutorials](https://www.sfml-dev.org/tutorials/) (C++ but applicable)
- [CSFML API Reference](https://www.sfml-dev.org/documentation/)

### Community
- Report bugs on [GitHub Issues](https://github.com/Xanhos/BreakerEngine/issues)
- Contribute via [Pull Requests](https://github.com/Xanhos/BreakerEngine/pulls)

### Support
For questions or support, please open an issue on GitHub with:
- A clear description of your problem
- Steps to reproduce
- Expected vs actual behavior
- System information (OS, compiler version)

**Last Updated**: December 2024  
**Engine Version**: Development  
**CSFML Version**: 2.5+
