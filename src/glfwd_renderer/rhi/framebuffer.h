#pragma once

#include <cstdint>
#include <variant>

#include "glfwd_core/resource_handle.h"
#include "glfwd_renderer/rhi/texture.h"

namespace glfwd {

enum class RenderBufferType
{
    Depth24,
    Depth24Stencil8,
    Depth32F,
};

class RenderBuffer
{
public:
    RenderBuffer() = default;
    RenderBuffer(RenderBufferType type, uint32_t width, uint32_t height, uint32_t sample_count = 1);
    ~RenderBuffer();

    RenderBuffer(RenderBuffer&& other);
    RenderBuffer& operator=(RenderBuffer&& other);
    RenderBuffer(const RenderBuffer& other)            = delete;
    RenderBuffer& operator=(const RenderBuffer& other) = delete;

    bool IsValid() const;
    void Bind() const;

private:
    RenderBufferType m_Type        = RenderBufferType::Depth24;
    uint32_t         m_ID          = 0;
    uint32_t         m_Width       = 0;
    uint32_t         m_Height      = 0;
    uint32_t         m_SampleCount = 1;
};

class Framebuffer
{
public:
    using Attachment = std::variant<ResourceHandle<RenderBuffer>, ResourceHandle<Texture>>;

    Framebuffer() = default;
    ~Framebuffer();

    Framebuffer(Framebuffer&& other);
    Framebuffer& operator=(Framebuffer&& other);
    Framebuffer(const Framebuffer& other)            = delete;
    Framebuffer& operator=(const Framebuffer& other) = delete;

    bool     IsValid() const;
    uint32_t GetWidth() const { return m_CachedWidth; }
    uint32_t GetHeight() const { return m_CachedHeight; }

private:
    uint32_t                m_ID                = 0;
    std::vector<Attachment> m_ColourAttachments = {};
    Attachment              m_DepthAttachment   = {};
    uint32_t                m_CachedWidth       = 0;
    uint32_t                m_CachedHeight      = 0;
};

} // namespace glfwd