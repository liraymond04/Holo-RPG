#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "Scene.h"

class MainScene : public Scene {
public:
    MainScene() {}

private:
    Game* game;
    GameState* state;

    string sSceneName = "Main";

    /* Options */
    vector<string> vOptions = {
        "Play",
        "Options",
        "Tutorial",
        "Credits",
        "Quit"
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