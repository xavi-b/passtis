#ifndef QUITWINDOW_H
#define QUITWINDOW_H

#include "window.h"

class QuitWindow : public Window
{
public:
    QuitWindow();

    virtual WindowAction onKeyEvent(int ch);
};

#endif // QUITWINDOW_H