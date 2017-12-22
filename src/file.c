//
// Created by yangbodong on 12/20/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

int readUser() {
    FILE *fp;
    user *p = (user *) malloc(sizeof(user));
    user *q = (user *) malloc(sizeof(user));
    USER_HEAD = p;
    USER_MAXID = -1;

    fp = fopen(USER_PATH, "r+");
    if (fp == NULL) {
        fp = fopen(USER_PATH, "w+");
        free(q);
        return 0;
    }

    while (!feof(fp)) {
        fscanf(fp, "%d %s %s %s %s %d\n", &(q->user_id), q->user_stid,
               q->user_name, q->user_address, q->user_mail, &(q->user_status));

        /* update USER_MAXID*/
        if (q->user_id > USER_MAXID) {
            USER_MAXID = q->user_id;
        }

        p->next = q;
        p = q;
        q = (user *) malloc(sizeof(user));
    }

    p->next = NULL;
    free(q);
    fclose(fp);
    return 0;
}

int writeUser() {
    FILE *fp;
    fp = fopen(USER_PATH, "w+");
    if (fp == NULL) {
        perror("write_user() fopen error");
        return -1;
    }

    user *p = USER_HEAD->next;
    while (p) {
        fprintf(fp, "%d %s %s %s %s %d\n", p->user_id, p->user_stid,
                p->user_name, p->user_address, p->user_mail, p->user_status);
        p = p->next;
    }

    fclose(fp);
    return 0;
}

user *getUserByUserStid(char *user_stid) {
    user *p = USER_HEAD->next;

    while (p) {
        if (strcmp(p->user_stid, user_stid) == 0) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

user *getUserByUserName(char *user_name) {
    user *p = USER_HEAD->next;
    user *h = (user *) malloc(sizeof(user));
    user *q = h;
    user *t;

    while (p) {
        if (strcmp(p->user_name, user_name) == 0) {
            t = (user *) malloc(sizeof(user));
            memcpy(t, p, sizeof(user));
            q->next = t;
            q = t;
        }
        p = p->next;
    }
    q->next = NULL;
    return h;
}

user *getUserByUserId(int user_id) {
    user *p = USER_HEAD->next;

    while (p) {
        if (p->user_id == user_id) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

int readPasswd() {
    FILE *fp;
    passwd *p = (passwd *) malloc(sizeof(passwd));
    passwd *q = (passwd *) malloc(sizeof(passwd));
    PASSWD_HEAD = p;
    PASSWD_MAXID = -1;

    fp = fopen(PASSWD_PATH, "r+");
    if (fp == NULL) {
        fp = fopen(PASSWD_PATH, "w+");
        free(q);
        return 0;
    }

    while (!feof(fp)) {
        fscanf(fp, "%d %d %s\n", &(q->passwd_id), &(q->user_id), q->passwd_content);

        /* update PASSWD_MAXID*/
        if (q->passwd_id > PASSWD_MAXID) {
            PASSWD_MAXID = q->passwd_id;
        }
        p->next = q;
        p = q;
        q = (passwd *) malloc(sizeof(passwd));
    }

    p->next = NULL;
    free(q);
    fclose(fp);
    return 0;
}

int writePasswd() {
    FILE *fp;
    fp = fopen(PASSWD_PATH, "w+");
    if (fp == NULL) {
        perror("write_passwd() fopen error");
        return -1;
    }

    passwd *p = PASSWD_HEAD->next;
    while (p) {
        fprintf(fp, "%d %d %s\n", p->passwd_id, p->user_id, p->passwd_content);

        p = p->next;
    }

    fclose(fp);
    return 0;
}

passwd *getPasswdByUserId(int user_id) {
    passwd *p = PASSWD_HEAD;

    while (p) {
        if (p->user_id == user_id) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

int readLog() {
    FILE *fp;
    log *p = (log *) malloc(sizeof(log));
    log *q = (log *) malloc(sizeof(log));
    LOG_HEAD = p;
    LOG_MAXID = -1;

    fp = fopen(LOG_PATH, "r+");
    if (fp == NULL) {
        fp = fopen(LOG_PATH, "w+");
        free(q);
        return 0;
    }

    while (!feof(fp)) {
        fscanf(fp, "%d %d %d %s %d\n", &(q->log_id), &(q->user_id), &(q->book_id),
                q->log_content, &(q->log_status));

        /* update log_MAXID*/
        if (q->log_id > LOG_MAXID) {
            LOG_MAXID = q->log_id;
        }

        p->next = q;
        p = q;
        q = (log *) malloc(sizeof(log));
    }

    p->next = NULL;
    free(q);
    fclose(fp);
    return 0;
}

int writeLog() {
    FILE *fp;
    fp = fopen(LOG_PATH, "w+");
    if (fp == NULL) {
        perror("write_log() fopen error");
        return -1;
    }

    log *p = LOG_HEAD->next;
    while (p) {
        fprintf(fp, "%d %d %d %s %d\n", p->log_id, p->user_id, p->book_id,
               p->log_content, p->log_status);
        p = p->next;
    }
    fclose(fp);
    return 0;
}

int readBook() {
    FILE *fp;
    book *p = (book *) malloc(sizeof(book));
    book *q = (book *) malloc(sizeof(book));
    BOOK_HEAD = p;
    BOOK_MAXID = -1;

    fp = fopen(BOOK_PATH, "r+");
    if (fp == NULL) {
        fp = fopen(BOOK_PATH, "w+");
        free(q);
        return 0;
    }

    while (!feof(fp)) {
        fscanf(fp, "%d %s %s %s %d %lf\n", &(q->book_id), q->book_isbn, q->book_name,
               q->book_author, &(q->book_number), &(q->book_price));

        /* update book_MAXID*/
        if (q->book_id > BOOK_MAXID) {
            BOOK_MAXID = q->book_id;
        }

        p->next = q;
        p = q;
        q = (book *) malloc(sizeof(book));
    }

    p->next = NULL;
    free(q);
    fclose(fp);
    return 0;

}

int writeBook() {
    FILE *fp;
    fp = fopen(BOOK_PATH, "w+");
    if (fp == NULL) {
        perror("write_book() fopen error");
        return -1;
    }

    book *p = BOOK_HEAD->next;
    while (p) {
        fprintf(fp, "%d %s %s %s %d %lf\n", p->book_id, p->book_isbn, p->book_name,
                p->book_author, p->book_number, p->book_price);
        p = p->next;
    }
    fclose(fp);
    return 0;
}

book *getBookByBookIsbn(char *book_isbn) {
    book *p = BOOK_HEAD->next;

    while (p) {
        if (strcmp(p->book_isbn, book_isbn) == 0) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

book *getBookByBookName(char *book_name) {
    book *p = BOOK_HEAD->next;
    book *h = (book *) malloc(sizeof(book));
    book *q = h;
    book *t;

    while (p) {
        if (strcmp(p->book_name, book_name) == 0) {
            t = (book *) malloc(sizeof(book));
            memcpy(t, p, sizeof(book));
            q->next = t;
            q = t;
        }
        p = p->next;
    }
    q->next = NULL;
    return h;
}

book *getBookByBookId(int book_id) {
    book *p = BOOK_HEAD->next;

    while (p) {
        if (p->book_id == book_id) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

int readBorrow() {
    FILE *fp;
    borrow *p = (borrow *) malloc(sizeof(borrow));
    borrow *q = (borrow *) malloc(sizeof(borrow));
    BORROW_HEAD = p;
    BORROW_MAXID = -1;

    fp = fopen(BORROW_PATH, "r+");
    if (fp == NULL) {
        fp = fopen(BORROW_PATH, "w+");
        free(q);
        return 0;
    }

    while (!feof(fp)) {
        fscanf(fp, "%d %d %d\n", &(q->borrow_id), &(q->user_id), &(q->book_id));
        /* update borrow_MAXID*/
        if (q->borrow_id > BORROW_MAXID) {
            BORROW_MAXID = q->borrow_id;
        }

        p->next = q;
        p = q;
        q = (borrow *) malloc(sizeof(borrow));
    }

    p->next = NULL;
    free(q);
    fclose(fp);
    return 0;
}

int writeBorrow() {
    FILE *fp;
    fp = fopen(BORROW_PATH, "w+");
    if (fp == NULL) {
        perror("write_borrow() fopen error");
        return -1;
    }

    borrow *p = BORROW_HEAD->next;
    while (p) {
        fprintf(fp, "%d %d %d\n", p->borrow_id, p->user_id, p->book_id);
        p = p->next;
    }
    fclose(fp);
    return 0;
}

int dataInit() {
    readUser();
    readPasswd();
    readLog();
    readBook();
    readBorrow();
}
