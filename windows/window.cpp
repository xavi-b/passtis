#include "window.h"

Window::Window()
  : _database(nullptr)
{

}

void Window::setDatabase(Database* database)
{
    _database = database;
}

void Window::onResizeEvent()
{
    clear();
    refresh();
    mvprintw(0, 0, "resize");

    update();
}

WindowAction Window::onKeyEvent(int ch)
{
    WindowAction wa;
    wa.type = WindowAction::Nothing;

    if(std::string(keyname(ch)) == "^C")
    {
        wa.type = WindowAction::Quit;
        return wa;
    }

    return wa;
}

void Window::update()
{
    refresh();
}
