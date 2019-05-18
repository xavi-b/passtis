#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <ncurses.h>

#include "../database.h"

struct WindowAction
{
    enum Type {
        Nothing = 0,
        GoToDisplayWindow,
        GoToNewWindow,
        GoToEditWindow,
        GoToRemoveWindow,
        GoToQuitWindow,
        Resize,
        Quit
    };

    Type type;
    std::string route;
};

class Window
{
protected:
    Database* _database;

public:
    Window();

    void setDatabase(Database* database);

    virtual WindowAction onKeyEvent(int ch);
    //virtual onMouseEvent() = 0;
};

#endif // WINDOW_H