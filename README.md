<img src="./media/logo.png" alt="Dynamo Engine" width="500"/>

`Dynamo` is a 2D game engine written in C++ and 
powered by SDL2 and its extension libraries.

# Features

Aside from abstracting away low-level rendering, audio handling,
and resource management, the library offers additional game 
development utilities. The engine can also be used for general
multimedia application development

- Optimized Entity-Component-System (Data Oriented) and 
Actor (Object Oriented) based architectures for designing
game objects.
- A simplified Scene management system (menus, gameplay, etc.)
- GUI widgets
- Artificial Intelligence utilities like Finite State Machines
- Sprite animation and rasterizing primitives and TTF fonts
- Audio engine that allows multiple streaming tracks and raw PCM manipulation
- Network capabilities with custom protocol **(unstable, under development)**

# Dependencies

This engine wraps around SDL2 and its extensions:
- SDL2_image
- SDL2_ttf
- SDL2_net

# TODO
- Use smart pointers
- Implement custom network protocol
- Add support for dynamic 2D lighting
- Implement AI utilities for 2D pathfinding (A* with generic nodes)
- Refactor GUI framework
- Add documentation for usage (sample games, wiki)

# MAJOR UPDATE
- Migrate to using Vulkan API for low-level render calls
- Restructure core modules to interface with Vulkan