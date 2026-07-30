#include "glfwd_renderer/rhi/framebuffer.h"

#include <glad/gl.h>

#include "context.h"
#include "glfwd_core/utility/error.h"

namespace glfwd {

RenderBuffer::RenderBuffer(RenderBufferType type, uint32_t width, uint32_t height,
                           uint32_t sample_count)
    : m_Type(type),
      m_Width(width),
      m_Height(height),
      m_SampleCount(sample_count)
{
    if (m_Width == 0 || m_Height == 0)
    {
        GLFWD_ERROR("Failed to create render buffer as invalid width {} and height {} where given",
                    m_Width,
                    m_Height);
        return;
    }

    glGenRenderbuffers(1, &m_ID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_ID);

    uint32_t format = OpenGLContext::ConvertRenderBufferTypeToOpenGL(m_Type);
    if (m_SampleCount > 1)
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, sample_count, format, m_Width, m_Height);
    else
        glRenderbufferStorage(GL_RENDERBUFFER, format, m_Width, m_Height);
}

RenderBuffer::~RenderBuffer()
{
    if (m_ID != 0)
    {
        glDeleteRenderbuffers(1, &m_ID);
        m_ID = 0;
    }
}

RenderBuffer::RenderBuffer(RenderBuffer&& other)
{
}

RenderBuffer& RenderBuffer::operator=(RenderBuffer&& other)
{
    return *this;
}

bool RenderBuffer::IsValid() const
{
    return false;
}

void RenderBuffer::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
}

Framebuffer::~Framebuffer()
{
    if (m_ID != 0)
    {
        glDeleteFramebuffers(1, &m_ID);
        m_ID = 0;
    }
}

Framebuffer::Framebuffer(Framebuffer&& other)
    : m_ID(other.m_ID),
      m_ColourAttachments(std::move(other.m_ColourAttachments)),
      m_DepthAttachment(std::move(other.m_DepthAttachment)),
      m_CachedWidth(other.m_CachedWidth),
      m_CachedHeight(other.m_CachedHeight)
{
    other.m_ID = 0;
}

Framebuffer& Framebuffer::operator=(Framebuffer&& other)
{
    if (this != &other)
    {
        if (m_ID != 0)
            glDeleteFramebuffers(1, &m_ID);

        m_ID                = other.m_ID;
        m_ColourAttachments = std::move(other.m_ColourAttachments);
        m_DepthAttachment   = std::move(other.m_DepthAttachment);
        m_CachedWidth       = other.m_CachedWidth;
        m_CachedHeight      = other.m_CachedHeight;

        other.m_ID = 0;
    }
    return *this;
}

bool Framebuffer::IsValid() const
{
    return true;
}

} // namespace glfwd