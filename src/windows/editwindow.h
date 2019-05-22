#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include "routeawarewindow.h"

class EditWindow : public RouteAwareWindow
{
public:
    EditWindow();

    virtual WindowAction onKeyEvent(int ch);
};

#endif // EDITWINDOW_H