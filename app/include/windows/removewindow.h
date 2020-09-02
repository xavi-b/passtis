#ifndef REMOVEWINDOW_H
#define REMOVEWINDOW_H

#include "routeawarewindow.h"

class RemoveWindow : public RouteAwareWindow
{
    WINDOW* _ncTitleWin;

protected:
    virtual void clean();

public:
    RemoveWindow();
    ~RemoveWindow();

    virtual WindowAction onKeyEvent(int ch);
    virtual void         update();
};

#endif // REMOVEWINDOW_H