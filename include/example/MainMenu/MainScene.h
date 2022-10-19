#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "Scene.h"

class MainScene : public Scene {
  public:
    MainScene() {}

  private:
    Holo::RPG *game;
    GameState *state;

    std::string sSceneName = "Main";

    /* Options */
    std::vector<std::string> vOptions = { "Play", "Options", "Tutorial",
                                          "Credits", "Quit" };
    std::string sOptionsIndicator = "> ";
    olc::vi2d vOptionsOffset = { 50, 0 };
    int nOptionSpacing = 15;
    int nOptionSelected = 0;

  public:
    /* Core */
    void Init(Holo::RPG *g, GameState *s);
    void Cleanup();

    /* Flow */
    void Pause();
    void Resume();

    /* Loop */
    bool HandleEvents(float fElapsedTime);
    bool Update(float fElapsedTime);
    bool Draw(float fElapsedTime);
};

#endif