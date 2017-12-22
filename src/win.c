//
// Created by yangbodong on 12/20/17.
//

#include "win.h"

void initWin(WIN *p_win, int height, int width, int starty, int startx) {
    p_win->height = height;
    p_win->width = width;
    p_win->starty = starty;
    p_win->startx = startx;
}

void initWindow() {
    initscr();
    start_color();
    cbreak();
    keypad(stdscr, TRUE);


    /* init location */
    initWin(&WIN_TITLE, LINES*0.25, COLS, 0 , 0);
    initWin(&WIN_MENU, LINES*0.75, COLS*0.60, LINES*0.25, 0);
    initWin(&WIN_NEWS, LINES*0.75, COLS*0.40, LINES*0.25, COLS*0.60);

    /*init mutex */
    pthread_mutex_init(&MUTEX, NULL);
}

WINDOW *createWindow(WIN p_win) {
    WINDOW *local_win;
    local_win = newwin(p_win.height, p_win.width, p_win.starty, p_win.startx);
    box(local_win, 0, 0);
    wrefresh(local_win);
    return local_win;
}

void clearWindow(WINDOW *win) {
    wclear(win);
    box(win, 0, 0);
    wrefresh(win);
}

void exitWindow() {
    pthread_mutex_destroy(&MUTEX);
    endwin();
}