#ifndef MOVEWINDOW_H
#define MOVEWINDOW_H

#include <menu.h>
#include <algorithm>

#include "routeawarewindow.h"

class MoveWindow : public RouteAwareWindow
{
private:
    struct MenuItem
    {
        std::string name    = "ITEM";
        bool        isGroup = false;
        std::string pos     = "0";
    };

    WINDOW*               _ncTitleWin;
    WINDOW*               _ncMsgWin;
    WINDOW*               _ncMenuWin;
    WINDOW*               _ncSubMenuWin;
    std::vector<ITEM*>    _ncMenuItems;
    std::vector<MenuItem> _menuItems;
    MENU*                 _ncMenu;
    std::string           _oldRoute;
    std::string           _message;

protected:
    virtual void clean();

    void enterPressed();
    void selectionChanged();

    std::string selectedRoute();

    WindowAction tryMove();

public:
    MoveWindow();
    ~MoveWindow();

    virtual WindowAction onKeyEvent(int ch);
    virtual void         update();

    virtual void setRoute(const std::string& route) override;
};

#endif // MOVEWINDOW_H