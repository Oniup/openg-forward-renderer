#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

#include <vector>

#include "glfwd_core/resource_handle.h"
#include "glfwd_renderer/resources/camera.h"
#include "glfwd_renderer/resources/model.h"
#include "glfwd_renderer/rhi/framebuffer.h"

namespace glfwd {

class ForwardRenderer;

class RenderQueue
{
public:
    struct DrawTarget
    {
        RenderCamera                Camera;
        ResourceHandle<Framebuffer> Target;
    };

    struct DrawCommand
    {
        ResourceHandle<Shader> ShaderHandle;
        glm::mat4              ModelMatrix   = glm::mat4(1.0f);
        ResourceHandle<Model>  ModelHandle   = ResourceHandle<Model>{};
        glm::vec3              ColorOverlay  = glm::vec3(1.0f);
        bool                   EnableShadows = true;

        glm::vec3 GetPosition() const;
    };

    void SubmitDrawTarget(const RenderCamera&                camera,
                          const ResourceHandle<Framebuffer>& render_target);
    void SubmitModel(const ResourceHandle<Shader>& shader_handle, const glm::mat4& model_matrix,
                     const ResourceHandle<Model>& model_handle, bool enable_shadows = true,
                     bool transparent = false, glm::vec3 overlay_color = glm::vec3(1.0f));

    void Clear();

    const std::vector<DrawTarget>&  GetDrawTargets() const { return m_DrawTargets; }
    const std::vector<DrawCommand>& GetOpaqueCommands() const { return m_OpaqueCommands; }
    const std::vector<DrawCommand>& GetTransparentCommands() const { return m_TransparentCommands; }

private:
    std::vector<DrawTarget>  m_DrawTargets;
    std::vector<DrawCommand> m_OpaqueCommands;
    std::vector<DrawCommand> m_TransparentCommands;
};

} // namespace glfwd
