#ifndef QUITWINDOW_H
#define QUITWINDOW_H

#include <form.h>

#include "window.h"

class QuitWindow : public Window
{
private:
    WINDOW* _ncTitleWin;
    WINDOW* _ncMsgWin;
    std::array<FIELD*, 2> _ncFields;
    FORM* _ncForm;
    std::string _message;

protected:
    virtual void clean();
    WindowAction tryQuit();

public:
    QuitWindow();
    ~QuitWindow();

    virtual WindowAction onKeyEvent(int ch);

    virtual void update();
};

#endif // QUITWINDOW_H