#ifndef REMOVEWINDOW_H
#define REMOVEWINDOW_H

#include "routeawarewindow.h"

class RemoveWindow : public RouteAwareWindow
{
public:
    RemoveWindow();

    virtual WindowAction onKeyEvent(int ch);
};

#endif // REMOVEWINDOW_H