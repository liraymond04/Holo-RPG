#include "Game.h"
#include "GameState.h"

bool Game::HandleEvents() {
    if (states.empty()) return false;
    return states.back()->HandleEvents();
}

bool Game::Update() {
    if (states.empty()) return false;
    return states.back()->Update();
}

bool Game::Draw() {
    if (states.empty()) return false;
    return states.back()->Draw();
}