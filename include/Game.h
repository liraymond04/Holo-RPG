#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

#include "olcPixelGameEngine.h"

class GameState;

class Game : public olc::PixelGameEngine {
public:
    Game() {
        sAppName = "HoloRPG";
    }

private:
    vector<GameState*> states;

public:
    /* Core */
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
    void Cleanup();

    /* State */
    void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();

    /* Loop */
	bool HandleEvents();
	bool Update();
	bool Draw();
};

#endif