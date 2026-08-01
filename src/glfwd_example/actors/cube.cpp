#include "glfwd_example/actors/cube.h"

#include "glfwd_core/core_context.h"
#include "glfwd_core/resource_manager.h"

namespace glfwd_example {

void Cube::OnInitialize()
{
    glfwd::ResourceManager* resources = glfwd::CoreContext::GetResourceManager();

    // Get texture
    m_ShaderHandle = resources->QueryHandle<glfwd::Shader>("Basic Blinn-Phong");
    glfwd::BlinnPhongMaterial::SetupShaderMaterial(resources->QueryResource(m_ShaderHandle));

    // Create model texture
    glfwd::BlinnPhongMaterial material{
        .Diffuse = resources->PushResource(
            "Container Color",
            glfwd::Texture(resources->GetAssetPath("textures/container/color.png"))),
        .Specular = resources->PushResource(
            "Container Specular",
            glfwd::Texture(resources->GetAssetPath("textures/container/specular.png"))),
    };
    m_ModelHandle =
        resources->PushResource("Container", glfwd::Model(glfwd::Mesh::GenerateCube(material)));
}

void Cube::SubmitToRenderQueue(glfwd::RenderQueue* render_queue) const
{
    render_queue->SubmitModel(m_ShaderHandle, m_Transform.CreateModelMatrix(), m_ModelHandle);
}

} // namespace glfwd_example