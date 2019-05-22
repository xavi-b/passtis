#include "editwindow.h"

EditWindow::EditWindow() : RouteAwareWindow()
{

}

WindowAction EditWindow::onKeyEvent(int ch)
{
    WindowAction wa;

    return RouteAwareWindow::onKeyEvent(ch);
}