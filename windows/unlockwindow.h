#ifndef UNLOCKWINDOW_H
#define UNLOCKWINDOW_H

#include <form.h>

#include "window.h"

class UnlockWindow : public Window
{
private:
    FIELD *field[3];
    FORM  *my_form;

public:
    UnlockWindow();

    virtual WindowAction onKeyEvent(int ch);
};

#endif // UNLOCKWINDOW_H