#include "glfwd_core/core_context.h"

namespace glfwd {

ResourceManager* CoreContext::m_ResourceManager = nullptr;
Window*          CoreContext::m_Window          = nullptr;
ForwardRenderer* CoreContext::m_Renderer        = nullptr;

void CoreContext::ProvideResourceManager(ResourceManager* resource_manager)
{
    m_ResourceManager = resource_manager;
}

void CoreContext::ProvideWindow(Window* window)
{
    m_Window = window;
}

void CoreContext::ProvideForwardRenderer(ForwardRenderer* renderer)
{
    m_Renderer = renderer;
}

} // namespace glfwd