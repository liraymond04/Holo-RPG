#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Game.h"

class GameState {
protected:
    GameState() {}

private:
    Game* game;

public:
    virtual void Init(Game* game) = 0;
    virtual void Cleanup() = 0;

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