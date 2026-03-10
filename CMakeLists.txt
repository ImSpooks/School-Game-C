cmake_minimum_required(VERSION 4.1)
project(TestRaylib LANGUAGES C)

find_package(raylib CONFIG REQUIRED)



# Create the executable target first
add_executable(TestRaylib
        main.c
        objects.h
        level/level.c
        level/level.h
        screen/title_screen.c
        screen/screen.h
        screen/game_screen.c
        screen/game_screen.h
        hud.c
        hud.h
        util/raylib_util.h
        util/raylib_util.c
        util/raylib_util.c
        level/level_start.c
        level/level_forest1.c
)

# Link raylib to the correct target
target_link_libraries(TestRaylib PRIVATE raylib)
