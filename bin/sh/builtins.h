/*
 * This file was generated by the mkbuiltins program.
 */

#include <sys/cdefs.h>
#define BLTINCMD 0
#define BREAKCMD 1
#define CDCMD 2
#define DOTCMD 3
#define ECHOCMD 4
#define EVALCMD 5
#define EXECCMD 6
#define EXITCMD 7
#define EXPCMD 8
#define EXPORTCMD 9
#define FALSECMD 10
#define HISTCMD 11
#define GETOPTSCMD 12
#define HASHCMD 13
#define JOBIDCMD 14
#define JOBSCMD 15
#define LOCALCMD 16
#define PWDCMD 17
#define READCMD 18
#define RETURNCMD 19
#define SETCMD 20
#define SETVARCMD 21
#define SHIFTCMD 22
#define TRAPCMD 23
#define TRUECMD 24
#define UMASKCMD 25
#define UNALIASCMD 26
#define UNSETCMD 27
#define WAITCMD 28
#define ALIASCMD 29
#define ULIMITCMD 30

struct builtincmd {
      char *name;
      int code;
};

extern int (*const builtinfunc[])();
extern const struct builtincmd builtincmd[];
