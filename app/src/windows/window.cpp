#include "windows/window.h"

Window::Window()
{
    init_pair(WindowColor::Title, COLOR_BLACK, COLOR_YELLOW);
}

void Window::onResizeEvent()
{
    clear();
    endwin(); // causes Ctrl+C to be doubled to quit but can't resize without it ?
    refresh();

    update();
}

WindowAction Window::onKeyEvent(int ch)
{
    WindowAction wa;
    wa.type = WindowAction::Nothing;

    if(std::string(keyname(ch)) == "^C")
    {
        wa.type = WindowAction::GoToQuitWindow;
        return wa;
    }

    return wa;
}

void Window::update()
{
    refresh();
}
