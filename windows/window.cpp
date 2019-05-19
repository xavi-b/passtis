#include "window.h"

Window::Window()
  : _database(nullptr)
{

}

void Window::setDatabase(Database* database)
{
    _database = database;
}

WindowAction Window::onKeyEvent(int ch)
{
    WindowAction wa;
    wa.type = WindowAction::Nothing;

    if(ch == KEY_RESIZE)
    {
        wa.type = WindowAction::Resize;
        return wa;
    }

    if(std::string(keyname(ch)) == "^C")
    {
        wa.type = WindowAction::Quit;
        return wa;
    }

    return wa;
}