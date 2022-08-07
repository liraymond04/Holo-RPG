#include "Game.h"
#include "GameState/MainMenu/MainScene.h"
#include "GameState/MainMenu/OptionsScene.h"

void MainScene::Init(Game* g, GameState* s) {
    game = g; state = s;
    vOptionsOffset.y = game->ScreenHeight() / 2 - (vOptions.size() / 2) * 10;
    cout << sSceneName << " scene initialized" << "\n";
}

void MainScene::Cleanup() {
    cout << sSceneName << " scene cleanup" << "\n";
}

void MainScene::Pause() {
    cout << sSceneName << " scene paused" << "\n";
}

void MainScene::Resume() {
    cout << sSceneName << " scene resumed" << "\n";
}

bool MainScene::HandleEvents() {
    if (game->GetKey(olc::F).bPressed) {
        cout << "F key pressed" << "\n";
    }

    if (game->GetKey(olc::UP).bPressed && nOptionSelected > 0) {
        nOptionSelected--;
    }
    if (game->GetKey(olc::DOWN).bPressed && nOptionSelected < vOptions.size()-1) {
        nOptionSelected++;
    }

    if (game->GetKey(olc::ENTER).bPressed) {
        switch (nOptionSelected) {
            case 0: // Play
                cout << "Play selected" << "\n";
                break;
            case 1: // Options
                cout << "Options selected" << "\n";
                state->PushScene(new OptionsScene);
                break;
            case 2: // Tutorial
                cout << "Tutorial selected" << "\n";
                break;
            case 3: // Credits
                cout << "Credits selected" << "\n";
                break;
            case 4: // Quit
                state->PopScene();
                break;
        }
    }

    return true;
}

bool MainScene::Update() {
    return true;
}

bool MainScene::Draw() {
    for (int i=0; i<vOptions.size(); i++) {
        string sPrefix = i == nOptionSelected ? sOptionsIndicator : "";
        game->DrawString(
            {vOptionsOffset.x, vOptionsOffset.y + i * nOptionSpacing},
            sPrefix + vOptions[i]
        );
    }

    if (state->TopScene() == this) {
        game->DrawRect(vOptionsOffset.x - 10, vOptionsOffset.y - nOptionSpacing / 2, 100, vOptions.size() * nOptionSpacing + nOptionSpacing / 2);    
    }

    return true;
}