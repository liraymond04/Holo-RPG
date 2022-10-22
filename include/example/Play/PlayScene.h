#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "Scene.h"
#include "Engine3D/Engine3D.h"

class PlayScene : public Scene {
  public:
    PlayScene() {}

  private:
    Holo::RPG *game;
    GameState *state;

    std::string sSceneName = "Main";

    Engine3D e3d;

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