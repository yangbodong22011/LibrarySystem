//
// Created by yangbodong on 12/20/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "file.h"

int readUser() {
    int fd;
    int size;
    user *p = (user *) malloc(sizeof(user));
    user *q = (user *) malloc(sizeof(user));
    USER_HEAD = p;
    USER_MAXID = 0;

    fd = open(USER_PATH, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    while ((size = read(fd, q, sizeof(user))) != 0) {
        if (q->user_id > USER_MAXID) {
            USER_MAXID = q->user_id;
        }
        p->next = q;
        p = q;
        q = (user *) malloc(sizeof(user));
    }

    p->next = NULL;
    free(q);
    close(fd);
    return 0;
}

int writeUser() {
    int fd;

    fd = open(USER_PATH, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    user *p = USER_HEAD->next;
    while (p) {
        write(fd, p, sizeof(user));
        p = p->next;
    }

    close(fd);
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
    int fd;
    int size;
    passwd *p = (passwd *) malloc(sizeof(passwd));
    passwd *q = (passwd *) malloc(sizeof(passwd));
    PASSWD_HEAD = p;
    PASSWD_MAXID = 0;

    fd = open(PASSWD_PATH, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    while ((size = read(fd, q, sizeof(passwd))) != 0) {
        if (q->passwd_id > PASSWD_MAXID) {
            PASSWD_MAXID = q->passwd_id;
        }
        p->next = q;
        p = q;
        q = (passwd *) malloc(sizeof(passwd));
    }

    p->next = NULL;
    free(q);
    close(fd);
    return 0;
}

int writePasswd() {
    int fd;

    fd = open(PASSWD_PATH, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    passwd *p = PASSWD_HEAD->next;
    while (p) {
        write(fd, p, sizeof(passwd));
        p = p->next;
    }

    close(fd);
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
    int fd;
    int size;
    log *p = (log *) malloc(sizeof(log));
    log *q = (log *) malloc(sizeof(log));
    LOG_HEAD = p;
    LOG_MAXID = 0;

    fd = open(LOG_PATH, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    while ((size = read(fd, q, sizeof(log))) != 0) {
        if (q->log_id > LOG_MAXID) {
            LOG_MAXID = q->log_id;
        }
        p->next = q;
        p = q;
        q = (log *) malloc(sizeof(log));
    }

    p->next = NULL;
    free(q);
    close(fd);
    return 0;
}

int writeLog() {
    int fd;

    fd = open(LOG_PATH, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    log *p = LOG_HEAD->next;
    while (p) {
        write(fd, p, sizeof(log));
        p = p->next;
    }

    close(fd);
    return 0;
}

int readBook() {
    int fd;
    int size;
    book *p = (book *) malloc(sizeof(book));
    book *q = (book *) malloc(sizeof(book));
    BOOK_HEAD = p;
    BOOK_MAXID = 0;

    fd = open(BOOK_PATH, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    while ((size = read(fd, q, sizeof(book))) != 0) {
        if (q->book_id > BOOK_MAXID) {
            BOOK_MAXID = q->book_id;
        }
        p->next = q;
        p = q;
        q = (book *) malloc(sizeof(book));
    }

    p->next = NULL;
    free(q);
    close(fd);
    return 0;
}

int writeBook() {
    int fd;

    fd = open(BOOK_PATH, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    book *p = BOOK_HEAD->next;
    while (p) {
        write(fd, p, sizeof(book));
        p = p->next;
    }

    close(fd);
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
    int fd;
    int size;
    borrow *p = (borrow *) malloc(sizeof(borrow));
    borrow *q = (borrow *) malloc(sizeof(borrow));
    BORROW_HEAD = p;
    BORROW_MAXID = 0;

    fd = open(BORROW_PATH, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    while ((size = read(fd, q, sizeof(borrow))) != 0) {
        if (q->borrow_id > BORROW_MAXID) {
            BORROW_MAXID = q->borrow_id;
        }
        p->next = q;
        p = q;
        q = (borrow *) malloc(sizeof(borrow));
    }

    p->next = NULL;
    free(q);
    close(fd);
    return 0;
}

int writeBorrow() {
    int fd;

    fd = open(BORROW_PATH, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    borrow *p = BORROW_HEAD->next;
    while (p) {
        write(fd, p, sizeof(borrow));
        p = p->next;
    }

    close(fd);
    return 0;
}

int dataInit() {
    readUser();
    readPasswd();
    readLog();
    readBook();
    readBorrow();
}
