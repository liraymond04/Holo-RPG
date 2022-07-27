#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Game.h"

class Scene;

class GameState {
protected:
    GameState() {}

private:
    Game* game;
    vector<Scene*> scenes;

public:
    virtual void Init(Game* game) = 0;
    virtual void Cleanup() = 0;

    virtual void ChangeScene(Scene* scene) = 0;
    virtual void PushScene(Scene* scene) = 0;
    virtual void PopScene() = 0;

    virtual void Pause() = 0;
    virtual void Resume() = 0;

    virtual bool HandleEvents() = 0;
	virtual bool Update() = 0;
	virtual bool Draw() = 0;

	void ChangeState(GameState* state) {
		game->ChangeState(state);
	}
};

#endif