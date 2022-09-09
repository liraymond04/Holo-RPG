#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::ordered_json;

#include "lib/olcPixelGameEngine.h"
#include "lib/olcPGEX_SplashScreen.h"
#include "lib/miniaudio.h"

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

    ma_result audio_result;
    ma_engine audio_engine;

private:
    olcPGEX_SplashScreen splashScreen;
    std::vector<GameState*> states;

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