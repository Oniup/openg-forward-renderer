#pragma once

namespace glfwd {

enum class SwapInterval
{
    DoubleBuffering = 0,  // VSync Disabled
    VSync           = 1,  // VSync Enabled, if misses vertical retrace, it'll wait for the next
    AdaptiveVSync   = -1, // VSync Enabled, if misses vertical retrace, it'll swap immediately
};

struct RendererCreateInfo
{
    SwapInterval SwapIntervalMode                = SwapInterval::AdaptiveVSync;
    bool         EnableMSAA                      = true;
    bool         EnableVSync                     = true;
    bool         EnableHardwareDebugCallback     = true;
    bool         EnableDepthForScreenFramebuffer = true; // TODO: default to false
};

} // namespace glfwd
