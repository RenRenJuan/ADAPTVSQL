/*             
  MODULE:      ADAPTIVE.C

  PURPOSE:     Provide backend for BACK2400.EXE, AS400PCS.DLL,
               XSQLDBMS.DLL, etc.

  DESCRIPTION:

    The above mentioned co-routines of this program were developed
    using the Remote SQL API. This module re-implements the subset
    of the functional protocol of that PC Support 2.0 sub-service
    used by those programs. The re-implementation provides a common
    interface for both PC Support and DDCS/DBM access. It also supports
    the features of PCS 2.2 including DRDA.

    ADPTVSQL.DLL supports minimal preparation of SQL statements for
    databases to which it has been bound. It does so via the functions
    in SQLKB.C (q.v). System applications such as XSQLDBMS.DLL must
    first link this backend library via setKB defined in that module.
    ADPTVSQL.DLL is not meant as an end application server but can be
    used as an SQL interface by system applications ready to do embedded
    SQL programming (although this application should largely eliminate
    the need to embed any SQL) such as BATCHSQL.EXE. Non-system applications
    should use an API package such as XSQLDBMS.DLL.

    The original functions of REMOTSQL.C and the Remote SQL functions
    (as used by AS400PCS.C and BACK2400.SQC) define the functional protocol
    of this module. The actual functions accessing PC Support or DBM
    are isolated (by necessity) to REMOTSQL.C (a revised version of the
    AS400PCS.DLL original) and ADPTVSQL.SQC.

    This library presents a system API meant for use by system programs.
    End-user programs should access it via XSQLDBMS.DLL. This module
    is largely a switch for ADPTVSQL.SQC and REMOTSQL.C and a connector
    to the global segment.

  FUNCTION:

    The function call switch form is inferior to an line-form but 
    is being maintained until the full encapsulation semantics of the
    following are known.

     dumpSQL(char *sqlString)

      Common statement dumper used by all DBMS specific libs.

     execAdaptive(STATE **state,CURSOR **context,CONNECTION **dbs);

      Link an application process to the backend. The parameters are
      output. System applications using this library must call this
      function to get the common interface structures.

    The following functions have the same protocol and semantics as in 
    AS400PCS but are implemented using the common interface.

     execConnect(int thisConnect)

      Start using the connection. The system application has assured that
      the CONNECTION is consistently configured.

     execEndSQL(int thisSQL,int thisConnection)

      Terminate the indicated statement. If the cursor handle is negative,
      then the persisting cursor will be released.

     execError(int wasCode,char *whichAPI,int thisConnect)  

      Print message to log re SQL error. This call feeds but is seperate
      from those (osErr, etc.) in XSQLDBMS. System applications can call
      execError.

     execSelect(char *sqlText)
      
      Execute a select.

     execSQL(int thisConnect)

      Execute a non-select SQL statement. Returns a non-zero cursor handle
      if successful, zero otherwise.
      
     execSQLERRD3(long *value,int thisConnection)
 
      Set the argument to the ERRD3 value of the SQLCA.

    The following functions from EHNRQAPI.DLL were called directly in
    AS400PCS.C:

       EHNRQCLOSE, EHNRQATTR, EHNRQFETCH

    they are replaced by functions which have EHNRQ replaced by RDBMS.
    When accessMode >= DATABASE_MANAGER they are handled by the actual
    PC_SUPPORT functions of the same name.

     RDBMSCLOSE(int cursorHandle,int connectionHandle)

      Terminate a cursor.

     RDBMSATTR(int cursorHandle,void *attributes,short *size)
 
      Get the attributes for columns of current result table.

     RDBMSFETCH(int cursorHandle,char *data,short *size)

      Perform the next row fetch.

    The following function is used to give BATCHSQL.DLL access to
    internal state.

      execGetState(COMMON_STATE **);

  HISTORY:

   2/4/93  - In-line design begun. DDCS.SQC recovered as this module.
   2/8/93  - Switch plus connect function structure.
   2/10/94 - Revised switching logic allowing full runtime linking of
             underlying DBMS.
  03/26/96 - Modifications for XSQL 0.3, primarily for OS/2 2.3 tools.
  11/11/98 - XSQLCore begin in earnest. OS/2 leg dropped but not removed. 
             Core engine may only need to run on NT, but retaining
             and where possible extending platform independence. Taking this
             as opportunity to clean up and consolidate, no new function as
             such.
*/
  
#include <xsqlmods.h>
#define SOURCE_GROUP   XSQLDBMS_DLL
#define SOURCE_MODULE  ADAPTIVE_C
#include <xsqlc.h>

 #define checkEngineBind {if (thisDB.accessMode>=NATIVE && !tcpBound)\
                             bindXSQLPEER();\
 else if (thisDB.accessMode<INTERSOLV && !pcSupportBound)\
                             bindXSQLPCS2();\
 else if ((thisDB.accessMode>=DATABASE_MANAGER && thisDB.accessMode < NATIVE) && !dbmBound)\
                             bindXSQLDB22();\
                     else if (thisDB.accessMode==INTERSOLV && !qeBound)\
                             bindXSQLODBC();\
                     else if (thisDB.accessMode==SQL_SERVER && !sybaseBound)\
                             bindXSQLMSQL();}


 #define switchToFunction(which) switch(thisDB.accessMode) \
         {case LOCAL_PC_SUPPORT: case DRDA_PC_SUPPORT: case LOCAL_COLLECTION: case DDM_PC_SUPPORT:\
               return(ehn##which());                       \
          case DATABASE_MANAGER: case LOCAL_DOMAIN: case_DDCS_2:\
               return(dbm##which());                       \
          case INTERSOLV:  return(qNe##which());           \
          case SQL_SERVER: return(db##which());            \
         }

 #define switchToFunction1(which,x) switch(thisDB.accessMode) \
         {case LOCAL_PC_SUPPORT: case DRDA_PC_SUPPORT: case LOCAL_COLLECTION: case DDM_PC_SUPPORT:\
               return(ehn##which(x));                       \
          case DATABASE_MANAGER: case LOCAL_DOMAIN: case_DDCS_2:\
               return((dbm##which)(x));                     \
          case INTERSOLV:  return(qNe##which(x));           \
          case SQL_SERVER: return(db##which(x));            \
         }

 #define switchToFunction2(which,x,y) switch(thisDB.accessMode) \
         {case LOCAL_PC_SUPPORT: case DRDA_PC_SUPPORT: case LOCAL_COLLECTION: case DDM_PC_SUPPORT:\
               return(ehn##which(x,y));                       \
          case DATABASE_MANAGER: case LOCAL_DOMAIN: case_DDCS_2:\
               return(dbm##which(x,y));                       \
          case INTERSOLV:  return(qNe##which(x,y));           \
          case SQL_SERVER: return(db##which(x,y));            \
         }
                               
 #define switchToFunction3(which,x,y,z) switch(thisDB.accessMode) \
         {case LOCAL_PC_SUPPORT: case DRDA_PC_SUPPORT: case LOCAL_COLLECTION: case DDM_PC_SUPPORT:\
               return(ehn##which(x,y,z));                       \
          case DATABASE_MANAGER: case LOCAL_DOMAIN: case_DDCS_2:\
               return(dbm##which(x,y,z));                       \
          case INTERSOLV:  return(qNe##which(x,y,z));           \
          case SQL_SERVER: return(db##which(x,y,z));            \
         }


 //if (DosGetProcAddr(XSQLDBM2,"_dbmConnect",&((PFN)dbmConnect)))
 //{xsqlLogPrint("XSQLDBM2.DLL is damaged!"); return value;}

 #ifndef INCL_16DOSMODULEMGR
  #define DosGetProcAddr(handle,name,ptr) DosQueryProcAddr(handle,0,name,ptr)
 #endif

 #define linkDBMSProc(X,Y,Z,N) {int rc; if ((rc=DosGetProcAddr(Z,Y,&X))) \
           {xsqlLogPrint("Got %d trying to access "N". "N" is damaged!",rc);\
            return NOT_OK;}}

 dclModeStrings;                  
 dclModeArities;

 static short pcSupportBound;
 static short dbmBound;
 static short sybaseBound;
 static short qeBound;
 static short tcpBound;
 int          bindXSQLPCS2(void);
 int          bindXSQLDB22(void);
 int          bindXSQLMSQL(void);
 int          bindXSQLODBC(void);
 int          bindXSQLPEER(void);
 HMODULE      XSQLPCS2;
 HMODULE      XSQLDB22;
 HMODULE      XSQLMSQL;
 HMODULE      XSQLODBC;
 HMODULE      XSQLPEER;

 
//int xType2
// xheapwalk( void *x )
//{
// int rc=_heapwalk( x ) ;
// return( rc );
//}
//int xType2
// xheapchk( )
//{
//int rc=_heapchk();
// return( rc );
//}
//void xType2 *
// xmalloc( int x )
//{
// void *rc=malloc( x );
// return( rc );
//}
//int xType2
// xfree( void *x )
//{
// Free( x );
//}
 int xType2
    execAdaptive(STATE **xState,CURSOR **xContext,CONNECTION **xDatabases)
{int      value=OK, validReference;
 unsigned returnCode,globalSelector,slotFound,thisConnect=0,
           Flags=PAG_WRITE | PAG_READ;
 PVOID    infoSeg;

 checkReference( xState, validReference );
 if ( !validReference )
       return 90;
 checkReference( xContext, validReference );
 if ( !validReference )
       return 91;
 checkReference( xDatabases, validReference );
 if ( !validReference )
       return 92;

 if ( state && db && sql )
 {
  *xState     = state;
  *xContext   = sql;
  *xDatabases = db;
  return( 0 );
 }

 if ((returnCode=
      DosAllocSharedMem(&infoSeg,globalSegName,sizeof(COMMON_STATE),
                        (Flags|PAG_COMMIT)))) 
      {if (returnCode==ERROR_ALREADY_EXISTS)
       {if (returnCode=DosGetNamedSharedMem((PPVOID)&cs,globalSegName,Flags))
            return( 1 );
       }
        else return( returnCode );
      } 
 else {memset(infoSeg,0,sizeof(COMMON_STATE));
       cs = infoSeg;
      }

 if (returnCode=DosSemRequest((void *)&(cs->gate), 5000))
     return( 3 );

 for (slotFound=thisProcess=0;thisProcess<MAX_APPLICATION;thisProcess++)
  if (!(thisApp)) {slotFound=TRUE; break;}

 if (slotFound)
 { thisApp          = malloc(sizeof(APPLICATION));
   memset(thisApp,'\0',sizeof(APPLICATION));
   thisState        = malloc(sizeof(STATE));
   memset(thisState,'\0',sizeof(STATE));
   thisApp->context = malloc(sizeof(CURSOR) * MAX_CURSORS);
   memset(thisApp->context,'\0',sizeof(CURSOR) * MAX_CURSORS);
   cs->nApps++;
   *xState                 = thisState;
    thisState->Work        = malloc(1024);
    thisState->UserErrText = malloc(1024);
    thisState->UserColumn  = malloc(MAX_COLUMN_TEXT);
    thisState->MySQLbuffer = malloc(MAX_SQL_TEXT);
    thisState->lastLoc     = "Nowhere";
   *xContext               = thisApp->context; 
   *xDatabases             = &thisDB;               
    state                  = thisState;
    sql                    = thisApp->context;
    db                     = &thisDB;
    state->sites           = setSiteDependencies(NULL);
 } else value              = 4;

 state->pointerCheckingDisabled = TRUE; // Enabled by osTraceOn

 #ifdef RELEASE
 state->proFileName = "xsql" RELEASE ".ini";
 loadProfile();
 #endif

 DosSemClear(&(cs->gate));
 
 return(value);
   
}
int execConnect(int thisConnect)
{
 checkEngineBind;

 switchToFunction1(Connect,thisConnect);
}  
int execDisconnect(int thisConnect)
{
 checkEngineBind;
 switchToFunction1(Disconnect,thisConnect);
}  
int execSQL(int thisCursor)
{
 int thisConnect=thisContext.path;

 checkEngineBind;
 switchToFunction1(ExecSQL,thisCursor); 
}
int execSelect(int thisCursor)
{
 int thisConnect=thisContext.path;
  
 checkEngineBind;
 switchToFunction1(ExecSelect,thisCursor); 
}
int execSQLERRD3(long *value,int thisCursor)
{ 
 int thisConnect=thisContext.path;

 checkEngineBind;
 switchToFunction2(FetchSQLERRD3,value,thisConnect);
}
int execError(int wasCode,char *whichAPI,int thisConnect)
{ 
  switchToFunction3(Error,wasCode,whichAPI,thisConnect); 
}
int RDBMSATTR(int thisCursor)
{
 int  thisConnect=thisContext.path;

  if (thisDB.accessMode>=INTERSOLV) return(OK);
  checkEngineBind;
  return(ehnDescribe(thisCursor)); 
}
int RDBMSCLOSE(int thisCursor)
{
 int thisConnect=thisContext.path;

  checkEngineBind;
  switchToFunction1(CloseCursor,thisCursor);
}
int RDBMSFETCH(int thisCursor,char **fetched)
{
 int  thisConnect=thisContext.path;
 
  checkEngineBind;
  switchToFunction2(Fetch,thisCursor,fetched); 
}
int execOfflineCursor(int thisCursor) 
{
 int thisConnect=thisContext.path;

 if (thisHit)
     thisHit->cursor = 0;
 checkEngineBind;
 switchToFunction1(OfflineCursor,thisCursor); 

}
#if (PLATFORM == XSQL_OS2)
  SITE * xType2 setSiteDependencies(char *connectName)
#endif
#if (PLATFORM == XSQL_WINNT)
  xType2 SITE * setSiteDependencies(char *connectName)
#endif
{
 SITE     *value=NULL,*head=NULL,*tail=NULL,*next=NULL;
 char     line[110],thisConnectName[20],thisModeString[40],*lineCursor;

 if (!connectName)
 {unsigned tokenSize,i,found;
  FILE     *siteTable;

   if (!(siteTable=fopen("SITES.SQL","r"))) goto done;
   while(fgets(line,sizeof(line)-10,siteTable))
   {if (line[0]=='/') continue;
    memset(thisModeString,'\0',sizeof(thisModeString));
    memset(thisConnectName,'\0',sizeof(thisConnectName));
    lineCursor = line + strspn(line," \t\r\n");
    tokenSize  = strcspn(lineCursor," \t\r\n");
    if (!tokenSize || tokenSize > sizeof(thisConnectName)-1) continue;
    memcpy(thisConnectName,lineCursor,tokenSize);
    lineCursor = 
        lineCursor + tokenSize + strspn(lineCursor+tokenSize," \t\r\n");
    tokenSize  = strcspn(lineCursor," \t\r\n");
    if (!tokenSize || tokenSize > sizeof(thisModeString)-1) continue;
    memcpy(thisModeString,lineCursor,tokenSize);
    next = malloc(sizeof(SITE));
    memset(next,'\0',sizeof(SITE));
    next->name = malloc(strlen(thisConnectName)+1);
    strcpy(next->name,thisConnectName);
    for (i=found=0;!found && i < DDCS_2;i++)
     if (modeStrings[i] && !stricmp(thisModeString,modeStrings[i]))
     {next->accessMode = i + 1; found = TRUE;}
    if (!found) goto done;
    if (!head) {tail = head = next; 
                state->sites= head;
               }
    else       tail->next = next;
    tail = next;
   }

   fclose(siteTable);
   value = head;
   goto done;
 }
  else next = state->sites;

 if (connectName)
  while(next && !value)
   if (!stricmp(connectName,next->name)) value = next;
   else next = next->next;

 // if (_heapchk()!=_HEAPOK) abort();
 done: return(value);
  
}
bindXSQLDB22()
{
#if (PLATFORM == XSQL_OS2)

 char failedObject[129];
 int  value=NOT_OK;
 PFN  funcRef;

 if (DosLoadModule(failedObject,128,"XSQLDB22",&XSQLDB22))
 {xsqlLogPrint("Can't access XSQLDB22.DLL!"); return value;}

 linkDBMSProc(dbmConnect,       "dbmConnect",      XSQLDB22,"XSQLDB22.DLL");
 linkDBMSProc(dbmDisconnect,    "dbmDisconnect",   XSQLDB22,"XSQLDB22.DLL");
 linkDBMSProc(dbmError,         "dbmError",        XSQLDB22,"XSQLDB22.DLL");
 linkDBMSProc(dbmFetch,         "dbmFetch",        XSQLDB22,"XSQLDB22.DLL");
 linkDBMSProc(dbmFetchSQLERRD3, "dbmFetchSQLERRD3",XSQLDB22,"XSQLDB22.DLL");
 linkDBMSProc(dbmExecSQL,       "dbmExecSQL",      XSQLDB22,"XSQLDB22.DLL");
 linkDBMSProc(dbmExecSelect,    "dbmExecSelect",   XSQLDB22,"XSQLDB22.DLL");
 linkDBMSProc(dbmOfflineCursor, "dbmOfflineCursor",XSQLDB22,"XSQLDB22.DLL");
 linkDBMSProc(dbmCloseCursor,   "dbmCloseCursor",  XSQLDB22,"XSQLDB22.DLL");

 if (DosGetProcAddr(XSQLDB22,"dbmInit",&funcRef))
 {xsqlLogPrint("XSQLDB22.DLL is damaged!"); return value;}

 value=funcRef();  

 maxCursors[DATABASE_MANAGER][0] = MAX_CURSORS;
 maxCursors[LOCAL_DOMAIN][0]     = MAX_CURSORS;
 maxCursors[DDCS_2][0]           = MAX_CURSORS;

 if (!value)
     dbmBound = TRUE;

 return value;

#endif
}
bindXSQLPCS2()
{
#if (PLATFORM == XSQL_OS2)

 char failedObject[129];
 int  value=NOT_OK;
 PFN  funcRef;

 if (DosLoadModule(failedObject,128,"XSQLPCS2",&XSQLPCS2))
 {xsqlLogPrint("Can't access XSQLPCS2.DLL!"); return value;}

 linkDBMSProc(ehnConnect,       "_ehnConnect",      XSQLPCS2,"XSQLPCS2.DLL");
 linkDBMSProc(ehnDescribe,      "_ehnDescribe",     XSQLPCS2,"XSQLPCS2.DLL");
 linkDBMSProc(ehnDisconnect,    "_ehnDisconnect",   XSQLPCS2,"XSQLPCS2.DLL");
 linkDBMSProc(ehnError,         "_ehnError",        XSQLPCS2,"XSQLPCS2.DLL");
 linkDBMSProc(ehnFetch,         "_ehnFetch",        XSQLPCS2,"XSQLPCS2.DLL");
 linkDBMSProc(ehnFetchSQLERRD3, "_ehnFetchSQLERRD3",XSQLPCS2,"XSQLPCS2.DLL");
 linkDBMSProc(ehnExecSQL,       "_ehnExecSQL",      XSQLPCS2,"XSQLPCS2.DLL");
 linkDBMSProc(ehnExecSelect,    "_ehnExecSelect",   XSQLPCS2,"XSQLPCS2.DLL");
 linkDBMSProc(ehnOfflineCursor, "_ehnOfflineCursor",XSQLPCS2,"XSQLPCS2.DLL");
 linkDBMSProc(ehnCloseCursor,   "_ehnCloseCursor",  XSQLPCS2,"XSQLPCS2.DLL");

 if (DosGetProcAddr(XSQLPCS2,"_ehnInit",&funcRef))
 {xsqlLogPrint("XSQLPCS2.DLL is damaged!"); return value;}

 maxCursors[LOCAL_PC_SUPPORT][0] = 10;
 maxCursors[LOCAL_COLLECTION][0] = 10;
 maxCursors[DRDA_PC_SUPPORT][0]  = 10;

 value=funcRef();  

 if (!value)
     pcSupportBound = TRUE;

 return value;

#endif
}
bindXSQLMSQL()
{
 char failedObject[129];
 int  value=NOT_OK;
 int (*funcRef)();

 if (DosLoadModule(failedObject,128,"XSQLMSQL",&XSQLMSQL))
 {xsqlLogPrint("Can't access XSQLMSQL.DLL!"); return value;}

 linkDBMSProc(dbConnect,       "_dbConnect",      XSQLMSQL,"XSQLMSQL.DLL");
 linkDBMSProc(dbDescribe,      "_dbDescribe",     XSQLMSQL,"XSQLMSQL.DLL");
 linkDBMSProc(dbDisconnect,    "_dbDisconnect",   XSQLMSQL,"XSQLMSQL.DLL");
 linkDBMSProc(dbError,         "_dbError",        XSQLMSQL,"XSQLMSQL.DLL");
 linkDBMSProc(dbFetch,         "_dbFetch",        XSQLMSQL,"XSQLMSQL.DLL");
 linkDBMSProc(dbFetchSQLERRD3, "_dbFetchSQLERRD3",XSQLMSQL,"XSQLMSQL.DLL");
 linkDBMSProc(dbExecSQL,       "_dbExecSQL",      XSQLMSQL,"XSQLMSQL.DLL");
 linkDBMSProc(dbExecSelect,    "_dbExecSelect",   XSQLMSQL,"XSQLMSQL.DLL");
 linkDBMSProc(dbOfflineCursor, "_dbOfflineCursor",XSQLMSQL,"XSQLMSQL.DLL");
 linkDBMSProc(dbCloseCursor,   "_dbCloseCursor",  XSQLMSQL,"XSQLMSQL.DLL");

 if (DosGetProcAddr(XSQLMSQL,"_dbInit",&funcRef))
 {xsqlLogPrint("XSQLMSQL.DLL is damaged!"); return value;}

 maxCursors[SQL_SERVER][0] = 10;

 value=funcRef();  

 if (!value)
     sybaseBound = TRUE;

 return value;

}
bindXSQLODBC()
{
 char failedObject[129];
 int  value=NOT_OK;
 int (*funcRef)();

 if ((value=DosLoadModule(failedObject,128,"XSQLODBC",&XSQLODBC)))
 {xsqlLogPrint("Error loading XSQLODBC.DLL: %d!",value); return value;}

 linkDBMSProc(qNeConnect,      "qNeConnect",      XSQLODBC, "XSQLODBC.DLL");
 linkDBMSProc(qNeDescribe,     "qNeDescribe",     XSQLODBC, "XSQLODBC.DLL");
 linkDBMSProc(qNeDisconnect,   "qNeDisconnect",   XSQLODBC, "XSQLODBC.DLL");
 linkDBMSProc(qNeError,        "qNeError",        XSQLODBC, "XSQLODBC.DLL");
 linkDBMSProc(qNeFetch,        "qNeFetch",        XSQLODBC, "XSQLODBC.DLL");
 linkDBMSProc(qNeFetchSQLERRD3,"qNeFetchSQLERRD3",XSQLODBC, "XSQLODBC.DLL");
 linkDBMSProc(qNeExecSQL,      "qNeExecSQL",      XSQLODBC, "XSQLODBC.DLL");
 linkDBMSProc(qNeExecSelect,   "qNeExecSelect",   XSQLODBC, "XSQLODBC.DLL");
 linkDBMSProc(qNeOfflineCursor,"qNeOfflineCursor",XSQLODBC, "XSQLODBC.DLL");
 linkDBMSProc(qNeCloseCursor,  "qNeCloseCursor",  XSQLODBC, "XSQLODBC.DLL");

 if (DosGetProcAddr(XSQLODBC,"qNeInit",&funcRef))
 {xsqlLogPrint("XSQLODBC.DLL is damaged!"); return value;}

 maxCursors[INTERSOLV][0] = 10;

 value=funcRef();  

 if (!value)
     qeBound = TRUE;

 return value;

}
bindXSQLPEER()
{
 char failedObject[129];
 int  value=NOT_OK;
 int (*funcRef)();

 if (DosLoadModule(failedObject,128,"XSQLPEER",&XSQLPEER))
 {xsqlLogPrint("Can't access XSQLPEER.DLL!"); return value;}

 linkDBMSProc(akConnect,       "akConnect",      XSQLPEER,"XSQLPEER.DLL");
 linkDBMSProc(akDescribe,      "akDescribe",     XSQLPEER,"XSQLPEER.DLL");
 linkDBMSProc(akDisconnect,    "akDisconnect",   XSQLPEER,"XSQLPEER.DLL");
 linkDBMSProc(akError,         "akError",        XSQLPEER,"XSQLPEER.DLL");
 linkDBMSProc(akFetch,         "akFetch",        XSQLPEER,"XSQLPEER.DLL");
 linkDBMSProc(akFetchSQLERRD3, "akFetchSQLERRD3",XSQLPEER,"XSQLPEER.DLL");
 linkDBMSProc(akExecSQL,       "akExecSQL",      XSQLPEER,"XSQLPEER.DLL");
 linkDBMSProc(akExecSelect,    "akExecSelect",   XSQLPEER,"XSQLPEER.DLL");
 linkDBMSProc(akOfflineCursor, "akOfflineCursor",XSQLPEER,"XSQLPEER.DLL");
 linkDBMSProc(akCloseCursor,   "akCloseCursor",  XSQLPEER,"XSQLPEER.DLL");

 if (DosGetProcAddr(XSQLPEER,"akInit",&funcRef))
 {xsqlLogPrint("XSQLPEER.DLL is damaged!"); return value;}

 maxCursors[NATIVE][0] = 10;

 value=funcRef();  

 if (!value)
     tcpBound = TRUE;

 return value;

}
//===========================================================================================
  xType2 dumpSQL(char *sqlString)
{
 char  preText[71],*sqlCursor=sqlString, theTime[20];
 int   i;

 if (!debugFile) debugFile=fopen("debug","w");
 _strtime( theTime );
 fprintf( debugFile, "%s ==============================================\n",
          theTime );
 fflush(debugFile);

 do {memset(preText,'\0',sizeof(preText));
     strncpy(preText,sqlCursor,70);
     for(i=0;i<strlen(preText);i++)
        {if (preText[i] == 10) preText[i] = '~';
         if (preText[i] == 13) preText[i] = '^';
        }
      fprintf(debugFile,"%s %-70s\n",theTime,preText); 
      fflush(debugFile);
      if (strlen(sqlCursor) > 70) sqlCursor += 70;
      else                        sqlCursor += strlen(sqlCursor);
    }
 while( strlen( sqlCursor ) );

 fprintf( debugFile, "%s ==============================================\n",
          theTime );
 fflush(debugFile);


}

