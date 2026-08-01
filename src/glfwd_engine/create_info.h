#pragma once

#include "glfwd_core/utility/error.h"
#include "glfwd_core/window.h"
#include "glfwd_renderer/renderer_options.h"

namespace glfwd {

struct ApplicationCreateInfo
{
    RendererOptions  Renderer;       // Renderer settings
    WindowCreateInfo Window;         // Window settings
    std::string      AssetDirectory; // Leave empty to find assets/ in parents directories
    std::string      InitialMapFilePath     = "maps/test_level.map"; // TrenchBroom map level data
    ErrorSeverity ErrorLogMinSeverityFilter = ErrorSeverity::Low; // Leave to show all log messages
};

} // namespace glfwd
