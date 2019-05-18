#include "quitwindow.h"

QuitWindow::QuitWindow() : Window()
{

}

WindowAction QuitWindow::onKeyEvent(int ch)
{
    WindowAction wa;

    if(ch == KEY_RESIZE)
    {
        wa.type = WindowAction::Resize;
        return wa;
    }

    return Window::onKeyEvent(ch);
}