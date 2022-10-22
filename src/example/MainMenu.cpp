#include "RPG.h"
#include "Scene.h"
#include "example/MainMenu.h"
#include "example/MainMenu/MainScene.h"

void MainMenu::Init(Holo::RPG *g) {
    game = g;
    ChangeScene(new MainScene);
    game->InitTrack("assets/music/Failing Defense.mp3", &game->audio_music,
                    &game->audio_current_track);
    ma_sound_start(&game->audio_current_track);
    ma_sound_set_looping(&game->audio_current_track, true);
    std::cout << sStateName << " state initialized"
              << "\n";
}

void MainMenu::Cleanup() {
    ma_sound_stop(&game->audio_current_track);
    std::cout << sStateName << " state cleanup"
              << "\n";
}

void MainMenu::ChangeScene(Scene *scene) {
    if (!scenes.empty()) {
        scenes.back()->Cleanup();
        scenes.pop_back();
    }

    scenes.push_back(scene);
    scenes.back()->Init(game, this);
}

void MainMenu::PushScene(Scene *scene) {
    if (!scenes.empty()) {
        scenes.back()->Pause();
    }

    scenes.push_back(scene);
    scenes.back()->Init(game, this);
}

void MainMenu::PopScene() {
    if (!scenes.empty()) {
        scenes.back()->Cleanup();
        scenes.pop_back();
    }

    if (!scenes.empty()) {
        scenes.back()->Resume();
    }
}

Scene *MainMenu::TopScene() { return scenes.back(); }

void MainMenu::Pause() {
    std::cout << sStateName << " state paused"
              << "\n";
}

void MainMenu::Resume() {
    std::cout << sStateName << " state resumed"
              << "\n";
}

bool MainMenu::HandleSceneEmpty(Holo::RPG *game) {
    game->PopState();
    return false;
}

bool MainMenu::HandleEvents(float fElapsedTime) {
    if (scenes.empty())
        return HandleSceneEmpty(game);
    return scenes.back()->HandleEvents(fElapsedTime);
}

bool MainMenu::Update(float fElapsedTime) {
    if (scenes.empty())
        return HandleSceneEmpty(game);
    return scenes.back()->Update(fElapsedTime);
}

bool MainMenu::Draw(float fElapsedTime) {
    if (scenes.empty())
        return HandleSceneEmpty(game);

    game->Clear(olc::BLACK);

    bool result = true;
    for (Scene *scene : scenes) {
        if (!result)
            break;
        result &= scene->Draw(fElapsedTime);
    }
    return result;
}