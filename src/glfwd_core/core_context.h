#pragma once

namespace glfwd {

class ResourceManager;
class Window;
class ForwardRenderer;

struct CoreContext
{
    static void ProvideResourceManager(ResourceManager* resource_manager);
    static void ProvideWindow(Window* window);
    static void ProvideForwardRenderer(ForwardRenderer* renderer);

    static ResourceManager* GetResourceManager() { return m_ResourceManager; }
    static Window*          GetWindow() { return m_Window; }
    static ForwardRenderer* GetRenderer() { return m_Renderer; }

private:
    static ResourceManager* m_ResourceManager;
    static Window*          m_Window;
    static ForwardRenderer* m_Renderer;
};

} // namespace glfwd