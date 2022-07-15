#include "Game.h"
#include "GameState/MainMenu.h"

bool Game::OnUserCreate() {
    ChangeState(MainMenu::Instance());

    return true;
}

bool Game::OnUserUpdate(float fElapsedTime) {
    return HandleEvents() &&
        Update() && 
        Draw();
}

void Game::Cleanup() {
    cout << "Finish" << "\n";
}