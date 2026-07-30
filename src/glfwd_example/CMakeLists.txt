set(SRC
        actors/camera_controller.h
        actors/camera_controller.cpp
        actors/cube.h
        actors/cube.cpp

        actor.h
        game.h
        game.cpp
        main.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})

add_executable(glfwd_example ${SRC})

target_link_libraries(glfwd_example
PRIVATE
    glfwd::engine
)
