#include "windows/displaywindow.h"

DisplayWindow::DisplayWindow()
  : RouteAwareWindow(),
    _ncTitleWin(nullptr),
    _ncMenuWin(nullptr),
    _ncSubMenuWin(nullptr),
    _ncPanelWin(nullptr),
    _ncMenu(nullptr),
    _toggle(false)
{

}

DisplayWindow::~DisplayWindow()
{
    clean();
}

void DisplayWindow::clean()
{
    _toggle = false;
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
    delwin(_ncPanelWin);
}

void DisplayWindow::selectionChanged()
{
    _toggle = false;

    updatePanel();

    wrefresh(_ncMenuWin);
}

void DisplayWindow::enterPressed()
{
    ITEM* item = current_item(_ncMenu);
    if(item != nullptr)
    {
        int index = std::stoi(item_description(item));
        if(_database->nodeContent(_route).children[index].isGroup())
        {
            _toggle = false;
            setRoute(_route + item_name(item) + '/');
            update();
        }
        else
        {
            _toggle = !_toggle;
            updatePanel();
        }
    }
}

void DisplayWindow::updatePanel()
{
    wclear(_ncPanelWin);
    ITEM* item = current_item(_ncMenu);
    if(item != nullptr)
    {
        int index = std::stoi(item_description(item));
        if(!_database->nodeContent(_route).children[index].isGroup())
        {
            std::string name = _database->nodeContent(_route).children[index].name;
            wattron(_ncPanelWin, A_BOLD);
            mvwprintw(_ncPanelWin, 0, 0, "NAME:");
            wattroff(_ncPanelWin, A_BOLD);
            mvwprintw(_ncPanelWin, 1, 0, name.c_str());

            std::string identity = _database->nodeContent(_route).children[index].identity;
            wattron(_ncPanelWin, A_BOLD);
            mvwprintw(_ncPanelWin, 2, 0, "IDENTITY:");
            wattroff(_ncPanelWin, A_BOLD);
            mvwprintw(_ncPanelWin, 3, 0, identity.c_str());

            wattron(_ncPanelWin, A_BOLD);
            mvwprintw(_ncPanelWin, 4, 0, "PASSWORD:");
            wattroff(_ncPanelWin, A_BOLD);
            if(_toggle)
            {
                std::string password = _database->nodeContent(_route).children[index].password;
                mvwprintw(_ncPanelWin, 5, 0, password.c_str());
            }
            else
            {
                mvwprintw(_ncPanelWin, 5, 0, "********");
            }

            std::string more = _database->nodeContent(_route).children[index].more;
            wattron(_ncPanelWin, A_BOLD);
            mvwprintw(_ncPanelWin, 6, 0, "MORE:");
            wattroff(_ncPanelWin, A_BOLD);
            mvwprintw(_ncPanelWin, 7, 0, more.c_str());
        }
    }
    wrefresh(_ncPanelWin);
}

void DisplayWindow::copyIdentityToClipboard()
{
    ITEM* item = current_item(_ncMenu);
    if(item != nullptr)
    {
        int index = std::stoi(item_description(item));
        if(!_database->nodeContent(_route).children[index].isGroup())
        {
            std::string identity = _database->nodeContent(_route).children[index].identity;
            clip::set_text(identity);
        }
    }
}

void DisplayWindow::copyPasswordToClipboard()
{
    ITEM* item = current_item(_ncMenu);
    if(item != nullptr)
    {
        int index = std::stoi(item_description(item));
        if(!_database->nodeContent(_route).children[index].isGroup())
        {
            std::string password = _database->nodeContent(_route).children[index].password;
            clip::set_text(password);
        }
    }
}

std::string DisplayWindow::selectedRoute()
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

WindowAction DisplayWindow::onKeyEvent(int ch)
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
                setRoute(_route.substr(0, pos+1));
                update();
            }
            break;
        case 10: // enter
            enterPressed();
            break;
        case KEY_F(1):
            wa.type = WindowAction::GoToNewWindow;
            wa.data = _route;
            return wa;
            break;
        case KEY_F(2):
            if(selectedRoute() != "")
            {
                wa.type = WindowAction::GoToEditWindow;
                wa.data = selectedRoute();
            }
            return wa;
            break;
        case KEY_F(3):
            if(selectedRoute() != "")
            {
                wa.type = WindowAction::GoToMoveWindow;
                wa.data = selectedRoute();
            }
            return wa;
            break;
        case KEY_F(4):
            if(selectedRoute() != "")
            {
                wa.type = WindowAction::GoToRemoveWindow;
                wa.data = selectedRoute();
            }
            return wa;
            break;
        case KEY_F(5):
            copyIdentityToClipboard();
            break;
        case KEY_F(6):
            copyPasswordToClipboard();
            break;
    }
    return Window::onKeyEvent(ch);
}

void DisplayWindow::update()
{
    clean();

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    int nbShortcuts = 4;
    mvprintw(rows-4, 0, "F1: new");
    mvprintw(rows-3, 0, "F2: edit");
    mvprintw(rows-2, 0, "F3: move");
    mvprintw(rows-1, 0, "F4: remove");
    mvprintw(rows-4, cols/3, "F5: copy identity");
    mvprintw(rows-3, cols/3, "F6: copy password");
    mvprintw(rows-4, 2*cols/3, "Enter: go down / toggle");
    mvprintw(rows-3, 2*cols/3, "Backspace: go back up");
    mvprintw(rows-2, 2*cols/3, "Ctrl+C: exit");

    Node currentNode = _database->nodeContent(_route);

    _menuItems.clear();
    _ncMenuItems.clear();
    for(size_t i = 0; i < currentNode.children.size(); i++)
    {
        if(!currentNode.children[i].name.empty())
        {
            MenuItem menuitem;
            menuitem.name = currentNode.children[i].name;
            menuitem.isGroup = currentNode.children[i].isGroup();
            menuitem.pos = std::to_string(i);

            _menuItems.push_back(menuitem);
        }
    }

    std::sort(_menuItems.begin(), _menuItems.end(), [] (auto a, auto b) {
        if(a.isGroup && !b.isGroup)
            return true;

        if(!a.isGroup && b.isGroup)
            return false;

        return a.name.compare(b.name) < 0;
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
    set_menu_format(_ncMenu, rows-2-nbShortcuts-1, 1);
    menu_opts_off(_ncMenu, O_SHOWDESC);
    post_menu(_ncMenu);

    refresh();

    _ncPanelWin = newwin(rows-2-nbShortcuts, cols/2, 1, cols/2);

    updatePanel();

    if(_database)
    {
        _ncTitleWin = newwin(1, cols, 0, 0);
        wbkgd(_ncTitleWin, COLOR_PAIR(WindowColor::Title));

        std::string title = _database->filename() + " - " + _route;
        mvwprintw(_ncTitleWin, 0, (cols-title.size())/2, title.c_str());

        wrefresh(_ncTitleWin);
    }

    wrefresh(_ncMenuWin);
    wrefresh(_ncPanelWin);
    move(1, 0);
}