#ifndef MAINMENU_H
#define MAINMENU_H

#include "GameState.h"

class MainMenu : public GameState {
protected:
    MainMenu() {}

private:
    static MainMenu m_MainMenu;

    Game* game;

    string sStateName = "Main Menu";
    
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
    void Init(Game* g);
    void Cleanup();

    /* Flow */
    void Pause();
    void Resume();

    /* Loop */
    bool HandleEvents();
    bool Update();
    bool Draw();

    static MainMenu* Instance() {
		return &m_MainMenu;
	}
};

#endif