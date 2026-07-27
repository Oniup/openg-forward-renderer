#pragma once

namespace glfwd {

enum class PrimitiveMode
{
    Points,        // Individual vertex point
    Lines,         // Individual lines, disconnected
    LineStrip,     // Connected lines, curved lines, Bezier curves
    LineLoop,      // Connected lines which wraps back to first vertex
    Triangles,     // Individual objects, models
    TriangleStrip, // Ribbons, generating roads
    TriangleFan,   // Circular objects
    Patches,       // Let the gpu subdivide geometry, tessellation
};

enum class PolygonMode
{
    Lines,  // Wireframe
    Points, // Vertices points
    Fill,   // Render full face
};

enum class FaceMode
{
    Front,        // Cull back faces
    Back,         // Cull front faces
    FrontAndBack, // Keep both front and back faces
};

} // namespace glfwd
