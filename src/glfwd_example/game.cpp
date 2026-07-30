#include "glfwd_example/game.h"

#include <SDL3/SDL_events.h>

#include "glfwd_core/resource_manager.h"
#include "glfwd_example/actors/camera_controller.h"
#include "glfwd_example/actors/cube.h"

namespace glfwd_example {

void Game::OnInitialize()
{
    m_Actors.push_back(std::make_unique<CameraController>(
        glfwd::ResourceHandle<glfwd::Framebuffer>{}, glm::vec3(5.0f, 5.0f, -5.0f)));
    m_Actors.push_back(std::make_unique<Cube>());

    for (std::unique_ptr<Actor>& actor : m_Actors)
    {
        actor->m_Owner = this;
        actor->OnInitialize();
    }
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

    for (std::unique_ptr<Actor>& actor : m_Actors)
        actor->OnEvent(event);
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
}

} // namespace glfwd_example
