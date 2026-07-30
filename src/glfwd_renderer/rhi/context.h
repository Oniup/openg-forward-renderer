#pragma once

#include <cstdint>

#include "glfwd_renderer/create_info.h"

typedef struct SDL_GLContextState* SDL_GLContext;

namespace glfwd {

class Window;

enum class PrimitiveMode;
enum class PolygonMode;
enum class FaceMode;
enum class TextureType;
enum class TextureWrap;
enum class TextureFormat;
enum class TextureFilter;
enum class MipmapMode;
enum class RenderBufferType;

class OpenGLContext
{
public:
    // Draw mode
    static int32_t ConvertPrimitiveModeToOpenGL(PrimitiveMode mode);
    static int32_t ConvertPolygonModeToOpenGL(PolygonMode mode);
    static int32_t ConvertFaceModeToOpenGL(FaceMode mode);

    // Texture
    static uint32_t ConvertTextureTypeToOpenGL(TextureType type);
    static int32_t  ConvertTextureWrapToOpenGL(TextureWrap wrap);
    static void     ConvertTextureFormatToOpenGL(TextureFormat format, uint32_t& base_format,
                                                 uint32_t& output_format);
    static void ConvertTextureFiltersToOpenGL(TextureFilter min_filter, TextureFilter mag_filter,
                                              MipmapMode mipmap, int32_t& min_out,
                                              int32_t& mag_out);

    static uint32_t ConvertRenderBufferTypeToOpenGL(RenderBufferType type);

    static int32_t GetMaxMSAASamples();
    static int32_t GetMaxAnisotropy();

    OpenGLContext(const RendererCreateInfo& info);
    ~OpenGLContext();

    void InitializeBackend();
    void Shutdown();
    void SetSwapIntervalMode(SwapInterval mode);

    SDL_GLContext GetInternalContext() const { return m_InternalContext; }

private:
    static OpenGLContext* s_Instance;

    SwapInterval  m_SwapIntervalMode;
    SDL_GLContext m_InternalContext = nullptr;
    int32_t       m_MaxMSAASamples  = 0;
    int32_t       m_MaxAnisotropy   = 0;
};

} // namespace glfwd
