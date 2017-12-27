#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define LOG_PATH "log.txt"
int LOG_MAXID;

char *getTimeNow() {
    char *timestr;
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    timestr = asctime(timeinfo);
    timestr[strlen(timestr) - 1] = '\0';
    return timestr;
}

typedef struct log {
    int log_id;
    int user_id;
    int book_id;
    char log_content[32];
    uint8_t log_status;
    char log_time[32];
    struct log *next;
} log;

log *LOG_HEAD;

int read_log() {
    int fd;
    int size;
    log *p = (log *) malloc(sizeof(log));
    log *q = (log *) malloc(sizeof(log));
    LOG_HEAD = p;
    LOG_MAXID = -1;

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

int write_log() {
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


int main()
{
    log *p;
    log *q = (log *) malloc(sizeof(log));

    read_log();

    p = LOG_HEAD->next;
    if (p == NULL) {
        printf("no data!\n");
    } else {
        while (p) {
            printf("%d %d %d %s %d %s\n", p->log_id, p->user_id, p->book_id,
                   p->log_content, p->log_status, p->log_time);
            p = p->next;
        }
    }
    

    p = LOG_HEAD;
    while (p->next) p = p->next;
    for (int i = 0; i < 2; ++i) {
        scanf("%d %d %d %s %d", &(q->log_id), &(q->user_id), &(q->book_id),
              q->log_content, &(q->log_status));
        strcpy(q->log_time, getTimeNow());
        p->next = q;
        p = q;
        q = (log *) malloc(sizeof(log));
    }
    p->next = NULL;

    write_log();

    read_log();

    p = LOG_HEAD->next;
    if (p == NULL) {
        printf("no data!\n");
    } else {
        while (p) {
            printf("%d %d %d %s %d %s\n", p->log_id, p->user_id, p->book_id,
                   p->log_content, p->log_status, p->log_time);
            p = p->next;
        }
    }

    /*p = log_HEAD;
    while (p->next) p = p->next;
    for (int i = 0; i < 2; ++i) {
        scanf("%d %s %s %s %s %d", &(q->log_id), q->log_stid,
              q->log_name, q->log_address, q->log_mail, &(q->log_status));
        p->next = q;
        p = q;
        q = (log *) malloc(sizeof(log));
    }
    p->next = NULL;

    write_log();

    read_log();

    p = log_HEAD->next;
    if (p == NULL) {
        printf("no data!\n");
    }
    
    while (p) {
        printf("%d %s %s %s %s %d\n", p->log_id, p->log_stid, p->log_name,
               p->log_address, p->log_mail, p->log_status);
        p = p->next;
    }*/
    
    free(q);
    return 0;
}

