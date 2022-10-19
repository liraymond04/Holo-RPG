#ifndef PLAY_H
#define PLAY_H

#include "GameState.h"

class Scene;

class Play : public GameState {
  public:
    Play() {}

  private:
    Holo::RPG *game;
    std::vector<Scene *> scenes;

    std::string sStateName = "Play";

  public:
    /* Core */
    void Init(Holo::RPG *g);
    void Cleanup();

    /* Scene */
    void ChangeScene(Scene *scene);
    void PushScene(Scene *scene);
    void PopScene();
    Scene *TopScene();

    /* Flow */
    void Pause();
    void Resume();

    bool HandleSceneEmpty(Holo::RPG *game);

    /* Loop */
    bool HandleEvents(float fElapsedTime);
    bool Update(float fElapsedTime);
    bool Draw(float fElapsedTime);
};

#endif