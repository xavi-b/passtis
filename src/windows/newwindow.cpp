#include "newwindow.h"

NewWindow::NewWindow()
  : RouteAwareWindow(),
    _ncTitleWin(nullptr),
    _ncMsgWin(nullptr),
    _ncForm(nullptr),
    _ncFormWin(nullptr),
    _ncFormSubWin(nullptr)
{
    _message = "Leaving identity empty will create a group instead of a key.";
}

NewWindow::~NewWindow()
{
    clean();
}

void NewWindow::clean()
{
    delwin(_ncTitleWin);
    delwin(_ncMsgWin);
    unpost_form(_ncForm);
    free_form(_ncForm);
    for(size_t i = 0; i < _ncFields.size(); i++)
    {
        if(_ncFields[i])
            free_field(_ncFields[i]);
    }
    delwin(_ncFormWin);
    delwin(_ncFormSubWin);
}

WindowAction NewWindow::newNode()
{
    form_driver(_ncForm, REQ_VALIDATION);

    std::string name = field_buffer(_ncFields[0], 0);
    name.resize(name.find_first_of(' '));

    std::string identity = field_buffer(_ncFields[1], 0);
    identity.resize(identity.find_first_of(' '));

    std::string password = field_buffer(_ncFields[2], 0);
    password.resize(password.find_first_of(' '));

    std::string more = field_buffer(_ncFields[3], 0);
    more.resize(more.find_first_of(' '));

    WindowAction wa;
    wa.type = WindowAction::GoToDisplayWindow;
    wa.data = _route;

    if(name == "")
        return wa;

    if(identity != "")
        _database->addKeyNode(_route, name, identity, password, more); // TODO check if group exists
    else
        _database->nodeContent(_route + name + '/'); // TODO check if key exists

    return wa;
}

WindowAction NewWindow::onKeyEvent(int ch)
{
    WindowAction wa;

    switch(ch)
    {
        case KEY_DOWN:
            form_driver(_ncForm, REQ_NEXT_FIELD);
            form_driver(_ncForm, REQ_END_LINE);
            break;
        case KEY_UP:
            form_driver(_ncForm, REQ_PREV_FIELD);
            form_driver(_ncForm, REQ_END_LINE);
            break;
        case KEY_LEFT:
            form_driver(_ncForm, REQ_PREV_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(_ncForm, REQ_NEXT_CHAR);
            break;
        case KEY_DC:
            form_driver(_ncForm, REQ_DEL_CHAR);
            break;
        case 127: // backspace
            form_driver(_ncForm, REQ_DEL_PREV);
            break;
        case 10: // enter
            return newNode();
            break;
        case 27: // escape
            wa.type = WindowAction::GoToDisplayWindow;
            wa.data = _route;
            return wa;
            break;
        default:
            form_driver(_ncForm, ch);
            break;
    }

    wrefresh(_ncFormWin);

    return RouteAwareWindow::onKeyEvent(ch);
}

void NewWindow::update()
{
    clean();

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    int fieldLength = 32;
    int labelLength = 16;

    for(size_t i = 0; i < _ncFields.size()-1; i++)
    {
        _ncFields[i] = new_field(1, fieldLength, 2*(i+1), 1, 0, 0);
        set_field_back(_ncFields[i], A_UNDERLINE);
        field_opts_off(_ncFields[i], O_AUTOSKIP);
        field_opts_on(_ncFields[i], O_EDIT);
        set_field_type(_ncFields[i], TYPE_ALNUM, 1); // TODO issue
    }

    _ncFields[_ncFields.size()-1] = nullptr;

    _ncForm = new_form(&_ncFields[0]);

    int fcols, frows;
    scale_form(_ncForm, &frows, &fcols);

    int fieldPos = cols/2;

    _ncFormWin = newwin(frows, fcols, 1, cols/2);
    _ncFormSubWin = derwin(_ncFormWin, frows, fcols, 0, 0);

    set_form_win(_ncForm, _ncFormWin);
    set_form_sub(_ncForm, _ncFormSubWin);

    post_form(_ncForm);

    refresh();

    mvprintw(3, cols/2-fcols, "Name:");
    mvprintw(5, cols/2-fcols, "Identity:");
    mvprintw(7, cols/2-fcols, "Password:");
    mvprintw(9, cols/2-fcols, "More:");

    if(_database)
    {
        _ncTitleWin = newwin(1, cols, 0, 0);
        wbkgd(_ncTitleWin, COLOR_PAIR(WindowColor::Title));

        std::string title = _database->filename() + " - " + _route;
        mvwprintw(_ncTitleWin, 0, (cols-title.size())/2, title.c_str());

        wrefresh(_ncTitleWin);
    }

    _ncMsgWin = newwin(1, cols, rows-1, 0);
    wbkgd(_ncMsgWin, COLOR_PAIR(WindowColor::Title));

    mvwprintw(_ncMsgWin, 0, 0, _message.c_str());

    wrefresh(_ncMsgWin);

    wrefresh(_ncFormWin);
    move(3, cols/2+1);
}
