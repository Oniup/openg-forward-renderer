#pragma once

#include "glfwd_renderer/create_info.h"
#include "glfwd_renderer/render_queue.h"

typedef struct SDL_GLContextState* SDL_GLContext;

namespace glfwd {

class Window;
class ResourceManager;
class OpenGLContext;

class ForwardRenderer
{
    struct DrawOrder
    {
        std::vector<size_t> OpaqueIndices;
        std::vector<size_t> TransparentIndices;
    };

public:
    ForwardRenderer(const RendererCreateInfo& info);
    ~ForwardRenderer();

    ForwardRenderer(const ForwardRenderer& other)            = delete;
    ForwardRenderer& operator=(const ForwardRenderer& other) = delete;

    void InitializeBackend();
    void InitializeResources() const;

    RenderQueue*       GetRenderQueue() { return &m_RenderQueue; }
    const RenderQueue* GetRenderQueue() const { return &m_RenderQueue; }

    void Render();

private:
    void SortDrawCommands(std::vector<DrawOrder>& light_draw_order,
                          std::vector<DrawOrder>& target_draw_order) const;
    void SortRenderTargetDrawCommand(size_t draw_target_index, std::vector<size_t>& output_data,
                                     const std::vector<RenderQueue::DrawCommand>& draw_commands,
                                     bool reverse_sort) const;

    void RenderShadowMaps(const std::vector<DrawOrder>& draw_orders) const;
    void RenderGeometry(const std::vector<DrawOrder>& draw_orders) const;
    void RenderGeometryDrawPass(const RenderQueue::DrawTarget&               draw_target,
                                const std::vector<RenderQueue::DrawCommand>& commands,
                                const std::vector<size_t>&                   draw_order) const;

    RenderQueue        m_RenderQueue;
    OpenGLContext*     m_Context = nullptr;
    RendererCreateInfo m_Options;
};

} // namespace glfwd
