#include "displaywindow.h"

DisplayWindow::DisplayWindow() : Window()
{

}

WindowAction DisplayWindow::onKeyEvent(int ch)
{
    WindowAction wa;

    if(ch == KEY_RESIZE)
    {
        wa.type = WindowAction::Resize;
        return wa;
    }

    return Window::onKeyEvent(ch);
}