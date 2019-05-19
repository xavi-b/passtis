#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <menu.h>

#include "routeawarewindow.h"

class DisplayWindow : public RouteAwareWindow
{
private:
    WINDOW* _ncTitleWin;
    WINDOW* _ncMenuWin;
    WINDOW* _ncSubMenuWin;
    std::vector<ITEM*> _ncMenuItems;
    std::vector<std::string> _menuItems;
    MENU* _ncMenu;

protected:
    virtual void clean();

public:
    DisplayWindow();
    ~DisplayWindow();

    virtual WindowAction onKeyEvent(int ch);
    virtual void update();
};

#endif // DISPLAYWINDOW_H