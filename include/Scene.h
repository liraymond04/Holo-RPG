#ifndef SCENE_H
#define SCENE_H

#include "GameState.h"

class Scene {
public:
    Scene() {}

private:
    Holo::RPG* game;
    GameState* state;

public:
    virtual void Init(Holo::RPG* g, GameState* state) = 0;
    virtual void Cleanup() = 0;

    virtual void Pause() = 0;
    virtual void Resume() = 0;

    virtual bool HandleEvents(float fElapsedTime) = 0;
	virtual bool Update(float fElapsedTime) = 0;
	virtual bool Draw(float fElapsedTime) = 0;
};


#endif