#include "glfwd_renderer/rhi/context.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <glad/gl.h>

#include "glfwd_core/core_context.h"
#include "glfwd_core/utility/error.h"
#include "glfwd_core/window.h"
#include "glfwd_renderer/create_info.h"
#include "glfwd_renderer/draw_mode.h"
#include "glfwd_renderer/rhi/framebuffer.h"
#include "glfwd_renderer/rhi/texture.h"

namespace glfwd {

OpenGLContext* OpenGLContext::s_Instance = nullptr;

#define EFMT "OpenGL ID: {}, Source: {}, Type: {}: {}"
void GLAPIENTRY glDebugOutputCallback(GLenum source, GLenum type, unsigned int id, GLenum severity,
                                      GLsizei length, const char* message, const void* userParam)
{
    // Ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::string_view source_str;
    std::string_view type_str;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             source_str = "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   source_str = "Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: source_str = "Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     source_str = "Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     source_str = "Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           source_str = "Other"; break;
    default:                              source_str = "Unknown"; break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               type_str = "Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  type_str = "Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         type_str = "Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         type_str = "Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              type_str = "Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          type_str = "Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           type_str = "Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               type_str = "Other"; break;
    default:                                type_str = "Unknown"; break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         GLFWD_FATAL(EFMT, id, source_str, type_str, message); 
    case GL_DEBUG_SEVERITY_MEDIUM:       GLFWD_ERROR(EFMT, id, source_str, type_str, message); break;
    case GL_DEBUG_SEVERITY_LOW:          GLFWD_WARN(EFMT, id, source_str, type_str, message); break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: GLFWD_INFO(EFMT, id, source_str, type_str, message); break;
    default:                             GLFWD_WARN(EFMT, id, source_str, type_str, message); break;
    }
}

int32_t OpenGLContext::ConvertPrimitiveModeToOpenGL(PrimitiveMode mode)
{
    switch (mode)
    {
    case PrimitiveMode::Points:        return GL_POINTS;
    case PrimitiveMode::Lines:         return GL_LINE;
    case PrimitiveMode::LineStrip:     return GL_LINE_STRIP;
    case PrimitiveMode::LineLoop:      return GL_LINE_LOOP;
    case PrimitiveMode::Triangles:     return GL_TRIANGLES;
    case PrimitiveMode::TriangleStrip: return GL_TRIANGLE_STRIP;
    case PrimitiveMode::TriangleFan:   return GL_TRIANGLE_FAN;
    case PrimitiveMode::Patches:       return GL_PATCHES;
    }
    return -1;
}

int32_t OpenGLContext::ConvertPolygonModeToOpenGL(PolygonMode mode)
{
    switch (mode)
    {
    case PolygonMode::Lines:  return GL_LINE;
    case PolygonMode::Points: return GL_POINT;
    case PolygonMode::Fill:   return GL_FILL;
    }
    return -1;
}

int32_t OpenGLContext::ConvertFaceModeToOpenGL(FaceMode mode)
{
    switch (mode)
    {
    case FaceMode::Front:        return GL_FRONT;
    case FaceMode::Back:         return GL_BACK;
    case FaceMode::FrontAndBack: return GL_FRONT_AND_BACK;
    }
    return -1;
}

uint32_t OpenGLContext::ConvertTextureTypeToOpenGL(TextureType type)
{
    switch (type)
    {
    case TextureType::Tex2D:   return GL_TEXTURE_2D;
    case TextureType::Tex3D:   return GL_TEXTURE_3D;
    case TextureType::CubeMap: return GL_TEXTURE_CUBE_MAP;
    }
    return -1;
}

int32_t OpenGLContext::ConvertTextureWrapToOpenGL(TextureWrap wrap)
{
    switch (wrap)
    {
    case TextureWrap::Repeat:         return GL_REPEAT;
    case TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
    case TextureWrap::ClampToEdge:    return GL_CLAMP_TO_EDGE;
    case TextureWrap::ClampToBorder:  return GL_CLAMP_TO_BORDER;
    }
    return -1;
}

void OpenGLContext::ConvertTextureFormatToOpenGL(TextureFormat format, uint32_t& base_format,
                                                 uint32_t& output_format)
{
    switch (format)
    {
    case TextureFormat::RGB:     base_format = output_format = GL_RGB; break;
    case TextureFormat::RGB8:    base_format = output_format = GL_RGB8; break;
    case TextureFormat::RGBA:    base_format = output_format = GL_RGBA; break;
    case TextureFormat::RGBA8:   base_format = output_format = GL_RGBA8; break;
    case TextureFormat::Depth24: base_format = output_format = GL_DEPTH_COMPONENT; break;
    case TextureFormat::Depth32f:
        base_format   = GL_DEPTH_COMPONENT32F;
        output_format = GL_DEPTH_COMPONENT;
        break;
    case TextureFormat::Depth24Stencil8: base_format = output_format = GL_DEPTH24_STENCIL8; break;
    }
}

void OpenGLContext::ConvertTextureFiltersToOpenGL(TextureFilter min_filter,
                                                  TextureFilter mag_filter, MipmapMode mipmap,
                                                  int32_t& min_out, int32_t& mag_out)
{
    mag_out = mag_filter == TextureFilter::Linear ? GL_LINEAR : GL_NEAREST;

    if (mipmap == MipmapMode::None)
        min_out = min_filter == TextureFilter::Linear ? GL_LINEAR : GL_NEAREST;
    else
    {
        switch (min_filter)
        {
        case TextureFilter::Nearest:
            min_out =
                mipmap == MipmapMode::Linear ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST;
            break;
        case TextureFilter::Linear:
            min_out =
                mipmap == MipmapMode::Linear ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_NEAREST;
            break;
        }
    }
}

uint32_t OpenGLContext::ConvertRenderBufferTypeToOpenGL(RenderBufferType type)
{
    switch (type)
    {
    case RenderBufferType::Depth24:         return GL_DEPTH_COMPONENT;
    case RenderBufferType::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
    case RenderBufferType::Depth32F:        return GL_DEPTH_COMPONENT32F;
    }
    return -1;
}

int32_t OpenGLContext::GetMaxMSAASamples()
{
    return s_Instance->m_MaxMSAASamples;
}

int32_t OpenGLContext::GetMaxAnisotropy()
{
    return s_Instance->m_MaxAnisotropy;
}

OpenGLContext::OpenGLContext(const RendererCreateInfo& info)
    : m_SwapIntervalMode(info.SwapIntervalMode)
{
    GLFWD_ASSERT(!s_Instance,
                 "RHI/OpenGL Context has already been initialized, cannot have 2 instances");

    if (!SDL_Init(SDL_INIT_VIDEO))
        GLFWD_FATAL("Failed to initialize SDL: {}", SDL_GetError());

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, true); // Force hardware acceleration

    if (info.EnableDepthForScreenFramebuffer)
    {
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    }
    else
    {
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    }

    if (info.EnableHardwareDebugCallback)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
}

OpenGLContext::~OpenGLContext()
{
    Shutdown();
}

void OpenGLContext::InitializeBackend()
{
    m_InternalContext = SDL_GL_CreateContext(CoreContext::GetWindow()->GetInternalContext());
    if (!m_InternalContext)
        GLFWD_FATAL("Failed to initialize OpenGL context: {}", SDL_GetError());

    // Initialize glad
    int glad_version = gladLoadGL(static_cast<GLADloadfunc>(SDL_GL_GetProcAddress));
    if (glad_version == 0)
    {
        Shutdown();
        GLFWD_FATAL("Failed to initialize OpenGL pointer functions through glad loader");
    }
    GLFWD_INFO("Initialized OpenGL {}.{}",
               GLAD_VERSION_MAJOR(glad_version),
               GLAD_VERSION_MINOR(glad_version));

    // Debug callback setup
    int32_t flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutputCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    glGetIntegerv(GL_MAX_SAMPLES, &m_MaxMSAASamples);
    glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &m_MaxAnisotropy);

    SDL_GL_SetSwapInterval(static_cast<int32_t>(m_SwapIntervalMode));
}

void OpenGLContext::Shutdown()
{
    if (m_InternalContext)
    {
        SDL_GL_DestroyContext(m_InternalContext);
        SDL_Quit();

        m_InternalContext = nullptr;
        s_Instance        = nullptr;
    }
}

void OpenGLContext::SetSwapIntervalMode(SwapInterval mode)
{
    m_SwapIntervalMode = mode;
    SDL_GL_SetSwapInterval(static_cast<int32_t>(mode));
}

} // namespace glfwd
