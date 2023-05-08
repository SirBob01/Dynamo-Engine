#include <Dynamo.hpp>

int main() {
    Dynamo::ApplicationConfiguration config;
    config.title = "Renderer!";
    config.width = 640;
    config.height = 480;
    config.asset_directory = "../assets/";

    Dynamo::Application app(config);
    const Dynamo::Core &core = app.get_core();
    Dynamo::Graphics::Vulkan::Renderer &renderer = app.get_renderer();
    {
        Dynamo::Asset<Dynamo::Graphics::Texture> texture =
            app.get_assets().get<Dynamo::Graphics::Texture>(
                "./textures/ball.png");
        {
            Dynamo::Asset<Dynamo::Graphics::Texture> texture =
                app.get_assets().get<Dynamo::Graphics::Texture>(
                    "./textures/ball.png");
        }
    }
    Dynamo::Asset<Dynamo::Graphics::Texture> texture =
        app.get_assets().get<Dynamo::Graphics::Texture>("./textures/ball.png");
    Dynamo::Log::info("Texture size: {}x{}", texture->size.x, texture->size.y);

    // std::vector<Dynamo::Mat4> transforms = {Dynamo::Mat4()};
    // Dynamo::Asset<Dynamo::Graphics::Mesh> mesh =
    //     renderer.get_mesh_assets().get("/meshes/viking_room.obj");
    // Dynamo::Asset<Dynamo::Graphics::Texture> texture =
    //     renderer.get_texture_assets().get("/textures/viking_room.png");
    // std::unique_ptr<Dynamo::Graphics::MeshInstance> instance =
    //     renderer.upload_mesh(*mesh);
    // Dynamo::Asset<Dynamo::Graphics::Shader> vert_shader =
    //     renderer.get_shader_assets().get("/shaders/default.vert
    //     copy.spv");
    // Dynamo::Asset<Dynamo::Graphics::Shader> frag_shader =
    //     renderer.get_shader_assets().get("/shaders/default.frag
    //     copy.spv");

    // Dynamo::Graphics::ShaderList shaders = {*vert_shader,
    // *frag_shader}; std::unique_ptr<Dynamo::Graphics::Material>
    // material =
    //     renderer.get_material_system().build(
    //         shaders,
    //         Dynamo::Graphics::PolygonMode::Fill);

    while (app.is_running()) {
        if (core.input.is_pressed(Dynamo::KeyCode::Q)) {
            renderer.clear(Dynamo::Color(1, 0, 0));
        }
        if (core.input.is_pressed(Dynamo::KeyCode::W)) {
            renderer.clear(Dynamo::Color(0, 1, 0));
        }
        if (core.input.is_pressed(Dynamo::KeyCode::E)) {
            renderer.clear(Dynamo::Color(0, 0, 1));
        }
        if (core.input.is_pressed(Dynamo::KeyCode::S)) {
            renderer.clear(Dynamo::Color(0, 0, 0));
        }
        if (core.input.is_pressed(Dynamo::KeyCode::D)) {
            renderer.clear(Dynamo::Color(1, 1, 1));
        }
        // renderer.draw(*instance, *material, transforms, 0);

        // Run the main loop
        app.run();
    }
    return 0;
}
