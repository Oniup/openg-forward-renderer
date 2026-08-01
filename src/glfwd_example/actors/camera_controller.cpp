#include "glfwd_example/actors/camera_controller.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>

#include "glfwd_core/core_context.h"
#include "glfwd_core/time.h"
#include "glfwd_core/utility/error.h"
#include "glfwd_core/window.h"

namespace glfwd_example {

CameraController::CameraController(const glfwd::ResourceHandle<glfwd::Framebuffer>& render_target,
                                   const glm::vec3& position, const glm::vec3& look_at_target,
                                   float move_speed, float sensitivity)
    : m_RenderTarget(render_target),
      m_Sensitivity(sensitivity),
      m_MoveSpeed(move_speed)
{
    m_Camera.Position = position;
    m_Camera.Forward  = glm::normalize(look_at_target - position);

    // Correct angle when setting forward direction
    m_Pitch = std::asin(std::clamp(m_Camera.Forward.y, -1.0f, 1.0f));
    m_Yaw   = std::atan2(m_Camera.Forward.z, m_Camera.Forward.x);
}

void CameraController::OnEvent(const SDL_Event& event)
{
    // World state machine
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        switch (event.key.key)
        {
        case SDLK_ESCAPE:
            m_HideMouse = !m_HideMouse;
            SDL_SetWindowRelativeMouseMode(glfwd::CoreContext::GetWindow()->GetInternalContext(),
                                           m_HideMouse);

            // TODO: When implementing ImGui, must handle mouse configuration properly
            // ImGuiIO& io = ImGui::GetIO();
            // if (m_HideMouse)
            //     io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
            // else
            //     io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
            break;
        case SDLK_F1: GLFWD_INFO("Switching to wireframe mode is not implemented yet"); break;
        default:      break;
        }
    }

    if (m_HideMouse && event.type == SDL_EVENT_MOUSE_MOTION)
    {
        constexpr float max_rotation = glm::radians(89.9f);
        float           sensitivity  = m_Sensitivity * 0.0001f;

        m_Yaw   += event.motion.xrel * sensitivity;
        m_Pitch -= event.motion.yrel * sensitivity;
        m_Pitch  = std::clamp(m_Pitch, -max_rotation, max_rotation);

        glm::vec3 direction;
        direction.x = std::cos(m_Yaw) * std::cos(m_Pitch);
        direction.y = std::sin(m_Pitch);
        direction.z = std::sin(m_Yaw) * std::cos(m_Pitch);

        m_Camera.Forward = glm::normalize(direction);
    }
}

void CameraController::OnUpdate(const glfwd::Timestep& timestep)
{
    if (m_HideMouse)
    {
        const bool* keyboard       = SDL_GetKeyboardState(nullptr);
        auto        move_direction = glm::vec3(0.0f);

        // WASD Controls

        if (keyboard[SDL_SCANCODE_A])
            move_direction -= glm::cross(m_Camera.Forward, m_Camera.Up);
        if (keyboard[SDL_SCANCODE_D])
            move_direction += glm::cross(m_Camera.Forward, m_Camera.Up);

        if (keyboard[SDL_SCANCODE_W])
            move_direction += m_Camera.Forward;
        if (keyboard[SDL_SCANCODE_S])
            move_direction -= m_Camera.Forward;

        float move_speed = m_MoveSpeed;
        if (keyboard[SDL_SCANCODE_LSHIFT])
            move_speed *= 2.0f;

        float length_sqr = glm::dot(move_direction, move_direction);
        if (length_sqr != 0.0f)
        {
            m_Camera.Position += move_speed * (move_direction / std::sqrt(length_sqr)) *
                                 static_cast<float>(timestep.GetDeltaTime());
        }

        // UP and DOWN

        float up_direction = 0.0f;
        if (keyboard[SDL_SCANCODE_E] || keyboard[SDL_SCANCODE_SPACE])
            up_direction = 1.0f;
        if (keyboard[SDL_SCANCODE_Q] || keyboard[SDL_SCANCODE_LCTRL])
            up_direction = -1.0f;

        if (up_direction != 0.0f)
        {
            m_Camera.Position.y +=
                move_speed * up_direction * static_cast<float>(timestep.GetDeltaTime());
        }
    }
}

void CameraController::SubmitToRenderQueue(glfwd::RenderQueue* render_queue) const
{
    render_queue->SubmitDrawTarget(
        m_Camera.GetRenderCamera(glfwd::CoreContext::GetWindow()->GetWidth(),
                                 glfwd::CoreContext::GetWindow()->GetHeight()),
        m_RenderTarget);
}

} // namespace glfwd_example