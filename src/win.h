//
// Created by yangbodong on 12/20/17.
//

#ifndef BOOKMANAGE_WIN_H
#define BOOKMANAGE_WIN_H

#include <curses.h>
#include <sys/types.h>
#include <pthread.h>

typedef struct _WIN_struct {
    int startx, starty;
    int height, width;
} WIN;

/* glob vars */
pthread_mutex_t MUTEX;  /* mutex for sync display */
WIN WIN_TITLE; /* title win */
WIN WIN_MENU; /* Main menu win */
WIN WIN_NEWS; /* win news */


void initWin(WIN *p_win, int height, int width, int starty, int startx);
void initWindow();
WINDOW *createWindow(WIN p_win);
void clearWindow(WINDOW *win);
void exitWindow();

#endif //BOOKMANAGE_WIN_H
