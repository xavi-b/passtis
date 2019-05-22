#include "movewindow.h"

MoveWindow::MoveWindow() : RouteAwareWindow()
{

}

WindowAction MoveWindow::onKeyEvent(int ch)
{
    WindowAction wa;

    return RouteAwareWindow::onKeyEvent(ch);
}