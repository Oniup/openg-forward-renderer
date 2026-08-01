# Renderer and OpenGL Abstraction Layer
# ==============================================================================
set(SRC
        resources/camera.h
        resources/camera.cpp
        resources/material.h
        resources/material.cpp
        resources/model.h
        resources/model.cpp

        rhi/buffer.h
        rhi/buffer.cpp
        rhi/context.h
        rhi/context.cpp
        rhi/framebuffer.h
        rhi/framebuffer.cpp
        rhi/shader.h
        rhi/shader.cpp
        rhi/texture.h
        rhi/texture.cpp

        draw_mode.h
        forward_renderer.h
        forward_renderer.cpp
        render_queue.h
        render_queue.cpp
        renderer_options.h
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC})

add_library(glfwd_renderer STATIC ${SRC})

target_include_directories(glfwd_renderer PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

target_compile_definitions(glfwd_renderer PUBLIC
        GLFWD_SHADER_HOT_RELOAD=$<BOOL:${GLFWD_SHADER_HOT_RELOAD}>
)

# Dependencies
# ==============================================================================
target_link_libraries(glfwd_renderer PUBLIC
        glfwd::core
        glm::glm
)
target_link_libraries(glfwd_renderer PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
        glad::glad
        assimp::assimp
)

# Alias for external linking
# ==============================================================================
add_library(glfwd::renderer ALIAS glfwd_renderer)
