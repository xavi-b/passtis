#include "editwindow.h"

EditWindow::EditWindow() : RouteAwareWindow()
{

}

WindowAction EditWindow::onKeyEvent(int ch)
{
    WindowAction wa;

    if(ch == KEY_RESIZE)
    {
        wa.type = WindowAction::Resize;
        return wa;
    }

    return RouteAwareWindow::onKeyEvent(ch);
}