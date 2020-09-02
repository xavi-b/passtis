#ifndef PASSTIS_H
#define PASSTIS_H

#include <iostream>
#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "database.h"
#include "windows/window.h"
#include "windows/unlockwindow.h"
#include "windows/displaywindow.h"
#include "windows/newwindow.h"
#include "windows/editwindow.h"
#include "windows/removewindow.h"
#include "windows/movewindow.h"
#include "windows/quitwindow.h"

class Passtis
{
private:
    bool _initialized;
    Database* _database;

    Window* _currentWindow;

    std::unique_ptr<UnlockWindow>   _unlockWindow;
    std::unique_ptr<DisplayWindow>  _displayWindow;
    std::unique_ptr<NewWindow>      _newWindow;
    std::unique_ptr<EditWindow>     _editWindow;
    std::unique_ptr<RemoveWindow>   _removeWindow;
    std::unique_ptr<MoveWindow>     _moveWindow;
    std::unique_ptr<QuitWindow>     _quitWindow;

    static Passtis* Instance();

public:
    Passtis();
    ~Passtis();

    static void OnResizeEvent(int sig);

    bool init(int argc, char* argv[]);
    int exec();

    static bool Init(int argc, char* argv[]);
    static int Exec();
};

#endif // PASTEES_H
