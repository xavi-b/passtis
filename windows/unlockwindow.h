#ifndef UNLOCKWINDOW_H
#define UNLOCKWINDOW_H

#include <form.h>

#include "window.h"

class UnlockWindow : public Window
{
private:
    WINDOW* _ncTitleWin;
    WINDOW* _ncMsgWin;
    FIELD* _ncFields[2];
    FORM* _ncForm;
    std::string _message;

protected:
    virtual void clean();
    WindowAction tryUnlock();

public:
    UnlockWindow();
    ~UnlockWindow();

    virtual WindowAction onKeyEvent(int ch);

    virtual void update();
};

#endif // UNLOCKWINDOW_H