#ifndef ROUTEAWAREWINDOW_H
#define ROUTEAWAREWINDOW_H

#include <string>

#include "window.h"

class RouteAwareWindow : public Window
{
protected:
    std::string _route;

public:
    RouteAwareWindow();
    virtual void setRoute(const std::string& route);
};

#endif // ROUTEAWAREWINDOW_H