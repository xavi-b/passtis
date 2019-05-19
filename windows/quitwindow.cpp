#include "quitwindow.h"

QuitWindow::QuitWindow() : Window()
{

}

WindowAction QuitWindow::onKeyEvent(int ch)
{
    WindowAction wa;

    return Window::onKeyEvent(ch);
}