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
        GoToMoveWindow,
        GoToQuitWindow,
        Quit
    };

    Type type;
    std::string data;

    WindowAction()
      : type(Type::Nothing),
        data("")
    {

    }
};

enum WindowColor
{
    Title = 1
};

class Window
{
protected:
    Database* _database;

public:
    Window();

    void setDatabase(Database* database);

    virtual void onResizeEvent();
    virtual WindowAction onKeyEvent(int ch);
    //virtual onMouseEvent() = 0;

    virtual void update();
};

#endif // WINDOW_H