#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct user {
    int user_id;
    char user_stid[16];
    char user_name[16];
    char user_address[32];
    char user_mail[16];
    uint8_t user_status;
    struct user *next;
} user;

user *USER_HEAD;

int read_user() {
    FILE *fp;
    user *p = (user *) malloc(sizeof(user));
    user *q = (user *) malloc(sizeof(user));
    USER_HEAD = p;

    fp = fopen("user.txt", "r+");
    if (fp == NULL) {
        fp = fopen("user.txt", "w+");
        free(q);
        return 0;
    }

    while (!feof(fp)) {
        fscanf(fp, "%d %s %s %s %s %d\n", &(q->user_id), q->user_stid,
           q->user_name, q->user_address, q->user_mail, &(q->user_status));
        p->next = q;
        p = q;
        q = (user *) malloc(sizeof(user));
    }

    p->next = NULL;
    free(q);
    fclose(fp);
    return 0;
}

int write_user() {
    FILE *fp;
    fp = fopen("user.txt", "w+");
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


int main()
{
    //user *p = (user *) malloc(sizeof(user));
    user *q = (user *) malloc(sizeof(user));
    read_user();

    user *p = USER_HEAD->next;
    if (p == NULL) {
        printf("no data!\n");
    }

    while (p) {
        printf("%d %s %s %s %s %d\n", p->user_id, p->user_stid, p->user_name,
               p->user_address, p->user_mail, p->user_status);
        p = p->next;
    }

    //USER_HEAD = p;
    /*p = USER_HEAD;
    while (p->next) p = p->next;
    for (int i = 0; i < 2; ++i) {
        scanf("%d %s %s %s %s %d", &(q->user_id), q->user_stid,
              q->user_name, q->user_address, q->user_mail, &(q->user_status));
        p->next = q;
        p = q;
        q = (user *) malloc(sizeof(user));
    }
    p->next = NULL;

    write_user();

    read_user();

    if (p == NULL) {
        printf("no data!\n");
    }
    
    p = USER_HEAD->next;
    while (p) {
        printf("%d %s %s %s %s %d\n", p->user_id, p->user_stid, p->user_name,
               p->user_address, p->user_mail, p->user_status);
        p = p->next;
    }*/

    /*p = USER_HEAD;
    while (p->next) p = p->next;
    for (int i = 0; i < 2; ++i) {
        scanf("%d %s %s %s %s %d", &(q->user_id), q->user_stid,
              q->user_name, q->user_address, q->user_mail, &(q->user_status));
        p->next = q;
        p = q;
        q = (user *) malloc(sizeof(user));
    }
    p->next = NULL;

    write_user();

    read_user();

    if (p == NULL) {
        printf("no data!\n");
    }
    
    p = USER_HEAD->next;
    while (p) {
        printf("%d %s %s %s %s %d\n", p->user_id, p->user_stid, p->user_name,
               p->user_address, p->user_mail, p->user_status);
        p = p->next;
    }*/

    return 0;
}

