#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include "GameState.h"

class OptionsMenu : public GameState {
protected:
    OptionsMenu () {}

private:
    static OptionsMenu m_OptionsMenu;

    Game* game;

    string sStateName = "Options Menu";
};

#endif