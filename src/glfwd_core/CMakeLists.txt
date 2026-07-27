# Core
# ==============================================================================
set(SRC
    ./utility/color.h
    ./utility/color.cpp
    ./utility/error.h
    ./utility/error.cpp
    ./utility/hash.h
    ./utility/type_info.h
    ./resource_handle.h
    ./resource_manager.h
    ./time.h
    ./time.cpp
    ./transform.h
    ./transform.cpp
    ./window.h
    ./window.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})

add_library(glfwd_core STATIC ${SRC})
target_include_directories(glfwd_core
PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

# Options
# ==============================================================================
if (MSVC)
    target_compile_options(glfwd_core
    PUBLIC
        /Zc:preprocessor # Needed for __VA_OPT__() macro
    )
endif()

# Dependencies
# ==============================================================================
target_link_libraries(glfwd_core
PUBLIC
    fmt::fmt
    glm::glm
    unordered_dense::unordered_dense
PRIVATE
    SDL3::SDL3
)

# Alias for external linking
# ==============================================================================
add_library(glfwd::core ALIAS glfwd_core)
