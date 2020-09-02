#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <form.h>

#include "routeawarewindow.h"

class EditWindow : public RouteAwareWindow
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
    WindowAction editNode();

public:
    EditWindow();
    ~EditWindow();

    virtual WindowAction onKeyEvent(int ch);
    virtual void         update();
};

#endif // EDITWINDOW_H