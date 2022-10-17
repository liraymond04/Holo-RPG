#include "RPG.h"

int main() {
    Holo::RPG game;

    if (game.Construct(512, 384, 2, 2)) {
        game.Start();
        game.Cleanup();
    } else {
        std::wcout << L"Could not construct console" << std::endl;
    }

    return 0;
}