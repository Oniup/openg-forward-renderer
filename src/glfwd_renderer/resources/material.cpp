#include "glfwd_renderer/resources/material.h"

#include "glfwd_core/resource_manager.h"
#include "glfwd_renderer/rhi/shader.h"

namespace glfwd {

namespace {
    ResourceHandle<Texture> s_DefaultWhiteTexture = {};
}

namespace intern {

    void InitializeMaterialDefaultTextures(const ResourceHandle<Texture>& white_texture)
    {
        s_DefaultWhiteTexture = white_texture;
    }

} // namespace intern

BlinnPhongMaterial::ColorAttribute::ColorAttribute()
    : Color(glm::vec3(1.0f)),
      TextureHandle(s_DefaultWhiteTexture)
{
}

BlinnPhongMaterial::ColorAttribute::ColorAttribute(glm::vec3 color)
    : Color(color),
      TextureHandle(s_DefaultWhiteTexture)
{
}

BlinnPhongMaterial::ColorAttribute::ColorAttribute(const ResourceHandle<Texture>& texture)
    : Color(1.0),
      TextureHandle(texture)
{
}

BlinnPhongMaterial::ColorAttribute::ColorAttribute(glm::vec3                      color,
                                                   const ResourceHandle<Texture>& texture)
    : Color(color),
      TextureHandle(texture)
{
}

void BlinnPhongMaterial::SetupShaderMaterial(const Shader* shader, bool bind_shader)
{
    if (bind_shader)
        shader->Bind();

    shader->PushConstant("u_Material.Diffuse.Texture", 0);
    shader->PushConstant("u_Material.Specular.Texture", 1);
    shader->PushConstant("u_Material.Emission.Texture", 2);
}

bool BlinnPhongMaterial::IsValid() const
{
    return true;
}

void BlinnPhongMaterial::PushConstantsToShader(const Shader*    shader,
                                               ResourceManager* resources) const
{
    // Flat Colors
    shader->PushConstant("u_Material.Diffuse.Color", Diffuse.Color);
    shader->PushConstant("u_Material.Specular.Color", Specular.Color);
    shader->PushConstant("u_Material.Emission.Color", Emission.Color);

    // Textures
    shader->PushConstant(resources->QueryResource(Diffuse.TextureHandle), 0);
    shader->PushConstant(resources->QueryResource(Specular.TextureHandle), 1);
    shader->PushConstant(resources->QueryResource(Emission.TextureHandle), 2);

    // Other properties
    shader->PushConstant("u_Material.Shininess", Shininess);
    shader->PushConstant("u_Material_TilingFactor", TilingFactor);
}

} // namespace glfwd
