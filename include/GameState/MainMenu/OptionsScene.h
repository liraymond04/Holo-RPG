#ifndef OPTIONSSCENE_H
#define OPTIONSSCENE_H

#include "Scene.h"

class OptionsScene : public Scene {
public:
    OptionsScene() {}

private:
    Game* game;
    GameState* state;

    std::string sSceneName = "Options";

    /* Options */
    std::vector<std::string> vOptions = {
        "Back",
        "Master Volume",
        "SFX",
        "Music"
    };
    std::string sOptionsIndicator = "> ";
    olc::vi2d vOptionsOffset = {50, 0};
    int nOptionSpacing = 15;
    int nOptionSelected = 0;

    /* Sound options */
    float fBarWidth = 50;
    int nValueStep = 10;

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