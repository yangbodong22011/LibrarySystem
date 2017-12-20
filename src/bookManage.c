//
// Created by yangbodong on 12/20/17.
//

#include <stdio.h>
#include <ncurses.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <utmp.h>
#include <stdlib.h>

#include "bookManage.h"
#include "file.h"
#include "menu.h"
#include "win.h"

char *getTimeNow() {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return asctime(timeinfo);
}

//int Mvwprintw(WINDOW *win, int y, int x, const char *fmt, ...) {
//    LOCK;
//
//    va_list agrs;
//    va_start(agrs, fmt);
//    int ret = mvwprintw(win, y, x, fmt, agrs);
//    wrefresh(win);
//    va_end(agrs);
//
//    UNLOCK;
//    return ret;
//}

void *threadTitle(void *p) {
    WINDOW *windowTitle;
    LOCK;
    windowTitle = createWindow(WIN_TITLE);
    UNLOCK;

    /* show title and time */
    for (;;) {
        LOCK;
        mvwprintw(windowTitle, WIN_TITLE.height/2, WIN_TITLE.width/2 - 10, "Library Management System");
        if (ME) {
            mvwprintw(windowTitle, WIN_TITLE.height - 2, 1, "Welcome, %s:%s", ME->user_status == 0 ? "admin" : "user",
                      ME->user_name);
        }
        mvwprintw(windowTitle, WIN_TITLE.height-2, WIN_TITLE.width-30, "%s", getTimeNow());
        wrefresh(windowTitle);
        UNLOCK;
        sleep(1);
    }
}

void initMe(user *u) {
    if (u) {
        ME = (user *) malloc(sizeof(user));
        ME->user_id = u->user_id;
        strcpy(ME->user_stid, u->user_stid);
        strcpy(ME->user_name, u->user_name);
        strcpy(ME->user_address, u->user_address);
        strcpy(ME->user_mail, u->user_mail);
        ME->user_status = u->user_status;
    }
}

int loginSys(WINDOW *win) {
    char user_stid[16];
    char passwd_content[32];
    user *u;
    passwd *p;

    LOCK;
    clearWindow(win);
    mvwprintw(win, 1, 1, "1. please input userID:");
    wrefresh(win);
    UNLOCK;

    wscanw(win, "%s", user_stid);

    LOCK;
    mvwprintw(win, 2, 1, "2. please input userPasswd:");
    wrefresh(win);
    UNLOCK;

    noecho();
    wscanw(win, "%s", passwd_content);
    echo();

    u = getUserByUserStid(user_stid);
    if (u == NULL) {
        return -1;
    }

    p = getPasswdByUserId(u->user_id);

    if (strcmp(passwd_content, p->passwd_content) == 0) {
        initMe(u);
        return 0;
    } else {
        return -1;
    }

//    LOCK;
//    mvwprintw(win, 8, 1, "%s %s", ME.user_name, p->passwd_content);
//    wrefresh(win);
//    UNLOCK;
}

void adminFunc(WINDOW *win) {
    int choice;

    LOCK;
    clearWindow(win);
    UNLOCK;
    do {
        LOCK;
        menuAdmin(win);
        UNLOCK;
        wscanw(win, "%d", &choice);
        switch (choice) {
            case 1:

                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 0:
                system("clear");
                exit(1);
            default:
                LOCK;
                mvwprintw(win, WIN_MENU.height-2, 1, "Error: choice invalid");
                wrefresh(win);
                UNLOCK;
        }
    } while (choice != 0);
}

void userFunc(WINDOW *win) {
    int choice;

    LOCK;
    clearWindow(win);
    UNLOCK;
    do {
        LOCK;
        menuUser(win);
        UNLOCK;
        wscanw(win, "%d", &choice);
        switch (choice) {
            case 1:
                break;
            case 2:
                break;
            case 0:
                system("clear");
                exit(1);
            default:
                LOCK;
                mvwprintw(win, WIN_MENU.height-2, 1, "Error: choice invalid");
                wrefresh(win);
                UNLOCK;
        }
    } while (choice != 0);
}

void loop(WINDOW *win) {
    int ret;
    ret = loginSys(win);
    if (ret != 0) {
        LOCK;
        clearWindow(win);
        mvwprintw(win, WIN_MENU.height-2, 1, "Error: username or passwd error, relogin");
        wrefresh(win);
        UNLOCK;
        return ;
    }

    /* different user have different power */
    if (ME->user_status == 0) {
        adminFunc(win);
    } else if (ME->user_status == 1) {
        userFunc(win);
    }

}

void *threadMenu(void *p) {
    WINDOW *windowMenu;
    LOCK;
    windowMenu = createWindow(WIN_MENU);
    UNLOCK;

    int choice;
    do {
        LOCK;
        menuLogin(windowMenu);
        UNLOCK;
        wscanw(windowMenu, "%d", &choice);
        //mvwprintw(windowMenu, 8, 1, "%d", choice);
        //wrefresh(windowMenu);
        switch (choice) {
            case 1:
                /* login, we begin load data */
                dataInit();
                loop(windowMenu);
                break;
            case 0:
                system("clear");
                exit(1);
            default:
                LOCK;
                mvwprintw(windowMenu, WIN_MENU.height-2, 1, "Error: choice invalid");
                wrefresh(windowMenu);
                UNLOCK;
        }
    } while (choice != 0);
}

void *threadNews(void *p) {
    WINDOW *windowNews;
    LOCK;
    windowNews = createWindow(WIN_NEWS);
    UNLOCK;

    int x = 1;
    for (;;) {
        LOCK;
        for (int i = 0; i < 5; ++i) {
            mvwprintw(windowNews, x, 2, "%d. %s   %s\n", i, getTimeNow(), "a borrow a book from c");
            x += 2;
        }
        wrefresh(windowNews);
        //wclear(windowNews);
        UNLOCK;

        sleep(5);
    }
}

int main(int argc, char *argv[])
{
    pthread_t pidTitle, pidMenu, pidNews;

    initWindow();

    pthread_create(&pidTitle, NULL, threadTitle, NULL);
    pthread_create(&pidMenu, NULL, threadMenu, NULL);
    pthread_create(&pidNews, NULL, threadNews, NULL);

    pthread_join(pidTitle, NULL);
    pthread_join(pidMenu, NULL);
    pthread_join(pidNews, NULL);

    exitWindow();
    return 0;
}