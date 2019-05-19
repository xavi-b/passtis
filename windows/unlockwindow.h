#ifndef UNLOCKWINDOW_H
#define UNLOCKWINDOW_H

#include <form.h>

#include "window.h"

class UnlockWindow : public Window
{
private:
    WINDOW* _ncTitleWin;
    FIELD* _ncFields[2];
    FORM* _ncForm;

protected:
    virtual void clean();

public:
    UnlockWindow();
    ~UnlockWindow();

    virtual WindowAction onKeyEvent(int ch);

    virtual void update();
};

#endif // UNLOCKWINDOW_H