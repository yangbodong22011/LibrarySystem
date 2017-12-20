//
// Created by yangbodong on 12/20/17.
//

#include "menu.h"

void menuLogin(WINDOW *win) {
    mvwprintw(win, 1, 1, "1. login");
    mvwprintw(win, 2, 1, "0. exit");
    mvwprintw(win, 3, 1, "input choice: ");
    wrefresh(win);
}

void menuAdmin(WINDOW *win) {
    mvwprintw(win, 1, 1, "1. reader management");
    mvwprintw(win, 2, 1, "2. book management");
    mvwprintw(win, 3, 1, "3. borrow management");
    mvwprintw(win, 4, 1, "4. info summary");
    mvwprintw(win, 5, 1, "0. exit");
    mvwprintw(win, 6, 1, "input choice:");
    wrefresh(win);
}

void menuUser(WINDOW *win) {
    mvwprintw(win, 1, 1, "1. book management");
    mvwprintw(win, 2, 1, "2. borrow management");
    mvwprintw(win, 3, 1, "0. exit");
    mvwprintw(win, 4, 1, "input choice:");
    wrefresh(win);
}