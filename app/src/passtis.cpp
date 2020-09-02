#include "passtis.h"

#include <iostream>
#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <boost/program_options.hpp>
#include <spdlog/spdlog.h>

#include "database.h"
#include "windows/window.h"
#include "windows/unlockwindow.h"
#include "windows/displaywindow.h"
#include "windows/newwindow.h"
#include "windows/editwindow.h"
#include "windows/removewindow.h"
#include "windows/movewindow.h"
#include "windows/quitwindow.h"

namespace po = boost::program_options;

static void onResizeEvent(int /*sig*/)
{
    Passtis::instance()->resize();
}

void Passtis::resize()
{
    _currentWindow->onResizeEvent();
}

Passtis::Passtis() : _run(false), _currentWindow(nullptr)
{
}

Passtis::~Passtis()
{
    endwin();
}

Passtis* Passtis::instance()
{
    static Passtis instance;
    return &instance;
}

void Passtis::init(int argc, char* argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "produce help message")("file,f", po::value<std::string>(), "database file")(
        "route,r", po::value<std::string>(), "set compression level");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return;
    }

    if (!vm.count("file"))
    {
        std::cerr << "Filename argument is missing !" << std::endl;
        return;
    }

    std::string filename = vm["file"].as<std::string>();

    if (vm.count("route"))
    {
        std::string password = getpass("Password: ");
        if (Database::instance()->open(filename, password))
        {
            Node* node = Database::instance()->getNode(vm["route"].as<std::string>());

            if (node && !node->isGroup())
            {
                std::cout << "=========" << std::endl;
                std::cout << "Identity: " << node->identity << std::endl;
                std::cout << "Password: " << node->password << std::endl;
                std::cout << "More: " << node->more << std::endl;
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
        return;
    }

    setlocale(LC_ALL, "");
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    start_color();

    signal(SIGWINCH, &onResizeEvent);

    _unlockWindow  = std::make_unique<UnlockWindow>(filename);
    _displayWindow = std::make_unique<DisplayWindow>();
    _newWindow     = std::make_unique<NewWindow>();
    _editWindow    = std::make_unique<EditWindow>();
    _removeWindow  = std::make_unique<RemoveWindow>();
    _moveWindow    = std::make_unique<MoveWindow>();
    _quitWindow    = std::make_unique<QuitWindow>();

    _currentWindow = _unlockWindow;

    _run = true;
}

int Passtis::exec()
{
    if (_currentWindow)
        _currentWindow->update();

    while (_run)
    {
        int ch = getch();
        spdlog::get("filelogger")->debug("getch: {} {} {}", ch, (char)ch, std::string(keyname(ch)));

        WindowAction wa = _currentWindow->onKeyEvent(ch);
        spdlog::get("filelogger")->debug("WindowAction.type {}", wa.type);

        switch (wa.type)
        {
        case WindowAction::GoToDisplayWindow:
            _currentWindow = _displayWindow;
            break;
        case WindowAction::GoToNewWindow:
            _currentWindow = _newWindow;
            break;
        case WindowAction::GoToEditWindow:
            _currentWindow = _editWindow;
            break;
        case WindowAction::GoToRemoveWindow:
            _currentWindow = _removeWindow;
            break;
        case WindowAction::GoToMoveWindow:
            _currentWindow = _moveWindow;
            break;
        case WindowAction::GoToQuitWindow:
            _currentWindow = _quitWindow;
            break;
        case WindowAction::Nothing:
            break;
        default:
            return 0;
        }

        if (wa.type != WindowAction::Nothing)
        {
            spdlog::get("filelogger")->debug("clear");
            clear();

            if (wa.data != "")
            {
                if (auto raw = dynamic_cast<RouteAwareWindow*>(_currentWindow.get()))
                    raw->setRoute(wa.data);
            }

            spdlog::get("filelogger")->debug("update");
            _currentWindow->update();
        }
    }

    return 0;
}
