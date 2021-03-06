//
// Created by yangbodong on 12/20/17.
//

#ifndef BOOKMANAGE_FILE_H
#define BOOKMANAGE_FILE_H

#include "bookManage.h"

#define USER_PATH "/home/yangbodong/CLionProjects/bookManage/data/user.txt"
#define PASSWD_PATH "/home/yangbodong/CLionProjects/bookManage/data/passwd.txt"
#define BOOK_PATH "/home/yangbodong/CLionProjects/bookManage/data/book.txt"
#define BORROW_PATH "/home/yangbodong/CLionProjects/bookManage/data/borrow.txt"
#define LOG_PATH "/home/yangbodong/CLionProjects/bookManage/data/log.txt"

user *USER_HEAD;
passwd *PASSWD_HEAD;
book *BOOK_HEAD;
borrow *BORROW_HEAD;
log *LOG_HEAD;
user *ME;
int USER_MAXID;
int PASSWD_MAXID;
int BOOK_MAXID;
int BORROW_MAXID;
int LOG_MAXID;


int dataInit();
int readUser();
int writeUser();
user *getUserByUserStid(char *user_stid);
user *getUserByUserName(char *user_name);
user *getUserByUserId(int user_id);

int readPasswd();
int writePasswd();
passwd *getPasswdByUserId(int user_id);

int readLog();
int writeLog();

int readBook();
int writeBook();
book *getBookByBookIsbn(char *book_isbn);
book *getBookByBookName(char *book_name);
book *getBookByBookId(int book_id);

int readBorrow();
int writeBorrow();

#endif //BOOKMANAGE_FILE_H
