#include "glfwd_core/core_context.h"

namespace glfwd {

ResourceManager* CoreContext::m_ResourceManager = nullptr;
Window*          CoreContext::m_Window          = nullptr;

void CoreContext::ProvideResourceManager(ResourceManager* resource_manager)
{
    m_ResourceManager = resource_manager;
}

void CoreContext::ProvideWindow(Window* window)
{
    m_Window = window;
}

} // namespace glfwd