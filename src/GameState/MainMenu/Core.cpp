#include "GameState/MainMenu.h"
#include "Game.h"

MainMenu MainMenu::m_MainMenu;

void MainMenu::Init(Game* g) {
    game = g;
    vOptionsOffset.y = game->ScreenHeight() / 2 - (vOptions.size() / 2) * 10;

    cout << sStateName << " state initialized" << "\n";
}

void MainMenu::Cleanup() {
    cout << sStateName << " state cleanup" << "\n";
}