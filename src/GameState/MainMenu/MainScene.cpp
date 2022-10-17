#include "RPG.h"
#include "GameState/MainMenu/MainScene.h"
#include "GameState/MainMenu/OptionsScene.h"
#include "GameState/Play.h"

void MainScene::Init(Holo::RPG* g, GameState* s) {
    game = g; state = s;
    vOptionsOffset.y = game->ScreenHeight() / 2 - (vOptions.size() / 2) * 10;
    std::cout << sSceneName << " scene initialized" << "\n";
}

void MainScene::Cleanup() {
    std::cout << sSceneName << " scene cleanup" << "\n";
}

void MainScene::Pause() {
    std::cout << sSceneName << " scene paused" << "\n";
}

void MainScene::Resume() {
    std::cout << sSceneName << " scene resumed" << "\n";
}

bool MainScene::HandleEvents(float fElapsedTime) {
    if (game->GetKey(olc::F).bPressed) {
        std::cout << "F key pressed" << "\n";
        game->PlaySound("assets/sounds/bottle-smash-101001.mp3", &game->audio_sfx);
    }

    if (game->GetKey(olc::UP).bPressed && nOptionSelected > 0) {
        nOptionSelected--;
    }
    if (game->GetKey(olc::DOWN).bPressed && nOptionSelected < vOptions.size()-1) {
        nOptionSelected++;
    }

    if (game->GetKey(olc::ENTER).bPressed || game->GetKey(olc::RIGHT).bPressed) {
        switch (nOptionSelected) {
            case 0: // Play
                std::cout << "Play selected" << "\n";
                game->ChangeState(new Play);
                break;
            case 1: // Options
                std::cout << "Options selected" << "\n";
                state->PushScene(new OptionsScene);
                break;
            case 2: // Tutorial
                std::cout << "Tutorial selected" << "\n";
                break;
            case 3: // Credits
                std::cout << "Credits selected" << "\n";
                break;
            case 4: // Quit
                state->PopScene();
                break;
        }
    }
    
    return true;
}

bool MainScene::Update(float fElapsedTime) {
    return true;
}

bool MainScene::Draw(float fElapsedTime) {
    for (int i=0; i<vOptions.size(); i++) {
        std::string sPrefix = i == nOptionSelected ? sOptionsIndicator : "";
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