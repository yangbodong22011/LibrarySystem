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
#include "log.h"

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
    mvwprintw(win, 2, 1, "2. please input userPasswd, <not display>:");
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
        /* if passwd right, we initMe */
        initMe(u);
        return 0;
    } else {
        return -1;
    }
}

void adminReaderList(WINDOW *win) {
    int index = 1;
    LOCK;
    clearWindow(win);
    user *p = USER_HEAD->next;
    while (p) {
        mvwprintw(win, index, 1, "%d. %s %s %s %s %s\n", p->user_id, p->user_stid, p->user_name, p->user_address,
                p->user_mail, p->user_status == 0 ? "admin":"user");
        p = p->next;
        index += 1;
    }
    mvwprintw(win, WIN_MENU.height-2, 1, "Press any key to <quit>");
    wrefresh(win);
    UNLOCK;

    noecho();
    wscanw(win, "%d", &index);
    echo();
    LOCK;
    clearWindow(win);
    UNLOCK;
}

void adminReaderAdd(WINDOW *win) {
    int choice;
    int retu, retp, retl;
    user *p, *q;
    passwd *v, *w;
    log *y, *z;

    do {
        p = USER_HEAD;
        while (p->next) p = p->next;
        q = (user *) malloc(sizeof(user));

        v = PASSWD_HEAD;
        while (v->next) v = v->next;
        w = (passwd *) malloc(sizeof(passwd));

        y = LOG_HEAD;
        while (y->next) y = y->next;
        z = (log *) malloc(sizeof(log));

        q->user_id = ++USER_MAXID;
        w->passwd_id = ++PASSWD_MAXID;
        z->log_id = ++LOG_MAXID;

        LOCK;
        clearWindow(win);
        mvwprintw(win, 1, 1, "1. please input user_stid:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%s", q->user_stid);

        LOCK;
        mvwprintw(win, 2, 1, "2. please input user_name:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%s", q->user_name);

        LOCK;
        mvwprintw(win, 3, 1, "3. please input user_address:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%s", q->user_address);

        LOCK;
        mvwprintw(win, 4, 1, "4. please input user_mail:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%s", q->user_mail);

        LOCK;
        mvwprintw(win, 5, 1, "5. please input user_status, root<0>, user<1>:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%d", &(q->user_status));

        LOCK;
        mvwprintw(win, 6, 1, "6. please input user_passwd, <not display>:");
        wrefresh(win);
        UNLOCK;
        noecho();
        wscanw(win, "%s", w->passwd_content);
        echo();

        /* init passwd->user_id */
        w->user_id = q->user_id;

        /* init log  */
        z->user_id = q->user_id;
        z->book_id = -1;
        strcpy(z->log_content, LOG_ADD_USER);
        z->log_status = 0;

        /* add q and w */
        p->next = q;
        v->next = w;
        y->next = z;

        retu = writeUser();
        retp = writePasswd();
        retl = writeLog();
        /*
         * writeUser();
         * wrtitePasswd();
         * writeLog();
         * */
        if (retp == 0 && retu == 0 && retl == 0) {
            LOCK;
            mvwprintw(win, WIN_MENU.height-2, 1, "reader add success");
            wrefresh(win);
            UNLOCK;
        } else {
            LOCK;
            mvwprintw(win, WIN_MENU.height-2, 1, "reader add fail");
            wrefresh(win);
            clearWindow(win);
            UNLOCK;
            return ;
        }

        LOCK;
        mvwprintw(win, 7, 1, "add more ? yes<1>, no<0>:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%d", &choice);

        LOCK;
        clearWindow(win);
        UNLOCK;
    } while(choice != 0);
}

void adminReaderDel(WINDOW *win) {
    int choice;
    int delFlag;
    char user_stid[16];
    int retu, retp, retl;
    user *p, *q;
    passwd *v, *w;
    log *y, *z;

    do {
        delFlag = 0;
        p = USER_HEAD;
        v = PASSWD_HEAD;
        memset(user_stid, 0, 16);

        y = LOG_HEAD;
        while (y->next) y = y->next;
        z = (log *) malloc(sizeof(log));

        LOCK;
        clearWindow(win);
        mvwprintw(win, 1, 1, "1. please input del_user_stid:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%s", user_stid);

        while (p->next) {
            if (strcmp(p->next->user_stid, user_stid) == 0) {
                q = p->next;
                p->next = p->next->next;
                delFlag = 1;
                break;
            }
            p = p->next;
        }

        if (delFlag == 0) {
            LOCK;
            mvwprintw(win, WIN_MENU.height-2, 1, "not found this user");
            wrefresh(win);
            UNLOCK;
        } else {
            LOCK;
            mvwprintw(win, WIN_MENU.height-2, 1, "delFlag: %d, q->user_id:%d", delFlag, q->user_id);
            wrefresh(win);
            UNLOCK;

            while (v->next) {
                if (q->user_id == v->next->user_id) {
                    w = v->next;
                    v->next = v->next->next;
                    break;
                }
                v = v->next;
            }

            /* init log  */
            z->log_id = ++LOG_MAXID;
            z->user_id = q->user_id;
            z->book_id = -1;
            strcpy(z->log_content, LOG_DEL_USER);
            z->log_status = 0;

            y->next = z;

            if (q) free(q);
            if (w) free(w);

            retu = writeUser();
            retp = writePasswd();
            retl = writeLog();

            if (retp == 0 && retu == 0 && retl == 0) {
                LOCK;
                mvwprintw(win, WIN_MENU.height-2, 1, "reader del success");
                wrefresh(win);
                UNLOCK;
            }
        }

        LOCK;
        mvwprintw(win, 7, 1, "del more ? yes<1>, no<0>:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%d", &choice);

        LOCK;
        clearWindow(win);
        UNLOCK;
    } while(choice != 0);
}


void adminReaderModify(WINDOW *win) {
    int index;
    char user_stid[16];
    user *p;
    int retu;

    LOCK;
    clearWindow(win);
    mvwprintw(win, 1, 1, "1. please input user_stid:");
    wrefresh(win);
    UNLOCK;
    wscanw(win, "%s", user_stid);

    p = getUserByUserStid(user_stid);

    if (p) {
        LOCK;
        mvwprintw(win, 3, 1, "%d. %s %s %s %s %s\n", p->user_id, p->user_stid, p->user_name, p->user_address,
                  p->user_mail, p->user_status == 0 ? "admin":"user");

        mvwprintw(win, 4, 1, "1. please input user_stid:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%s", p->user_stid);

        LOCK;
        mvwprintw(win, 5, 1, "2. please input user_name:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%s", p->user_name);

        LOCK;
        mvwprintw(win, 6, 1, "3. please input user_address:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%s", p->user_address);

        LOCK;
        mvwprintw(win, 7, 1, "4. please input user_mail:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%s", p->user_mail);

        LOCK;
        mvwprintw(win, 8, 1, "5. please input user_status, root<0>, user<1>:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%d", &(p->user_status));

        retu = writeUser();

        if (retu == 0) {
            LOCK;
            mvwprintw(win, WIN_MENU.height-2, 1, "modify user success, Press any key to <quit>");
            wrefresh(win);
            UNLOCK;
        } else {
            LOCK;
            mvwprintw(win, WIN_MENU.height-2, 1, "write file faild, Press any key to <quit>");
            wrefresh(win);
            UNLOCK;
        }

    } else {
        LOCK;
        mvwprintw(win, WIN_MENU.height-2, 1, "not found this user, Press any key to <quit>");
        UNLOCK;
    }

    noecho();
    wscanw(win, "%d", &index);
    echo();
    LOCK;
    clearWindow(win);
    UNLOCK;
}

void adminReaderFindByUserStid(WINDOW *win) {
    int index;
    char user_stid[16];
    user *p;

    LOCK;
    clearWindow(win);
    mvwprintw(win, 1, 1, "1. please input user_stid:");
    wrefresh(win);
    UNLOCK;
    wscanw(win, "%s", user_stid);

    p = getUserByUserStid(user_stid);

    if (p) {
        LOCK;
        mvwprintw(win, 3, 1, "%d. %s %s %s %s %s\n", p->user_id, p->user_stid, p->user_name, p->user_address,
                  p->user_mail, p->user_status == 0 ? "admin":"user");
        mvwprintw(win, WIN_MENU.height-2, 1, "Press any key to <quit>");
        wrefresh(win);
        UNLOCK;
    } else {
        LOCK;
        mvwprintw(win, WIN_MENU.height-2, 1, "not found this user, Press any key to <quit>");
        UNLOCK;
    }

    noecho();
    wscanw(win, "%d", &index);
    echo();
    LOCK;
    clearWindow(win);
    UNLOCK;
}

void adminReaderFindByUserName(WINDOW *win) {
    int index = 3;
    char user_name[16];
    user *p;

    LOCK;
    clearWindow(win);
    mvwprintw(win, 1, 1, "1. please input user_name:");
    wrefresh(win);
    UNLOCK;
    wscanw(win, "%s", user_name);

    //p = getUserByUserStid(user_stid);
    p = getUserByUserName(user_name);

    if (p) {
        LOCK;
        while (p) {
            mvwprintw(win, index, 1, "%d. %s %s %s %s %s\n", p->user_id, p->user_stid, p->user_name, p->user_address,
                      p->user_mail, p->user_status == 0 ? "admin" : "user");
            p = p->next;
            index += 1;
        }
        mvwprintw(win, WIN_MENU.height - 2, 1, "Press any key to <quit>");
        wrefresh(win);
        UNLOCK;
    } else {
        LOCK;
        mvwprintw(win, WIN_MENU.height-2, 1, "not found this name, Press any key to <quit>");
        UNLOCK;
    }

    noecho();
    wscanw(win, "%d", &index);
    echo();
    LOCK;
    clearWindow(win);
    UNLOCK;
}


void adminReaderFind(WINDOW *win) {
    int choice;
    LOCK;
    clearWindow(win);
    UNLOCK;

    do {
        LOCK;
        menuAdminUserFind(win);
        UNLOCK;
        wscanw(win, "%d", &choice);
        switch (choice) {
            case 1:
                adminReaderFindByUserStid(win);
                break;
            case 2:
                adminReaderFindByUserName(win);
                break;
            default:
                LOCK;
                mvwprintw(win, WIN_MENU.height-2, 1, "Error: choice invalid");
                wrefresh(win);
                UNLOCK;
        }
    } while (choice != 0);
    LOCK;
    /* clearWindow for back to previous */
    clearWindow(win);
    UNLOCK;
}

void adminReader(WINDOW *win) {
    int choice;

    LOCK;
    clearWindow(win);
    UNLOCK;
    do {
        LOCK;
        menuAdminUser(win);
        UNLOCK;
        wscanw(win, "%d", &choice);
        switch (choice) {
            case 1:
                adminReaderList(win);
                break;
            case 2:
                adminReaderAdd(win);
                break;
            case 3:
                adminReaderDel(win);
                break;
            case 4:
                adminReaderModify(win);
                break;
            case 5:
                adminReaderFind(win);
                break;
            default:
                LOCK;
                mvwprintw(win, WIN_MENU.height-2, 1, "Error: choice invalid");
                wrefresh(win);
                UNLOCK;
        }
    } while (choice != 0);
    LOCK;
    /* clearWindow for back to previous */
    clearWindow(win);
    UNLOCK;
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
                adminReader(win);
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            default:
                LOCK;
                mvwprintw(win, WIN_MENU.height-2, 1, "Error: choice invalid");
                wrefresh(win);
                UNLOCK;
        }
    } while (choice != 0);
    system("clear");
    delwin(win);
    exit(1);
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
            default:
                LOCK;
                mvwprintw(win, WIN_MENU.height-2, 1, "Error: choice invalid");
                wrefresh(win);
                UNLOCK;
        }
    } while (choice != 0);
    system("clear");
    delwin(win);
    exit(1);
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
            default:
                LOCK;
                mvwprintw(windowMenu, WIN_MENU.height-2, 1, "Error: choice invalid");
                wrefresh(windowMenu);
                UNLOCK;
        }
    } while (choice != 0);
    system("clear");
    delwin(windowMenu);
    exit(1);
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