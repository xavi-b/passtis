#include "movewindow.h"

MoveWindow::MoveWindow()
  : RouteAwareWindow(),
    _ncTitleWin(nullptr),
    _ncMenuWin(nullptr),
    _ncSubMenuWin(nullptr),
    _ncMenu(nullptr)
{

}

MoveWindow::~MoveWindow()
{
    clean();
}

void MoveWindow::setRoute(const std::string& route)
{
    RouteAwareWindow::setRoute(_database->parentRoute(route));
    _oldRoute = route;
}

void MoveWindow::clean()
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

void MoveWindow::selectionChanged()
{
    wrefresh(_ncMenuWin);
}

void MoveWindow::enterPressed()
{
    ITEM* item = current_item(_ncMenu);
    if(item != nullptr)
    {
        _route = _route + item_name(item) + '/';
        update();
    }
}

std::string MoveWindow::selectedRoute()
{
    std::string route = "";
    ITEM* item = current_item(_ncMenu);
    if(item != nullptr)
    {
        int index = std::stoi(item_description(item));
        route = _route + _database->nodeContent(_route).children[index].name;
    }
    return route;
}

WindowAction MoveWindow::tryMove()
{
    WindowAction wa;

    // TODO check if already exists
    // TODO forbid move in subroute of moved route
    Node oldNode = _database->nodeContent(_oldRoute);
    _database->nodeContent(_route).children.push_back(oldNode);
    _database->removeNode(_oldRoute);

    wa.type = WindowAction::GoToDisplayWindow;
    wa.data = _route;

    return wa;
}

WindowAction MoveWindow::onKeyEvent(int ch)
{
    WindowAction wa;

    switch(ch)
    {
        case KEY_DOWN:
            menu_driver(_ncMenu, REQ_DOWN_ITEM);
            selectionChanged();
            break;
        case KEY_UP:
            menu_driver(_ncMenu, REQ_UP_ITEM);
            selectionChanged();
            break;
        // TODO KEYPAGEDOWN
        case KEY_NPAGE:
            menu_driver(_ncMenu, REQ_SCR_DPAGE);
            selectionChanged();
            break;
        // TODO KEYPAGEUP
        case KEY_PPAGE:
            menu_driver(_ncMenu, REQ_SCR_UPAGE);
            selectionChanged();
            break;
        case 127: // backspace
            if(_route != "/")
            {
                size_t pos = _route.substr(0, _route.size()-1).find_last_of('/');
                _route = _route.substr(0, pos+1);
                update();
            }
            break;
        case 10: // enter
            enterPressed();
            break;
        case 27: // escape
            wa.type = WindowAction::GoToDisplayWindow;
            wa.data = _database->parentRoute(_oldRoute);
            return wa;
            break;
        case KEY_F(1):
            return tryMove();
            break;
    }

    if(std::string(keyname(ch)) == "^C")
    {
        wa.type = WindowAction::GoToDisplayWindow;
        wa.data = _database->parentRoute(_oldRoute);
        return wa;
    }

    return Window::onKeyEvent(ch);
}

void MoveWindow::update()
{
    clean();

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    int nbShortcuts = 4;
    mvprintw(rows-4, 0, "F1: move to current route");
    mvprintw(rows-4, cols/2, "Enter: go down");
    mvprintw(rows-3, cols/2, "Backspace: go back up");
    mvprintw(rows-2, cols/2, "Ctrl+C: exit");

    Node currentNode = _database->nodeContent(_route);

    _menuItems.clear();
    _ncMenuItems.clear();
    for(size_t i = 0; i < currentNode.children.size(); i++)
    {
        if(!currentNode.children[i].name.empty() && currentNode.children[i].isGroup())
        {
            MenuItem menuitem;
            menuitem.name = currentNode.children[i].name;
            menuitem.isGroup = currentNode.children[i].isGroup();
            menuitem.pos = std::to_string(i);

            _menuItems.push_back(menuitem);
        }
    }

    std::sort(_menuItems.begin(), _menuItems.end(), [] (auto a, auto b) {
        std::vector<std::string> names;
        names.push_back(a.name);
        names.push_back(b.name);
        auto oldBegin = names.begin();
        std::sort(names.begin(), names.end());
        if(oldBegin == names.begin())
            return false;

        return true;
    });

    for(size_t i = 0; i < _menuItems.size(); i++)
    {
        ITEM* item = new_item(
            _menuItems[i].name.c_str(),
            _menuItems[i].pos.c_str()
            );
        _ncMenuItems.push_back(item);
    }

    _ncMenuItems.push_back(nullptr);

    _ncMenu = new_menu(&_ncMenuItems[0]);

    _ncMenuWin = newwin(rows-2-nbShortcuts, cols/2, 1, 0);
    _ncSubMenuWin = derwin(_ncMenuWin, rows-2-nbShortcuts, cols/2, 0, 0);

    set_menu_win(_ncMenu, _ncMenuWin);
    set_menu_sub(_ncMenu, _ncSubMenuWin);
    set_menu_mark(_ncMenu, "> ");
    menu_opts_off(_ncMenu, O_SHOWDESC);
    post_menu(_ncMenu);

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
    move(1, 0);
}