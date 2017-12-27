//
// Created by yangbodong on 12/20/17.
//

#ifndef BOOKMANAGE_BOOKMANAGE_H
#define BOOKMANAGE_BOOKMANAGE_H


#include <stdint.h>
#include <ncurses.h>

#define LOCK pthread_mutex_lock(&MUTEX)
#define UNLOCK pthread_mutex_unlock(&MUTEX)

typedef struct user {
    int user_id;
    char user_stid[16];
    char user_name[16];
    char user_address[32];
    char user_mail[16];
    int user_status;
    struct user *next;
} user;

typedef struct passwd {
    int passwd_id;
    int user_id;
    char passwd_content[32];
    struct passwd *next;
} passwd;

typedef struct book {
    int book_id;
    char book_isbn[16];
    char book_name[32];
    char book_author[16];
    int book_number;
    double book_price;
    struct book *next;
} book;

typedef struct borrow {
    int borrow_id;
    int user_id;
    int book_id;
    struct borrow *next;
} borrow;

typedef struct log {
    int log_id;
    char user_name[16];
    char book_name[16];
    char log_content[32];
    int log_status;
    char log_time[32];
    struct log *next;
} log;

char *getTimeNow();
void *threadTitle(void *p);
void *threadMenu(void *p);
void *threadNews(void *p);

void loop(WINDOW *win);
int loginSys(WINDOW *win);
void initMe(user *u);


void adminFunc(WINDOW *win);
void adminReader(WINDOW *win);
void adminReaderAdd(WINDOW *win);
void adminReaderList(WINDOW *win);
void adminReaderModify(WINDOW *win);
void adminReaderDel(WINDOW *win);
void adminReaderFind(WINDOW *win);
void adminReaderFindByUserStid(WINDOW *win);
void adminReaderFindByUserName(WINDOW *win);

void adminBook(WINDOW *win);
void adminBookAdd(WINDOW *win);
void adminBookList(WINDOW *win);
void adminBookModify(WINDOW *win);
void adminBookDel(WINDOW *win);
void adminBookFind(WINDOW *win);
void adminBookFindByBookIsbn(WINDOW *win);
void adminBookFindByBookName(WINDOW *win);

void adminBorrow(WINDOW *win);
void adminBorrowBorrow(WINDOW *win);
void adminBorrowReturn(WINDOW *win);
void adminBorrowList(WINDOW *win);

void adminInfo(WINDOW *win);

void userFunc(WINDOW *win);

#endif //BOOKMANAGE_BOOKMANAGE_H
