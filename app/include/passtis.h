#ifndef PASSTIS_H
#define PASSTIS_H

#include <memory>

class Window;
class UnlockWindow;
class DisplayWindow;
class NewWindow;
class EditWindow;
class RemoveWindow;
class MoveWindow;
class QuitWindow;

class Passtis
{
private:
    bool _run;

    std::shared_ptr<Window>        _currentWindow;
    std::shared_ptr<UnlockWindow>  _unlockWindow;
    std::shared_ptr<DisplayWindow> _displayWindow;
    std::shared_ptr<NewWindow>     _newWindow;
    std::shared_ptr<EditWindow>    _editWindow;
    std::shared_ptr<RemoveWindow>  _removeWindow;
    std::shared_ptr<MoveWindow>    _moveWindow;
    std::shared_ptr<QuitWindow>    _quitWindow;

    Passtis();

public:
    ~Passtis();

    static Passtis* instance();

    void resize();

    void init(int argc, char* argv[]);
    int  exec();
};

#endif // PASTEES_H
