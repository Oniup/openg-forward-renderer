# Engine
# ==============================================================================
set(SRC
    ./application.h
    ./application.cpp
    ./create_info.h
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})

add_library(glfwd_engine STATIC ${SRC})
target_include_directories(glfwd_engine
PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

# Dependencies
# ==============================================================================
target_link_libraries(glfwd_engine
PUBLIC
    glfwd::core
    glfwd::renderer

    SDL3::SDL3 # TODO: Remove later
    glad       # TODO: Remove Later
)

# Alias for external linking
# ==============================================================================
add_library(glfwd::engine ALIAS glfwd_engine)
