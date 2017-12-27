#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define USER_PATH "user.txt"
int USER_MAXID;

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
    int fd;
    int size;
    user *p = (user *) malloc(sizeof(user));
    user *q = (user *) malloc(sizeof(user));
    USER_HEAD = p;
    USER_MAXID = -1;

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

int write_user() {
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


int main()
{
    user *p;
    user *q = (user *) malloc(sizeof(user));

    read_user();

    p = USER_HEAD->next;
    if (p == NULL) {
        printf("no data!\n");
    }

    while (p) {
        printf("%d %s %s %s %s %d\n", p->user_id, p->user_stid, p->user_name,
               p->user_address, p->user_mail, p->user_status);
        p = p->next;
    }

    p = USER_HEAD;
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

    p = USER_HEAD->next;
    if (p == NULL) {
        printf("no data!\n");
    }
    
    while (p) {
        printf("%d %s %s %s %s %d\n", p->user_id, p->user_stid, p->user_name,
               p->user_address, p->user_mail, p->user_status);
        p = p->next;
    }

    p = USER_HEAD;
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

    p = USER_HEAD->next;
    if (p == NULL) {
        printf("no data!\n");
    }
    
    while (p) {
        printf("%d %s %s %s %s %d\n", p->user_id, p->user_stid, p->user_name,
               p->user_address, p->user_mail, p->user_status);
        p = p->next;
    }
    
    free(q);
    return 0;
}

