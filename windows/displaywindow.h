#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include "window.h"

class DisplayWindow : public Window
{
public:
    DisplayWindow();

    virtual WindowAction onKeyEvent(int ch);
};

#endif // DISPLAYWINDOW_H