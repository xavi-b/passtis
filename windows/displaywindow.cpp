#include "displaywindow.h"

DisplayWindow::DisplayWindow() : Window()
{

}

WindowAction DisplayWindow::onKeyEvent(int ch)
{
    WindowAction wa;

    return Window::onKeyEvent(ch);
}