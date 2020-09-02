#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <menu.h>
#include <algorithm>

#include "clip.h"

#include "routeawarewindow.h"

class DisplayWindow : public RouteAwareWindow
{
private:
    struct MenuItem
    {
        std::string name = "ITEM";
        bool isGroup = false;
        std::string pos = "0";
    };

    WINDOW* _ncTitleWin;
    WINDOW* _ncMenuWin;
    WINDOW* _ncSubMenuWin;
    WINDOW* _ncPanelWin;
    std::vector<ITEM*> _ncMenuItems;
    std::vector<MenuItem> _menuItems;
    MENU* _ncMenu;
    bool _toggle;

protected:
    virtual void clean();

    void enterPressed();
    void selectionChanged();
    void updatePanel();

    void copyIdentityToClipboard();
    void copyPasswordToClipboard();

    std::string selectedRoute();

public:
    DisplayWindow();
    ~DisplayWindow();

    virtual WindowAction onKeyEvent(int ch);
    virtual void update();
};

#endif // DISPLAYWINDOW_H