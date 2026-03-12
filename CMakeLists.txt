cmake_minimum_required(VERSION 4.1)

project(TestRaylib LANGUAGES C)

find_package(raylib CONFIG REQUIRED)


# Create the executable target first
add_executable(TestRaylib
        src/main.c
        src/objects.h
        src/level/level.c
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
        src/battle/enemy/enemy.h
        src/battle/enemy/projectile/projectile.h
        src/battle/enemy/flowey.c
        src/battle/enemy/flowey.h
)

add_library(c-vector lib/c-vector/vec.c lib/c-vector/vec.h)
include_directories(${CMAKE_SOURCE_DIR}/lib/c-vector)

# Link raylib to the correct target
target_link_libraries(TestRaylib PRIVATE raylib c-vector)


set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_DEST_DIR "${CMAKE_BINARY_DIR}/assets")

file(GLOB_RECURSE ASSET_FILES RELATIVE "${ASSETS_SOURCE_DIR}" "${ASSETS_SOURCE_DIR}/*")

foreach(ASSET IN LISTS ASSET_FILES)
    get_filename_component(ASSET_DIR "${ASSET}" PATH)
    add_custom_command(TARGET TestRaylib POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSETS_DEST_DIR}/${ASSET_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ASSETS_SOURCE_DIR}/${ASSET}" "${ASSETS_DEST_DIR}/${ASSET}"
            COMMENT "Copying asset: ${ASSET}"
    )
endforeach()

