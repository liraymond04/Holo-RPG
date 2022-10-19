#include "example/Play.h"
#include "RPG.h"
#include "Scene.h"
#include "example/Play/PlayScene.h"

void Play::Init(Holo::RPG *g) {
    game = g;
    ChangeScene(new PlayScene);
    std::cout << sStateName << " state initialized"
              << "\n";
}

void Play::Cleanup() {
    std::cout << sStateName << " state cleanup"
              << "\n";
}

void Play::ChangeScene(Scene *scene) {
    if (!scenes.empty()) {
        scenes.back()->Cleanup();
        scenes.pop_back();
    }

    scenes.push_back(scene);
    scenes.back()->Init(game, this);
}

void Play::PushScene(Scene *scene) {
    if (!scenes.empty()) {
        scenes.back()->Pause();
    }

    scenes.push_back(scene);
    scenes.back()->Init(game, this);
}

void Play::PopScene() {
    if (!scenes.empty()) {
        scenes.back()->Cleanup();
        scenes.pop_back();
    }

    if (!scenes.empty()) {
        scenes.back()->Resume();
    }
}

Scene *Play::TopScene() { return scenes.back(); }

void Play::Pause() {
    std::cout << sStateName << " state paused"
              << "\n";
}

void Play::Resume() {
    std::cout << sStateName << " state resumed"
              << "\n";
}

bool Play::HandleSceneEmpty(Holo::RPG *game) {
    game->PopState();
    return false;
}

bool Play::HandleEvents(float fElapsedTime) {
    if (scenes.empty())
        return HandleSceneEmpty(game);
    return scenes.back()->HandleEvents(fElapsedTime);
}

bool Play::Update(float fElapsedTime) {
    if (scenes.empty())
        return HandleSceneEmpty(game);
    return scenes.back()->Update(fElapsedTime);
}

bool Play::Draw(float fElapsedTime) {
    if (scenes.empty())
        return HandleSceneEmpty(game);

    // game->Clear(olc::BLACK);

    bool result = true;
    for (Scene *scene : scenes) {
        if (!result)
            break;
        result &= scene->Draw(fElapsedTime);
    }
    return result;
}