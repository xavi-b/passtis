#include "removewindow.h"

RemoveWindow::RemoveWindow() : RouteAwareWindow()
{

}

WindowAction RemoveWindow::onKeyEvent(int ch)
{
    WindowAction wa;

    if(ch == KEY_RESIZE)
    {
        wa.type = WindowAction::Resize;
        return wa;
    }

    return RouteAwareWindow::onKeyEvent(ch);
}