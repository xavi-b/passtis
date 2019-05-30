#include "removewindow.h"

RemoveWindow::RemoveWindow()
  : RouteAwareWindow(),
    _ncTitleWin(nullptr)
{

}

RemoveWindow::~RemoveWindow()
{
    clean();
}

void RemoveWindow::clean()
{
    delwin(_ncTitleWin);
}

WindowAction RemoveWindow::onKeyEvent(int ch)
{
    WindowAction wa;

    switch(ch)
    {
        case 'y':
            _database->removeNode(_route);
            wa.type = WindowAction::GoToDisplayWindow;
            return wa;
            break;
        case 'n':
            wa.type = WindowAction::GoToDisplayWindow;
            return wa;
            break;
        default:
            break;
    }

    return RouteAwareWindow::onKeyEvent(ch);
}

void RemoveWindow::update()
{
    clean();

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    refresh();

    if(_database)
    {
        _ncTitleWin = newwin(1, cols, 0, 0);
        wbkgd(_ncTitleWin, COLOR_PAIR(WindowColor::Title));

        std::string title = _database->filename() + " - " + _route;
        mvwprintw(_ncTitleWin, 0, (cols-title.size())/2, title.c_str());

        wrefresh(_ncTitleWin);
    }

    std::string str = "Confirm removal of " + _route + " ? (y/n)";
    int halfPosWidth = (cols-str.size())/2;
    int halfPosHeight = (rows-1)/2;
    mvprintw(halfPosHeight, halfPosWidth, str.c_str());
}