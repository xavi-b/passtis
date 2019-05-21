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
    WINDOW* _ncPanelWin;
    std::vector<ITEM*> _ncMenuItems;
    std::vector<std::string> _menuItems;
    MENU* _ncMenu;
    bool _toggle;

protected:
    virtual void clean();

    void enterPressed();
    void selectionChanged();
    void updatePanel();

public:
    DisplayWindow();
    ~DisplayWindow();

    virtual WindowAction onKeyEvent(int ch);
    virtual void update();
};

#endif // DISPLAYWINDOW_H