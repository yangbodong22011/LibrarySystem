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
    return h->next;
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

int dataInit() {
    readUser();
    readPasswd();
    readLog();
}
