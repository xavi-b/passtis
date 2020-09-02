#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <form.h>

#include "routeawarewindow.h"

class NewWindow : public RouteAwareWindow
{
private:
    WINDOW*               _ncTitleWin;
    WINDOW*               _ncMsgWin;
    std::array<FIELD*, 5> _ncFields;
    FORM*                 _ncForm;
    WINDOW*               _ncFormWin;
    WINDOW*               _ncFormSubWin;
    std::string           _message;

protected:
    virtual void clean();
    WindowAction newNode();

public:
    NewWindow();
    ~NewWindow();

    virtual WindowAction onKeyEvent(int ch);
    virtual void         update();
};

#endif // NEWWINDOW_H