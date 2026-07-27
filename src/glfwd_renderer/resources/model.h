#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>

#include <vector>

#include "glfwd_renderer/resources/material.h"
#include "glfwd_renderer/rhi/buffer.h"

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
    static Mesh GenerateCube(const BlinnPhongMaterial& material     = {},
                             glm::vec3                 vertex_color = glm::vec3(1.0f));
    /// Set vertex_color to glm::vec3(0.0f) to apply the rainbow UV vertex colors
    static Mesh GeneratePlane(const BlinnPhongMaterial& material     = {},
                              glm::vec3                 vertex_color = glm::vec3(1.0f));
    /// Set vertex_color to glm::vec3(0.0f) to apply the rainbow UV vertex colors
    static Mesh GenerateSphere(size_t revolutions_x = 20, size_t revolutions_y = 20,
                               const BlinnPhongMaterial& material     = {},
                               glm::vec3                 vertex_color = glm::vec3(1.0f));

    Mesh() = default;
    Mesh(const std::vector<Vertex>& vertices, const BlinnPhongMaterial& material = {},
         bool dynamic_draw = false, PrimitiveMode primitive_mode = PrimitiveMode::Triangles);
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
         const BlinnPhongMaterial& material = {}, bool dynamic_draw = false,
         PrimitiveMode primitive_mode = PrimitiveMode::Triangles);

    Mesh(Mesh&& other);
    Mesh& operator=(Mesh&& other);

    Mesh(const Mesh& other)            = delete;
    Mesh& operator=(const Mesh& other) = delete;

    bool IsValid() const;

    void Draw() const;
    void Draw(PrimitiveMode override_primitive_mode) const;

    BlinnPhongMaterial&       GetMaterial() { return m_Material; }
    const BlinnPhongMaterial& GetMaterial() const { return m_Material; }

private:
    BlinnPhongMaterial m_Material;
    VertexArray        m_Data;
};

class Model
{
    struct TextureCache
    {
        uint64_t                PathID;
        ResourceHandle<Texture> TextureHandle;
    };

public:
    Model() = default;
    Model(std::string_view path);
    Model(Mesh&& mesh);

    Model(Model&& other);
    Model& operator=(Model&& other);

    Model(const Model& other)            = delete;
    Model& operator=(const Model& other) = delete;

    bool IsValid() const;

    const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }

    void Draw(Shader& shader);
    void NotUsingThisFunction() = delete;

private:
    std::vector<Mesh>         m_Meshes;
    std::vector<TextureCache> m_Textures;
};

} // namespace glfwd
