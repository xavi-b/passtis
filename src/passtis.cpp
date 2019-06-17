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
    try
    {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "produce help message")
            ("file,f", po::value<std::string>(), "database file")
            ("route,r", po::value<std::string>(), "set compression level")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if(vm.count("help"))
        {
            std::cout << desc << std::endl;
            return false;
        }

        if(!vm.count("file"))
        {
            std::cerr << "Filename argument is missing !" << std::endl;
            return false;
        }

        _database = new Database(vm["file"].as<std::string>());

        if(vm.count("route"))
        {
            std::string password = getpass("Password: ");
            if(_database->open(password))
            {
                Node node = _database->nodeContent(vm["route"].as<std::string>());

                if(!node.isGroup())
                {
                    std::cout << "=========" << std::endl;
                    std::cout << "Identity: " <<  node.identity << std::endl;
                    std::cout << "Password: " << node.password << std::endl;
                    std::cout << "More: " << node.more << std::endl;
                }
                else
                {
                    std::cerr << "Key does not exist !" << std::endl;
                }
            }
            else
            {
                std::cerr << "Unlock failed !" << std::endl;
            }
            return false;
        }
    }
    catch(std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
    catch(...) {
        std::cerr << "Unknown exception!" << std::endl;
        return false;
    }

    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    start_color();

    signal(SIGWINCH, &Passtis::OnResizeEvent);

    _unlockWindow = std::make_unique<UnlockWindow>();
    _unlockWindow->setDatabase(_database);
    _unlockWindow->update();

    _displayWindow = std::make_unique<DisplayWindow>();
    _newWindow = std::make_unique<NewWindow>();
    _editWindow = std::make_unique<EditWindow>();
    _removeWindow = std::make_unique<RemoveWindow>();
    _moveWindow = std::make_unique<MoveWindow>();
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
            if(wa.data != "")
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

        if(wa.type == WindowAction::GoToMoveWindow)
        {
            clear();
            _currentWindow = _moveWindow.get();
            _moveWindow->setDatabase(_database);
            _moveWindow->setRoute(wa.data);
            _moveWindow->update();
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
            break;
        }
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
