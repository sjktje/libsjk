#define MAXBUF 100

int      isroot(void);
char    *getinput(const char *, char *);
char    *catargv(int, char **);
int      getyesno(const char *, int);
char    *myuser(void);
char    *mkdate(void);
void     pfctladd(char *, char *);
void     pfctlkill(char *);
void     pfctldel(char *, char *);
