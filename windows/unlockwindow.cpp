#include "unlockwindow.h"

UnlockWindow::UnlockWindow()
  : Window(),
    _ncTitleWin(nullptr),
    _ncForm(nullptr)
{
    _ncFields[0] = nullptr;
    _ncFields[1] = nullptr;
}

UnlockWindow::~UnlockWindow()
{
    clean();
}

void UnlockWindow::clean()
{
    delwin(_ncTitleWin);
    unpost_form(_ncForm);
    free_form(_ncForm);
    free_field(_ncFields[0]);
}

WindowAction UnlockWindow::onKeyEvent(int ch)
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
        default:
            form_driver(_ncForm, ch);
            break;
    }

    return Window::onKeyEvent(ch);
}

void UnlockWindow::update()
{
    clean();

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    int fieldLength = 32;
    int labelLength = 16;

    int fieldPos = (cols-labelLength-fieldLength)/2+labelLength;
    _ncFields[0] = new_field(1, fieldLength, 2, fieldPos, 0, 0);
    _ncFields[1] = nullptr;

    set_field_back(_ncFields[0], A_UNDERLINE);
    field_opts_off(_ncFields[0], O_AUTOSKIP);

    _ncForm = new_form(_ncFields);
    post_form(_ncForm);
    refresh();

    if(_database)
    {
        _ncTitleWin = newwin(1, cols, 0, 0);
        wbkgd(_ncTitleWin, COLOR_PAIR(WindowColor::Title));

        std::string title = _database->filename();
        mvwprintw(_ncTitleWin, 0, (cols-title.size())/2, title.c_str());

        wrefresh(_ncTitleWin);
    }

    mvprintw(2, (cols-labelLength-fieldLength)/2, "Password:");
    move(2, fieldPos);

    refresh();

    Window::update();
}