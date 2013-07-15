#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>

#define INCL_DOSMEMMGR
#define INCL_DOSERRORS
#define INCL_DOSMODULEMGR
#define INCL_16DOSSEMAPHORES

#if (PLATFORM == XSQL_WINNT)
   #include <winnt.ih>
   #include <xsqlenv.h>
   #include <xsqlca.h>
   #include <xsqlda.h>
#endif

#include <adptvsql.h>
#include <adptvsql.ih>
#include <appstate.h>
#include <xsqldbms.h>

#define INCL_DOSMEMMGR

#if (PLATFORM == XSQL_OS2)
  #include <os2286.ih>
#endif
#if (PLATFORM == XSQL_WINNT)
  #include <winnt.ih>
#endif

#include <xsql.h>
#include <xsql1.h>


