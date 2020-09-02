#include "windows/movewindow.h"

#include <spdlog/spdlog.h>

MoveWindow::MoveWindow()
    : RouteAwareWindow(), _ncTitleWin(nullptr), _ncMsgWin(nullptr), _ncMenuWin(nullptr), _ncSubMenuWin(nullptr),
      _ncMenu(nullptr)
{
}

MoveWindow::~MoveWindow()
{
    clean();
}

void MoveWindow::setRoute(const std::string& route)
{
    RouteAwareWindow::setRoute(Database::instance()->parentRoute(route));
    _oldRoute = route;
    spdlog::get("filelogger")->debug("MoveWindow::setRoute {} {}", _oldRoute, _route);
}

void MoveWindow::clean()
{
    delwin(_ncTitleWin);
    delwin(_ncMsgWin);
    unpost_menu(_ncMenu);
    free_menu(_ncMenu);
    for (auto item : _ncMenuItems)
    {
        if (item)
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
    if (item != nullptr)
    {
        std::string newRoute = _route + item_name(item);
        spdlog::get("filelogger")->debug("MoveWindow::enterPressed: {} {}", newRoute, _oldRoute);
        if (newRoute != _oldRoute)
        {
            _route = newRoute + '/';
            update();
        }
    }
}

std::string MoveWindow::selectedRoute()
{
    std::string route = "";
    ITEM*       item  = current_item(_ncMenu);
    if (item != nullptr)
    {
        int index = std::stoi(item_description(item));
        route     = _route + Database::instance()->getNode(_route)->children[index].name;
    }
    return route;
}

WindowAction MoveWindow::tryMove()
{
    WindowAction wa;

    Node* oldNode = Database::instance()->getNode(_oldRoute);

    if(Database::instance()->nodeExists(_route + oldNode->name))
    {
        _message = "Node " + oldNode->name + " already exists !";
        mvwprintw(_ncMsgWin, 0, 0, _message.c_str());
        wrefresh(_ncMsgWin);
        return wa;
    }

    Database::instance()->getNode(_route)->children.push_back(*oldNode);
    Database::instance()->removeNode(_oldRoute);

    wa.type = WindowAction::GoToDisplayWindow;
    wa.data = _route;

    return wa;
}

WindowAction MoveWindow::onKeyEvent(int ch)
{
    WindowAction wa;

    switch (ch)
    {
    case KEY_DOWN:
    case 9: // tab
        menu_driver(_ncMenu, REQ_DOWN_ITEM);
        selectionChanged();
        break;
    case KEY_UP:
    case KEY_BTAB:
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
        if (_route != "/")
        {
            size_t pos = _route.substr(0, _route.size() - 1).find_last_of('/');
            _route     = _route.substr(0, pos + 1);
            update();
        }
        break;
    case 10: // enter
        enterPressed();
        break;
    case 27: // escape
        wa.type = WindowAction::GoToDisplayWindow;
        wa.data = Database::instance()->parentRoute(_oldRoute);
        return wa;
        break;
    case KEY_F(1):
        return tryMove();
        break;
    }

    if (std::string(keyname(ch)) == "^L")
    {
        return tryMove();
    }

    if (std::string(keyname(ch)) == "^C")
    {
        wa.type = WindowAction::GoToDisplayWindow;
        wa.data = Database::instance()->parentRoute(_oldRoute);
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
    mvprintw(rows - 4, 0, "Ctrl+L: move to current route");
    mvprintw(rows - 4, cols / 2, "Enter: go down");
    mvprintw(rows - 3, cols / 2, "Backspace: go back up");
    mvprintw(rows - 2, cols / 2, "Ctrl+C: exit");

    Node* currentNode = Database::instance()->getNode(_route);

    _menuItems.clear();
    _ncMenuItems.clear();
    for (size_t i = 0; i < currentNode->children.size(); i++)
    {
        if (!currentNode->children[i].name.empty() && currentNode->children[i].isGroup())
        {
            MenuItem menuitem;
            menuitem.name    = currentNode->children[i].name;
            menuitem.isGroup = currentNode->children[i].isGroup();
            menuitem.pos     = std::to_string(i);

            _menuItems.push_back(menuitem);
        }
    }

    std::sort(_menuItems.begin(), _menuItems.end(), [](auto a, auto b) {
        std::vector<std::string> names;
        names.push_back(a.name);
        names.push_back(b.name);
        auto oldBegin = names.begin();
        std::sort(names.begin(), names.end());
        if (oldBegin == names.begin())
            return false;

        return true;
    });

    for (size_t i = 0; i < _menuItems.size(); i++)
    {
        ITEM* item = new_item(_menuItems[i].name.c_str(), _menuItems[i].pos.c_str());
        _ncMenuItems.push_back(item);
    }

    _ncMenuItems.push_back(nullptr);

    _ncMenu = new_menu(&_ncMenuItems[0]);

    _ncMenuWin    = newwin(rows - 2 - nbShortcuts, cols / 2, 1, 0);
    _ncSubMenuWin = derwin(_ncMenuWin, rows - 2 - nbShortcuts, cols / 2, 0, 0);

    set_menu_win(_ncMenu, _ncMenuWin);
    set_menu_sub(_ncMenu, _ncSubMenuWin);
    set_menu_mark(_ncMenu, "> ");
    set_menu_format(_ncMenu, rows - 2 - nbShortcuts - 1, 1);
    menu_opts_off(_ncMenu, O_SHOWDESC);
    post_menu(_ncMenu);

    refresh();

    _ncTitleWin = newwin(1, cols, 0, 0);
    wbkgd(_ncTitleWin, COLOR_PAIR(WindowColor::Title) | ' ');

    std::string title = Database::instance()->filename() + " - " + _route;
    mvwprintw(_ncTitleWin, 0, (cols - title.size()) / 2, title.c_str());

    wrefresh(_ncTitleWin);

    _ncMsgWin = newwin(1, cols, rows - 1, 0);
    wbkgd(_ncMsgWin, COLOR_PAIR(WindowColor::Title) | ' ');

    mvwprintw(_ncMsgWin, 0, 0, _message.c_str());

    wrefresh(_ncMsgWin);

    wrefresh(_ncMenuWin);
    move(1, 0);
}