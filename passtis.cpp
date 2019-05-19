#include "passtis.h"

Passtis::Passtis(int argc, char* argv[])
  : _initialized(false),
    _database(nullptr),
    _currentWindow(nullptr)
{
    std::cout << "PASSTIS" << std::endl;

    if(argc <= 1)
    {
        std::cerr << "Filename argument is missing !" << std::endl;
        return;
    }

    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);

    signal(SIGWINCH, NULL);

    _database = new Database();

    _unlockWindow = std::make_unique<UnlockWindow>();
    _unlockWindow->setDatabase(_database);

    _displayWindow = std::make_unique<DisplayWindow>();
    _newWindow = std::make_unique<NewWindow>();
    _editWindow = std::make_unique<EditWindow>();
    _removeWindow = std::make_unique<RemoveWindow>();
    _quitWindow = std::make_unique<QuitWindow>();

    _currentWindow = _unlockWindow.get();

    _initialized = true;
}

Passtis::~Passtis()
{
    endwin();
}

int Passtis::exec()
{
    if(!_initialized)
        return 1;

    while(1)
    {
        int ch = getch();

        WindowAction wa = _currentWindow->onKeyEvent(ch);

        if(wa.type == WindowAction::GoToDisplayWindow)
        {
            clear();
            _currentWindow = _displayWindow.get();
            _displayWindow->setDatabase(_database);
        }

        if(wa.type == WindowAction::GoToNewWindow)
        {
            clear();
            _currentWindow = _newWindow.get();
            _newWindow->setDatabase(_database);
            _newWindow->setRoute(wa.route);
        }

        if(wa.type == WindowAction::GoToEditWindow)
        {
            clear();
            _currentWindow = _editWindow.get();
            _editWindow->setDatabase(_database);
            _editWindow->setRoute(wa.route);
        }

        if(wa.type == WindowAction::GoToRemoveWindow)
        {
            clear();
            _currentWindow = _removeWindow.get();
            _removeWindow->setDatabase(_database);
            _removeWindow->setRoute(wa.route);
        }

        if(wa.type == WindowAction::GoToQuitWindow)
        {
            clear();
            _currentWindow = _quitWindow.get();
            _quitWindow->setDatabase(_database);
        }

        if(wa.type == WindowAction::Resize)
        {
            // TODO
            break;
        }

        if(wa.type == WindowAction::Quit)
        {
            break;
        }

        refresh();
    }

    return 0;
}