#include "Game.h"
#include "GameState/MainMenu/OptionsScene.h"

void OptionsScene::Init(Game* g, GameState* s) {
    game = g; state = s;
    vOptionsOffset.y = game->ScreenHeight() / 2 - (vOptions.size() / 2) * 10;
    vOptionsOffset.x += 100;
    cout << sSceneName << " scene initialized" << "\n";
}

void OptionsScene::Cleanup() {
    cout << sSceneName << " scene cleanup" << "\n";
}

void OptionsScene::Pause() {
    cout << sSceneName << " scene paused" << "\n";
}

void OptionsScene::Resume() {
    cout << sSceneName << " scene resumed" << "\n";
}

bool OptionsScene::HandleEvents() {
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
            case 0: // Back
                state->PopScene();
                break;
            case 1: // Master Volume
                cout << "Master Volume selected" << "\n";
                break;
            case 2: // SFX
                cout << "SFX selected" << "\n";
                break;
            case 3: // Music
                cout << "Music selected" << "\n";
                break;
        }
    }

    return true;
}

bool OptionsScene::Update() {
    return true;
}

bool OptionsScene::Draw() {
    for (int i=0; i<vOptions.size(); i++) {
        string sPrefix = i == nOptionSelected ? sOptionsIndicator : "";
        game->DrawString(
            {vOptionsOffset.x, vOptionsOffset.y + i * nOptionSpacing},
            sPrefix + vOptions[i]
        );
    }

    return true;
}