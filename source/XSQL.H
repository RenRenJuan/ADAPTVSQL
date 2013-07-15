// XSQL.H  

 #define MAX_CI_LEVELS     10
 #define MAX_CI_DESC_TEXT  8092

 enum  
 {
  RESERVED_CLASS_0,          //"CLASS_0" 
  CONFIGURATION_CONTROLS,    //"CONFIG"
  RESERVED_CLASS_1,          //"CLASS_1" 
  XSQL_PROFILE,              //"VARIABLE"
  GUI_CONTROLS,              //"CONTROL"
  MAX_PROFREC_TYPE,       // After this are non-class record types
  CONTROL_DEF,
  DATA_DEF
 } PROFILE_RECORD_TYPE ;

 enum {    // Relations of a name to point in a class graph
  NODE_ERROR,    // Indeterminate relation.
  NEW_SUB,       // Name would link between node and it's sub.
  PRE_SUB,       // Node has subs, this name prior to all.
  POST_SUB,      // Node has subs, this name after all
  PRE_NEXT,      // Name would go in next list prior to this node.
  POST_NEXT,     // Node is last next, this name after all
  NEW_RECORD,    // An aggregate relation, less than this, > NODE_ERROR are
  FOUND_LEAF,
  FOUND
      } CI_TREE_POINT;

 #if (SOURCE_MODULE == PROFILE_C)

  char *nodeSituation[FOUND +1] = {
                        "NODE_ERROR",
                        "NEW_SUB",    
                        "PRE_SUB",    
                        "POST_SUB",   
                        "PRE_NEXT",   
                        "POST_NEXT",  
                        "FOUND_LEAF"
                        "FOUND"
                                  };
 #endif

 typedef struct {
  unsigned                            levels:8; 
  unsigned                          reserved:8;
  unsigned short                   searchFocus; //Current level of search
  char                  *values[MAX_CI_LEVELS];
                } CI_NAME;
  
 typedef struct _CI_HEADER { 
  int short                           xsqlType;
  unsigned                             bound:1;
  unsigned                         reserved:15;
  unsigned short                          size;//Total byte size of value.
  char                                 *ciName;//Only the leaf
  char                                *ciTitle;
  char                          *ciDescription;
  void                                  *value;
  struct _CI_HEADER                      *next;
  void                                   *bind;

                } CI_HEADER; 

 typedef struct {
  int short                         recordType;
  unsigned short                      reserved;
  long                                  offset;
  int short                               size;
                } XSQL_HEADER_ELEMENT;

 // XSQL record type: 0         - Ordinary CI record,
 //                   Positive  - Class record.
 //                   -1        - Instance record.
 // (Not to be confused with xsqlType which is the extended SQL data type).

 typedef struct { // As stored in binary file.
  int short                         recordType;
  unsigned                          reserved:8;
  unsigned                             level:8;
  long                                    next;
  long                                     sub;
  unsigned short                          size;//Total size incl. this header
  unsigned short                        nItems;     
                } XSQL_RECORD_HEADER;

 typedef struct _XSQL_RECORD
               { // As stored in memory
  int short                         recordType;
  unsigned                             level:6;
  unsigned                        nextAbsent:1;
  unsigned                         subAbsent:1;
  unsigned                          nextMark:2;
  unsigned                           subMark:2;
  unsigned                          prevMark:2;
  unsigned                         superMark:2;
  unsigned short                       marking;
  struct _XSQL_RECORD                    *next;//All are sibs of same ...
  struct _XSQL_RECORD                    *prev;//... parent qualification.
  struct _XSQL_RECORD                     *sub;//Child and parent levels
  struct _XSQL_RECORD                   *super;//... of qualification.
  CI_HEADER                              *body;//CI_HEADERS followed by data 
  unsigned short                          size;//Total size excl. this header
  unsigned short                        nItems;//#(CIs) - 1 (Theres always
                                               // one for the record itself,
                                               // more indicates "leaves").     
  XSQL_RECORD_HEADER                     *toGo;
  long                                  offset;
                } XSQL_RECORD;

 typedef struct { 
  unsigned short                        offset;//From (0) start of record 
                                               //data area (after all CI
                                               //headers).
  unsigned short                          size;//In bytes
                } STORED_STRING;

 typedef struct { 
  int short                           xsqlType;
  int short                          xsqlState;
  STORED_STRING                         ciName;
  STORED_STRING                        ciTitle;
  STORED_STRING                  ciDescription;
  STORED_STRING                          value;
                } CI_FILE_HEADER; 

 typedef struct {
  char                                 *target;// i.e. connectName of 
  unsigned short                  targetHandle;
  unsigned short                    targetMode;
  char                        *sourceQualifier;
  char                        *targetQualifier;
  unsigned                     collectingSQL:1;              
  unsigned                       sqlDeclared:1;
  unsigned                         connected:1;
  unsigned                     localSQLError:1;
  unsigned                        cursorOpen:1;    
  unsigned           checkedForUpdateability:1;    
  unsigned                          reserved:2;
  unsigned                              mode:8;
  unsigned short                     nExtended; // Length of statements.
  unsigned short                   nStatements;
  char                              *selection; // CREATE selection query
  struct         sqlvar *keys[MAX_UPDATE_KEYS];
  EXTENDED_SQL                     *statements;
  char                            *sqlFileName;
                } BATCH_STATE;

 typedef struct {
  unsigned                         connected:1;
  unsigned                     virginDisplay:1;
  unsigned                       interactive:1;
  unsigned                             begun:1;
  unsigned                          batchSQL:1;
  unsigned                         storeText:1;
  unsigned                        guiEnabled:1;
  unsigned                          reserved:9;
  long                            thresholdFOM;
  int     short                   optimization;
  unsigned short                   nStatements;
  int      short                        sqlNow;
  unsigned short                      dbHandle; // default
  unsigned short                    accessMode; // "" 
  char                            *connectName; // ""
  char                                 *source;
  unsigned short                    sourceMode;
  unsigned short                  sourceHandle;
  char                             *parmString;
  EXTENDED_SQL                     *statements;
                } INTERACTIVE_STATE;

#if (PLATFORM == XSQL_OS2)
 EXTENDED_SQL * xType2 xsqlBatchLink(int short isFragment,char *xsqlFragment);
#elif (PLATFORM == XSQL_WINNT)
 xType2 EXTENDED_SQL * xsqlBatchLink(int short isFragment,char *xsqlFragment);
#endif
 int short xType2 xsqlBatchRun(EXTENDED_SQL * );


 typedef struct {
   unsigned         shutdown:1;
   unsigned         reserved:15;
                } XSQL_STATE;

#if (SOURCE_MODULE == XSQL_C)

 #define  MAX_STATEMENTS   100
 #define  STACK_SIZE       4096
 
 dclModeStrings;
 
  // Structure Simplification
 
  #define commit_mode       state->commitment
  #define connection        db[am.dbHandle]
  #define debug             state->debugCode
  #define debugFile         state->dump
  #define logFileName       state->LogFileName
  #define logFile           state->msgFile
  #define logMsgs           state->LogMsgs
  #define mySQLbuffer       state->MySQLBuffer
  #define sqlBuffer         result[thisCursor].userText
  #define sqlBufferCapacity result[thisCursor].highWater 
  #define userColumn        state->UserColumn
  #define userErrText       state->UserErrText
  #define work              state->Work
  #define oldSQL            state->sqlFileName  
  #define thisStatement     statementNow->text
  #define thatStatement     statementThen->text
  #define thisXStmnt        statementNow->augment
 
 char         textFileName[81], w[100],*a,*b,sqlNow[20];
 int short    theHandle;
 unsigned     nthCol,nCols,nthRow,nRows,rc,failure;

 EXTENDED_SQL *statementNow,*statementThen,*lastStatement,*myStatements,
              *otherLastStatement;

 BATCH_STATE       *other;
 STATE             *state;

 CONNECTION        *db;
 CURSOR            *result;
 INTERACTIVE_STATE *am;
 XSQL_RECORD      **theProfile;
 XSQL_STATE        *xSQLstate;

 CURSOR       * _System xsqlCursor(int short h);
 int short      _System xsqlLink(short *pid, BATCH_STATE **p,
                                 INTERACTIVE_STATE **c, COMMON_STATE **s); 
 SITE         * _System xsqlSetSiteDependencies(char *);
 CI_HEADER    * _System xsqlNamedCI(char *ciName,XSQL_RECORD **arg);
 int short      _System xsqlStoreProfile(void);
 int short      _System xsqlLoadProfile(void);
 EXTENDED_SQL * _System xsqlExecXSQL(int short, char *);
 void           _System xsqlEndXSQL(void);       

#else

 #if ( SOURCE_MODULE != BATCHSQL_C )

  #if (PLATFORM == XSQL_OS2)

  CURSOR       * _System xsqlCursor(int short h);
  int short      _System xsqlLink(int short *pid, BATCH_STATE **p,
                                INTERACTIVE_STATE **c, COMMON_STATE **s); 
  SITE         * _System xsqlSetSiteDependencies(char *);
  CI_HEADER    * _System xsqlNamedCI(char *ciName,XSQL_RECORD **arg);
  int short      _System xsqlStoreProfile(void);
  int short      _System xsqlLoadProfile(void);
  EXTENDED_SQL * _System xsqlExecXSQL(int short, char *);
  void           _System xsqlEndXSQL(void);       

  #elif (PLATFORM == XSQL_WINNT)

  CURSOR       * xsqlCursor(int short h);
  int short      xsqlLink(int short *pid, BATCH_STATE **p,
                        INTERACTIVE_STATE **c, COMMON_STATE **s); 
  SITE         * xsqlSetSiteDependencies(char *);
  CI_HEADER    * xsqlNamedCI(char *ciName,XSQL_RECORD **arg);
  int short      xsqlStoreProfile(void);
  int short      xsqlLoadProfile(void);
  EXTENDED_SQL * xsqlExecXSQL(int short, char *);
  void           xsqlEndXSQL(void);       

  #endif

 #endif

 #if ( SOURCE_MODULE == BATCHSQL_C || SOURCE_MODULE > DBMS_DRIVER)

  int                  nMarkings;
  BATCH_STATE         *am;
  INTERACTIVE_STATE   *other;
  XSQL_STATE          *xSQLstate;

 #else

  extern int                 nMarkings;
  extern BATCH_STATE        *am;
  extern INTERACTIVE_STATE  *other;
  extern XSQL_STATE         *xSQLstate;

 #endif

 #if ( SOURCE_MODULE == PROFILE_C )

  XSQL_RECORD             **theProfile;

 #else

  extern XSQL_RECORD      **theProfile;

 #endif


#endif
