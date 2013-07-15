//#include ..\misc\batchsql.cmt                                                                      
#include <xsqlmods.h>
#define SOURCE_GROUP   XSQLDBMS_DLL
#define SOURCE_MODULE  BATCHSQL_C 

#define INCL_DOSPROCESS
#define INCL_DOSNMPIPES
#define INCL_16DOSNMPIPES
#define INCL_DOSERRORS

#define XSQL_OS2               1
#define XSQL_Linux             2
#define XSQL_WINNT             3
#define DBNTWIN32

#include <xsqlc.h>

#define NP_ACCESS_OUTBOUND 1
#define NP_NOINHERIT       2
#define NP_NOWRITEBEHIND   4
#define NP_NOWAIT          8
#define NP_TYPE_MESSAGE   16

#define HPIPE  long
#define PHPIPE HPIPE *


 #define DEBUG_LIMIT_DEFAULT       1000
 #define MSG_ROW_QUANTUM           1000
 #define ROWS_PER_UNIT_OF_WORK     100

 #define TRUE                      1
 #define FALSE                     0
 #define OK                        FALSE
 #define NOT_OK                    TRUE 

 #define MAX_COLUMNS               200
 #define MAX_TEXT_LINE             255

 #define ARCHIVE_COL_NAME          "ARCHIVE"
 #define INSERT_VALUE              '\0'
 #define ALT_INSERT_VALUE          ' '
 #define DELETE_VALUE              'D'
 #define UPDATE_VALUE              'U'
 #define ARCHIVED_VALUE            'A'

 #define SOURCE                     0
 #define TARGET                     1

 // Structure Simplification

 #define sourceContext     sql[sourceHandle-1]
 #define targetContext     sql[targetHandle-1]
 #define sourceSQLDA       sourceContext.attributes
 #define targetSQLDA       targetContext->attributes
 #define sqlFile           state->sqlFile
 #define thatStatement     statementThen->text
 #define thisStatement     statementNow->text
 #define thisXStmnt        statementNow->augment
 #define lastXStmnt        lastStatement->augment
 #define sourceTable       thisXStmnt->sourceTableName
 #define targetTable       thisXStmnt->targetTableName
 #define sourcePrefix      am->sourceQualifier
 #define targetPrefix      am->targetQualifier

 // Functional 

 #define checkLocalSQL(code,api)   if (xsqlErr())       \
               {execError(code,api,thisConnect);      \
                if (am->localSQLError)    goto STOP;\
                am->localSQLError = TRUE; goto stop;}

 char                      *inFileName;
 char     columnValue[MAX_COLUMN_TEXT];
 char   sqlBufferTail[MAX_COLUMN_TEXT];
 char                       *sqlCursor;
 char                thisTableName[31];
 char                  *specifiedAS400;
 char                      *collection;

 unsigned                   lineNumber;
 unsigned                   debugLimit=DEBUG_LIMIT_DEFAULT;
 unsigned  nullIndicators[MAX_COLUMNS];

 char                        *localSQL; 
 char                         *updator;


 int         sourceHandle,targetHandle;

 struct sqlca                    sqlca;

 int         insertRow(EXTENDED_SQL *,unsigned,char *);
 char       *ConvertStrSQL( char *source, char *dest, int size );
 int         parseComment(EXTENDED_SQL *statementNow,char *sqlDeclaration);
 int         xsqlBatchMap(EXTENDED_SQL *statementNow);
 CONNECTION *xsqlBatchConnect(char *connectStatement,int role);
 int         updateRow(EXTENDED_SQL *,unsigned,char *);

 xdclModeStrings;
 xdclModeArities;

 char *sqlVerbs[11] = {
  "SQL_NOT",
  "SELECT",        
  "SQL_TRANSACTION", 
  "INSERT",     
  "UPDATE",     
  "SQL_HANDLE_RETURNED",
  "DELETE",        
  "CREATE",     
  "DROP",       
  "CONNECT",
  "SQL_OTHER"         };

#undef sqlBuffer
#undef sqlBufferCapacity
#undef work

char *xsqlgets(char *inFileBuffer,void *streamArg, int isFragment,
               char **fragmentCursor);
void freeStatement(EXTENDED_SQL *gone);
char **parseMapping(EXTENDED_SQL *s,char *rawMap,int startingLine);
//=========================================================================
#if (PLATFORM == XSQL_OS2)
  EXTENDED_SQL * xType2 batchLink(int short isFragment, char *xsqlFragment)
#endif
#if (PLATFORM == XSQL_WINNT)
  xType2 EXTENDED_SQL * batchLink(int short isFragment, char *xsqlFragment)
#endif
{
 char          work[128],*p,*sourcePath,*targetPath,*fragmentCursor=NULL,
              *sqlBuffer,inFileBuffer[MAX_TEXT_LINE+1],
               sqlDeclaration[MAX_TEXT_LINE+1];
 void         *streamArg;
 int short     firstArg,i,lastArg,thisCursor,thisArg,theArg,which,validRef,
               finished=FALSE,mapStart;
 unsigned     *mode,hasTargetUser=FALSE,sqlBufferCapacity;
 EXTENDED_SQL *statementNow=NULL,*lastStatement=NULL,*temp;

 sqlBuffer  = malloc(MAX_SQL_TEXT);

 checkReference( am->sqlFileName, validRef );
 if ( !validRef )
 {
    xsqlLogPrint( "Scan link failure: 1" ); 
    return NULL;
 }

 if (!isFragment)
 {if (!sqlFile && !(sqlFile=fopen(am->sqlFileName,"r")))
      return lastStatement;
  xsqlLogPrint( "Scanning XSQL in %s ....", am->sqlFileName );
  streamArg = sqlFile;
 }
  else {checkReference( xsqlFragment, validRef );
        if ( !validRef )
        {
         xsqlLogPrint( "Scan link failure: 1F" ); 
         return NULL;
        }
        xsqlLogPrint( "Scanning Extended SQL Fragment ..." );
        streamArg = xsqlFragment;
       }

 while(xsqlgets(inFileBuffer,streamArg,isFragment,&fragmentCursor))
 {lineNumber++;
  if (inFileBuffer[0]=='$')
  {char *commandCursor,keyLength=0;

   commandCursor = inFileBuffer + (keyLength=strcspn(inFileBuffer,"="));
   if (*commandCursor != '=') goto parseFailure;
   commandCursor++;

   statementNow = malloc(sizeof(EXTENDED_SQL));
   memset(statementNow,'\0',sizeof(EXTENDED_SQL));
   if (!lastStatement)
         am->statements      = statementNow;
   else {statementNow->prev  = lastStatement;
         lastStatement->next = statementNow;
        }
   statementNow->statementType
                             = SQL_NOT;
   lastStatement = statementNow;
   thisXStmnt= malloc(sizeof(BATCH_SQL));
   memset(thisXStmnt,'\0',sizeof(BATCH_SQL));
   am->nStatements++;
   thisXStmnt->nthStatement  = am->nStatements;

   if (!strnicmp(inFileBuffer,"$SOURCE=",keyLength))
   {thisXStmnt->command      = SET_SOURCE_CONNECT;
    goto commandRecognized;
   }

   if (!strnicmp(inFileBuffer,"$TARGET=",keyLength))
   {thisXStmnt->command      = SET_TARGET_CONNECT;
    goto commandRecognized;
   }

   if (!strnicmp(inFileBuffer,"$DEFAULT=",keyLength))
   {thisXStmnt->command      = SET_DEFAULT_CONNECT;
    goto commandRecognized;
   }

   goto parseFailure;

  commandRecognized:  { char *srcTableName;
   
   thisStatement               = malloc(strlen(inFileBuffer)+3);
   memcpy(thisStatement,inFileBuffer,strlen(inFileBuffer)-1);
   thisStatement[strlen(inFileBuffer)-1]
                               = 0;
   srcTableName                = thisStatement + keyLength + 1;
   thisXStmnt->sourceTableName = malloc( strlen(srcTableName)+1);
   strcpy(thisXStmnt->sourceTableName,srcTableName);
   continue;         
                      }
   
  }
  parseCompletion:
  if (!am->sqlDeclared)
  {if (!strncmp(inFileBuffer,"--",2))
   {//if (_heapchk()!=_HEAPOK) exit(1);
    if (!isalpha(inFileBuffer[2])) goto nextLine;
    else {am->sqlDeclared = TRUE;
          statementNow    = malloc(sizeof(EXTENDED_SQL));
          memset(statementNow,'\0',sizeof(EXTENDED_SQL));
          if (!lastStatement)
                am->statements      = statementNow;
          else {statementNow->prev  = lastStatement;
                lastStatement->next = statementNow;
               }
          lastStatement = statementNow;
          thisXStmnt= malloc(sizeof(BATCH_SQL));
          memset(thisXStmnt,'\0',sizeof(BATCH_SQL));
          strcpy(sqlDeclaration,&inFileBuffer[2]);
          sqlDeclaration[strlen(sqlDeclaration)] = '\0';
          if (parseComment(statementNow,sqlDeclaration)
              && !thisXStmnt->notExtended) goto parseFailure;
          if (thisXStmnt->sqlIsMapping || thisXStmnt->sqlIsUsing)
          {if (!statementNow->prev) {xsqlLogPrint(
             "Extended construct missing head at line %d",lineNumber);
                                     goto failed;
                                    };
           if (thisXStmnt->sqlIsMapping)
               mapStart = lineNumber;
           if (lastXStmnt->notExtended)
           {lastXStmnt->notExtended = FALSE;
            if (lastStatement->statementType != SQL_CREATE)
            {xsqlLogPrint("Batch clause invalid at line %d",lineNumber);
             goto failed;
            }
            lastStatement->statementType = SQL_XCREATE;
           }
          }
          else {thisXStmnt->prequel = malloc(strlen(sqlDeclaration)+3);
                if (sqlDeclaration[strlen(sqlDeclaration)-1] == '\n')
                    sqlDeclaration[strlen(sqlDeclaration)-1]  = '\0';
                strcpy(thisXStmnt->prequel,"--");
                strcat(thisXStmnt->prequel,sqlDeclaration);
                am->nStatements++;
                thisXStmnt->nthStatement = am->nStatements;
                // if (thisXStmnt->sqlIsUpdateTable &&
                //    !thisXStmnt->skipTableLoad)
                //    statementNow->text = thisXStmnt->prequel;
               }
         }
   }
    else {parseFailure: 
             xsqlLogPrint("Unrecognizable construct at line %d.",lineNumber);
          goto failed;
         }
  } 
   else // am->sqlDeclared
     {
    finish:
      if (!am->collectingSQL)
      {memset(sqlBuffer,'\0',MAX_SQL_TEXT);
       sqlCursor           = sqlBuffer;
       sqlBufferCapacity   = MAX_SQL_TEXT;
       am->collectingSQL   = TRUE;
      }
      if (inFileBuffer[strlen(inFileBuffer)-1] == '\n')
          inFileBuffer[strlen(inFileBuffer)-1]  = '\0';
      if (strlen(inFileBuffer) > sqlBufferCapacity)
      {xsqlLogPrint("Line %d: XSQL construct is > %d bytes.",
                   lineNumber,MAX_SQL_TEXT);
       goto failed;
      }
      if (!strncmp(inFileBuffer,"--",2) || finished) 
      {if (thisXStmnt->sqlIsCreateTable && thisXStmnt->nColumns)
           thisXStmnt->nColumns--;
       am->sqlDeclared   = FALSE;
       am->collectingSQL = FALSE;
       if (!thisXStmnt->sqlIsUpdateTable
           && !thisXStmnt->sqlIsMapping
           && !thisXStmnt->sqlIsUsing  )
       {statementNow->text   = malloc(strlen(sqlBuffer)+1);
        strcpy(statementNow->text,sqlBuffer);
       }
       if (thisXStmnt->sqlIsUsing)
       {lastStatement         = statementNow->prev;
        lastStatement->next   = NULL;
        temp                  = statementNow;
        statementNow          = lastStatement;
        thisXStmnt->sequel    = malloc(strlen(sqlBuffer)+1);
        thisXStmnt->sqlIsUsing=TRUE;
        strcpy(thisXStmnt->sequel,sqlBuffer);
        freeStatement(temp);
        Free(temp);
       }
       if (thisXStmnt->sqlIsMapping)
       {lastStatement            = statementNow->prev;
        lastStatement->next      = NULL;
        temp                     = statementNow;
        statementNow             = lastStatement;
        thisXStmnt->sqlIsMapping = TRUE;
        if (!(thisXStmnt->mapping
              =parseMapping(statementNow,sqlBuffer,mapStart)))
               goto failed;
        freeStatement(temp);
        Free(temp);
       }
       if (!finished) goto parseCompletion;
      } else {sqlBufferCapacity -= strlen(inFileBuffer);
              strcat(sqlBuffer,inFileBuffer);
              if (thisXStmnt->sqlIsMapping)
                  strcat(sqlBuffer,"\r");
             }
     }
  nextLine: memset(inFileBuffer,'\0',sizeof(inFileBuffer));     
  if (lastStatement && lastStatement->text)
  {if (lastStatement->statementType > SQL_NOT &&
       lastStatement->statementType < SQL_HANDLE_RETURNED &&
       lastStatement->augment && !lastStatement->augment->cache)
    lastStatement->augment->cache =
      knownSQL( 0 ,lastStatement->text,lastStatement->statementType);
  }
 }

 checkTheHeap("'BatchLink");

 if (am->sqlDeclared)
 {finished = TRUE;
  goto finish;
 }

 xsqlLogPrint("...%d XSQL constructs scanned OK. ",am->nStatements);  
 goto stop;

 failed: xsqlLogPrint("...Scan completed in error.");
         if (am->statements)
         {for(statementNow  = am->statements; statementNow;
              lastStatement = statementNow,
              statementNow = lastStatement->next)
             {freeStatement(statementNow);
              Free(lastStatement);
             }

          Free(lastStatement);
          am->statements = NULL;
         }
         lastStatement = NULL;

stop:

  if ( !isFragment )  { fclose(sqlFile);
                        sqlFile=NULL;
                      }

  other->statements  = am->statements;
  other->nStatements = am->nStatements;

  Free(sqlBuffer);

  //xsqlTraceOn( NULL, -301 );

  return(lastStatement);

}
//=========================================================================
int short xType2 batchRun(EXTENDED_SQL *statementNow) 
{char work[40];
 int short value=OK,connectOnly=FALSE,validRef;

 checkRefWithSize( thisXStmnt, sizeof( BATCH_SQL ), validRef );
 if ( !validRef )
 {
    xsqlLogPrint( "XSQL execution failure: 3.1" );
    return 0;
 }

 switch(thisXStmnt->command)
 {case SET_TARGET_CONNECT:
            {SITE *specifiedSite=NULL;
             int   targetWasNull=FALSE, differentTarget=FALSE,
                   crossConnect=FALSE;

              checkReference( thisXStmnt->sourceTableName, validRef );
              if ( !validRef )
              {
               xsqlLogPrint( "XSQL execution failure: 3.2" );
               return 0;
              }
              
              specifiedSite=setSiteDependencies(thisXStmnt->sourceTableName);

              if (!specifiedSite)
              {xsqlLogPrint("%s not a valid database or server.",
                            thisXStmnt->sourceTableName); 
               goto failure;
              }
              if (!am->target)
               {am->target = malloc(strlen(specifiedSite->name) + 1);
                strcpy( am->target, specifiedSite->name );
                targetWasNull = TRUE;
                if (other->source)
                    if (!stricmp(thisXStmnt->sourceTableName,other->source))
                        crossConnect=TRUE;
               }
              else {if (other->source)
                      if (!stricmp(other->source,am->target))
                        crossConnect=TRUE;
                    if (stricmp(am->target,thisXStmnt->sourceTableName))
                      differentTarget = TRUE;
                   }
              if (targetWasNull || differentTarget || crossConnect )
              {
               if (!targetWasNull)
               {xsqlLogPrint("Disconnecting from %s.",am->target);
                xsqlDisconnect(am->targetHandle);
                am->target = malloc(strlen(specifiedSite->name) + 1);
                strcpy( am->target, specifiedSite->name );
                am->target = specifiedSite->name;
               }
               if (crossConnect)
                 {am->target      = other->source;
                  am->connected   = other->connected;
                  am->targetMode  = other->sourceMode;
                  am->targetHandle= other->sourceHandle;
                  if (other->connected)
                      xsqlLogPrint("TARGET %s connected to SOURCE %s.",
                                 am->target, other->source);
                  else
                      xsqlLogPrint("Warning: TARGET set to unconnected SOURCE!");
                 }
               else
                 {am->connected   = FALSE;
                  am->targetMode  = specifiedSite->accessMode;
                  if ( !xsqlBatchConnect( NULL, TARGET )) goto failure;
                 }
              }
            }
       connectOnly=TRUE;
       break;
  case NOT_A_BATCH_COMMAND:;
       break;
  default:
       xsqlLogPrint("Unrecognized command -");
       xsqlLogPrint(thisStatement);
       goto failure;
 }

 if (!am->connected)
 {xsqlBatchConnect( NULL, TARGET );
  if (!am->connected)
  {didntConnect:
   xsqlLogPrint("Connect to target %s failed.",am->target);
   goto failure;
  }
 }

 if (connectOnly) goto done;

 if (xsqlBatchMap(statementNow))
 {failure:
  xsqlLogPrint("Aborting XSQL execution due to failure of the statement above.");
  value=NOT_OK;
  goto failed;
 }
 goto done;

 failed: value = NOT_OK;

 done:

  if (thisXStmnt->nthStatement == am->nStatements || value)
  {if (am->connected) 
   {xsqlDisconnect(am->targetHandle);
    am->connected = FALSE;
   }
  }

 return(value);

}
//===========================================================================
char *ConvertStrSQL( char *source, char *dest,int size)
  {
  char *s = source;
  char *d = dest;
  char *p = d + size;

  while ( *s )
    {
    *d++ = *s;                          // copy source to dest.
    if ( *s == '\'' )
      {
      *d++ = '\'';                      // replace single ' with ''
      ++p;
      }
    ++s;
    }
   while ( d < p )                       // fill the rest with spaces
    {
    *d++ = ' ';
    }
  *d = 0;                               // null terminate dest. string

  return ( dest );
  }
//=========================================================================
#if (PLATFORM == XSQL_OS2)
  char * xType2 displayMap(BATCH_SQL *thisX)
#endif
#if (PLATFORM == XSQL_WINNT)
  xType2 char * displayMap(BATCH_SQL *thisX)
#endif
{
 char *display=malloc(4096),*sourceColumn,*targetColumn,*value;
 int   mappedColumns=0;

 memset(display,0,4096);

 for (;mappedColumns<thisX->mappedColumns;mappedColumns++)
 {
  sourceColumn = thisX->mapping[mappedColumns*2];
  targetColumn = thisX->mapping[(mappedColumns*2)+1];
  strcat(display,sourceColumn);
  strcat(display,targetColumn);
  strcat(display,"\r");
 }

 value = malloc(strlen(display) + 1);
 strcpy(value,display);
 free(display);
 return value;

}
//=========================================================================
void freeStatement(EXTENDED_SQL *gone)
{
 EXTENDED_SQL *statementNow=gone;

  Free(thisXStmnt->prequel);
  Free(thisXStmnt->sequel);
  if (sourceTable && strcmp(sourceTable,"nil"))
     Free(thisXStmnt->sourceTableName);
  if (targetTable && strcmp(targetTable,"nil"))
     Free(thisXStmnt->targetTableName);
  Free(thisXStmnt);
  Free(thisStatement);
}
//=========================================================================
int short xType2 xsqlFreeXSQL(EXTENDED_SQL *toGo)
{
 EXTENDED_SQL *prev,*next;
 int           validP;

  checkReference( toGo, validP );
  if (!validP)
  {xsqlLogPrint("xsqlFreeXSQL - invalid argument");
   return FALSE;
  }

  if ( !am->nStatements || !other->nStatements)
  {xsqlLogPrint("xsqlFreeXSQL - nothing to delete!");
   return FALSE;
  }

  prev = toGo->prev;
  next = toGo->next;

  if (!prev)
  {    am->statements    = next;
       other->statements = next;
  }
  else 
       prev->next        = next;

  if (next)
      next->prev = prev;
        
  freeStatement( toGo );
  Free(toGo);

  am->nStatements--;
  other->nStatements--;

  return(TRUE);

}
//=========================================================================
int insertRow(EXTENDED_SQL *statementNow,unsigned row,char *sqlBuffer)
{
 char          work[40],*colValCursor,*theData;
 unsigned      i,j,nthColumn,thisCursor,sqlBufferCapacity;
 short         *theLength;
 struct sqlvar *thisColumn;

  sprintf(sqlBuffer,"INSERT INTO %s/%s VALUES (",collection,thisTableName);
  sqlBufferCapacity -= strlen(sqlBuffer);

  for (nthColumn=0;nthColumn<thisXStmnt->nColumns;nthColumn++)
  {memset(columnValue,'\0',sizeof(columnValue));
   thisColumn    = &(sourceSQLDA->sqlvar[nthColumn]);
   theData   = thisColumn->sqldata;
   theLength = &(thisColumn->sqllen);
   switch(thisColumn->sqltype)
   {case SQL_TYP_LONG:   
    case SQL_TYP_VARCHAR:  theLength = (short *)theData;    
                           theData   = theData + 2;
    case SQL_TYP_CHAR: 
     doColumn:
       columnValue[0] = '\'';
       theData[*theLength] = '\0';
       ConvertStrSQL(theData,&columnValue[1],0);
       sqlBufferCapacity -= strlen(columnValue);
     break;
    case SQL_TYP_NVARCHAR: 
    case SQL_TYP_NLONG:    theLength = (short *)theData;    
                           theData = theData + 2;    
    case SQL_TYP_NCHAR:      
       if (!nullIndicators[nthColumn]) goto doColumn;
       if (nthColumn == thisXStmnt->nColumns - 1)
            strcpy(columnValue,"''");
       else strcpy(columnValue,"'',");
       sqlBufferCapacity -= strlen(columnValue);
     break;
   }
   if (!nullIndicators[nthColumn])
   {if (nthColumn == thisXStmnt->nColumns - 1) {strcat(columnValue,"'");
                                                sqlBufferCapacity--;
                                               }
    else {strcat(columnValue,"',"); sqlBufferCapacity -= 2;}
   }
   if (sqlBufferCapacity < 1) 
   {xsqlLogPrint("SQL Insert Overflow at col. %s, row %d.",
                thisColumn->sqlname.data,row);
    return(NOT_OK);
   }
   strcat(sqlBuffer,columnValue);
  }
  strcat(sqlBuffer,")");

 return(OK);

}
//=========================================================================
int parseComment(EXTENDED_SQL *statementNow,char *sqlDeclaration)
{ 
 char     *cursor=sqlDeclaration,cBuggy[13]; // So named 4 bug in strnicmp
 unsigned tokenSize,isKeyword=TRUE,sqlType;

  classify(sqlDeclaration,statementNow->statementType);

 classified:

  if (strlen(sqlDeclaration) < 12) goto notExtendedSQLStart;
   memcpy(cBuggy,sqlDeclaration,12); cBuggy[12] = '\0';

  if (!strcmp(cBuggy,"CREATE TABLE"))
  {thisXStmnt->sqlIsCreateTable = TRUE;
   thisXStmnt->notExtended      = TRUE; // retractable;
   goto keywords;}

  if (!stricmp(cBuggy,"UPDATE TABLE"))
  {thisXStmnt->sqlIsUpdateTable = TRUE;
   statementNow->statementType  = SQL_XUPDATE;
   goto keywords;}

  notExtendedSQLStart:
   if (strlen(sqlDeclaration) < 7) goto notExtendedSQLSequel1;
   memcpy(cBuggy,sqlDeclaration,7);  cBuggy[7] = '\0';

  if (!strcmp(cBuggy,"MAPPING"))
  {thisXStmnt->sqlIsMapping = TRUE; goto done;}

  notExtendedSQLSequel1:
   if (strlen(sqlDeclaration) < 5) goto notExtendedSQL;
   memcpy(cBuggy,sqlDeclaration,12);  cBuggy[5] = '\0';

  if (!strcmp(cBuggy,"USING"))
  {thisXStmnt->sqlIsUsing = TRUE; goto done;}

 notExtendedSQL: 
  
  thisXStmnt->notExtended     = TRUE;
  thisXStmnt->sourceTableName = "nil";
  thisXStmnt->targetTableName = "nil";
  goto done;

 keywords:

  thisXStmnt->skipTableLoad   = TRUE;
  thisXStmnt->skipTableUpdate = FALSE;

  cursor    += 12 + strspn(cursor+12," \t\r\n");
  tokenSize  = strcspn(cursor," \t\r\n");
  if (!tokenSize) goto fail; 
  thisXStmnt->targetTableName = malloc(tokenSize+1);
  memcpy(thisXStmnt->targetTableName,cursor,tokenSize);
  thisXStmnt->targetTableName[tokenSize]='\0';
  cursor    += tokenSize;

  while(isKeyword)
  {cursor    += strspn(cursor," \t\r\n");
   tokenSize  = strcspn(cursor," \t\r\n");
   if (!tokenSize) {isKeyword=FALSE; continue;}
   if (tokenSize==4 && !strnicmp(cursor,"FROM",4))
   {cursor   += 4 + strspn(cursor+4," \t\r\n");
    tokenSize = strcspn(cursor," \t\r\n");
    if (!tokenSize) goto fail;
    thisXStmnt->sourceTableName = malloc(tokenSize+1);
    memcpy(thisXStmnt->sourceTableName,cursor,tokenSize);
    thisXStmnt->sourceTableName[tokenSize]='\0';
    cursor   += tokenSize;
    thisXStmnt->notExtended     = FALSE;
    continue;
   }
   if (tokenSize==6 && !strnicmp(cursor,"LOAD",6))
   {thisXStmnt->skipTableLoad   = FALSE;
    cursor                     += tokenSize;
    thisXStmnt->notExtended     = FALSE;
    continue;
   }
   if (tokenSize==8 && !strnicmp(cursor,"UPDATE",8))
   {thisXStmnt->skipTableUpdate = FALSE;
    cursor                     += tokenSize;
    thisXStmnt->notExtended     = FALSE;
    continue;
   }
   isKeyword = FALSE;
  }

  if (!thisXStmnt->notExtended)
  {if (thisXStmnt->sqlIsCreateTable)
   statementNow->statementType  = SQL_XCREATE;
  }

  if (!thisXStmnt->targetTableName)
   thisXStmnt->targetTableName = thisXStmnt->sourceTableName;

  if (thisXStmnt->sqlIsUpdateTable)
  {while(*cursor && *cursor != '\n' && *cursor != '\r')
   {cursor += strspn(cursor," \t\r\n");
    if (isalpha(*cursor) && 
        (tokenSize = strcspn(cursor," \t\r\n")))
    {thisXStmnt->updateColumns[thisXStmnt->updateKeys]
                   = malloc(tokenSize+1);
     memcpy(thisXStmnt->updateColumns[thisXStmnt->updateKeys],
            cursor,tokenSize);
     thisXStmnt->updateColumns[thisXStmnt->updateKeys++][tokenSize]='\0';
     cursor += tokenSize;
    }
   }
  }

  done: 

  return(OK);

  fail:

  return(NOT_OK);
}
//=========================================================================
char **parseMapping(EXTENDED_SQL *statementNow,char *rawMap,int startingLine)
{//Prefix source column name with an asterisk if a forcing token is 
 //present.

  char *token, *token1, *token2, **newMap, *mapSrcCol, **value=NULL;
  int  mappedColumns=0,rawMapSize=strlen(rawMap),rawIndex=0,forced=FALSE,
       tokenSize, tokenSize1, tokenSize2, i;

  newMap = malloc(1024);
  memset(newMap,0,1024);

  while(rawIndex < rawMapSize)
  {rawIndex  += strspn((rawMap + rawIndex)," \t\r\n");
   token1     = rawMap + rawIndex;
   tokenSize1 = strcspn(token1," \t\r\n");
   if (!tokenSize1) goto failed;
   rawIndex  += tokenSize1;
   rawIndex  += strspn((rawMap + rawIndex)," \t\r\n");
   token      = rawMap + rawIndex;
   tokenSize  = strcspn(token," \t\r\n");
   if (!tokenSize) goto failed;
   if (!isalpha(token[0]))
        {forced     = TRUE;
         rawIndex  += tokenSize;
         rawIndex  += strspn((rawMap + rawIndex)," \t\r\n");
         token2     = rawMap + rawIndex;
         tokenSize2 = strcspn(token1," \t\r\n");
         if (!tokenSize2) goto failed;
        }
   else {token2     = token;
         tokenSize2 = tokenSize;
        }
   rawIndex  += tokenSize2;
   rawIndex  += strcspn((rawMap + rawIndex),"\r");
   if (*(rawMap + rawIndex) != '\r')
   {xsqlLogPrint("Internal parsing error!");
    goto failed;
   }
   rawIndex++;
   newMap[(mappedColumns*2)]   = malloc(tokenSize1 + 5);
   newMap[(mappedColumns*2)+1] = malloc(tokenSize2 + 1);
   memcpy(newMap[(mappedColumns*2)]  ,token1,tokenSize1);
   memcpy(newMap[(mappedColumns*2)+1],token2,tokenSize2);
   newMap[(mappedColumns*2)][tokenSize1]   = 0;
   newMap[(mappedColumns*2)+1][tokenSize2] = 0;
   if (forced) strcat(newMap[(mappedColumns*2)]," => ");
   else        strcat(newMap[(mappedColumns*2)]," == ");
   mappedColumns++;
  }

  value = malloc( sizeof(char *) * (mappedColumns * 2) );
  for (i=0;i<mappedColumns;i++)
      {value[i*2]     = newMap[i*2];
       value[(i*2)+1] = newMap[(i*2)+1];
      }

  thisXStmnt->mappedColumns    = mappedColumns;

  goto done;

  failed: xsqlLogPrint("Column Mapping syntax error line %d",
                       startingLine + mappedColumns);
  
          for (i=0;i<(mappedColumns * 2);i++)
          {Free(newMap[i*2]);
           Free(newMap[(i*2)+1]);
          }

  done: Free(newMap);
        return value;

}
//=========================================================================
setMap(EXTENDED_SQL *statementNow)
{// Setup batch state for operation of the next mapping. 
 // If is re-map, arrange the columns of the statement to the name mapping
 // indicated in statementNow.


}
//=========================================================================
resetMap(EXTENDED_SQL *statementNow)
{// Reverse the effect of setMap.

}
//=========================================================================
int updateRow(EXTENDED_SQL *statementNow,unsigned row,char *sqlBuffer)
{
 char          work[40],*colValCursor,*tailCursor,*theData;
 unsigned      isKey,i,j,nthColumn,nthKey=0,nthNonKey=0,
               sqlBufferCapacity,thisCursor;
 short         *theLength;
 struct sqlvar *thisColumn;

  sprintf(sqlBuffer,"UPDATE %s/%s SET ",collection,thisTableName);
  sqlBufferCapacity -= strlen(sqlBuffer);
  strcpy (sqlBufferTail," WHERE ");
  tailCursor = sqlBufferTail + strlen(sqlBufferTail);
  for (i=0;i<thisXStmnt->updateKeys;i++)
  {thisColumn = am->keys[i];
   thisColumn->sqldata[thisColumn->sqllen] = '\0';
   if (!(nthKey++))
      sprintf(tailCursor," %s = '%s' ",
              thisColumn->sqlname.data,thisColumn->sqldata);
   else
      sprintf(tailCursor," AND %s = '%s' ",
              thisColumn->sqlname.data,thisColumn->sqldata);
  }

  for (nthColumn=0;nthColumn<thisXStmnt->nColumns;nthColumn++)
  {memset(columnValue,'\0',sizeof(columnValue));
   thisColumn    = &(sourceSQLDA->sqlvar[nthColumn]);
   for (i=0,isKey=FALSE;i<thisXStmnt->updateKeys && !isKey;i++)
        if (thisColumn == am->keys[i]) isKey = TRUE;
   if (isKey) continue;
   theData = thisColumn->sqldata;
   theLength = &(thisColumn->sqllen);
   switch(thisColumn->sqltype)
   {case SQL_TYP_LONG:   
    case SQL_TYP_VARCHAR:  theLength = (short *)theData;    
                           theData = theData + 2;    
    case SQL_TYP_CHAR: 
     doColumn:
       thisColumn->sqldata[thisColumn->sqllen] = '\0';
       if (!(nthNonKey++))
            sprintf(columnValue,"%s = '",thisColumn->sqlname.data);
       else sprintf(columnValue,",%s = '",thisColumn->sqlname.data);
       ConvertStrSQL(theData,&columnValue[strlen(columnValue)],0);
       sqlBufferCapacity -= strlen(columnValue);
     break;
    case SQL_TYP_NLONG:      
    case SQL_TYP_NVARCHAR: theLength = (short *)theData;    
                           theData = theData + 2;      
    case SQL_TYP_NCHAR:      
       if (!nullIndicators[nthColumn]) goto doColumn;
       if (nthColumn == thisXStmnt->nColumns - 1)
            strcpy(columnValue,"''");
       else strcpy(columnValue,"'',");
       sqlBufferCapacity -= strlen(columnValue);
     break;
   }
   if (!nullIndicators[nthColumn])
   {if (nthColumn == thisXStmnt->nColumns - 1) {strcat(columnValue,"'");
                                          sqlBufferCapacity--;
                                         }
    else {strcat(columnValue,"',"); sqlBufferCapacity -= 2;}
   }
   if (sqlBufferCapacity < strlen(sqlBufferTail)) 
   {xsqlLogPrint("%s SQL Update Overflow at col. %s, row %d.",
                thisColumn->sqlname.data,row);
    return(NOT_OK);
   }
   strcat(sqlBuffer,columnValue);
  }

  strcat(sqlBuffer,sqlBufferTail);

  return(OK);

}
//=========================================================================
CONNECTION      *xsqlBatchConnect(char *connectStatement,int role)
{int             beforeConnectName=TRUE,tokenSize,option,returnCode;
 char           *cursor, **stateDBName, *newString, *roleString;
 unsigned short *connectHandle;
 unsigned short *accessMode;
 CONNECTION     *c=NULL;
 SITE           *thisSite;

 // Clear up string allocation.(e.g. source, target names, etc.)
 // everywhere.

 if (role == SOURCE)
 {stateDBName   = &other->source;
  accessMode    = &other->sourceMode;
  connectHandle = &other->sourceHandle;
  roleString    = "SOURCE";
 }
 else
  {stateDBName  = &am->target;
   accessMode   = &am->targetMode;
   connectHandle= &am->targetHandle;
   roleString    = "TARGET";
  }

 if (connectStatement)
 {for(cursor=connectStatement;beforeConnectName;cursor++)
  {if ((*cursor=='t' || *cursor == 'T') &&
       (*(cursor+1)=='o' || *(cursor+1) == 'O'))
   {beforeConnectName=FALSE;
    cursor   += 2;
    cursor   += strspn(cursor," \t\r\n");
    tokenSize = strcspn(cursor," \t\r\n");
    newString   = malloc( tokenSize + 1);
    memcpy(newString,cursor,tokenSize);
    newString[tokenSize] = '\0';
    if (!(thisSite=setSiteDependencies(newString)))
    {xsqlLogPrint("%s is not a recognized database.",newString);
     xsqlLogPrint("The %s mode is forced to LOCAL_COLLECTION.",roleString);
     *accessMode = LOCAL_COLLECTION;
    }
    else
        *accessMode = thisSite->accessMode;
    *stateDBName = newString;
   }
  }
 }

 if (!*accessMode && *stateDBName )
 {if (!(thisSite=setSiteDependencies(*stateDBName)))
  {xsqlLogPrint("%s is not a recognized database.",*stateDBName);
   xsqlLogPrint("The %s mode is forced to LOCAL_COLLECTION.",roleString);
   *accessMode = LOCAL_COLLECTION;
  }
   else
        *accessMode = thisSite->accessMode;
 }

 if (!*accessMode || !*stateDBName)
 {
  xsqlLogPrint("Name or mode of % is missing, connect aborted.", roleString);
  return c;
 }

 xsqlLogPrint("Connecting to %s %s (%s)...", roleString, *stateDBName,
              modeStrings[*accessMode - 1] );

 if (!(c=xsqlGetConnectSlot(connectHandle)))
 {xsqlLogPrint("xsqlGetConnectSlot failed.");
  return c;
 }

 newString             = malloc(15);
 sprintf(newString,"XSQL(%s)",roleString);
 c->accessMode         = *accessMode;
 c->subject            = newString;
 c->object             = *stateDBName;
 c->optimizationLevel  = 1; // other->optimization;
 c->thresholdFrequency = 0; // other->thresholdFOM;

 c->sqlModulus         = 1;
 c->forceReport        = TRUE;


 if (!(*connectHandle=xsqlConnect(NULL,NULL,*connectHandle)))
      {returnCode = xsqlErr();
       xsqlLogPrint("Connect to %s %s failed: %d", 
                   roleString, *stateDBName, returnCode ); 
       return c;
      }

 if (role==SOURCE)
     {other->connected  = TRUE;
      other->dbHandle   = *connectHandle;}
 else{am->targetHandle  = *connectHandle;
      am->connected     = TRUE;}

 xsqlLogPrint("Connected to %s %s(%s) OK.", roleString, *stateDBName,
             modeStrings[*accessMode - 1] ); 

 if (!other->connectName)
 {other->connectName = *stateDBName;
  other->accessMode  = *accessMode;
 }

 return c;
  
}
//=========================================================================
// Private: access live cursor structures from app handle
//
#if (PLATFORM == XSQL_OS2)
   CURSOR * xType2 xsqlCursor(int short thisSQL)
#endif
#if (PLATFORM == XSQL_WINNT)
   xType2 CURSOR * xsqlCursor(int short thisSQL)
#endif
{
 int short thisCursor,thisConnect;
 char      work[30];
 CURSOR   *value = NULL;

  parseHandle;
  checkHandle("xsqlCursor 1");
  checkTheHeap("'Cursor");
  return &thisContext;

}
void xType2 xsqlEndXSQL( void )
{
 int          ithColumn;
 EXTENDED_SQL *statementNow=NULL,*lastStatement=NULL;

 // Adjust application state to reflect dropping of the current sqlFile.
 // Terminates any active connections, including log, resets batch state.

 if (am->statements)
 {for(statementNow  = am->statements; statementNow;
      lastStatement = statementNow, statementNow = lastStatement->next)
  {
  freeStatement(statementNow);
  Free(lastStatement);
  }

  Free(lastStatement);
  am->statements = NULL;
  am->nStatements= 0;
  }

 state->disconnectPipe = TRUE;
 xsqlLogPrint("Extended RDBMS Subprocess Halted.");

 other->dbHandle     = 0;
 other->connectName  = NULL;
 other->accessMode   = 0;

 other->source       = NULL;
 other->sourceMode   = 0;
 other->sourceHandle = 0;

 other->connected    = FALSE;

 am->target          = NULL;
 am->targetMode      = 0;
 am->targetHandle    = 0;
 am->connected       = FALSE;

 sqlFile          = NULL;
 lineNumber       = 0;

}
//=========================================================================
int short xType2 xsqlExecXSQL(int short dbHandle,EXTENDED_SQL *statementNow)
{char work[40], *token, *a, *b;
 int short
      i=0, j=0, value=OK, connectOnly=FALSE, theError, tokenSize, errCode,
      theHandle=0, nCols, rc, thisConnect = dbHandle ? dbHandle - 1 : 0,
      validRef, returnCode, thisCursor, thisSQL, wasConnect, wasCursor,
      cp=0;
 long nRows;

 checkRefWithSize( statementNow, sizeof( EXTENDED_SQL ), validRef );
 if ( !validRef )
 {
    xsqlLogPrint( "XSQL execution failure: 1.1" );
    goto exit;
 }

 checkRefWithSize( thisStatement, 10, validRef );
 if ( !validRef )
 {
    xsqlLogPrint( "XSQL execution failure: 1.2" );
    goto exit;
 }

 checkRefWithSize( other, sizeof( INTERACTIVE_STATE ), validRef );
 if ( !validRef )
 {
    xsqlLogPrint( "XSQL execution failure: 1.3" );
    goto exit;
 }

 checkRefWithSize( am, sizeof( BATCH_STATE ), validRef );
 if ( !validRef )
 {
    xsqlLogPrint( "XSQL execution failure: 1.4" );
    goto exit;
 }

 checkRefWithSize( thisXStmnt, sizeof( BATCH_SQL ), validRef );
 if ( !validRef )
 {
    xsqlLogPrint( "XSQL execution failure: 1.5" );
    goto exit;
 }

 other->sqlNow = statementNow->statementType;

 if (other->sqlNow == SQL_CONNECT_TO) 
   {xsqlLogPrint("CONNECT statement detected.");
    if (other->connected)
    {xsqlLogPrint("The existing connection is being closed...");
     xsqlDisconnect(other->dbHandle);
    }
    other->connected   = FALSE;
   }

 if (debug > 10)
 {if (other->sqlNow == SQL_NOT)
      xsqlLogPrint("Executing %s ...",statementNow->text); 
   else 
      xsqlLogPrint("Executing %s ...",sqlVerbs[other->sqlNow]); 
 }
 
 if (!thisXStmnt->notExtended)
 {if ( thisXStmnt->command || 
       (!thisXStmnt->skipTableLoad &&
        (thisXStmnt->sqlIsCreateTable || thisXStmnt->sqlIsUpdateTable)
       )
     ) {switch(thisXStmnt->command)
        {case SET_DEFAULT_CONNECT:
             if (!memicmp("TARGET",thisXStmnt->sourceTableName,6))
             {if (!am->connected)
              {xsqlLogPrint("Can't set target as default until connected.");
               xsqlLogPrint("Place $TARGET before $DEFAULT=TARGET.");
               goto fail;
              }
              other->connectName = am->target;
              other->dbHandle    = am->targetHandle;
              other->accessMode  = am->targetMode;
             }
             if (!stricmp("SOURCE",thisXStmnt->sourceTableName))
             {if (!other->connected)
              {xsqlLogPrint("Can't set source as default until connected.");
               xsqlLogPrint("Place $SOURCE before $DEFAULT=TARGET.");
               goto fail;
              }
              other->connectName = other->source;
              other->dbHandle    = other->sourceHandle;
              other->accessMode  = other->sourceMode;
             }
             goto done;
         case SET_SOURCE_CONNECT:
            {SITE *specifiedSite=NULL;
             int   sourceWasNull=FALSE, differentSource=FALSE,
                   crossConnect =FALSE;

              checkReference( thisXStmnt->sourceTableName, validRef );
              if ( !validRef )
              {
               xsqlLogPrint( "XSQL execution failure: 3.2" );
               goto exit;
              }

              specifiedSite=setSiteDependencies(thisXStmnt->sourceTableName);

              if (!specifiedSite)
              {xsqlLogPrint("%s not a valid database or server.",
                            thisXStmnt->sourceTableName); 
               goto fail;
              }
              if (!other->source)
               {other->source = malloc(strlen(specifiedSite->name) + 1);
                strcpy( other->source, specifiedSite->name );
                sourceWasNull = TRUE;
                if (am->target)
                    if (!stricmp(thisXStmnt->sourceTableName,am->target))
                        crossConnect=TRUE;
               }
              else {if (am->target)
                      if (!stricmp(other->source,am->target))
                        crossConnect=TRUE;
                    if (stricmp(other->source,thisXStmnt->sourceTableName))
                        differentSource = TRUE;
                   }
              if (sourceWasNull || differentSource || crossConnect )
              {
               if (!sourceWasNull)
               {xsqlLogPrint("Disconnecting from %s.",other->source);
                xsqlDisconnect(other->sourceHandle);
                other->source = malloc(strlen(specifiedSite->name) + 1);
                strcpy( other->source, specifiedSite->name );
                other->source = specifiedSite->name;
               }
               if (crossConnect)
                 {other->connected   = am->connected;
                  other->source      = am->target;
                  other->sourceMode  = am->targetMode;
                  other->sourceHandle= am->targetHandle;
                  if (am->connected)
                      xsqlLogPrint("SOURCE %s connected to TARGET %s.",
                                 other->source, am->target);
                  else
                      xsqlLogPrint("Warning: SOURCE set to unconnected TARGET!");
                 }
               else 
                 {other->connected   = FALSE;
                  other->sourceMode  = specifiedSite->accessMode;
                  if ( !xsqlBatchConnect( NULL, SOURCE )) 
                  {cp=1; goto fail;}
                 }
              }
            }
            goto done;
         default:;
             if (batchRun(statementNow))
                  {cp = 2; goto fail;}
             else goto done;
        }
       }
  else goto notExtended;
 }
  else {notExtended:
        theHandle=xsqlExecSQL(other->dbHandle,thisStatement);
        if (statementNow->statementType < SQL_HANDLE_RETURNED && !theHandle)
           {cp = 3; goto fail;}
        if (rc=xsqlErr())
        {if (rc==15) 
         {xsqlLogPrint("No rows match SQL.");
          goto done;
         }
         cp = 4 + rc;
         goto fail;
        }
       }

 if (state->debugCode)
     xsqlLogPrint("The SQL host execution completed OK."); 

 if (other->sqlNow != SQL_SELECT) 
 {if (statementNow->statementType < SQL_HANDLE_RETURNED)
  {long modRecs=xsqlNumModRecs(theHandle);
   xsqlLogPrint("%ld rows were modified by the executed SQL.",modRecs);
  }
  goto done;
 }

 if (theHandle)
 {nCols=xsqlNumCols(theHandle);
  nRows=xsqlFetchNumRows(theHandle);

 wasConnect = thisConnect;
 thisSQL    = theHandle;

 parseHandle;

 if (!thisHit)             
   {
    xsqlLogPrint("SQL Query: %ld row(s) X %d column(s).",nRows,nCols); 
   }
 else
   {
    xsqlLogPrint("XSQL Query #%ld: %ld row(s) X %d column(s).",
                 thisHit->fullHash,nRows,nCols); 
   }

 }

 thisConnect= wasConnect;

 done: if (state->debugCode)
           xsqlLogPrint("XSQL function completed OK."); 
       value = theHandle;
       goto exit;

 fail: errCode=xsqlErr();
          {xsqlLogPrint("XSQL function(%d) failed: %d(%d)",
                         cp,theHandle,errCode);
           a=xsqlErrMsg(); b=xsqlWhere();
           xsqlLogPrint("%s At %s.",a,b); 
           xSQLstate->shutdown = TRUE;
          }

       value=TRUE;

 exit: if ( state->logPipeHandle && state->pipeConnected  )
        {char           eventNotification[20],*eventName;
         unsigned long  written; 
              
          eventNotification[0] = 1;
          eventNotification[1] = 0;
          eventName            = &eventNotification[2];
          strcpy(eventName,"XSQL END");
          eventName[10] = 0;
          returnCode    =
          DosWrite( state->logPipeHandle, eventNotification,
                    strlen( eventName ) + 2, &written );
          if ( returnCode || written != strlen( eventName  ) + 2 )
           {
           fprintf( logFile, "****Log output failure %d,%d\n",
                     returnCode, written );
           }

        }

       return value;

}
//=========================================================================
char *xsqlgets(char *inFileBuffer,void *streamArg, int isFragment,
               char **fragmentCursor)
{
 int   lineLength = 0;
 char *value      = NULL, *streamCursor;
 
 if (!isFragment)
     value = fgets(inFileBuffer,MAX_TEXT_LINE-2,streamArg);
 else {if ( !fragmentCursor )
            {xsqlLogPrint("xsqlgets NULL"); return NULL; }
       streamCursor      =  *fragmentCursor;
       if (!streamCursor)
            streamCursor =  streamArg;
       streamCursor      += strspn(streamCursor,"\r\n"); 
       lineLength        =  strcspn(streamCursor,"\r\n"); 
       if (lineLength)
       {if (lineLength > MAX_TEXT_LINE)
        {xsqlLogPrint("Line too long.");
         return value;
        }
        memcpy(inFileBuffer,streamCursor,lineLength);
        inFileBuffer[lineLength] = 0;
        streamCursor             = streamCursor + lineLength;
        *fragmentCursor          = streamCursor;
        value                    = inFileBuffer;
       }
      }

 return value;

}
//=========================================================================
int short xType2 xsqlLink(int short *pid,
    BATCH_STATE **p,INTERACTIVE_STATE **c,COMMON_STATE **csp)
{

 if (!state->reserved)
   {xSQLstate       = (void *)malloc( sizeof(XSQL_STATE) );
    memset((void *)xSQLstate, 0, sizeof(XSQL_STATE) );
    state->reserved = xSQLstate;
   }
 if (logFileName[0] && !logFile && !pid)
  { char pipeName[40];
    int  returnCode;

    logFile = fopen(logFileName,"w");
    if (logFile)
    {fprintf( logFile, "%s Extended RDBMS Subprocess Initiated.\n",
             _strdate( pipeName ) );
     fflush( logFile );

     sprintf( pipeName, "\\PIPE\\%s", logFileName );
     returnCode=
       DosMakeNmPipe( pipeName, (PHPIPE)&state->logPipeHandle,
                     (NP_ACCESS_OUTBOUND | NP_NOINHERIT | NP_NOWRITEBEHIND),
                     (NP_NOWAIT | NP_TYPE_MESSAGE | 1),
                     LOG_PIPE_SIZE,
                     0,
                     1000L
                    ); 
     if (!returnCode)
      {
       DosConnectNmPipe( (HPIPE)state->logPipeHandle );
       state->pipeConnected = FALSE;
      }
    }
  }
 else if ( state->logPipeHandle && !pid )
   {
    state->disconnectPipe = FALSE;
    if ( state->pipeConnected )
         DosDisConnectNmPipe( (HPIPE)state->logPipeHandle );
    DosConnectNmPipe( (HPIPE)state->logPipeHandle );
    state->pipeConnected  = FALSE;
   }

 if (!am && !pid) 
 {am = malloc(sizeof(BATCH_STATE));
  memset(am,0,sizeof(BATCH_STATE));
 }
 if (p)
    *p = am;

 if (!other && !pid) 
 {other = malloc(sizeof(INTERACTIVE_STATE));
  memset(other,0,sizeof(INTERACTIVE_STATE));
 }
 if (c)
    *c = other;

 if (state->proFileName && !state->proFile) loadProfile();

 if (pid) 
 {  if (csp) *csp = cs;
    *pid          = thisProcess;
 }

 return( 0 );

}
//=========================================================================
int short xType2 xsqlSynthesizeMap(unsigned mapStatementIndex1,
                             unsigned mapStatementIndex2)
{
 return( FALSE );

}
//=========================================================================
int short xType2 xsqlMap(unsigned mapStatementIndex)
{
 char          work[40],*work2=NULL,*theValue=NULL,*sqlDeclaration,*sqlBuffer,
               seperator;
 short         i,j,nRows=0,nthColumn,tableNameLength,theLength,keysFound,
               thisCursor;
 int           foundArchiveColumn=FALSE,returnCode,value=OK;
 int           sourceHandle=0,targetHandle=0;
 struct sqlvar *thisColumn,*archiveCol;
 double        cumSeconds; 
 time_t        now,then; 
 EXTENDED_SQL *statementNow=NULL;

 for (i=0,statementNow=am->statements;i&&statementNow;
      i--,statementNow=statementNow->next);

 if (!statementNow)
 {xsqlLogPrint("No statement at %d",mapStatementIndex);
  return(FALSE);
 }

 if (statementNow->statementType != SQL_XCREATE &&
     statementNow->statementType != SQL_XUPDATE )
 {xsqlLogPrint("Statement type %d not a mapping.",statementNow->statementType);
  return(FALSE);
 }

 am->cursorOpen              = FALSE;
 am->checkedForUpdateability = FALSE;
 am->localSQLError           = FALSE;
 cumSeconds = 0;
 time(&then);

 xsqlLogPrint( "Entering xsqlBatchMap." );

 setMap(statementNow);

 if (!thisXStmnt->sequel)
 {if (!localSQL) localSQL=malloc(MAX_TEXT_LINE);
  sprintf(localSQL,
          "SELECT * FROM %s%c%s FOR FETCH ONLY",
            sourcePrefix,seperator,sourceTable);
 } else localSQL = thisXStmnt->sequel;

 work2 = malloc(MAX_COLUMN_TEXT);

 xsqlLogPrint("Inserting/Updating from %s.%s.%s.",
            other->source,sourcePrefix,sourceTable);

recycleForUpdate:

 if (!(returnCode=xsqlExecSQL(other->sourceHandle,localSQL))) goto done;

 sourceHandle = returnCode;

 if (!thisXStmnt->nColumns)
      thisXStmnt->nColumns = sourceContext.nColumns;

 if (thisXStmnt->sqlIsUpdateTable && 
     thisXStmnt->updateKeys + 1 <= thisXStmnt->nColumns)
 {xsqlLogPrint("%s needs to have at least 2 more columns ...",
              thisTableName);
  xsqlLogPrint("... than specified on UPDATE TABLE statement.");
  value = NOT_OK;
  goto done;
 }

 if (thisXStmnt->isUpdateable && !am->checkedForUpdateability)
 {am->checkedForUpdateability = TRUE;
  for(nthColumn=0;
      nthColumn < sourceContext.nColumns && !foundArchiveColumn;nthColumn++)
  {thisColumn = &(sourceSQLDA->sqlvar[nthColumn]);
   if(!strncmp(thisColumn->sqlname.data,ARCHIVE_COL_NAME,
               thisColumn->sqlname.length))
   {foundArchiveColumn = TRUE;
    if (!thisXStmnt->sequel)
    {if (!thisXStmnt->sqlIsUpdateTable)
      sprintf(localSQL,"SELECT * FROM %s%s FOR UPDATE OF %s",
              sourcePrefix,seperator,sourceTable,ARCHIVE_COL_NAME);
     else 
      sprintf(localSQL,"SELECT * FROM %s%s FOR UPDATE OF %s "
                       "WHERE %s <> %c",sourcePrefix,sourceTable,
              ARCHIVE_COL_NAME,ARCHIVE_COL_NAME,ARCHIVED_VALUE);
    }
   }
  }
  if (foundArchiveColumn)
  {if (sourceHandle) {xsqlEndSQL(sourceHandle); sourceHandle = FALSE;}
   goto recycleForUpdate;
  }
  else
  {xsqlLogPrint("Updatable source table %s does not have a column \"%s\".",
               thisTableName,ARCHIVE_COL_NAME);
   if (thisXStmnt->sqlIsUpdateTable)
        xsqlLogPrint("UPDATE TABLE rejected for %s.",thisTableName);
   else xsqlLogPrint("CREATE TABLE rejected for %s.",thisTableName);
   value = NOT_OK;
   goto done;
  }
 }

 for (nthColumn=0;nthColumn<sourceContext.nColumns;nthColumn++)
 {thisColumn = &(sourceSQLDA->sqlvar[nthColumn]);
  if (thisXStmnt->isUpdateable)
  {if(!strncmp(thisColumn->sqlname.data,ARCHIVE_COL_NAME,
              thisColumn->sqlname.length)) archiveCol = thisColumn;
  }
  if (thisXStmnt->sqlIsUpdateTable)
  {for(i=0;i<thisXStmnt->updateKeys;i++)
    if (!strnicmp(thisXStmnt->updateColumns[i],thisColumn->sqlname.data,
                  thisColumn->sqlname.length))
                     am->keys[i] = thisColumn;
  }
 }

 if (thisXStmnt->isUpdateable)
 {for (j=keysFound=0;keysFound < thisXStmnt->updateKeys
                     && j<thisXStmnt->nColumns;j++)
  {thisColumn = &sourceSQLDA->sqlvar[j];
   for (i=0;i<thisXStmnt->updateKeys;i++)
    if (!strncmp(thisColumn->sqlname.data,thisXStmnt->updateColumns[i],
               thisColumn->sqlname.length))
                 am->keys[keysFound++] = thisColumn;
  }
  if (keysFound < thisXStmnt->updateKeys)
  {xsqlLogPrint("%s is missing a specified key column!",sourceTable);
   xsqlLogPrint("Construct rejected for %s.",sourceTable);
   return(NOT_OK);
  }

  sprintf(updator,"UPDATE %s%s SET %s = '%c' WHERE CURRENT OF ADPTV%c1",
          sourcePrefix,sourceTable,ARCHIVE_COL_NAME,ARCHIVED_VALUE);

 }
 
 time(&now);

 cumSeconds += difftime(now,then);

 if (xsqlFetchSetOptions(sourceHandle,1L)) {value = NOT_OK; goto done;}

 sqlBuffer = malloc(MAX_SQL_TEXT+2);

 while(xsqlFetchNext(sourceHandle))
 {nRows++;
  time(&then);
  memset(sqlBuffer,'\0',MAX_SQL_TEXT);

  if (thisXStmnt->sqlIsCreateTable)
    {doInsert:
              if (insertRow(statementNow,nRows,sqlBuffer)) 
              {am->localSQLError = TRUE; goto stop;}
    }
  else {if (foundArchiveColumn)
        switch(archiveCol->sqldata[0])
        {case INSERT_VALUE:     ;
         case ALT_INSERT_VALUE: goto doInsert;
         case UPDATE_VALUE:    
              doUpdate:
               if (updateRow(statementNow,nRows,sqlBuffer))
               {am->localSQLError = TRUE; goto stop;}
               break;
         default: 
         xsqlLogPrint("Bad value in %s.%s, row %ld: row ignored.",
                    sourceTable,ARCHIVE_COL_NAME,nRows); 
        }
        else goto doUpdate;
       }
  time(&now);
  cumSeconds  += difftime(now,then);
  targetHandle = xsqlExecSQL(am->targetHandle,sqlBuffer);
  if (!xsqlErr())
  {if (nRows && !(nRows % MSG_ROW_QUANTUM))
   {xsqlLogPrint("%d rows of %s transferred to %s.",
               nRows,sourceTable,targetTable);
   }
   if (state->debugCode && nRows >= debugLimit) break;
  }
  else goto stop;
  if (thisXStmnt->isUpdateable && foundArchiveColumn)
  {
  }
 }

 stop: xsqlLogPrint("%d rows transferred from table %s.",
                    nRows-1,thisXStmnt->sourceTableName);

       xsqlLogPrint("%3.3f minutes %2.2f seconds source DBMS time.",
                    cumSeconds/60,cumSeconds-(cumSeconds/60));

 done: resetMap(statementNow);
 
       if (sourceHandle) xsqlEndSQL(sourceHandle);

       if (targetHandle) xsqlEndSQL(targetHandle);

       Free(work2);

       Free(sqlBuffer);

       return(value);

}
//=========================================================================
int xsqlBatchMap(EXTENDED_SQL *statementNow)
{
 char          work[40],*work2=NULL,*theValue=NULL,*sqlDeclaration,*sqlBuffer,
               seperator;
 unsigned      i,j,nRows=0,nthColumn,tableNameLength,theLength,keysFound,
               thisCursor;
 int           foundArchiveColumn=FALSE,returnCode,value=OK;
 int           sourceHandle=0,targetHandle=0;
 struct sqlvar *thisColumn,*archiveCol;
 double        cumSeconds; 
 time_t        now,then; 

 am->cursorOpen              = FALSE;
 am->checkedForUpdateability = FALSE;
 am->localSQLError           = FALSE;
 cumSeconds = 0;
 time(&then);

 xsqlLogPrint( "Entering xsqlBatchMap." );

 setMap(statementNow);

 if (!thisXStmnt->sequel)
 {if (!localSQL) localSQL=malloc(MAX_TEXT_LINE);
  sprintf(localSQL,
          "SELECT * FROM %s%c%s FOR FETCH ONLY",
            sourcePrefix,seperator,sourceTable);
 } else localSQL = thisXStmnt->sequel;

 work2 = malloc(MAX_COLUMN_TEXT);

 xsqlLogPrint("Inserting/Updating from %s.%s.%s.",
            other->source,sourcePrefix,sourceTable);

recycleForUpdate:

 if (!(returnCode=xsqlExecSQL(other->sourceHandle,localSQL))) goto done;

 sourceHandle = returnCode;

 if (!thisXStmnt->nColumns)
      thisXStmnt->nColumns = sourceContext.nColumns;

 if (thisXStmnt->sqlIsUpdateTable && 
     thisXStmnt->updateKeys + 1 <= thisXStmnt->nColumns)
 {xsqlLogPrint("%s needs to have at least 2 more columns ...",
              thisTableName);
  xsqlLogPrint("... than specified on UPDATE TABLE statement.");
  value = NOT_OK;
  goto done;
 }

 if (thisXStmnt->isUpdateable && !am->checkedForUpdateability)
 {am->checkedForUpdateability = TRUE;
  for(nthColumn=0;
      nthColumn < sourceContext.nColumns && !foundArchiveColumn;nthColumn++)
  {thisColumn = &(sourceSQLDA->sqlvar[nthColumn]);
   if(!strncmp(thisColumn->sqlname.data,ARCHIVE_COL_NAME,
               thisColumn->sqlname.length))
   {foundArchiveColumn = TRUE;
    if (!thisXStmnt->sequel)
    {if (!thisXStmnt->sqlIsUpdateTable)
      sprintf(localSQL,"SELECT * FROM %s%s FOR UPDATE OF %s",
              sourcePrefix,seperator,sourceTable,ARCHIVE_COL_NAME);
     else 
      sprintf(localSQL,"SELECT * FROM %s%s FOR UPDATE OF %s "
                       "WHERE %s <> %c",sourcePrefix,sourceTable,
              ARCHIVE_COL_NAME,ARCHIVE_COL_NAME,ARCHIVED_VALUE);
    }
   }
  }
  if (foundArchiveColumn)
  {if (sourceHandle) {xsqlEndSQL(sourceHandle); sourceHandle = FALSE;}
   goto recycleForUpdate;
  }
  else
  {xsqlLogPrint("Updatable source table %s does not have a column \"%s\".",
               thisTableName,ARCHIVE_COL_NAME);
   if (thisXStmnt->sqlIsUpdateTable)
        xsqlLogPrint("UPDATE TABLE rejected for %s.",thisTableName);
   else xsqlLogPrint("CREATE TABLE rejected for %s.",thisTableName);
   value = NOT_OK;
   goto done;
  }
 }

 for (nthColumn=0;nthColumn<sourceContext.nColumns;nthColumn++)
 {thisColumn = &(sourceSQLDA->sqlvar[nthColumn]);
  if (thisXStmnt->isUpdateable)
  {if(!strncmp(thisColumn->sqlname.data,ARCHIVE_COL_NAME,
              thisColumn->sqlname.length)) archiveCol = thisColumn;
  }
  if (thisXStmnt->sqlIsUpdateTable)
  {for(i=0;i<thisXStmnt->updateKeys;i++)
    if (!strnicmp(thisXStmnt->updateColumns[i],thisColumn->sqlname.data,
                  thisColumn->sqlname.length))
                     am->keys[i] = thisColumn;
  }
 }

 if (thisXStmnt->isUpdateable)
 {for (j=keysFound=0;keysFound < thisXStmnt->updateKeys
                     && j<thisXStmnt->nColumns;j++)
  {thisColumn = &sourceSQLDA->sqlvar[j];
   for (i=0;i<thisXStmnt->updateKeys;i++)
    if (!strncmp(thisColumn->sqlname.data,thisXStmnt->updateColumns[i],
               thisColumn->sqlname.length))
                 am->keys[keysFound++] = thisColumn;
  }
  if (keysFound < thisXStmnt->updateKeys)
  {xsqlLogPrint("%s is missing a specified key column!",sourceTable);
   xsqlLogPrint("Construct rejected for %s.",sourceTable);
   return(NOT_OK);
  }

  sprintf(updator,"UPDATE %s%s SET %s = '%c' WHERE CURRENT OF ADPTV%c1",
          sourcePrefix,sourceTable,ARCHIVE_COL_NAME,ARCHIVED_VALUE);

 }
 
 time(&now);

 cumSeconds += difftime(now,then);

 if (xsqlFetchSetOptions(sourceHandle,1L)) {value = NOT_OK; goto done;}

 sqlBuffer = malloc(MAX_SQL_TEXT+2);

 while(xsqlFetchNext(sourceHandle))
 {nRows++;
  time(&then);
  memset(sqlBuffer,'\0',MAX_SQL_TEXT);

  if (thisXStmnt->sqlIsCreateTable)
    {doInsert:
              if (insertRow(statementNow,nRows,sqlBuffer)) 
              {am->localSQLError = TRUE; goto stop;}
    }
  else {if (foundArchiveColumn)
        switch(archiveCol->sqldata[0])
        {case INSERT_VALUE:     ;
         case ALT_INSERT_VALUE: goto doInsert;
         case UPDATE_VALUE:    
              doUpdate:
               if (updateRow(statementNow,nRows,sqlBuffer))
               {am->localSQLError = TRUE; goto stop;}
               break;
         default: 
         xsqlLogPrint("Bad value in %s.%s, row %ld: row ignored.",
                    sourceTable,ARCHIVE_COL_NAME,nRows); 
        }
        else goto doUpdate;
       }
  time(&now);
  cumSeconds  += difftime(now,then);
  targetHandle = xsqlExecSQL(am->targetHandle,sqlBuffer);
  if (!xsqlErr())
  {if (nRows && !(nRows % MSG_ROW_QUANTUM))
   {xsqlLogPrint("%d rows of %s transferred to %s.",
               nRows,sourceTable,targetTable);
   }
   if (state->debugCode && nRows >= debugLimit) break;
  }
  else goto stop;
  if (thisXStmnt->isUpdateable && foundArchiveColumn)
  {
  }
 }

 stop: xsqlLogPrint("%d rows transferred from table %s.",
                   nRows-1,thisXStmnt->sourceTableName);

       xsqlLogPrint("%3.3f minutes %2.2f seconds source DBMS time.",
                  cumSeconds/60,cumSeconds-(cumSeconds/60));

 done: resetMap(statementNow);
 
       if (sourceHandle) xsqlEndSQL(sourceHandle);

       if (targetHandle) xsqlEndSQL(targetHandle);

       Free(work2);

       Free(sqlBuffer);

       return(value);

}
//=========================================================================
int short xType2 xsqlSplice(char *xsqlFragment,unsigned statementIndex,unsigned code)
{
 int          nParsed=0,nthXStatement,orgNStatements=am->nStatements,
              isInsert=FALSE,validRef,insertSize,stopPoint;
 EXTENDED_SQL *org=am->statements,*bottomCut=NULL,
              *topCut=NULL,*insertHead,*insertTail,*lost,*statementNow;

 // Get statement state for insertion at statementIndex.
 // Let batchLink process the text. Adjust statement state and
 // return number of statements in processed fragment.

  if (code != 3)
  {checkReference( xsqlFragment, validRef );
   // insertSize = strlen(xsqlFragment);
   // xsqlLogPrint( "** Splice(%d) %d chars XSQL text @ %d\n%s",
   //            code, insertSize, statementIndex, xsqlFragment );
   if ( !validRef )
   {
    xsqlLogPrint( "Splice failure: 1F" ); 
    return 0;
   }
  }

  if (statementIndex > am->nStatements) return 0;
  if (!statementIndex && code != 1) return 0;
  bottomCut = org;
  if ( code == 1 )
       isInsert = TRUE;
         
  am->nStatements  = 0;
  am->statements   = NULL;
  insertTail       = batchLink(TRUE,xsqlFragment);

  nParsed          = am->nStatements;
  insertHead       = am->statements;

  if (insertTail && orgNStatements)
  {stopPoint       = statementIndex ? statementIndex - 1 : statementIndex;
   for(nthXStatement=0,topCut = org, bottomCut = org->next;
       nthXStatement < stopPoint;
       nthXStatement++,topCut    = topCut->next
                      ,bottomCut = bottomCut ? bottomCut->next : NULL);
   if (isInsert)
   {if (statementIndex)
    {
     topCut->next        = insertHead;
     insertHead->prev    = topCut;
     if (bottomCut) 
        insertTail->next = bottomCut;
    }
     else {
           org               = insertHead;
           insertTail        = bottomCut;
           if (bottomCut)
           {insertTail->next = bottomCut;
            bottomCut->prev  = insertTail;
           }
          }

   }
   else {lost = topCut;
         if (!bottomCut) 
            {if (!lost->prev) 
                   org              = insertHead;
             else {lost->prev->next = insertHead;
                   insertHead->prev = lost->prev;
                  }
            }
         else {lost->prev->next = insertHead;
               insertHead->prev = lost->prev;
               insertTail->next = bottomCut;
               bottomCut->prev  = insertTail;
              }
         freeStatement(lost);
         Free(lost);
        }
  }
   else if (insertTail)
            org = insertHead;


 done:

  if (org)
      am->statements = org;

  switch(code)
  {case 1: am->nStatements = orgNStatements + nParsed;     break;
   case 2: am->nStatements = orgNStatements + nParsed - 1; break;
  }
  
  return nParsed;  
}
