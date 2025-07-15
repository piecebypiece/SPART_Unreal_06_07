# Development Guidelines

## Project Overview

This project is a C++-based Unreal Engine project. The primary language for development is C++, with Blueprints used for visual scripting and data-driven aspects.

## Project Architecture

- **`/Source`**: Contains all C++ source code.
  - **`/Source/SPARTA_STARTER/Public`**: Public header files (`.h`).
  - **`/Source/SPARTA_STARTER/Private`**: Private implementation files (`.cpp`).
- **`/Content`**: Contains all game assets, including Blueprints, Materials, Textures, and Maps.
- **`/Config`**: Contains configuration files (`.ini`) for the project, engine, and game settings.
- **`/Plugins`**: Contains all project-specific plugins.

## Code Standards

### C++

- Adhere to the official [Epic Games C++ Coding Standard](https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/CodingStandard/index.html).
- Class names must have the correct prefix:
  - `A`: Actor-derived classes (e.g., `AMyCharacter`).
  - `U`: UObject-derived classes (e.g., `UMyObject`).
  - `S`: SWidget-derived classes (e.g., `SMyWidget`).
  - `I`: Interface classes (e.g., `IMyInterface`).
  - `T`: Template classes (e.g., `TArray`).
  - `F`: Structs and other classes (e.g., `FMyStruct`).

### Blueprints

- **Naming Conventions**:
  - **`BP_`**: Actor Blueprints (e.g., `BP_PlayerCharacter`).
  - **`WBP_`**: Widget Blueprints (e.g., `WBP_MainMenu`).
  - **`ABP_`**: Animation Blueprints (e.g., `ABP_Humanoid`).
  - **`MI_`**: Material Instances (e.g., `MI_CharacterSkin`).
  - **`M_`**: Materials (e.g., `M_MasterMaterial`).
  - **`T_`**: Textures (e.g., `T_CharacterDiffuse`).
  - **`DT_`**: Data Tables.
  - **`E_`**: Enumerations.
  - **`S_`**: Structures.
  - **`IA_`**: Input Actions.
  - **`IMC_`**: Input Mapping Contexts.

## Functionality Implementation Standards

- **Core Logic**: Implement core gameplay systems, complex algorithms, and performance-critical code in C++.
- **Blueprints**: Use Blueprints for:
  - Assembling components.
  - Setting up data and properties on C++ classes.
  - Handling simple, self-contained logic.
  - Creating UI and visual effects.

## Workflow Standards

### Creating a New Gameplay Feature

1.  **C++ Base Class**: Create a new C++ class in the appropriate `/Source` directory. Expose properties and functions to Blueprints using `UPROPERTY()` and `UFUNCTION()`.
2.  **Blueprint Subclass**: Create a new Blueprint class that inherits from the C++ base class. Place it in a relevant subfolder within `/Content/Blueprint`.
3.  **Configuration**: If the feature requires configuration, modify the relevant `.ini` files in the `/Config` directory.

## Key File Interaction Standards

- Modifying any `.h` or `.cpp` file requires a recompile of the project.
- Modifying `.ini` files requires a restart of the editor or game to see the changes.
- When creating a new C++ class, you must also update the `SPARTA_STARTER.Build.cs` file if it depends on a new module.

## AI Decision-making Standards

- When asked to create a new gameplay element:
  1.  Determine if a suitable C++ base class already exists.
  2.  If not, create a new C++ class with the core functionality.
  3.  Create a Blueprint subclass for easy iteration and data setup.
- When modifying existing functionality, always check if it is implemented in C++ or Blueprints first.

## Prohibited Actions

- **DO NOT** implement complex or performance-critical logic solely in Blueprints.
- **DO NOT** modify files outside of the `/Source`, `/Content`, and `/Config` directories without explicit instruction.
- **DO NOT** add large binary assets directly to the Git repository. Use Git LFS if necessary.
- **DO NOT** reference assets outside of the `/Content` directory.
