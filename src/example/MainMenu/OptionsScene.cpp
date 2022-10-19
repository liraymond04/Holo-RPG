#include "example/MainMenu/OptionsScene.h"
#include "RPG.h"

void OptionsScene::Init(Holo::RPG *g, GameState *s) {
    game = g;
    state = s;
    vOptionsOffset.y = game->ScreenHeight() / 2 - (vOptions.size() / 2) * 10;
    vOptionsOffset.x += 100;
    std::cout << sSceneName << " scene initialized"
              << "\n";
}

void OptionsScene::Cleanup() {
    std::cout << sSceneName << " scene cleanup"
              << "\n";
}

void OptionsScene::Pause() {
    std::cout << sSceneName << " scene paused"
              << "\n";
}

void OptionsScene::Resume() {
    std::cout << sSceneName << " scene resumed"
              << "\n";
}

bool OptionsScene::HandleEvents(float fElapsedTime) {
    if (game->GetKey(olc::F).bPressed) {
        std::cout << "F key pressed"
                  << "\n";
    }

    if (game->GetKey(olc::UP).bPressed && nOptionSelected > 0) {
        nOptionSelected--;
    }
    if (game->GetKey(olc::DOWN).bPressed &&
        nOptionSelected < vOptions.size() - 1) {
        nOptionSelected++;
    }

    int value;
    switch (nOptionSelected) {
    case 0: // Back
        if (game->GetKey(olc::ENTER).bPressed ||
            game->GetKey(olc::LEFT).bPressed) {
            state->PopScene();
        }
        break;
    case 1: // Master Volume
        value = game->config["Master Volume"];
        if (game->GetKey(olc::LEFT).bPressed && value > 0) {
            game->config["Master Volume"] = value - nValueStep;
            game->WriteConfig();
        }
        if (game->GetKey(olc::RIGHT).bPressed && value < 100) {
            game->config["Master Volume"] = value + nValueStep;
            game->WriteConfig();
        }
        break;
    case 2: // SFX
        value = game->config["SFX"];
        if (game->GetKey(olc::LEFT).bPressed && value > 0) {
            game->config["SFX"] = value - nValueStep;
            game->WriteConfig();
        }
        if (game->GetKey(olc::RIGHT).bPressed && value < 100) {
            game->config["SFX"] = value + nValueStep;
            game->WriteConfig();
        }
        break;
    case 3: // Music
        value = game->config["Music"];
        if (game->GetKey(olc::LEFT).bPressed && value > 0) {
            game->config["Music"] = value - nValueStep;
            game->WriteConfig();
        }
        if (game->GetKey(olc::RIGHT).bPressed && value < 100) {
            game->config["Music"] = value + nValueStep;
            game->WriteConfig();
        }
        break;
    }

    return true;
}

bool OptionsScene::Update(float fElapsedTime) { return true; }

bool OptionsScene::Draw(float fElapsedTime) {
    for (int i = 0; i < vOptions.size(); i++) {
        std::string sPrefix = i == nOptionSelected ? sOptionsIndicator : "";
        game->DrawString(
            { vOptionsOffset.x, vOptionsOffset.y + i * nOptionSpacing },
            sPrefix + vOptions[i]);
        if (game->config.contains(vOptions[i])) {
            int val = game->config[vOptions[i]];
            game->FillRect(vOptionsOffset.x * 2,
                           vOptionsOffset.y + i * nOptionSpacing,
                           fBarWidth * (0.01f * val), 10);
        }
    }

    if (state->TopScene() == this) {
        game->DrawRect(vOptionsOffset.x - 10,
                       vOptionsOffset.y - nOptionSpacing / 2, 220,
                       vOptions.size() * nOptionSpacing + nOptionSpacing / 2);
    }

    return true;
}