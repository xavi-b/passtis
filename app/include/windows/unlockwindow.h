#ifndef UNLOCKWINDOW_H
#define UNLOCKWINDOW_H

#include <form.h>

#include "window.h"

class UnlockWindow : public Window
{
private:
    WINDOW*               _ncTitleWin;
    WINDOW*               _ncMsgWin;
    std::array<FIELD*, 2> _ncFields;
    FORM*                 _ncForm;
    std::string           _message;
    std::string           _filename;

protected:
    virtual void clean();
    WindowAction tryUnlock();

public:
    UnlockWindow(std::string const& filename);
    ~UnlockWindow();

    virtual WindowAction onKeyEvent(int ch);

    virtual void update();
};

#endif // UNLOCKWINDOW_H