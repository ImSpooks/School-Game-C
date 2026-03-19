cmake_minimum_required(VERSION 4.1)

set(CMAKE_C_STANDARD 99)
project(SchoolGameC C)

include(FetchContent)
FetchContent_Declare(raylib GIT_REPOSITORY "https://github.com/raysan5/raylib.git")
FetchContent_MakeAvailable(raylib)


# Create the executable target first
add_executable(SchoolGameC
        src/main.c
        src/items/items.h
        src/level/level.h
        src/screen/title_screen.c
        src/screen/screen.h
        src/screen/game_screen.c
        src/screen/game_screen.h
        src/hud.c
        src/hud.h
        src/util/util.h
        src/util/util.c
        src/util/util.c
        src/level/level_start.c
        src/level/level_forest1.c
        src/level/level_forest2.c
        src/level/level_forest3.c
        src/level/level_village1.c
        src/level/level_village2.c
        src/level/level_village3.c
        src/level/level_village4.c
        src/level/level_wizard1.c
        src/level/level_wizard2.c
        src/level/level_wizard3.c
        src/screen/battle_screen.c
        src/screen/battle_screen.h
        src/player/player.c
        src/player/player.h
        src/battle/battle.h
        src/battle/enemy/bear.c
        src/battle/enemy/bear.h
        src/battle/enemy/enemy.h
        src/battle/enemy/golem.c
        src/battle/enemy/golem.h
        src/battle/enemy/hawk.c
        src/battle/enemy/hawk.h
        src/battle/enemy/projectile/projectile.h
        src/battle/enemy/flowey.c
        src/battle/enemy/flowey.h
        src/battle/enemy/projectile/projectile.c
        src/battle/enemy/wizard.c
        src/battle/enemy/wizard.h
        src/globals.h
        src/items/items.c
        src/asset_manager.c
        src/asset_manager.h
)

add_library(c-vector lib/c-vector/vec.c lib/c-vector/vec.h)
include_directories(${CMAKE_SOURCE_DIR}/lib/c-vector)

# Link raylib to the correct target
target_link_libraries(SchoolGameC raylib c-vector)

set_target_properties(SchoolGameC PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/out)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_SOURCE_DIR}/out)