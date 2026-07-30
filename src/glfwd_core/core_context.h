#pragma once
#include "resource_manager.h"
#include "window.h"

namespace glfwd {

struct CoreContext
{
    static void ProvideResourceManager(ResourceManager* resource_manager);
    static void ProvideWindow(Window* window);

    static ResourceManager* GetResourceManager() { return m_ResourceManager; }
    static Window*          GetWindow() { return m_Window; }

private:
    static ResourceManager* m_ResourceManager;
    static Window*          m_Window;
};

} // namespace glfwd