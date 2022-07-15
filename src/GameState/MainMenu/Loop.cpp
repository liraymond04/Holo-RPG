#include "GameState/MainMenu.h"

bool MainMenu::HandleEvents() {
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
                break;
            case 2: // Tutorial
                cout << "Tutorial selected" << "\n";
                break;
            case 3: // Credits
                cout << "Credits selected" << "\n";
                break;
            case 4: // Quit
                game->PopState();
                break;
        }
    }

    return true;
}

bool MainMenu::Update() {
    return true;
}

bool MainMenu::Draw() {
    game->Clear(olc::BLACK);

    for (int i=0; i<vOptions.size(); i++) {
        string sPrefix = i == nOptionSelected ? sOptionsIndicator : "";
        game->DrawString(
            {vOptionsOffset.x, vOptionsOffset.y + i * nOptionSpacing},
            sPrefix + vOptions[i]
        );
    }

    return true;
}