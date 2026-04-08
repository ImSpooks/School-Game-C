cmake_minimum_required(VERSION 4.1)

set(CMAKE_C_STANDARD 99)
project(SchoolGameC C)

include(FetchContent)
FetchContent_Declare(raylib GIT_REPOSITORY "https://github.com/raysan5/raylib.git")
FetchContent_MakeAvailable(raylib)


# Create the executable target first
include_directories(SchoolGameC include)
add_executable(SchoolGameC
        src/main.c
        src/scene/scene_title.c
        src/scene/scene_battle.c
        src/scene/scene_level.c
        src/hud.c
        src/util/util.c
        src/level/type/level_start.c
        src/level/type/level_forest1.c
        src/level/type/level_forest2.c
        src/level/type/level_forest3.c
        src/level/type/level_village1.c
        src/level/type/level_village2.c
        src/level/type/level_village3.c
        src/level/type/level_village4.c
        src/level/type/level_wizard1.c
        src/level/type/level_wizard2.c
        src/level/type/level_wizard3.c
        src/player/player.c
        src/enemy/bear.c
        src/enemy/golem.c
        src/enemy/hawk.c
        src/enemy/flowey.c
        src/enemy/projectile/projectile.c
        src/enemy/wizard.c
        src/asset_manager.c
        src/util/array.c
        src/level/level_registry.c
        src/scene/scene_gameover.c
        src/scene/scene_credits.c
        src/enemy/nick.c
        src/enemy/renoir.c
)

# Link raylib to the correct target
target_link_libraries(SchoolGameC raylib)

set_target_properties(SchoolGameC PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/out)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_SOURCE_DIR}/out)