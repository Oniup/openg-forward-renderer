#pragma once

#include "glfwd_renderer/rhi/texture.h"

namespace glfwd {

enum class SwapInterval
{
    DoubleBuffering = 0,  // VSync disabled
    VSync           = 1,  // VSync enabled, if it misses vertical retrace, it'll wait for the next
    AdaptiveVSync   = -1, // VSync enabled, if it misses vertical retrace, it'll swap immediately
};

struct RendererOptions
{
    SwapInterval         SwapIntervalMode     = SwapInterval::AdaptiveVSync;
    TextureFilterOptions DefaultTextureFilter = {
        .MinFilter  = TextureFilter::Linear,
        .MagFilter  = TextureFilter::Linear,
        .Mipmap     = MipmapMode::Linear,
        .Anisotropy = TextureFilterOptions::Anisotropy_Max,
    };
    bool EnableMSAA                      = true;
    bool EnableVSync                     = true;
    bool EnableHardwareDebugCallback     = true;
    bool EnableDepthForScreenFramebuffer = true; // TODO: default to false
};

} // namespace glfwd
