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
        q->next = NULL;

        v = PASSWD_HEAD;
        while (v->next) v = v->next;
        w = (passwd *) malloc(sizeof(passwd));
        w->next = NULL;

        y = LOG_HEAD;
        while (y->next) y = y->next;
        z = (log *) malloc(sizeof(log));
        z->next = NULL;

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
        z->next = NULL;

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
    user *p, *q, *t;

    LOCK;
    clearWindow(win);
    mvwprintw(win, 1, 1, "1. please input user_name:");
    wrefresh(win);
    UNLOCK;
    wscanw(win, "%s", user_name);

    q = getUserByUserName(user_name);
    p = q->next;

    if (p) {
        LOCK;
        while (p) {
            mvwprintw(win, index, 1, "%d. %s %s %s %s %s\n", p->user_id, p->user_stid, p->user_name, p->user_address,
                      p->user_mail, p->user_status == 0 ? "admin" : "user");
            t = p;
            p = p->next;
            /* need to free */
            free(t);
            index += 1;
        }
        free(q);
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

void adminBookAdd(WINDOW *win) {
    int choice;
    int retb, retl;
    book *p, *q;
    log *y, *z;

    do {
        p = BOOK_HEAD;
        while (p->next) p = p->next;
        q = (book *) malloc(sizeof(book));
        q->next = NULL;


        y = LOG_HEAD;
        while (y->next) y = y->next;
        z = (log *) malloc(sizeof(log));
        z->next = NULL;

        q->book_id = ++BOOK_MAXID;
        z->log_id = ++LOG_MAXID;

        LOCK;
        clearWindow(win);
        mvwprintw(win, 1, 1, "1. please input book_isbn:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%s", q->book_isbn);

        LOCK;
        mvwprintw(win, 2, 1, "2. please input book_name:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%s", q->book_name);

        LOCK;
        mvwprintw(win, 3, 1, "3. please input book_author:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%s", q->book_author);

        LOCK;
        mvwprintw(win, 4, 1, "4. please input book_number:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%d", &(q->book_number));

        LOCK;
        mvwprintw(win, 5, 1, "5. please input book_price:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%lf", &(q->book_price));

        /* init log  */
        z->user_id = ME->user_id;
        z->book_id = q->book_id;
        strcpy(z->log_content, LOG_ADD_BOOK);
        z->log_status = 1;

        /* add q and w */
        p->next = q;
        y->next = z;

        retb = writeBook();
        retl = writeLog();
        /*
         * writeUser();
         * wrtitePasswd();
         * writeLog();
         * */
        if (retb == 0 && retl == 0) {
            LOCK;
            mvwprintw(win, WIN_MENU.height-2, 1, "book add success");
            wrefresh(win);
            UNLOCK;
        } else {
            LOCK;
            clearWindow(win);
            mvwprintw(win, WIN_MENU.height-2, 1, "book add fail");
            wrefresh(win);
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
void adminBookList(WINDOW *win) {
    int index = 1;
    LOCK;
    clearWindow(win);
    book *p = BOOK_HEAD->next;
    while (p) {
        mvwprintw(win, index, 1, "%d. %s %s %s %d %f\n", p->book_id, p->book_isbn, p->book_name, p->book_author,
                  p->book_number, p->book_price);
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


void adminBookModify(WINDOW *win) {

}

void adminBookDel(WINDOW *win) {
    int choice;
    int delFlag;
    char book_isbn[16];
    int retb, retl;
    book *p, *q;
    log *y, *z;

    do {
        delFlag = 0;
        p = BOOK_HEAD;
        memset(book_isbn, 0, 16);

        y = LOG_HEAD;
        while (y->next) y = y->next;
        z = (log *) malloc(sizeof(log));
        z->next = NULL;

        LOCK;
        clearWindow(win);
        mvwprintw(win, 1, 1, "1. please input del_book_isbn:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%s", book_isbn);

        while (p->next) {
            if (strcmp(p->next->book_isbn, book_isbn) == 0) {
                q = p->next;
                p->next = p->next->next;
                delFlag = 1;
                break;
            }
            p = p->next;
        }

        if (delFlag == 0) {
            LOCK;
            mvwprintw(win, WIN_MENU.height-2, 1, "not found this book");
            wrefresh(win);
            UNLOCK;
        } else {

            /* init log  */
            z->log_id = ++LOG_MAXID;
            z->user_id = ME->user_id;
            z->book_id = q->book_id;
            strcpy(z->log_content, LOG_DEL_BOOK);
            z->log_status = 1;

            y->next = z;

            if (q) free(q);

            retb = writeBook();
            retl = writeLog();

            if (retb == 0 && retl == 0) {
                LOCK;
                mvwprintw(win, WIN_MENU.height-2, 1, "book del success");
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

void adminBookFindByBookIsbn(WINDOW *win) {
    int index;
    char book_isbn[16];
    book *p;

    LOCK;
    clearWindow(win);
    mvwprintw(win, 1, 1, "1. please input book_isbn:");
    wrefresh(win);
    UNLOCK;
    wscanw(win, "%s", book_isbn);

    p = getBookByBookIsbn(book_isbn);

    if (p) {
        LOCK;
        mvwprintw(win, 3, 1, "%d. %s %s %s %d %lf\n", p->book_id, p->book_isbn, p->book_name, p->book_author,
                  p->book_number, p->book_price);
        mvwprintw(win, WIN_MENU.height-2, 1, "Press any key to <quit>");
        wrefresh(win);
        UNLOCK;
    } else {
        LOCK;
        mvwprintw(win, WIN_MENU.height-2, 1, "not found this book, Press any key to <quit>");
        UNLOCK;
    }

    noecho();
    wscanw(win, "%d", &index);
    echo();
    LOCK;
    clearWindow(win);
    UNLOCK;
}

void adminBookFindByBookName(WINDOW *win) {
    int index = 3;
    char book_name[16];
    book *p, *q, *t;

    LOCK;
    clearWindow(win);
    mvwprintw(win, 1, 1, "1. please input book_name:");
    wrefresh(win);
    UNLOCK;
    wscanw(win, "%s", book_name);

    q = getBookByBookName(book_name);
    p = q->next;

    if (p) {
        LOCK;
        while (p) {
            mvwprintw(win, index, 1, "%d. %s %s %s %d %lf\n", p->book_id, p->book_isbn, p->book_name, p->book_author,
                      p->book_number, p->book_price);
            t = p;
            p = p->next;
            /* need to free */
            free(t);
            index += 1;
        }
        free(q);
        mvwprintw(win, WIN_MENU.height - 2, 1, "Press any key to <quit>");
        wrefresh(win);
        UNLOCK;
    } else {
        LOCK;
        mvwprintw(win, WIN_MENU.height-2, 1, "not found this book, Press any key to <quit>");
        UNLOCK;
    }

    noecho();
    wscanw(win, "%d", &index);
    echo();
    LOCK;
    clearWindow(win);
    UNLOCK;
}


void adminBookFind(WINDOW *win) {
    int choice;
    LOCK;
    clearWindow(win);
    UNLOCK;

    do {
        LOCK;
        menuAdminBookFind(win);
        UNLOCK;
        wscanw(win, "%d", &choice);
        switch (choice) {
            case 1:
                adminBookFindByBookIsbn(win);
                break;
            case 2:
                adminBookFindByBookName(win);
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

void adminBook(WINDOW *win) {
    int choice;

    LOCK;
    clearWindow(win);
    UNLOCK;
    do {
        LOCK;
        menuAdminBook(win);
        UNLOCK;
        wscanw(win, "%d", &choice);
        switch (choice) {
            case 1:
                adminBookList(win);
                break;
            case 2:
                adminBookAdd(win);
                break;
            case 3:
                adminBookDel(win);
                break;
            case 4:
                adminBookModify(win);
                break;
            case 5:
                adminBookFind(win);
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

void adminBorrowBorrow(WINDOW *win) {
    char book_isbn[16];
    char user_stid[16];
    int choice;
    int ret, retb, retl;
    borrow *p, *q;
    log *y, *z;
    book *b;
    user *u;

    do {
        p = BORROW_HEAD;
        while (p->next) p = p->next;
        q = (borrow *) malloc(sizeof(borrow));
        q->next = NULL;


        y = LOG_HEAD;
        while (y->next) y = y->next;
        z = (log *) malloc(sizeof(log));
        z->next = NULL;

        q->borrow_id = ++BORROW_MAXID;
        z->log_id = ++LOG_MAXID;

        LOCK;
        clearWindow(win);
        mvwprintw(win, 1, 1, "1. please input book_isbn:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%s", book_isbn);

        b = getBookByBookIsbn(book_isbn);

        if (b && b->book_number > 0) {
            q->book_id = b->book_id;
            b->book_number--;

            LOCK;
            mvwprintw(win, 2, 1, "2. please input user_stid:");
            wrefresh(win);
            UNLOCK;
            wscanw(win, "%s", user_stid);

            u = getUserByUserStid(user_stid);

            if (u) {
                q->user_id = u->user_id;
            } else {
                LOCK;
                clearWindow(win);
                mvwprintw(win, WIN_MENU.height-2, 1, "user not found");
                wrefresh(win);
                UNLOCK;
                free(q);
                free(z);
                return ;
            }

            /* init log  */
            z->user_id = u->user_id;
            z->book_id = b->book_id;
            strcpy(z->log_content, LOG_BORROW_BOOK);
            z->log_status = 1;

            /* add q and w */
            p->next = q;
            y->next = z;

            ret = writeBook();
            retb = writeBorrow();
            retl = writeLog();
            /*
             * writeUser();
             * wrtitePasswd();
             * writeLog();
             * */
            if (ret == 0 && retb == 0 && retl == 0) {
                LOCK;
                mvwprintw(win, WIN_MENU.height-2, 1, "borrow book success");
                wrefresh(win);
                UNLOCK;
            } else {
                LOCK;
                mvwprintw(win, WIN_MENU.height-2, 1, "borrow book fail");
                wrefresh(win);
                UNLOCK;
            }
        }  else {
            LOCK;
            mvwprintw(win, WIN_MENU.height-2, 1, "borrow book number less or not found");
            wrefresh(win);
            UNLOCK;
        }

        LOCK;
        mvwprintw(win, 7, 1, "borrow more ? yes<1>, no<0>:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%d", &choice);

        LOCK;
        clearWindow(win);
        UNLOCK;
    } while(choice != 0);
}

void adminBorrowReturn(WINDOW *win) {
    int choice;
    int delFlag;
    char book_isbn[16];
    char user_stid[16];
    int ret, retb, retl;
    borrow *p, *q;
    log *y, *z;
    book *b;
    user *u;

    do {
        delFlag = 0;
        p = BORROW_HEAD;
        memset(book_isbn, 0, 16);
        memset(user_stid, 0, 16);

        y = LOG_HEAD;
        while (y->next) y = y->next;
        z = (log *) malloc(sizeof(log));
        z->next = NULL;

        LOCK;
        clearWindow(win);
        mvwprintw(win, 1, 1, "1. please input return_book_isbn:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%s", book_isbn);

        LOCK;
        mvwprintw(win, 1, 1, "2. please input return_user_stid:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%s", user_stid);

        b = getBookByBookIsbn(book_isbn);
        u = getUserByUserStid(user_stid);

        if (b && u) {
            while (p->next) {
                if (p->next->book_id == b->book_id && p->next->user_id == u->user_id) {
                    q = p->next;
                    p->next = p->next->next;
                    delFlag = 1;
                    break;
                }
                p = p->next;
            }

            if (delFlag == 0) {
                LOCK;
                mvwprintw(win, WIN_MENU.height-2, 1, "not found this msg");
                wrefresh(win);
                UNLOCK;
            } else {

                /* init log  */
                z->log_id = ++LOG_MAXID;
                z->user_id = u->user_id;
                z->book_id = b->book_id;
                strcpy(z->log_content, LOG_RETURN_BOOK);
                z->log_status = 1;

                y->next = z;

                b->book_number++;
                if (q) free(q);

                ret = writeBorrow();
                retb = writeBook();
                retl = writeLog();

                if (ret == 0 && retb == 0 && retl == 0) {
                    LOCK;
                    mvwprintw(win, WIN_MENU.height-2, 1, "book return success");
                    wrefresh(win);
                    UNLOCK;
                }
            }
        } else {
            LOCK;
            mvwprintw(win, WIN_MENU.height-2, 1, "getbook or getuser return null");
            wrefresh(win);
            UNLOCK;
        }

        LOCK;
        mvwprintw(win, 7, 1, "return more ? yes<1>, no<0>:");
        wrefresh(win);
        UNLOCK;
        wscanw(win, "%d", &choice);

        LOCK;
        clearWindow(win);
        UNLOCK;
    } while(choice != 0);
}


void adminBorrowList(WINDOW *win) {
    int index = 1;
    book *b;
    user *u;
    LOCK;
    clearWindow(win);
    borrow *p = BORROW_HEAD->next;
    while (p) {
        b = getBookByBookId(p->book_id);
        u = getUserByUserId(p->user_id);
        if (b && u) {
            mvwprintw(win, index, 1, "%d. %s->%s\n", p->borrow_id, u->user_name, b->book_name);
        }
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


void adminBorrow(WINDOW *win) {
    int choice;

    LOCK;
    clearWindow(win);
    UNLOCK;
    do {
        LOCK;
        menuAdminBorrow(win);
        UNLOCK;
        wscanw(win, "%d", &choice);
        switch (choice) {
            case 1:
                adminBorrowBorrow(win);
                break;
            case 2:
                adminBorrowReturn(win);
                break;
            case 3:
                adminBorrowList(win);
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

void adminInfo(WINDOW *win) {
    int index;
    LOCK;
    clearWindow(win);
    mvwprintw(win, 1, 1, "The system total have %d users", USER_MAXID);
    mvwprintw(win, 2, 1, "The system total have %d books", BOOK_HEAD);
    mvwprintw(win, 3, 1, "The system total have %d borrows", BORROW_HEAD);
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
                adminBook(win);
                break;
            case 3:
                adminBorrow(win);
                break;
            case 4:
                adminInfo(win);
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