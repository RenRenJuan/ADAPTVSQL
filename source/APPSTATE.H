#ifndef  ADPTVSQL_ACCESS_MODES
 #define ADPTVSQL_ACCESS_MODES
                      
 enum ADPTVSQL_ACCESS_MODES {
 SYSTEM_TRANSPORT,   // System mode, not valid for xsqlConnect
 LOCAL_PC_SUPPORT,   // Local  AS400 via PC Support Remote SQL
 DDM_PC_SUPPORT,     // Remote AS400 via PC Support Remote SQL (For Non-Relational Files)
 RELATIONAL_DBMS,    // Less than this are NOT, greater ARE
  LOCAL_COLLECTION,  // Local OS/400 Database Manager via PC Support Remote SQL
  DRDA_PC_SUPPORT,   // Remote OS/400 Database Manager via DRDA/PC Support
 INTERSOLV,          // Q+E Lib ODBC
 SQL_SERVER,         // MS SQL Server
 DATABASE_MANAGER,   // Greater than or equal this and less than NATIVE are...
  LOCAL_DOMAIN,      //  ... Local  or remote DB/2 database,
  DDCS_2,            //  ... Remote IBM RDBMS via DDCS.
 NATIVE,             // < => via this (OS/2) host.
                     // = => remote TCP/IP host via XSQLPEER.
                     // > => via this Linux host.
                     // Linux.
  POSTGRES,          // Postgres 
 NOT_AN_ACCESS_MODE  // This or greater aren't.   
                            };

 enum OSX_API {      // XSQL Function Classes.
   OS_EXEC_SQL,       
   OS_SYNCHRONOUS,   // Greater than this asynch eligible.
   OS_CONNECT,       // They are the only ones the application 
   OS_DISCONNECT     //  receives notification for. All others
                     //  operate synchronously in the user's
                     //  thread.
              };

 enum IBMRDBMS_VERSIONS {
   DB22_211,         // The default for the current release of XSQL.
   DBM_32,           // Database Manager, 32bit driver
   DBM_16,           // Database Manager, 16bit driver
   OS400_211,        // PC Support Remote SQL v 2.11 
   NOT_A_XSQL_IBMRDBMS  
                        };

 #define N_MODES  13

 #define dclModeStrings char *modeStrings[N_MODES] = { "LOCAL_PC_SUPPORT", \
            "DDM_PC_SUPPORT", NULL, "LOCAL_COLLECTION", "DRDA_PC_SUPPORT", \
            "Q+E", "SYBASE", "DATABASE_MANAGER", "LOCAL_DOMAIN", \
            "DDCS/2", "NATIVE", "LAN", "MODEM"  }

 // maxCursors[anAccessMode,0] will be 0 if that mode not supported, a
 // system imposed upper limit otherwise. maxCursors[anAccessMode] must
 // be greater than 1 for the mode to have persisting cursors. Use
 // xsqlSetWorkingSet prior to the first xsqlConnect to or set values or
 // at anytime to get values. maxCursors[anAccessMode,1] is continuously
 // updated to reflect the number open in anAccessMode.

 #define dclModeArities int   maxCursors[N_MODES][2];

 #define xdclModeStrings extern char *modeStrings[N_MODES]
 #define xdclModeArities extern int maxCursors[N_MODES][2]

 // NB: Parameters in the CONNECTION structure should only be changed
 //     between the execution of an osGetConnectSlot and osConnect for
 //     a connection. It is NOT safe to alter these parameters on a live
 //     connection. The only parameters which an application can set are
 //     the accessMode, optimizationLevel, thresholdFrequency, database
 //     elements, and reporting controls.

 typedef struct {
  long                                    gate; // Serialization Sem
  void                                  *state; // SQLCA
  char                                 *object; // Connected To 
  char                                *subject; // Connecting Agent
  char                                *channel; // Connected Via
  // Significance is mode dependendent -
  // 
  //  for PC Support - APPC partner name, default: "5250PLU"
  //  for Q+E        - connect string, default:  "DRV=QEDBF"
  //  for TCP/IP     - host name or IP address, default: xsqlsrvr.kybernet.com
  // 
  unsigned                        accessMode:4;
  //  
  //  Set accessMode to a value from the enum ADPTVSQL_ACCESS_MODES
  //
  unsigned                             inUse:1;
  unsigned                       forceReport:2; // 0 - never
    // i.e. print SQL statement info:           // 1 - once
                                                // 2 - always
  unsigned                           pipeLog:1; // Copy log stream to user.
  unsigned                      roleIsSource:1; // FALSE -> role is target.
  unsigned                          reserved:5;
  unsigned                         connected:1;
  unsigned                        connecting:1;
  int short                cursorIneligibility; // =accessMode(self)
  int short                         unitOfWork; // transactions held per
  int short                        uncommitted; // in unitOfWork
  long                                    nSQL; // Statements executed
  int short                         sqlModulus; // Print report every
  char                            *userMessage; //    with user report info
  int short                  optimizationLevel;
  //
  //    0:  Disallow persisting cursors (the default).
  //    1:  Cursor persistence, without learning, no statement profiles.
  //    2:  Cursor persistence, with learning. Maintain profiles at each
  //        sqlModulus, add average historical frequency to current epoch
  //        frequency in determination of working set. Read profile if found
  //        when osConnect executed, and set cache from contents.
  //    3:  Cursor persistence with selective learning. Maintain a cache
  //        entry in the profile only when the user has forced optimization
  //          
  //
  long                      thresholdFrequency;
  //
  //  Specifies the number of times an SQL statement must be executed
  //  before it gets a persisting cursor. If zero and optimizationLevel
  //  is non-zero then all SQL statements executed are given persisting
  //  cursors in the current application state, up to the internal limit.
  //  The default is 0. Note that the figure is compared against average
  //  historical occurences plus those in current application state if
  //  optimizationLevel > 1.
  long                               appWindow; // GUI owner window. 
                                                //  Application should set
                                                //  if not in background.
  void                                 *module; // The session level
                                                //  module when active.
  long                                  handle; // DBMS Specific handle.
  void                                *details; // DBMS Specific info.
                } CONNECTION;

#endif
