#ifndef SCENE_H
#define SCENE_H

#include "GameState.h"

class Scene {
public:
    Scene() {}

private:
    Game* game;
    GameState* state;

public:
    virtual void Init(Game* g, GameState* state) = 0;
    virtual void Cleanup() = 0;

    virtual void Pause() = 0;
    virtual void Resume() = 0;

    virtual bool HandleEvents() = 0;
	virtual bool Update() = 0;
	virtual bool Draw() = 0;
};


#endif