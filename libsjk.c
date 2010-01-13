#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "libsjk.h"

/* 
 * Returns 1 if we're root, 0 if not.
 */
int
isroot(void)
{
    uid_t uid = getuid();
    return (uid == 0) ? 1 : 0;
}

/*
 * Returns username of executing user
 */
char
*myuser(void)
{
    struct passwd *passwd;
    passwd = getpwuid(getuid());
    return passwd->pw_name;
}


/* 
 * Prints message and listens for answer. If defvalue is not NULL it will be 
 * shown in [brackets] and used as default value.
 */
char
*getinput(const char *message, char *defvalue)
{
    printf("%s [%s]: ", message, (defvalue == NULL) ? "" : defvalue);
    fflush(stdout);

    char *line = NULL;
    char *p = NULL;

    if ((line = malloc(MAXBUF+1)) == NULL) {
        perror("getinput() could not malloc");
        exit(1);
    }

    p = fgets(line, MAXBUF, stdin);

    if (p != NULL) {
        int last = strlen(line) - 1;

        if (line[0] == '\n') {   /* User just hit enter - use default value if it exists*/
            if (defvalue != NULL)
                p = strdup(defvalue);
        }

        if (strchr(line, '\n') == NULL) {
            /* Line did not fit in buffer */
            fprintf(stderr, "Input line would not fit in buffer. Exiting.");
            exit(1);
        }

        /* Remove trailing \n if there is one */
        if (line[last] == '\n')
            line[last] = '\0';

    }

    return p;
}

/*
 * Creates string consisting of all entries in *argv[]
 */
char *
catargv(int argc, char *argv[])
{
    char *string = NULL;
    int   length = 0;
    int   i;

    for (i = 0; i < argc; i++) {
        length += strlen(argv[i])+1;

        if (string == NULL) {
            if ((string = malloc(length)) == NULL) {
                perror("catargv() could not malloc");
                exit(1);
            }
        } else {
            if ((string = realloc(string, length)) == NULL) {
                perror("catargv() could not realloc");
                exit(1);
            }
        }

        strncat(string, argv[i], length);
        strncat(string, " ", length);
    }

    return string;
}

/*
 * Returns string YYYY-MM-DD HH:SS UTC(+/-)HH.
 */
char
*mkdate(void)
{
    struct tm *tm_ptr;
    time_t the_time;
    char buf[256];
    char *our_date;

    (void)time(&the_time);
    tm_ptr = gmtime(&the_time);

    strftime(buf, sizeof(buf), "%y-%m-%d %H:%M UTC%z", tm_ptr);

    our_date = malloc(strlen(buf) + 1);
    if (our_date == NULL) {
        perror("mkdate() could not malloc");
        exit(1);
    }

    strcpy(our_date, buf);

    return our_date;
}

/* 
 * Adds *ips to OpenBSD packet filter table *table
 */
void 
pfctladd(char *ips, char *table)
{
    int      len = 0;
    char    *cmd = NULL;

    asprintf(&cmd, "%spfctl -t %s -T add ",
            isroot() ? "" : "sudo ", table);

    len = strlen(cmd) + strlen(ips) + 1;

    if ((cmd = realloc(cmd, len)) == NULL) {
        perror("pfctladd() could not realloc");
        exit(1);
    }

    strncat(cmd, ips, len);

    system(cmd);

    free(cmd);
    cmd = NULL;
}

/*
 * Seperates string *ips by whitespace and issues pfctl -k on all arguments.
 * Uses sudo if user is not root.
 */
void 
pfctlkill(char *ips)
{
    char    *ip = NULL;
    char    *cmd = NULL;
    int      len = 0;

    while((ip = strsep(&ips, " ")) != NULL) {
        if (ip[0] == '\0')
            continue;

        asprintf(&cmd, "%spfctl -k ", isroot() ? "" : "sudo ");
        len = strlen(cmd) + strlen(ip) + 1;

        if ((cmd = realloc(cmd, len)) == NULL) {
            perror("pfctlkill could not realloc");
            exit(1);
        }

        strncat(cmd, ip, len);

        system(cmd);
    }
}

/*
 * Tries to remove *ips from PF table *table. Uses sudo if user is not root.
 */
void 
pfctldel(char *ips, char *table)
{
    int len = 0;
    char *cmd = NULL;

    asprintf(&cmd, "%spfctl -t %s -T del ", isroot() ? "" : "sudo ", table);
    len = strlen(cmd) + strlen(ips) + 1;

    if ((cmd = realloc(cmd, len)) == NULL) {
        perror("pfctldel() could not realloc");
        exit(1);
    }

    strncat(cmd, ips, cmdlength);

    system(cmd);

    free(cmd);
    cmd = NULL;
}
