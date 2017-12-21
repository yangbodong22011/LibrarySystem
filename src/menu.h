//
// Created by yangbodong on 12/20/17.
//

#ifndef BOOKMANAGE_MENU_H
#define BOOKMANAGE_MENU_H


#include <curses.h>

void menuLogin(WINDOW *win);
void menuAdmin(WINDOW *win);
void menuAdminUser(WINDOW *win);
void menuAdminUserFind(WINDOW *win);
void menuAdminBook(WINDOW *win);
void menuAdminBookFind(WINDOW *win);
void menuAdminBorrow(WINDOW *win);

void menuUser(WINDOW *win);

#endif //BOOKMANAGE_MENU_H
