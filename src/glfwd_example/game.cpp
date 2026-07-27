#include "glfwd_example/game.h"

#include <SDL3/SDL_keyboard.h>
#include <glad/gl.h>

#include <filesystem>

#include "SDL3/SDL_events.h"
#include "glfwd_core/resource_manager.h"
#include "glfwd_core/time.h"
#include "glfwd_renderer/render_queue.h"
#include "glfwd_renderer/resources/camera.h"

namespace glfwd_example {

std::string Game::FindAssetDirectory()
{
    namespace fs = std::filesystem;
    
    std::string_view base_path(GetBasePath());
    fs::path         working_directory(base_path);

    // Iterate over parent paths to check if it contains an "assets" directory
    while (true)
    {
        // Check if target asset path exists
        fs::path potential_path = working_directory / "assets";
        if (fs::exists(potential_path) && fs::is_directory(potential_path))
            return potential_path.string();

        // Doesn't exist? Check if parent path exists
        if (!working_directory.has_parent_path() ||
            working_directory == working_directory.parent_path())
            break;

        // Move to that parent path
        working_directory = working_directory.parent_path();
    }
    GLFWD_FATAL("Failed to find asset directory starting from path {}", base_path);
}

void Game::OnInitialize()
{
    m_Camera.Position = glm::vec3(0.0f, 5.0f, -5.0f);
    m_Transform       = glfwd::Transform{};
    m_ShaderHandle    = m_ResourceManager->QueryHandle<glfwd::Shader>("Basic Blinn-Phong");

    glfwd::TextureCreateInfo default_create_info{
        .MinFilter  = glfwd::TextureFilter::Linear,
        .MagFilter  = glfwd::TextureFilter::Linear,
        .Anisotropy = 16,
    };

    m_Mesh = glfwd::Mesh::GenerateCube(
        glfwd::BlinnPhongMaterial{
            .Diffuse = m_ResourceManager->PushResource(
                "Container Color",
                glfwd::Texture(m_ResourceManager->GetAssetPath("textures/container/color.png"),
                               default_create_info)),
            .Specular = m_ResourceManager->PushResource(
                "Container Specular",
                glfwd::Texture(m_ResourceManager->GetAssetPath("textures/container/specular.png"),
                               default_create_info)),
        },
        glm::vec3(0.0f, 0.0f, 0.0f));

    glfwd::Shader* shader = m_ResourceManager->QueryResource<glfwd::Shader>(m_ShaderHandle);
    glfwd::BlinnPhongMaterial::SetupShaderMaterial(shader, true);
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
}

void Game::OnLateUpdate(const glfwd::Timestep& timestep)
{
    const bool* keyboard = SDL_GetKeyboardState(nullptr);

    float speed = 15.0f;
    if (keyboard[SDL_SCANCODE_D])
        m_Camera.Position.x -= speed * static_cast<float>(timestep.GetDeltaTime());
    if (keyboard[SDL_SCANCODE_A])
        m_Camera.Position.x += speed * static_cast<float>(timestep.GetDeltaTime());

    if (keyboard[SDL_SCANCODE_W])
        m_Camera.Position.z += speed * static_cast<float>(timestep.GetDeltaTime());
    if (keyboard[SDL_SCANCODE_S])
        m_Camera.Position.z -= speed * static_cast<float>(timestep.GetDeltaTime());
}

void Game::SubmitToRenderQueue(glfwd::RenderQueue* render_queue)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // TODO: Move to Forward renderer
    glClearColor(
        m_Camera.ClearColor.r, m_Camera.ClearColor.g, m_Camera.ClearColor.b, m_Camera.ClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwd::Shader* shader = m_ResourceManager->QueryResource(m_ShaderHandle);
    shader->Bind();
    shader->PushConstant("u_Model", m_Transform.CreateModelMatrix());

    glfwd::RenderCamera render_camera =
        m_Camera.GetRenderCamera(m_Window->GetWidth(), m_Window->GetHeight());

    shader->PushConstant("u_Projection", render_camera.ProjectionMatrix);
    shader->PushConstant("u_View", render_camera.ViewMatrix);
    m_Mesh.GetMaterial().PushConstantsToShader(shader, m_ResourceManager);

    m_Mesh.Draw();
}

} // namespace glfwd_example
