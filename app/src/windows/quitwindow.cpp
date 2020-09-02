#include "windows/quitwindow.h"

QuitWindow::QuitWindow() : Window(), _ncTitleWin(nullptr), _ncMsgWin(nullptr), _ncForm(nullptr), _message("")
{
    _ncFields[0] = nullptr;
    _ncFields[1] = nullptr;
}

QuitWindow::~QuitWindow()
{
    clean();
}

void QuitWindow::clean()
{
    delwin(_ncTitleWin);
    delwin(_ncMsgWin);
    unpost_form(_ncForm);
    free_form(_ncForm);
    free_field(_ncFields[0]);
}

WindowAction QuitWindow::tryQuit()
{
    WindowAction wa;

    form_driver(_ncForm, REQ_VALIDATION);
    std::string filename = field_buffer(_ncFields[0], 0);
    filename.resize(filename.find_first_of(' '));

    if (Database::instance()->save(filename))
    {
        wa.type = WindowAction::Quit;
    }
    else
    {
        _message = "SAVE FAILED !";
        mvwprintw(_ncMsgWin, 0, 0, _message.c_str());
        wrefresh(_ncMsgWin);
    }

    return wa;
}

WindowAction QuitWindow::onKeyEvent(int ch)
{
    WindowAction wa;

    switch (ch)
    {
    case KEY_DOWN:
    case 9: // tab
        form_driver(_ncForm, REQ_NEXT_FIELD);
        form_driver(_ncForm, REQ_END_LINE);
        break;
    case KEY_UP:
    case KEY_BTAB:
        form_driver(_ncForm, REQ_PREV_FIELD);
        form_driver(_ncForm, REQ_END_LINE);
        break;
    case 127: // backspace
        form_driver(_ncForm, REQ_DEL_PREV);
        break;
    case 27: // escape
        wa.type = WindowAction::GoToDisplayWindow;
        return wa;
        break;
    case 10: // enter
        return tryQuit();
        break;
    default:
        if (ch == '/' && current_field(_ncForm) == _ncFields[0])
        {
            form_driver(_ncForm, '-');
            break;
        }
        form_driver(_ncForm, ch);
        break;
    }

    if (std::string(keyname(ch)) == "^C")
    {
        wa.type = WindowAction::Quit;
        return wa;
    }

    return Window::onKeyEvent(ch);
}

void QuitWindow::update()
{
    clean();

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    int fieldLength = 32;
    int labelLength = 16;

    int fieldPos = (cols - labelLength - fieldLength) / 2 + labelLength;
    _ncFields[0] = new_field(1, fieldLength, 2, fieldPos, 0, 0);
    _ncFields[1] = nullptr;

    set_field_back(_ncFields[0], A_UNDERLINE);
    field_opts_off(_ncFields[0], O_AUTOSKIP);
    field_opts_on(_ncFields[0], O_EDIT);
    // set_field_type(_ncFields[0], TYPE_ALNUM, 1); // TODO issue

    set_field_buffer(_ncFields[0], 0, Database::instance()->filename().c_str());

    _ncForm = new_form(&_ncFields[0]);
    post_form(_ncForm);
    refresh();

    _ncTitleWin = newwin(1, cols, 0, 0);
    wbkgd(_ncTitleWin, COLOR_PAIR(WindowColor::Title) | ' ');

    std::string title = Database::instance()->filename();
    mvwprintw(_ncTitleWin, 0, (cols - title.size()) / 2, title.c_str());

    wrefresh(_ncTitleWin);

    _ncMsgWin = newwin(1, cols, rows - 1, 0);
    wbkgd(_ncMsgWin, COLOR_PAIR(WindowColor::Title) | ' ');

    mvwprintw(_ncMsgWin, 0, 0, _message.c_str());

    wrefresh(_ncMsgWin);

    mvprintw(2, (cols - labelLength - fieldLength) / 2, "Filename:");

    form_driver(_ncForm, REQ_END_LINE);
}