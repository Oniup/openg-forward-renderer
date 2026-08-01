#pragma once

#include <glm/ext/vector_float4.hpp>

#include <array>
#include <cstdint>
#include <string_view>

struct SDL_Surface;

namespace glfwd {

enum class TextureType
{
    Tex2D,
    Tex3D,
    CubeMap,
};

constexpr std::array<std::string_view, 3> TextureTypeNames = {
    "Texture 2D",
    "Texture 3D",
    "Cube Map",
};

enum class TextureWrap
{
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder,
};

enum class TextureFormat
{
    RGB,
    RGB8,
    RGBA,
    RGBA8,
    Depth24,
    Depth24Stencil8,
    Depth32f,
};

enum class TextureFilter
{
    Default,
    Nearest,
    Linear,
};

enum class MipmapMode
{
    Default,
    None,
    Nearest,
    Linear,
};

struct TextureFilterOptions
{
    enum AnisotropyConstants : uint32_t
    {
        Anisotropy_Disabled,
        Anisotropy_Max,
        Anisotropy_Default = std::numeric_limits<uint32_t>::max(),
    };

    TextureFilter MinFilter  = TextureFilter::Default;
    TextureFilter MagFilter  = TextureFilter::Default;
    MipmapMode    Mipmap     = MipmapMode::Default;
    uint32_t      Anisotropy = Anisotropy_Default;

    bool operator==(const TextureFilterOptions& other) const;
    bool operator!=(const TextureFilterOptions& other) const;

    void AssignDefaultValues();
};

struct TextureCreateInfo
{
    uint32_t Width  = 0; // Must define if creating an empty texture
    uint32_t Height = 0; // Must define if creating an empty texture
    uint32_t Depth  = 0;

    TextureType   Type   = TextureType::Tex2D;
    TextureFormat Format = TextureFormat::RGBA;
    TextureWrap   Wrap   = TextureWrap::Repeat;

    TextureFilterOptions Filter;

    glm::vec4 BorderColor         = glm::vec4(0.0f);
    bool      ApplySRGBCorrection = true;
    bool      FlipVertically      = true;
    bool      FlipHorizontally    = false;
    uint32_t  Sample              = 1; // 1 => Disables, 0 => Max
};

class Texture
{
public:
    Texture() = default;
    Texture(const TextureCreateInfo& info);
    Texture(const uint8_t* buffer, size_t buffer_size, const TextureCreateInfo& info);
    Texture(std::string_view path, const TextureCreateInfo& info = {});
    ~Texture();

    Texture(Texture&& other);
    Texture& operator=(Texture&& other);
    Texture(const Texture& other)            = delete;
    Texture& operator=(const Texture& other) = delete;

    void Bind() const;
    bool IsValid() const;

    const TextureFilterOptions& GetFilterOptions() const { return m_FilterOptions; }
    void                        SetFilterOptions(TextureFilterOptions filter);

    TextureType GetType() const { return m_Type; }
    uint32_t    GetWidth() const { return m_Width; }
    uint32_t    GetHeight() const { return m_Height; }

private:
    void LoadFromSurface(SDL_Surface* image_surface, const TextureCreateInfo& info);
    bool LoadOpenGLTexture(const SDL_Surface* surface, const TextureCreateInfo& info);
    bool LoadOpenGLCubeMapFromSurface(const SDL_Surface* surface, const TextureCreateInfo& info,
                                      int32_t base_format, int32_t output_format) const;

    void SetOpenGLFilterOptions();
    bool SetSizeFromSurface(const SDL_Surface* surface, const TextureCreateInfo& info);

    uint32_t             m_ID = 0;
    TextureType          m_Type;
    uint32_t             m_Width  = 0;
    uint32_t             m_Height = 0;
    uint32_t             m_Depth  = 0;
    uint32_t             m_Sample = 1;
    TextureFilterOptions m_FilterOptions;
};

} // namespace glfwd
