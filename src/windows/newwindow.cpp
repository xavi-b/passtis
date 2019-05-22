#include "newwindow.h"

NewWindow::NewWindow() : RouteAwareWindow()
{

}

WindowAction NewWindow::onKeyEvent(int ch)
{
    WindowAction wa;

    return RouteAwareWindow::onKeyEvent(ch);
}