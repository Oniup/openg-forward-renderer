#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

#include "glfwd_core/resource_handle.h"

namespace glfwd {

enum class ProjectionMode
{
    Perspective,
    Orthographic,
};

struct RenderCamera
{
    glm::vec3 Position;
    glm::vec3 ClearColor;
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;
};

struct CameraBase
{
    glm::vec3      Position          = glm::vec3(0.0f);
    glm::vec4      ClearColor        = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    float          NearClippingPlane = 0.1f;
    float          FarClippingPlane  = 1000.0f;
    float          FieldOfView       = 45.0f;
    ProjectionMode Projection        = ProjectionMode::Perspective;

    virtual ~CameraBase() = default;

    RenderCamera GetRenderCamera(int32_t viewport_width, int32_t viewport_height) const;
    glm::mat4    CreateProjectionMatrix(int32_t viewport_width, int32_t viewport_height) const;

    virtual glm::mat4 CreateViewMatrix() const = 0;
};

struct TargetCamera : CameraBase
{
    glm::vec3 Target = glm::vec3(0.0f);
    glm::vec3 Up     = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 CreateViewMatrix() const override;
};

struct DirectionalCamera : CameraBase
{
    glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up      = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 CreateViewMatrix() const override;
};

} // namespace glfwd
