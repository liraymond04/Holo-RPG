#include "Game.h"
#include "GameState/MainMenu.h"

bool Game::OnUserCreate() {
    splashScreen.SetOptions(2, 1, 2.0f, 0.5f, olc::BLACK, olc::BLUE, olc::DARK_GREY, olc::WHITE);
    ChangeState(MainMenu::Instance());

    return true;
}

bool Game::OnUserUpdate(float fElapsedTime) {
    if (splashScreen.AnimateSplashScreen(fElapsedTime)) return true;
    return HandleEvents() &&
        Update() && 
        Draw();
}

void Game::Cleanup() {
    cout << "Finish" << "\n";
}

void Game::ChangeState(GameState* state) {
    if (!states.empty()) {
        states.back()->Cleanup();
        states.pop_back();
    }

    states.push_back(state);
    states.back()->Init(this);
}

void Game::PushState(GameState* state) {
    if (!states.empty()) {
        states.back()->Pause();
    }

    states.push_back(state);
    states.back()->Init(this);
}

void Game::PopState() {
    if (!states.empty()) {
        states.back()->Cleanup();
        states.pop_back();
    }

    if (!states.empty()) {
        states.back()->Resume();
    }
}

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