set(CMAKE_FOLDER "thirdparty") # All targets will be grouped under this heading

# SDL
#---------------------------------------------------------------------------------------------------
add_subdirectory(SDL)

# SDL_image
#---------------------------------------------------------------------------------------------------
set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_GIF OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_JXL OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_LBM OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_PCX OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_PNM OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_TGA OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_TIF OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_WEBP OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_XCF OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_XPM OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_XV OFF CACHE BOOL "" FORCE)

# Enable what you do need
set(SDLIMAGE_SVG ON CACHE BOOL "" FORCE)
set(SDLIMAGE_JPG ON CACHE BOOL "" FORCE)
set(SDLIMAGE_PNG ON CACHE BOOL "" FORCE)
add_subdirectory(SDL_image)

# GLM
#---------------------------------------------------------------------------------------------------
add_library(glm INTERFACE) # Math Library
target_include_directories(glm SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/glm)
add_library(glm::glm ALIAS glm)


# GLAD
# ----------------------------------------------------------------------------------------------------------------------
add_subdirectory(glad)

# fmt
# ----------------------------------------------------------------------------------------------------------------------
add_subdirectory(fmt)

# unordered_dense
# ----------------------------------------------------------------------------------------------------------------------
add_subdirectory(unordered_dense)

# # Assimp
# # ----------------------------------------------------------------------------------------------------------------------
# if (WIN32 AND MSVC)
#     # Disable building again, SDL_image already does so. To avoid a circular dependency, must disable
#     # Assimp from building zlib and feed target from SDL_image to Assimp
#     set(ASSIMP_BUILD_ZLIB OFF CACHE BOOL "" FORCE)
#     set(ZLIB_LIBRARY zlibstatic CACHE STRING "" FORCE)
#     set(ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL_image/external/zlib" CACHE PATH "" FORCE)
# endif()
#
# # Optimize build targets
# set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "Disable all importers" FORCE)
# set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "Disable all exporters" FORCE)
# set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Disable tests" FORCE)
# set(ASSIMP_INSTALL OFF CACHE BOOL "Disable installation" FORCE)
# set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Disable CLI tools" FORCE)
# # Explicit model support
# set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "Enable FBX" FORCE)
# set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "Enable OBJ" FORCE)
# set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "Enable GLTF and GLB" FORCE)
#
# set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "Treat all warnings as errors." FORCE)
# add_subdirectory(assimp)
#
# # ImGUI
# # ----------------------------------------------------------------------------------------------------------------------
# add_library(imgui STATIC
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imconfig.h
#     # ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_internal.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_rectpack.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_textedit.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_truetype.h
#
#     # Backend
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdl3.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdl3.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
# )
#
# target_include_directories(imgui
# PUBLIC
#     ${CMAKE_CURRENT_SOURCE_DIR}/imgui
# )
#
# target_link_libraries(imgui
# PUBLIC
#     SDL3::SDL3
#     glad
# )
#
# if (WIN32 AND MSVC)
#     target_compile_definitions(imgui
#     PRIVATE
#         "_CRT_SECURE_NO_WARNINGS"
#     )
# endif()
