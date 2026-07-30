#include "glfwd_renderer/render_queue.h"

#include <algorithm>

namespace glfwd {

glm::vec3 RenderQueue::DrawCommand::GetPosition() const
{
    return glm::vec3{
        ModelMatrix[3][0],
        ModelMatrix[3][1],
        ModelMatrix[3][2],
    };
}

void RenderQueue::SubmitDrawTarget(const RenderCamera&                camera,
                                   const ResourceHandle<Framebuffer>& render_target)
{
    m_DrawTargets.push_back(DrawTarget{
        .Camera = camera,
        .Target = render_target,
    });
}

void RenderQueue::SubmitModel(const ResourceHandle<Shader>& shader_handle,
                              const glm::mat4&              model_matrix,
                              const ResourceHandle<Model>& model_handle, bool enable_shadows,
                              bool transparent, glm::vec3 overlay_color)
{
    std::vector<DrawCommand>& commands = transparent ? m_TransparentCommands : m_OpaqueCommands;
    commands.push_back(DrawCommand{
        .ShaderHandle  = shader_handle,
        .ModelMatrix   = model_matrix,
        .ModelHandle   = model_handle,
        .ColorOverlay  = overlay_color,
        .EnableShadows = enable_shadows,
    });
}

void RenderQueue::Clear()
{
    m_DrawTargets.clear();
    m_OpaqueCommands.clear();
    m_TransparentCommands.clear();
}

} // namespace glfwd