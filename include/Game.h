#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::ordered_json;

#include "olcPixelGameEngine.h"
#include "olcPGEX_SplashScreen.h"

class GameState;

class Game : public olc::PixelGameEngine {
public:
    Game() {
        sAppName = "HoloRPG";
    }

    json config = json::parse(R"(
        {
            "Master Volume": 50,
            "SFX": 100,
            "Music": 100
        }
    )");

private:
    olcPGEX_SplashScreen splashScreen;
    vector<GameState*> states;

    /* Core */
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

    /* Loop */
	bool HandleEvents();
	bool Update();
	bool Draw();

public:
    void Cleanup();
    
    /* State */
    void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();

    /* Config */
    void ReadConfig();
    void WriteConfig();
};

#endif