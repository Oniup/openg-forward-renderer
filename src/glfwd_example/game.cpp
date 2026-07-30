#include "glfwd_example/game.h"

#include <SDL3/SDL_events.h>

#include "glfwd_core/resource_manager.h"
#include "glfwd_example/actors/camera_controller.h"
#include "glfwd_example/actors/cube.h"

namespace glfwd_example {

void Game::OnInitialize()
{
    m_Actors.push_back(std::make_unique<CameraController>());
    m_Actors.push_back(std::make_unique<Cube>());

    for (std::unique_ptr<Actor>& actor : m_Actors)
    {
        actor->m_Owner = this;
        actor->OnInitialize();
    }

    // m_Camera.Position = glm::vec3(0.0f, 5.0f, -5.0f);
    // m_Transform       = glfwd::Transform{};

    // glfwd::TextureCreateInfo default_create_info{
    //     .MinFilter  = glfwd::TextureFilter::Linear,
    //     .MagFilter  = glfwd::TextureFilter::Linear,
    //     .Anisotropy = 16,
    // };

    // m_Mesh = glfwd::Mesh::GenerateCube(
    //     glfwd::BlinnPhongMaterial{
    //         .Diffuse = m_ResourceManager->PushResource(
    //             "Container Color",
    //             glfwd::Texture(m_ResourceManager->GetAssetPath("textures/container/color.png"),
    //                            default_create_info)),
    //         .Specular = m_ResourceManager->PushResource(
    //             "Container Specular",
    //             glfwd::Texture(m_ResourceManager->GetAssetPath("textures/container/specular.png"),
    //                            default_create_info)),
    //     },
    //     glm::vec3(0.0f, 0.0f, 0.0f));

    // // Query handle based on name
    // glfwd::ResourceHandle<glfwd::Shader> shader_handle =
    //     m_ResourceManager->QueryHandle<glfwd::Shader>("Basic Blinn-Phong");
    // // Query actual resource when needed at runtime (the pointer is temporary)
    // glfwd::Shader* shader = m_ResourceManager->QueryResource<glfwd::Shader>(shader_handle);
    // // Usage example
    // glfwd::BlinnPhongMaterial::SetupShaderMaterial(shader);
}

void Game::OnEvent(const SDL_Event& event)
{
#ifdef GLFWD_SHADER_HOT_RELOAD
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if ((event.key.mod & (SDL_KMOD_CTRL | SDL_KMOD_SHIFT)) && event.key.key == SDLK_R)
        {
            m_ResourceManager->ExecuteOverPool<glfwd::Shader>(
                [](glfwd::Shader* shader) -> bool
                {
                    shader->ReloadShader();
                    return true;
                });
        }
    }
#endif
}

void Game::OnUpdate(const glfwd::Timestep& timestep)
{
    for (std::unique_ptr<Actor>& actor : m_Actors)
        actor->OnUpdate(timestep);
}

void Game::SubmitToRenderQueue(glfwd::RenderQueue* render_queue)
{
    for (std::unique_ptr<Actor>& actor : m_Actors)
        actor->SubmitToRenderQueue(render_queue);

    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);

    // TODO: Move to Forward renderer
    // glClearColor(
    //     m_Camera.ClearColor.r, m_Camera.ClearColor.g, m_Camera.ClearColor.b,
    //     m_Camera.ClearColor.a);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glfwd::Shader* shader = m_ResourceManager->QueryResource(m_ShaderHandle);
    // shader->Bind();
    // shader->PushConstant("u_Model", m_Transform.CreateModelMatrix());

    // glfwd::RenderCamera render_camera =
    //     m_Camera.GetRenderCamera(m_Window->GetWidth(), m_Window->GetHeight());

    // shader->PushConstant("u_Projection", render_camera.ProjectionMatrix);
    // shader->PushConstant("u_View", render_camera.ViewMatrix);
    // m_Mesh.GetMaterial().PushConstantsToShader(shader, m_ResourceManager);

    // m_Mesh.Draw();
}

} // namespace glfwd_example
