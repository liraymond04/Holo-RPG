#ifndef OPTIONSSCENE_H
#define OPTIONSSCENE_H

#include "Scene.h"

class OptionsScene : public Scene {
public:
    OptionsScene() {}

private:
    Game* game;
    GameState* state;

    string sSceneName = "Options";

    /* Options */
    vector<string> vOptions = {
        "Back",
        "Master Volume",
        "SFX",
        "Music"
    };
    string sOptionsIndicator = "> ";
    olc::vi2d vOptionsOffset = {50, 0};
    int nOptionSpacing = 15;
    int nOptionSelected = 0;

public:
    /* Core */
    void Init(Game *g, GameState* s);
    void Cleanup();

    /* Flow */
    void Pause();
    void Resume();

    /* Loop */
    bool HandleEvents();
    bool Update();
    bool Draw();
};

#endif