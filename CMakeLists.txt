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
