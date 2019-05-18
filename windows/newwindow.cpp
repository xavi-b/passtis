#include "newwindow.h"

NewWindow::NewWindow() : RouteAwareWindow()
{

}

WindowAction NewWindow::onKeyEvent(int ch)
{
    WindowAction wa;

    if(ch == KEY_RESIZE)
    {
        wa.type = WindowAction::Resize;
        return wa;
    }

    return RouteAwareWindow::onKeyEvent(ch);
}