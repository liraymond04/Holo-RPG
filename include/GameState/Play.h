#ifndef PLAY_H
#define PLAY_H

#include "GameState.h"

class Scene;

class Play : public GameState {
public:
    Play() {}

private:
    Game* game;
    std::vector<Scene*> scenes;

    std::string sStateName = "Play";

public:
    /* Core */
    void Init(Game* g);
    void Cleanup();

    /* Scene */
    void ChangeScene(Scene* scene);
    void PushScene(Scene* scene);
    void PopScene();
    Scene* TopScene();

    /* Flow */
    void Pause();
    void Resume();

    bool HandleSceneEmpty(Game* game);

    /* Loop */
    bool HandleEvents(float fElapsedTime);
    bool Update(float fElapsedTime);
    bool Draw(float fElapsedTime);
};

#endif