#include "glfwd_example/actors/camera_controller.h"

#include <SDL3/SDL_keyboard.h>

#include "glfwd_core/core_context.h"
#include "glfwd_core/time.h"

namespace glfwd_example {

void CameraController::OnInitialize()
{
    m_Camera.Position = glm::vec3(0.0f, 5.0f, -5.0f);
}

void CameraController::OnUpdate(const glfwd::Timestep& timestep)
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

void CameraController::SubmitToRenderQueue(glfwd::RenderQueue* render_queue) const
{
    render_queue->SubmitDrawTarget(
        m_Camera.GetRenderCamera(glfwd::CoreContext::GetWindow()->GetWidth(),
                                 glfwd::CoreContext::GetWindow()->GetHeight()),
        m_RenderTarget);
}

} // namespace glfwd_example