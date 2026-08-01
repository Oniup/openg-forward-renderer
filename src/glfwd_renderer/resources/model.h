#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>

#include <vector>

#include "glfwd_renderer/resources/material.h"
#include "glfwd_renderer/rhi/buffer.h"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;

namespace glfwd {

// TODO: Support instancing vertex array attributes
struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 Color;
    glm::vec2 TexCoord;

    static std::array<VertexAttribute, 4> GetVertexAttribute();
    static std::array<VertexAttribute, 4> GetInstancedVertexAttribute() = delete;
};

class Mesh
{
public:
    /// Set vertex_color to glm::vec3(0.0f) to apply the rainbow UV vertex colors
    static Mesh GenerateCube(const BlinnPhongMaterial& material          = {},
                             glm::vec3                 vertex_color      = glm::vec3(1.0f),
                             FaceMode                  face_culling_mode = FaceMode::Back);
    /// Set vertex_color to glm::vec3(0.0f) to apply the rainbow UV vertex colors
    static Mesh GeneratePlane(const BlinnPhongMaterial& material          = {},
                              glm::vec3                 vertex_color      = glm::vec3(1.0f),
                              FaceMode                  face_culling_mode = FaceMode::Back);
    /// Set vertex_color to glm::vec3(0.0f) to apply the rainbow UV vertex colors
    static Mesh GenerateSphere(size_t revolutions_x = 20, size_t revolutions_y = 20,
                               const BlinnPhongMaterial& material          = {},
                               glm::vec3                 vertex_color      = glm::vec3(1.0f),
                               FaceMode                  face_culling_mode = FaceMode::Back);

    Mesh() = default;
    Mesh(const std::vector<Vertex>& vertices, const BlinnPhongMaterial& material = {},
         bool dynamic_draw = false, PrimitiveMode primitive_mode = PrimitiveMode::Triangles,
         FaceMode face_culling_mode = FaceMode::Back);
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
         const BlinnPhongMaterial& material = {}, bool dynamic_draw = false,
         PrimitiveMode primitive_mode    = PrimitiveMode::Triangles,
         FaceMode      face_culling_mode = FaceMode::Back);

    Mesh(Mesh&& other);
    Mesh& operator=(Mesh&& other);

    Mesh(const Mesh& other)            = delete;
    Mesh& operator=(const Mesh& other) = delete;

    bool IsValid() const;

    void Draw() const;
    void Draw(PrimitiveMode override_primitive_mode) const;

    BlinnPhongMaterial&       GetMaterial() { return m_Material; }
    const BlinnPhongMaterial& GetMaterial() const { return m_Material; }
    FaceMode                  GetFaceCullingMode() const { return m_FaceCullingMode; }

private:
    BlinnPhongMaterial m_Material;
    VertexArray        m_Data;
    FaceMode           m_FaceCullingMode;
};

struct ModelLoadCreateInfo
{
    enum class QualityPreset
    {
        Fast,
        Quality,
        MaxQuality,
    };

    std::string_view Path               = "";
    QualityPreset    MeshLoadingQuality = QualityPreset::Fast;
    bool             FlipUVs            = true;
    bool             GenerateSmoothMesh = true;
};

class Model
{
    struct TextureCacheEntry
    {
        uint64_t                PathID;
        ResourceHandle<Texture> TextureHandle;
    };

public:
    static std::vector<std::tuple<std::string, Model>> LoadSplitModel(
        const ModelLoadCreateInfo& info, size_t split_layer_depth = 1);

    Model() = default;
    Model(const ModelLoadCreateInfo& info);
    Model(Mesh&& mesh);
    Model(std::vector<Mesh>&& meshes);

    Model(Model&& other);
    Model& operator=(Model&& other);
    Model(const Model& other)            = delete;
    Model& operator=(const Model& other) = delete;

    bool IsValid() const;

    const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }

    void Draw(const Shader* shader) const;

private:
    void PushTextureToCache(std::vector<TextureCacheEntry>& texture_cache,
                            const ResourceHandle<Texture>&  texture_handle,
                            uint64_t                        path_id = 0) const;

    void LoadFromPath(const ModelLoadCreateInfo& info);
    void ProcessNode(const aiNode* node, const aiScene* scene, uint32_t flags);
    Mesh ProcessMesh(const aiMesh* mesh, const aiScene* scene, uint32_t flags);

    std::vector<Mesh> m_Meshes;
};

} // namespace glfwd
