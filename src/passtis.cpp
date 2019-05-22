#include "passtis.h"

Passtis::Passtis()
  : _initialized(false),
    _database(nullptr),
    _currentWindow(nullptr)
{

}

Passtis::~Passtis()
{
    if(_database)
        delete _database;
    endwin();
}

Passtis* Passtis::Instance()
{
    static Passtis instance;
    return &instance;
}

void Passtis::OnResizeEvent(int sig)
{
    Instance()->_currentWindow->onResizeEvent();
}

bool Passtis::init(int argc, char* argv[])
{
    std::cout << "PASSTIS" << std::endl;

    if(argc <= 1)
    {
        std::cerr << "Filename argument is missing !" << std::endl;
        return false;
    }

    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    start_color();

    signal(SIGWINCH, &Passtis::OnResizeEvent);

    _database = new Database(argv[1]);

    _unlockWindow = std::make_unique<UnlockWindow>();
    _unlockWindow->setDatabase(_database);
    _unlockWindow->update();

    _displayWindow = std::make_unique<DisplayWindow>();
    _newWindow = std::make_unique<NewWindow>();
    _editWindow = std::make_unique<EditWindow>();
    _removeWindow = std::make_unique<RemoveWindow>();
    _quitWindow = std::make_unique<QuitWindow>();

    _currentWindow = _unlockWindow.get();

    _initialized = true;

    return true;
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
            _displayWindow->setRoute(wa.data);
            _displayWindow->update();
        }

        if(wa.type == WindowAction::GoToNewWindow)
        {
            clear();
            _currentWindow = _newWindow.get();
            _newWindow->setDatabase(_database);
            _newWindow->setRoute(wa.data);
            _newWindow->update();
        }

        if(wa.type == WindowAction::GoToEditWindow)
        {
            clear();
            _currentWindow = _editWindow.get();
            _editWindow->setDatabase(_database);
            _editWindow->setRoute(wa.data);
            _editWindow->update();
        }

        if(wa.type == WindowAction::GoToRemoveWindow)
        {
            clear();
            _currentWindow = _removeWindow.get();
            _removeWindow->setDatabase(_database);
            _removeWindow->setRoute(wa.data);
            _removeWindow->update();
        }

        if(wa.type == WindowAction::GoToQuitWindow)
        {
            clear();
            _currentWindow = _quitWindow.get();
            _quitWindow->setDatabase(_database);
            _quitWindow->update();
        }

        if(wa.type == WindowAction::Quit)
        {
            _database->save("out"); // TODO
            break;
        }

        refresh();
    }

    return 0;
}

bool Passtis::Init(int argc, char* argv[])
{
    return Instance()->init(argc, argv);
}

int Passtis::Exec()
{
    return Instance()->exec();
}
