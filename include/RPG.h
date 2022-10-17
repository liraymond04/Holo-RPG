#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <strstream>
#include <lib/nlohmann/json.hpp>

using json = nlohmann::ordered_json;

#include "lib/olcPixelGameEngine.h"
#include "lib/olcPGEX_SplashScreen.h"
#include "lib/miniaudio.h"

class GameState;

namespace Holo {
    class RPG : public olc::PixelGameEngine {
    public:
        RPG() {
            sAppName = "Holo::RPG";
        }

        json config = json::parse(R"(
            {
                "Master Volume": 50,
                "SFX": 100,
                "Music": 100
            }
        )");

        ma_sound_group audio_master;
        ma_sound_group audio_sfx;
        ma_sound_group audio_music;

        ma_sound audio_current_track;

    private:
        bool splashPlaying = true;
        olcPGEX_SplashScreen splashScreen;
        std::vector<GameState*> states;
        
        ma_result audio_result;
        ma_engine audio_engine;

        /* Core */
        bool OnUserCreate() override;
        bool OnUserUpdate(float fElapsedTime) override;

        /* Loop */
        bool HandleEvents(float fElapsedTime);
        bool Update(float fElapsedTime);
        bool Render(float fElapsedTime);

    public:
        void Cleanup();
        
        /* State */
        void ChangeState(GameState* state);
        void PushState(GameState* state);
        void PopState();

        /* Config */
        void ReadConfig();
        void WriteConfig();

        /* Audio */
        void PlaySound(const char* file, ma_sound_group* group);
        void InitTrack(const char* file, ma_sound_group* group, ma_sound* sound);
        void AudioSetConfig();
    };
}

#endif