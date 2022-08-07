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

    switch (nOptionSelected) {
        case 0: // Back
            if (game->GetKey(olc::ENTER).bPressed) {
                state->PopScene();
            }
            break;
        case 1: // Master Volume
            if (game->GetKey(olc::LEFT).bPressed && vecSoundValue[1] > 0) {
                vecSoundValue[1] -= nValueStep;
            }
            if (game->GetKey(olc::RIGHT).bPressed && vecSoundValue[1] < 100) {
                vecSoundValue[1] += nValueStep;
            }
            break;
        case 2: // SFX
            if (game->GetKey(olc::LEFT).bPressed && vecSoundValue[2] > 0) {
                vecSoundValue[2] -= nValueStep;
            }
            if (game->GetKey(olc::RIGHT).bPressed && vecSoundValue[2] < 100) {
                vecSoundValue[2] += nValueStep;
            }
            break;
        case 3: // Music
            if (game->GetKey(olc::LEFT).bPressed && vecSoundValue[3] > 0) {
                vecSoundValue[3] -= nValueStep;
            }
            if (game->GetKey(olc::RIGHT).bPressed && vecSoundValue[3] < 100) {
                vecSoundValue[3] += nValueStep;
            }
            break;
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
        if (vecSoundValue[i] != -1) {
            game->FillRect(vOptionsOffset.x * 2, vOptionsOffset.y + i * nOptionSpacing, fBarWidth * (0.01f * vecSoundValue[i]), 10);
        }
    }

    if (state->TopScene() == this) {
        game->DrawRect(vOptionsOffset.x - 10, vOptionsOffset.y - nOptionSpacing / 2, 220, vOptions.size() * nOptionSpacing + nOptionSpacing / 2);    
    }

    return true;
}