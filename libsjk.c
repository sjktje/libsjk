#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

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
