/* ADPTVSQL.H Common XSQL Backend Interface Header File */
 
 #define HASH_TABLE_SIZE     47
 #define MAX_PARMS_PER_SQL  128
 #define MAX_DATABASE         4  // Per Application Process
 #define MAX_CURSORS        100  // Architectural Limit  
 #define MAX_APPLICATION      4  // Per Machine
 #define MAX_SQL_TEXT     31744  // PC Support determined limit
 #define MAX_COLUMN_TEXT  16376  // A sqlda/static limit, dynamic may be less

#if (PLATFORM == XSQL_WINNT)

 #define DllExport   __declspec( dllexport )
 #define DllImport   __declspec( dllimport )

#endif

#if (SOURCE_GROUP == XSQLDBMS_DLL)

   #if (PLATFORM == XSQL_OS2)

     #define xType2 _Export _System

   #endif

   #if (PLATFORM == XSQL_WINNT)

     #define xType2 DllExport

   #endif

  #else

   #if (PLATFORM == XSQL_OS2)

     #define xType2 _System

   #endif

   #if (PLATFORM == XSQL_WINNT)

     #define xType2 DllImport

   #endif

 #endif


 #if (SOURCE_MODULE >= DBMS_DRIVER)

    #if (PLATFORM == XSQL_OS2)

     #define xType3 _Export _System

    #endif
   
    #if (PLATFORM == XSQL_WINNT)

     #define xType3 DllExport

    #endif
   
  #else

    #if (PLATFORM == XSQL_OS2)

      #define xType3 _System

    #endif
 
    #if (PLATFORM == XSQL_WINNT)

     #define xType3 DllImport

    #endif

 #endif

 #if PLATFORM == XSQL_OS2

  #define malloc _tmalloc
  #define free   _tfree

 #endif

 #ifndef SQL16TO32

  #define SQL_TYP_ZONED       488
  #define SQL_TYP_NZONED      SQL_TYP_ZONED + SQL_TYP_NULINC

 #endif

 typedef struct _KNOWN_SQL {
  unsigned                            nParms:8;
  unsigned                        nextAbsent:1; // next is a CACHE_ELEMENT
  unsigned                           printed:1;
  unsigned                           sqlType:4; // From enum SQL_STATEMENT
  unsigned                              xsql:1; // DBMS independence. 
  unsigned                          reserved:1;
  unsigned short                           app; 
  unsigned short                          path; // Connection
  unsigned short                        cursor; // Index into context
  unsigned short                     *parmType;    
  unsigned short                   *parmLength;    
  long                                fullHash;
  long                               frequency; // Cumulative all epochs
  long                                    hits; // This epoch
  short int                     preparedNtimes; 
  char                               *exemplar; // The original text.
  char                              *prototype; // The parameterization.
  char                                 **parms; // Array of pointers.
  struct _KNOWN_SQL                      *next; // Collision chain
                           } KNOWN_SQL;

 #ifdef PC_SUPPORT_ONLY

  #define  BATCH_SQL  void

 #endif

// #include "sql.h"
// #include "sqlda.h"
  #include <xsql1.h>


 #if PLATFORM == WINNT

 #define _SQL_OS_WINNT

  #include <sqlfront.h>
  #include <sqldb.h>
  #include <xsql.h>
  #include <xsqlda.h>

 #endif



  #if (SOURCE_MODULE == ADPTVSQL_SQC)
    #include <sqlca.h>
    #include <sqlenv.h>
    #include <sqlutil.h>
  #endif

 
 enum SQL_STATEMENT {
  SQL_NOT,
   SQL_SELECT,        
  SQL_TRANSACTION,   // Greater than this are iff < SQL_CONNECT_TO ...
   SQL_INSERT,       // Each connection has it's cursor ...
   SQL_UPDATE,       // e.g. remotsql.c, adptvsql.sql, etc.
   SQL_XUPDATE,      // Mapping as opposed to simple update.
  SQL_HANDLE_RETURNED,
   SQL_DELETE,        
   SQL_CREATE,       // eligibility set at connect time
   SQL_XCREATE,      // Mapping as opposed to simple create.
   SQL_DROP,         // by the carrier specific module,
  SQL_CONNECT_TO,
  SQL_OTHER         };

 enum HOST_OS {
  IBM_OS2,
  LINUX,
  WINDOWS_3_1,
  WINDOWS_NT  };
              


 #include "appstate.h"

 typedef struct _SITE_ACCESS_MODE {
  char                                   *name;
  int short                         accessMode;
  struct _SITE_ACCESS_MODE               *next;
                                  } SITE;

 typedef struct {
  unsigned                         debugCode:8;
  unsigned                           logMsgs:3;
  unsigned                           traceOn:1;
  unsigned             canDistributeRequests:1;
  unsigned                   databaseStarted:1;
  unsigned                     DRDArequester:1;
  unsigned               actualCarrierLinked:1;
  unsigned                          nLogMsgs:4;
  unsigned                 activeConnections:4;
  unsigned           pointerCheckingDisabled:1; // Run fast and loose
  unsigned                          poppedUp:1; // VIO active
  unsigned                     pipeConnected:1;
  unsigned                    disconnectPipe:1; // User set's this
  unsigned                            hostOS:2;
  unsigned                      reservedBits:2;
  unsigned short                  sqlInProcess;
  short int                         commitment;
  char                                *lastLoc;
  short int                   lastCodeReturned;
  long                           logPipeHandle; // Copied to CONNECTION
  long                      logMsgCodeReturned;
  FILE                                *msgFile;
  FILE                                   *dump;
  void                                *proFile; // XSQL profile
  FILE                                *sqlFile; // XSQL text    filestream
  char                            *proFileName; // XSQL profile filename
  char                            *sqlFileName;
  char                         LogFileName[15];
  char                         LogMsgs[3][100];
  char                            *MySQLbuffer; // For library use only.
  char                             *UserColumn;
  char                            *UserErrText;
  char                                   *Work;
  SITE                                  *sites;
  void                               *reserved;
  unsigned short                      lastMark; // Profile DFS mark
                } STATE;

 typedef struct {
  unsigned                      boundColumn:12;
  unsigned                           isBound:1;
  unsigned                        isInternal:1;
  unsigned                        hasVarSize:1;
  unsigned                          reserved:1;
  unsigned short                       varSize;
  char                                  *value;
  short                             *sqlLength; // To conform to IBMRDMSs
  long                                 *length; // To conform to Q+E
  long                          internalLength; 
                } BINDING;

 typedef struct _A_ROW_BINDING {
  struct _A_ROW_BINDING                  *next;
  unsigned short                     nBindings;
  BINDING                                 *row;
                               } ROW_BINDING;
  
 typedef struct _A_CURSOR {
  unsigned                      actualHandle:8;
  unsigned                            active:1; 
  unsigned                          prepared:1;
  unsigned                        persisting:1;
  unsigned                    buildingScroll:1;
  unsigned                   hasUserBindings:1;
  unsigned                    bindingsCopied:1;
  unsigned                             inUse:1;
  unsigned                        cursorOpen:1;
  unsigned                        nullScroll:1;
  unsigned                           closing:1;
  unsigned                      firstClosing:1;
  unsigned                         reserved:13;
  unsigned short                      nColumns;
  unsigned short                     nBindings;
  KNOWN_SQL                             *cache; // KB cache entry
  unsigned                           highWater; // mark in SQL buffer
  enum SQL_STATEMENT                    sqlNow;
  short int                               path; // db index
  long                                   nRows;
  long                                  rowPos; // Current scroll ordinal
  long                                   stamp; // Timestamp
  char                               *userText;
  struct sqlda                     *attributes;
  void                             *parameters; 
  void                    *augmentedAttributes;
  char                                 *format; // PCS format string
  BINDING                            *bindings;
  ROW_BINDING                            *next; // Row in scroll
  ROW_BINDING                          *scroll; // Points to first ...
  ROW_BINDING                            first; // ... when scroll complete.
  ROW_BINDING                             user; 
                } CURSOR;

 typedef union  {
  long                              fileOffset; // Into historical file
  KNOWN_SQL                             *entry; // Non-NULL iff present
                } CACHE_ELEMENT;
 
 typedef struct _APPLICATION {
  CACHE_ELEMENT                         *cache; // Common Application
  CURSOR                              *context; //     Interface
  STATE                                   *has; 
  CONNECTION                  db[MAX_DATABASE];
                             } APPLICATION;

 typedef struct {
  unsigned                             nApps:8;
  unsigned                          reserved:8;
  long                                    gate; // For serialization
  APPLICATION            *app[MAX_APPLICATION];
                } COMMON_STATE;

 typedef struct _EXTENDED_SQL {
  unsigned                     statementType:8;
  unsigned                          reserved:8;
  BATCH_SQL                           *augment;
  char                                   *text;
  struct _EXTENDED_SQL                   *next;
  struct _EXTENDED_SQL                   *prev;
                              } EXTENDED_SQL;


 #if (PLATFORM == XSQL_OS2)

  int xType2 execAdaptive(STATE **state,CURSOR **context,CONNECTION **dbs);

 #endif
 #if (PLATFORM == XSQL_WINNT)

  xType2 int execAdaptive(STATE **state,CURSOR **context,CONNECTION **dbs);

 #endif
  int execConnect(int thisConnect);
  int execDisconnect(int thisConnect);
  int execEndSQL(int thisSQL);
  int execError(int wasCode,char *whichAPI,int thisConnect);  
  int execSelect(int thisCursor);
  int execSQL(int thisCursor);
  int execSQLERRD3(long *value,int thisCursor);
  KNOWN_SQL * knownSQL(int thisConnect,char *SQLstring,int sqlNow);
  int RDBMSATTR(int thisCursor);
  int RDBMSCLOSE(int thisCursor);
  int RDBMSFETCH(int thisCursor,char **fetched);
