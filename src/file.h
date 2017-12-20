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
int readPasswd();
int writePasswd();
passwd *getPasswdByUserId(int user_id);

#endif //BOOKMANAGE_FILE_H
