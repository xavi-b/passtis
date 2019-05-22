#include "removewindow.h"

RemoveWindow::RemoveWindow() : RouteAwareWindow()
{

}

WindowAction RemoveWindow::onKeyEvent(int ch)
{
    WindowAction wa;

    return RouteAwareWindow::onKeyEvent(ch);
}