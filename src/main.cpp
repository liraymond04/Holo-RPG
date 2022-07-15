#include "Game.h"

int main() {
    Game game;

    if (game.Construct(512, 384, 2, 2)) {
        game.Start();
        game.Cleanup();
    } else {
        wcout << L"Could not construct console" << endl;
    }

    return 0;
}