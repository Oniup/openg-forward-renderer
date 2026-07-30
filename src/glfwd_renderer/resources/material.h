#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>

#include "glfwd_core/resource_handle.h"
#include "glfwd_renderer/rhi/texture.h"

namespace glfwd {

namespace intern {
    void InitializeMaterialDefaultTextures(const ResourceHandle<Texture>& white_texture);
}

class Shader;

struct BlinnPhongMaterial
{
    struct ColorAttribute
    {
        glm::vec3               Color;
        ResourceHandle<Texture> TextureHandle;

        ColorAttribute();
        ColorAttribute(glm::vec3 color);
        ColorAttribute(const ResourceHandle<Texture>& texture);
        ColorAttribute(glm::vec3 color, const ResourceHandle<Texture>& texture);
    };

    static void SetupShaderMaterial(const Shader* shader);

    ColorAttribute Diffuse;
    ColorAttribute Specular;
    ColorAttribute Emission;

    glm::vec2 TilingFactor = glm::vec2(1.0f);
    float     Shininess    = 1.0f;

    bool IsValid() const;
    void PushConstantsToShader(const Shader* shader) const;
};

} // namespace glfwd
