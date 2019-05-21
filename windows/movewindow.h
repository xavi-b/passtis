#ifndef MOVEWINDOW_H
#define MOVEWINDOW_H

#include "routeawarewindow.h"

class MoveWindow : public RouteAwareWindow
{
public:
    MoveWindow();

    virtual WindowAction onKeyEvent(int ch);
};

#endif // MOVEWINDOW_H