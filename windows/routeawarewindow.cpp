#include "routeawarewindow.h"

RouteAwareWindow::RouteAwareWindow()
  : Window(),
    _route("/")
{

}

void RouteAwareWindow::setRoute(const std::string& route)
{
    _route = route;
}