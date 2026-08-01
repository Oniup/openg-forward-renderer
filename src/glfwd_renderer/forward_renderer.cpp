#include "glfwd_renderer/forward_renderer.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <glad/gl.h>

#include "glfwd_core/core_context.h"
#include "glfwd_core/resource_manager.h"
#include "glfwd_core/window.h"
#include "glfwd_renderer/rhi/context.h"
#include "glfwd_renderer/rhi/shader.h"
#include "glfwd_renderer/rhi/texture.h"

namespace glfwd {

ForwardRenderer::ForwardRenderer(const RendererOptions& info)
    : m_Context(new OpenGLContext(info)), // SDL and OpenGL Attributes
      m_Options(info)
{
}

ForwardRenderer::~ForwardRenderer()
{
    if (m_Context)
    {
        delete m_Context;
        m_Context = nullptr;
    }
}

void ForwardRenderer::InitializeBackend()
{
    m_Context->InitializeBackend();

    glEnable(GL_BLEND);
    if (m_Options.EnableMSAA)
        glEnable(GL_MULTISAMPLE);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW); // GL_CCW is default but I'll include it anyway
}

void ForwardRenderer::InitializeResources() const
{
    // Initialize pools
    CoreContext::GetResourceManager()->InitializePool<Shader>();
    CoreContext::GetResourceManager()->InitializePool<Texture>();
    CoreContext::GetResourceManager()->InitializePool<Model>();

    // Initialize default Shaders
    // ---------------------------------------------------------------------------------------------
    CoreContext::GetResourceManager()->PushResource<Shader>(
        "Flat Color",
        Shader(CoreContext::GetResourceManager()->GetAssetPath("shaders/base.vert"),
               CoreContext::GetResourceManager()->GetAssetPath("shaders/flat_color.frag")));

    CoreContext::GetResourceManager()->PushResource<Shader>(
        "Basic Blinn-Phong",
        Shader(CoreContext::GetResourceManager()->GetAssetPath("shaders/base.vert"),
               CoreContext::GetResourceManager()->GetAssetPath("shaders/base-blinn-phong.frag")));

    // Initialize default Textures
    // ---------------------------------------------------------------------------------------------
    ResourceHandle default_material_white =
        CoreContext::GetResourceManager()->PushResource<Texture>(
            "Default Texture",
            Texture(CoreContext::GetResourceManager()->GetAssetPath("textures/default.png"),
                    TextureCreateInfo{
                        // can afford terrible mipmaps as the texture is only white
                        .Filter = {.Mipmap = MipmapMode::Nearest},
                    }));

    intern::InitializeMaterialDefaultTextures(default_material_white);
}

const TextureFilterOptions& ForwardRenderer::GetDefaultTextureFilter() const
{
    return m_Options.DefaultTextureFilter;
}

void ForwardRenderer::SetDefaultTextureFilter(const TextureFilterOptions& filter)
{
    CoreContext::GetResourceManager()->ExecuteOverPool<Texture>(
        [&](Texture* texture) -> bool
        {
            if (texture->GetFilterOptions() == m_Options.DefaultTextureFilter)
            {
                texture->Bind();
                texture->SetFilterOptions(filter);
            }
            return ResourceManager::KeepItemInPool;
        });

    m_Options.DefaultTextureFilter = filter;
}

void ForwardRenderer::Render()
{
    std::vector<DrawOrder> light_draw_order; // TODO: Implement draw target first
    std::vector<DrawOrder> target_draw_order(m_RenderQueue.GetDrawTargets().size());
    SortDrawCommands(light_draw_order, target_draw_order);

    // Render frame
    RenderShadowMaps(light_draw_order);
    RenderGeometry(target_draw_order);

    // Swap buffers
    SDL_GL_SwapWindow(CoreContext::GetWindow()->GetInternalContext());
    m_RenderQueue.Clear();
}

void ForwardRenderer::SortDrawCommands(std::vector<DrawOrder>& light_draw_order,
                                       std::vector<DrawOrder>& target_draw_order) const
{
    // TODO: Update to use a worker thread model
    target_draw_order.reserve(m_RenderQueue.GetDrawTargets().size());

    for (size_t i = 0; i < m_RenderQueue.GetDrawTargets().size(); i++)
    {
        DrawOrder& draw_order = target_draw_order[i];
        SortRenderTargetDrawCommand(
            i, draw_order.OpaqueIndices, m_RenderQueue.GetOpaqueCommands(), false);
        SortRenderTargetDrawCommand(
            i, draw_order.TransparentIndices, m_RenderQueue.GetTransparentCommands(), true);
    }
}

void ForwardRenderer::SortRenderTargetDrawCommand(
    size_t draw_target_index, std::vector<size_t>& output_data,
    const std::vector<RenderQueue::DrawCommand>& draw_commands, bool reverse_sort) const
{
    if (draw_commands.empty())
        return;

    const RenderQueue::DrawTarget& target = m_RenderQueue.GetDrawTargets()[draw_target_index];
    output_data.resize(draw_commands.size());

    // Initialize index order in sequential order
    for (size_t i = 0; i < output_data.size(); i++)
        output_data[i] = i;

    // Sorts based on the distance between the camera and the object (front to back)
    auto CompareFn = [&target, &draw_commands, &reverse_sort](size_t current_index,
                                                              size_t compare_index) -> bool
    {
        const RenderQueue::DrawCommand& current = draw_commands[current_index];
        const RenderQueue::DrawCommand& compare = draw_commands[compare_index];

        glm::vec3 current_direction    = target.Camera.Position - current.GetPosition();
        glm::vec3 other_direction      = target.Camera.Position - compare.GetPosition();
        float     current_distance_sqr = glm::dot(current_direction, current_direction);
        float     other_distance_sqr   = glm::dot(other_direction, other_direction);

        if (reverse_sort)
            return current_distance_sqr > other_distance_sqr;
        return current_distance_sqr < other_distance_sqr;
    };

    std::sort(output_data.begin(), output_data.end(), CompareFn);
}

void ForwardRenderer::RenderShadowMaps(const std::vector<DrawOrder>& draw_orders) const
{
}

void ForwardRenderer::RenderGeometry(const std::vector<DrawOrder>& draw_orders) const
{
    glEnable(GL_DEPTH_TEST); // TODO: Read framebuffer if depth should be enabled
    glCullFace(m_Context->ConvertFaceModeToOpenGL(FaceMode::Back));

    for (size_t i = 0; i < m_RenderQueue.GetDrawTargets().size(); i++)
    {
        const RenderQueue::DrawTarget& target = m_RenderQueue.GetDrawTargets()[i];
        // TODO: Bind render target
        // TODO: Get Skybox

        // TODO: Remove window size in favour for framebuffer size
        glViewport(
            0, 0, CoreContext::GetWindow()->GetWidth(), CoreContext::GetWindow()->GetHeight());
        glClearColor(target.Camera.ClearColor.r,
                     target.Camera.ClearColor.g,
                     target.Camera.ClearColor.b,
                     1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // TODO: Base on render target

        RenderGeometryDrawPass(
            target, m_RenderQueue.GetOpaqueCommands(), draw_orders[i].OpaqueIndices);
        RenderGeometryDrawPass(
            target, m_RenderQueue.GetTransparentCommands(), draw_orders[i].TransparentIndices);
    }
}

void ForwardRenderer::RenderGeometryDrawPass(const RenderQueue::DrawTarget& draw_target,
                                             const std::vector<RenderQueue::DrawCommand>& commands,
                                             const std::vector<size_t>& draw_order) const
{
    if (commands.empty())
        return;

    const Shader*          shader        = nullptr;
    ResourceHandle<Shader> shader_handle = {};
    for (size_t index : draw_order)
    {
        const RenderQueue::DrawCommand& command = commands[index];

        // Bind shader if you haven't already
        if (shader_handle != command.ShaderHandle)
        {
            shader_handle = command.ShaderHandle;
            shader        = CoreContext::GetResourceManager()->QueryResource(command.ShaderHandle);
            shader->Bind();
            // Push one time, constant uniform values push here
            shader->PushConstant("u_ProjectionView",
                                 draw_target.Camera.ProjectionMatrix *
                                     draw_target.Camera.ViewMatrix);
        }

        // Push model push constants
        shader->PushConstant("u_TransposeInverseModel",
                             glm::transpose(glm::inverse(glm::mat3(command.ModelMatrix))));
        shader->PushConstant("u_Model", command.ModelMatrix);

        // Execute draw call
        const Model* model = CoreContext::GetResourceManager()->QueryResource(command.ModelHandle);
        model->Draw(shader);
    }
}

} // namespace glfwd
