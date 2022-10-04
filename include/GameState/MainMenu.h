#ifndef MAINMENU_H
#define MAINMENU_H

#include "GameState.h"

class Scene;

class MainMenu : public GameState {
public:
    MainMenu() {}

private:
    Game* game;
    std::vector<Scene*> scenes;

    std::string sStateName = "Main Menu";

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