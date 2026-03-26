#include "level/level_registry.h"
#include "asset_manager.h"

void level_registry_initialize(struct LevelRegistry* registry) {
    registry->levelStart = (struct Level) {
        .enter = level_start_enter,
        .leave = level_start_leave,
        .texture = &assets.texture_level_start,
        .music = &assets.music_forest,
    };

    registry->levelForest1 = (struct Level) {
        .enter = level_forest1_enter,
        .leave = level_forest1_leave,
        .texture = &assets.texture_level_forest1,
        .music = &assets.music_forest,
    };

    registry->levelForest2 = (struct Level) {
        .enter = level_forest2_enter,
        .leave = level_forest2_leave,
        .texture = &assets.texture_level_forest2_bear,
        .music = &assets.music_forest,
    };

    registry->levelForest3 = (struct Level) {
        .enter = level_forest3_enter,
        .leave = level_forest3_leave,
        .texture = &assets.texture_level_forest3,
        .music = &assets.music_forest,
    };

    registry->levelVillage1 = (struct Level) {
        .enter = level_village1_enter,
        .leave = level_village1_leave,
        .texture = &assets.texture_level_village1,
        .music = &assets.music_village,
    };

    registry->levelVillage2 = (struct Level) {
        .enter = level_village2_enter,
        .leave = level_village2_leave,
        .texture = &assets.texture_level_village2,
        .music = &assets.music_village,
    };

    registry->levelVillage3 = (struct Level) {
        .enter = level_village3_enter,
        .leave = level_village3_leave,
        .texture = &assets.texture_level_forest3,
        .music = &assets.music_village,
    };

    registry->levelVillage4 = (struct Level) {
        .enter = level_village4_enter,
        .leave = level_village4_leave,
        .texture = &assets.texture_level_village4,
        .music = &assets.music_village,
    };

    registry->levelWizard1 = (struct Level) {
        .enter = level_wizard1_enter,
        .leave = level_wizard1_leave,
        .texture = &assets.texture_level_wizard1,
        .music = &assets.music_forest,
    };

    registry->levelWizard2 = (struct Level) {
        .enter = level_wizard2_enter,
        .leave = level_wizard2_leave,
        .texture = &assets.texture_level_wizard2,
        .music = &assets.music_forest,
    };

    registry->levelWizard3 = (struct Level) {
        .enter = level_wizard3_enter,
        .leave = level_wizard3_leave,
        .texture = &assets.texture_level_wizard3,
        .music = &assets.music_forest,
    };
}

struct LevelRegistry level_registry;