#ifndef BATCH_EXTENSION

 #define BATCH_EXTENSION
 #define ADPTVSQL_SQC        2101
 #define BATCHSQL_C          2020
 #define PROFILE_C           2030
 #define XSQL_EXE            3000
 #define XSQL_C              3001

 #define LOG_PIPE_SIZE       32768
 #define MAX_UPDATE_KEYS     5

 typedef struct {
  unsigned                     mappedColumns:5;
  unsigned                      sqlIsMapping:1;
  unsigned                        sqlIsUsing:1;
  unsigned                      isUpdateable:1;
  unsigned                  sqlIsCreateTable:1;
  unsigned                  sqlIsUpdateTable:1;
  unsigned                     skipTableLoad:1;
  unsigned                        updateKeys:5;
  unsigned                   skipTableUpdate:1;
  unsigned                       notExtended:1;
  unsigned                           command:6;
  unsigned                          nColumns:8;
  unsigned short                  nthStatement; // Ordinal position in LL
  char                                *prequel; // The extended SQL stmnt
  char                                 *sequel; // Qualifying query
  char         *updateColumns[MAX_UPDATE_KEYS];
  char                        *sourceTableName;
  char                        *targetTableName;
  char                               **mapping; // Source-Target col. map
  KNOWN_SQL                             *cache;
                 } BATCH_SQL;

#endif
