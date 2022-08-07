#include "Game.h"
#include "Scene.h"
#include "GameState/MainMenu.h"
#include "GameState/MainMenu/MainScene.h"

MainMenu MainMenu::m_MainMenu;

void MainMenu::Init(Game* g) {
    game = g;
    ChangeScene(new MainScene);
    cout << sStateName << " state initialized" << "\n";
}

void MainMenu::Cleanup() {
    cout << sStateName << " state cleanup" << "\n";
}

void MainMenu::ChangeScene(Scene* scene) {
    if (!scenes.empty()) {
        scenes.back()->Cleanup();
        scenes.pop_back();
    }

    scenes.push_back(scene);
    scenes.back()->Init(game, this);
}

void MainMenu::PushScene(Scene* scene) {
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

Scene* MainMenu::TopScene() {
    return scenes.back();
}

void MainMenu::Pause() {
    cout << sStateName << " state paused" << "\n";
}

void MainMenu::Resume() {
    cout << sStateName << " state resumed" << "\n";
}

bool HandleSceneEmpty(Game* game) {
    game->PopState();
    return false;
}

bool MainMenu::HandleEvents() {
    if (scenes.empty()) return HandleSceneEmpty(game);
    return scenes.back()->HandleEvents();
}

bool MainMenu::Update() {
    if (scenes.empty()) return HandleSceneEmpty(game);
    return scenes.back()->Update();
}

bool MainMenu::Draw() {
    if (scenes.empty()) return HandleSceneEmpty(game);

    game->Clear(olc::BLACK);

    bool result = true;
    for (Scene* scene : scenes) {
        if (!result) break;
        result &= scene->Draw();
    }
    return result;
}