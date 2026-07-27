set(SRC
    ./game.h
    ./game.cpp
    ./main.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})

add_executable(glfwd_example ${SRC})

target_link_libraries(glfwd_example
PRIVATE
    glfwd::engine
)
