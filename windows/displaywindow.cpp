#include "displaywindow.h"

DisplayWindow::DisplayWindow()
  : RouteAwareWindow(),
    _ncTitleWin(nullptr),
    _ncMenuWin(nullptr),
    _ncSubMenuWin(nullptr),
    _ncMenu(nullptr)
{

}

DisplayWindow::~DisplayWindow()
{
    clean();
}

void DisplayWindow::clean()
{
    delwin(_ncTitleWin);
    unpost_menu(_ncMenu);
    free_menu(_ncMenu);
    for(auto item : _ncMenuItems)
    {
        if(item)
            free_item(item);
    }
    delwin(_ncMenuWin);
    delwin(_ncSubMenuWin);
}

WindowAction DisplayWindow::onKeyEvent(int ch)
{
    WindowAction wa;

    switch(ch)
    {
        case KEY_DOWN:
            menu_driver(_ncMenu, REQ_DOWN_ITEM);
            wrefresh(_ncMenuWin);
            break;
        case KEY_UP:
            menu_driver(_ncMenu, REQ_UP_ITEM);
            wrefresh(_ncMenuWin);
            break;
        // TODO KEYPAGEDOWN
        case KEY_NPAGE:
            menu_driver(_ncMenu, REQ_SCR_DPAGE);
            wrefresh(_ncMenuWin);
            break;
        // TODO KEYPAGEUP
        case KEY_PPAGE:
            menu_driver(_ncMenu, REQ_SCR_UPAGE);
            wrefresh(_ncMenuWin);
            break;
        case 127: // backspace
            if(_route != "/")
            {
                size_t pos = _route.substr(0, _route.size()-1).find_last_of('/');
                setRoute(_route.substr(0, pos+1));
                update();
            }
            break;
        case 10: // enter
            ITEM* item = current_item(_ncMenu);
            if(item != nullptr)
            {
                if(_database->nodeContent(_route)["children"][item_index(item)]["children"].IsDefined())
                {
                    setRoute(_route + item_name(item) + '/');
                    update();
                }
                else
                {
                    // TODO key
                }
            }
            break;
    }
    return Window::onKeyEvent(ch);
}

void DisplayWindow::update()
{
    clean();

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    YAML::Node currentNode = _database->nodeContent(_route);

    _menuItems.clear();
    _ncMenuItems.clear();
    for(size_t i = 0; i < currentNode["children"].size(); i++)
    {
        if(currentNode["children"][i]["name"].IsDefined())
            _menuItems.push_back(currentNode["children"][i]["name"].as<std::string>());
    }

    for(size_t i = 0; i < _menuItems.size(); i++)
    {
        ITEM* item = new_item(
            _menuItems[i].c_str(),
            ""
            );
        _ncMenuItems.push_back(item);
    }

    _ncMenuItems.push_back(nullptr);

    _ncMenu = new_menu(&_ncMenuItems[0]);

    _ncMenuWin = newwin(rows-3, cols, 1, 0);
    _ncSubMenuWin = derwin(_ncMenuWin, rows-3, cols, 0, 0);

    set_menu_win(_ncMenu, _ncMenuWin);
    set_menu_sub(_ncMenu, _ncSubMenuWin);
    set_menu_mark(_ncMenu, "> ");
    post_menu(_ncMenu);

    mvprintw(rows-1, 0, "Ctrl+C to Exit");
    refresh();

    if(_database)
    {
        _ncTitleWin = newwin(1, cols, 0, 0);
        wbkgd(_ncTitleWin, COLOR_PAIR(WindowColor::Title));

        std::string title = _database->filename() + " - " + _route;
        mvwprintw(_ncTitleWin, 0, (cols-title.size())/2, title.c_str());

        wrefresh(_ncTitleWin);
    }

    wrefresh(_ncMenuWin);

    Window::update();
}