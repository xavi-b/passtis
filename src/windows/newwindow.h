#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include "routeawarewindow.h"

class NewWindow : public RouteAwareWindow
{
public:
    NewWindow();

    virtual WindowAction onKeyEvent(int ch);
};

#endif // NEWWINDOW_H