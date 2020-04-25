<img src="./media/logo.png" alt="Dynamo Engine" width="500"/>

`Dynamo` is a 2D game engine written in C++ and powered by SDL2 and its extension libraries.

---

This engine is meant to simplify the application development process, automating core processes like the main loop and state handling. Aside from abstracting away low-level resource management, the library offers additional game development utilities. Below is an overview of its features:

- Support for both [OOP](https://en.wikipedia.org/wiki/Object-oriented_design) and [ECS](https://en.wikipedia.org/wiki/Entity_component_system) architectures in designing game objects
- A simplified Scene management system
- Artificial intelligence utilities including [FSM](https://en.wikipedia.org/wiki/Finite-state_machine)
- Animating sprites, rasterizing primitives, and rendering text
- An audio engine that supports multiple streaming tracks and raw PCM data manipulation
- And more!

These features allow it to be used as more than a just game engine, but as a framework for general multimedia applications.

For a more in-depth look at the engine's components, visit the [__wiki__](https://github.com/SirBob01/Dynamo-Engine/wiki).

# Dependencies

This engine wraps around SDL2 and its extensions:
- SDL2_image
- SDL2_ttf
- SDL2_net
- Ogg Vorbis

# TODO
- Use smart pointers
- Finish implementing custom network protocol
- Add support for dynamic 2D lighting
- Implement 2D pathfinding utility (A* with generic nodes)
- Refactor GUI framework
- Migrate to using Vulkan API for low-level render calls
- Track down memory leaks