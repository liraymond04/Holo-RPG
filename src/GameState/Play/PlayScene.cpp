#include "Game.h"
#include "GameState/Play/PlayScene.h"

void PlayScene::Init(Game* g, GameState* s) {
    game = g; state = s;
    e3d.Init(game);
    std::cout << sSceneName << " scene initialized" << "\n";
}

void PlayScene::Cleanup() {
    std::cout << sSceneName << " scene cleanup" << "\n";
}

void PlayScene::Pause() {
    std::cout << sSceneName << " scene paused" << "\n";
}

void PlayScene::Resume() {
    std::cout << sSceneName << " scene resumed" << "\n";
}

bool PlayScene::HandleEvents(float fElapsedTime) {
    return true;
}

bool PlayScene::Update(float fElapsedTime) {
    game->Clear(olc::BLACK);
    return e3d.Update(fElapsedTime);
}

bool PlayScene::Draw(float fElapsedTime) {
    return true;
}