#include <xsqlmods.h>

#define DEFAULT_CHANNEL "5250PLU"  // APPC Partner LU
#define  SOURCE_GROUP   XSQLDBMS_DLL
#define  SOURCE_MODULE  REMOTSQL_C
#define  FETCH_MARGIN   4
#define  INCL_DOSMEMMGR

#include <xsqlc.h>

  int dumpSQL(char *);
  int xType2 ehnError(int wasCode,char *whichAPI,int thisConnect);

 xType2 ehnConnect(int thisConnect)
{
 char       Work[40];
 int        wasCode;
 unsigned   returnCode=OK,unLength;

 if (thisDB.connecting) 
     DosSemRequest(&thisDB.gate,SEM_INDEFINITE_WAIT);

 thisDB.connecting = TRUE;
 
 DosSemSet(&thisDB.gate);

 if (!thisDB.channel) thisDB.channel = DEFAULT_CHANNEL;
 
 if (debug >= 5000) return(OK);

 if (!thisState->actualCarrierLinked)
 {returnCode=EHNRQSTART(thisDB.channel,commit_mode,MAX_SQL_TEXT);
  if (!returnCode || returnCode==RQ_ALRDY_INIT)
    {xsqlLogPrint("%s connected to AS400 OK.",thisDB.subject);
     returnCode = thisConnect;
     EHNRQSETROWS(200);
     if (!thisSQLstate) 
     {thisSQLstate = malloc(sizeof(struct sqlca));
      memset(thisSQLstate,'\0',sizeof(struct sqlca));
     }
    }
  else
  {xsqlLogPrint("%s Couldn't connect to %s.",thisDB.subject,thisDB.channel);
   xsqlLogPrint("%s EHNRQSTART returned %d.",thisDB.subject,returnCode);
   returnCode   = FALSE;
  }
 }

 thisState->actualCarrierLinked = TRUE;
 thisDB.cursorIneligibility     = SQL_INSERT;

 if ((thisDB.accessMode == DRDA_PC_SUPPORT) ||
     (thisDB.accessMode == DDM_PC_SUPPORT))
 {char connectStatement[200];
 
  if (!thisDB.object)
    {xsqlLogPrint("%s Didn't name an object to connect to.",thisDB.subject);
     returnCode = OK;
    }
  else {sprintf(connectStatement,
    //            "CONNECT TO %s USER DAUGHERJ USING 'BEOBACHT'",
                  "CONNECT TO %s ",
                thisDB.object);
        xsqlLogPrint(connectStatement);
        returnCode=EHNRQCONNECT(connectStatement,NULL,&unLength);
        if (!returnCode || returnCode==RQ_ALRDY_INIT)
           {xsqlLogPrint("%s Connected to %s OK.",
                       thisDB.subject,thisDB.object);
            returnCode = thisConnect;
            thisDB.connected = TRUE;
           }    
        else
        {xsqlLogPrint("%s Couldn't connect to %s.",
                    thisDB.subject,thisDB.object);
         xsqlLogPrint("%s EHNRQCONNECT returned %d.",
                    thisDB.subject,returnCode);
         if (returnCode == 8)
             ehnError(returnCode,"EHNRQCONNECT",thisConnect);
         returnCode   = FALSE;
        }
       }
 } else thisDB.connected = TRUE;

 thisDB.connecting = FALSE;
 DosSemClear(&thisDB.gate);
 return(returnCode);

}  
 xType2 ehnExecSQL(int thisCursor)
{
 char     Work[40],**valueArray,theHandle=0;
 int      i,wasCode,value=OK,thisConnect=thisContext.path;
 unsigned sqlcaSize,sqlSize;

 if (debug >= 5000)
 {fwrite(sqlBuffer,MAX_SQL_TEXT-sqlBufferCapacity,1,debugFile);
  fwrite("\n",1,1,debugFile);
  if (debug >= 201) return(OK);
 }

 if (!thisHit && !thisContext.prepared)
 {wasCode=EHNRQEXEC(sqlBuffer);
  if (RQ_OK != wasCode && RQ_LASTROW != wasCode)
  {ehnError(wasCode,"EHNRQEXEC",(int)thisContext.path);
   if (thisSQLstate->sqlcode == 7908)
       wasCode = RQ_OK ;
   else
    {if (thisSQLstate->sqlcode == 100) wasCode = RQ_LASTROW;
     else {xsqlLogPrint("PCS Code 1.1: %d",wasCode);
           dump:
            if (!thisHit)
               dumpSQL(sqlBuffer);
            else
               dumpSQL(thisHit->prototype);
          }
    }
  } 
   else thisContext.prepared = TRUE;
 }
 else 
  {if (wasCode=bindCursor(thisCursor)) return(wasCode);
   if (!thisContext.prepared)
   {if (RQ_OK !=
    (wasCode=EHNRQEXECPM(sqlBuffer,&theHandle)))
      {xsqlLogPrint("PCS Error 1.2: %d in SQL #%ld-",
                   wasCode,thisHit->fullHash);
         ehnError(wasCode,"EHNRQEXECPM",(int)thisContext.path);
         ehnError(wasCode,"EHNRQEXECPM",(int)thisContext.path);
         if (thisSQLstate->sqlcode == 7908)
              {wasCode = RQ_OK; goto didPrepare;}
         else  goto dump;
      }
   else {didPrepare:
         thisContext.actualHandle = theHandle;       
         thisContext.prepared     = TRUE;
         thisHit->preparedNtimes++;
        }
   }
   for (i=0;i<thisHit->nParms;i++)
    if ( !thisHit->parmLength[i] )
      Free( thisHit->parms[i] );
   {if (RQ_OK !=(wasCode=
       EHNRQEXECVAL(thisContext.actualHandle,thisContext.format,
                    thisContext.parameters))
      ) {ehnError(wasCode,"EHNRQEXECVAL",(int)thisContext.path);
         if (thisSQLstate->sqlcode == 7908)
              wasCode = RQ_OK;
         else {if (thisSQLstate->sqlcode == 100) wasCode = RQ_LASTROW;
               else {xsqlLogPrint("PCS Code 1.3: %d in SQL #%ld-",
                     wasCode,thisHit->fullHash);
                     goto dump;
                    }
              }
        }
   }
  }

 //ehnError(wasCode,"EHNRQEXECVAL",(int)thisContext.path);
 return(wasCode);

}
 xType2 ehnExecSelect(int thisCursor)
{  
 char     Work[40],theHandle=0;
 int      wasCode,value=OK,thisConnect=thisContext.path;
 long     sqlErrD5;
 unsigned sqlcaSize,sqlSize;

 if (debug >= 5000)
 {fwrite(sqlBuffer,MAX_SQL_TEXT-sqlBufferCapacity,1,debugFile);
  fwrite("\n",1,1,debugFile);
  if (debug >= 201) return(OK);
 }

 if (!thisHit)
 {if (RQ_OK !=
   (wasCode=EHNRQSELECT(sqlBuffer,0,&theHandle)))
  {ehnError(wasCode,"EHNRQSELECT",thisContext.path);
   if (thisSQLstate->sqlcode == 7908)
       wasCode = RQ_OK ;
   else {if (thisSQLstate->sqlcode == 100) wasCode = RQ_LASTROW;
         else {xsqlLogPrint("PCS Code 2.1: %d",wasCode);
               dump:
                 if (!thisHit)
                     dumpSQL(sqlBuffer);
                 else
                     dumpSQL(thisHit->prototype);
              }
        }
  }
  thisContext.actualHandle = theHandle;
 } else
  {if (!thisContext.prepared)
   {if (RQ_OK !=
  (wasCode=EHNRQSELECTPM(thisHit->prototype,&theHandle)))
    {xsqlLogPrint("PCS Error 2.2: %d in SQL #%ld-",wasCode,thisHit->fullHash);
     ehnError(wasCode,"EHNRQSELECTPM",thisContext.path); 
     thisHit->preparedNtimes++;
     if (thisSQLstate->sqlcode == 7908)
          wasCode = RQ_OK ;
     else goto dump;
    }
    thisHit->preparedNtimes++;
    thisContext.actualHandle = theHandle;
   }
   if (wasCode=bindCursor(thisCursor)) return(wasCode);
   {
    if (!state->pointerCheckingDisabled && state->debugCode > 100 )
    {int    nthColumn,validPointer;
     char **thisParm=thisContext.parameters;

     for(nthColumn=0;nthColumn<thisHit->nParms;nthColumn++,thisParm++)
     {
      checkRefWithSize(*thisParm,thisHit->parmLength[nthColumn],validPointer);
      if (!validPointer)
       {xsqlLogPrint("Parameter %d is invalid!",nthColumn+1);
        exit(1);
       }
     }
     checkReference(thisContext.format,validPointer);
     if (validPointer)
          {if (state->debugCode > 501)
           xsqlLogPrint("Format: %s",thisContext.format);
          }
     else {xsqlLogPrint("Invalid format!!!!!!!");
           exit(1);
          }
    }
    if (RQ_OK !=(wasCode=
        EHNRQSELECTVAL(thisContext.actualHandle,
                       thisContext.format,thisContext.parameters,FALSE))
       ) {ehnError(wasCode,"EHNRQSELECTVAL",(int)thisContext.path);
          if (thisSQLstate->sqlcode == 7908)
              wasCode = RQ_OK ;
          else {if (thisSQLstate->sqlcode == 100) wasCode = RQ_LASTROW;
                else {xsqlLogPrint("PCS Code 2.3: %d in SQL #%ld-",
                                 wasCode,thisHit->fullHash);
                      goto dump;
                     }
               }
         }
   }
  }

 //ehnError(wasCode,"EHNRQSELECTVAL",(int)thisContext.path);
 return(wasCode);

}
 xType2 ehnFetchSQLERRD3(long *value,int thisCursor)
{  
 char         Work[40];
 int          returnCode,thisConnect=thisContext.path;
 unsigned     sqlcaSize;
 struct sqlca sqlca;
  
  sqlcaSize = sizeof(struct sqlca);
  if (!(returnCode=EHNRQSQLCA(&sqlca,&sqlcaSize))) *value = sqlca.sqlerrd[2];
  else 
  {xsqlLogPrint("EHNRQSQLCA returned %d",returnCode);
   sprintf(logMsgs[0],"EHNRQSQLCA returned %d",returnCode);
   thisState->nLogMsgs = 1;
   thisState->logMsgCodeReturned = returnCode + NON_XSQLDBMS_ERROR;
  }
   
  memcpy(thisSQLstate,&sqlca,sizeof(struct sqlca));
  return(returnCode);

}
 xType2 ehnDisconnect(int thisCursor)
{
 int thisConnect=thisContext.path;

 if ( !connection.roleIsSource )
       other->connected = FALSE;
 else  am->connected    = FALSE;

 if ( !other->connected && !am->connected )
 {EHNRQEND();
  thisState->actualCarrierLinked = FALSE;
 }

}
 int xType2 ehnError(int wasCode,char *whichAPI,int thisConnect)
{  
 char         Work[40];
 int          value=OK,returnCode=OK;
 long         sqlErrD5;
 unsigned     sqlcaSize,sqlSize;
 struct sqlca sqlca;

 if (wasCode == 32)
  {memset(Work,' ',39);
   EHNRQERROR( Work, &Work[6] );
   Work[25] = 0;
   xsqlLogPrint( "The APPC major and minor codes are ----" );
   xsqlLogPrint( "---- %s ", Work );
   return 0;
  }

 sqlcaSize = sizeof(struct sqlca);
 if (!(returnCode=EHNRQSQLCA(&sqlca,&sqlcaSize)))
  {if (sqlca.sqlcode == 100) goto done;
   if (sqlca.sqlwarn[0] != ' ')
   {
    char warnings[9];

    memcpy(warnings, sqlca.sqlwarn, 8);
    warnings[8] = 0;
    xsqlLogPrint("(%s) SQLWARNINGS: %s",thisDB.subject,warnings);
    if ( !sqlca.sqlcode ) 
        goto done;
   }
   xsqlLogPrint("(%s) SQLCODE %ld",thisDB.subject,sqlca.sqlcode);
   thisState->logMsgCodeReturned
     = value = (int)sqlca.sqlcode + NON_XSQLDBMS_ERROR;
   xsqlLogPrint("(%s) %s",thisDB.subject,sqlca.sqlerrmc);
   sprintf(logMsgs[0],"(%s) SQLCODE %ld",thisDB.subject,sqlca.sqlcode);
   sprintf(logMsgs[1],"(%s) %s",thisDB.subject,sqlca.sqlerrmc);
   thisState->nLogMsgs = 2;
  }
 else 
  {xsqlLogPrint("(%s) EHNRQSQLCA returned %d",thisDB.subject,returnCode);
   sprintf(logMsgs[0],"(%s) EHNRQSQLCA returned %d",
           thisDB.subject,returnCode);
   thisState->nLogMsgs = 1;
   thisState->logMsgCodeReturned
     = value = returnCode + NON_XSQLDBMS_ERROR;
  }
 xsqlLogPrint("(%s) %s returned %d.",thisDB.subject,whichAPI,wasCode);

 done: if (thisSQLstate) memcpy(thisSQLstate,&sqlca,sizeof(struct sqlca));
       return(returnCode);

}
 xType2 ehnFetch(int thisCursor,char **fetched)
{
 int    thisConnect=thisContext.path;

  char     *receiver=NULL,*rawColumn,*theColumn;
  unsigned  receiverSize=0,returnCode,nthColumn,columnSize,*varCharSize,
           *indicator;

  if((returnCode=EHNRQFETCH(thisContext.actualHandle,receiver,&receiverSize))
       != RQ_NVLD_LEN)
   {ehnError(returnCode,"EHNRQFETCH 0",thisConnect); return(returnCode);}
  
  receiver = malloc(receiverSize+FETCH_MARGIN);

  if (returnCode=EHNRQFETCH(thisContext.actualHandle,receiver,&receiverSize))
   {if (returnCode==RQ_LASTROW)  {free(receiver); return(returnCode);}
    ehnError(returnCode,"EHNRQFETCH 1",thisConnect); return(returnCode);}

  for(nthColumn=0;nthColumn<thisContext.nColumns;nthColumn++)
  {nthAttribute.sqldata = receiver+nthAAttribute.Col_Data_Off;
   if (nthAAttribute.Col_Ind_Off)
        nthAttribute.sqlind = receiver+nthAAttribute.Col_Ind_Off;
   else nthAttribute.sqlind = &nil;
  }

  *fetched=receiver;

  return(OK);

}
 xType2 ehnDescribe(int thisCursor)
{
 int                    thisConnect=thisContext.path,returnCode;
 unsigned               noLength=0,nthColumn,sqlDAsize;
 struct Col_Attributes *dummy;
 struct sqlda          *dummy2;

  if (thisContext.augmentedAttributes) Free(thisContext.augmentedAttributes);
  
  returnCode=EHNRQATTR(thisContext.actualHandle,dummy,&noLength);  

  if (returnCode==RQ_NVLD_LEN)
  {thisContext.augmentedAttributes =
                       (struct Col_Attributes *)malloc(noLength);
   if (RQ_OK!=(returnCode=EHNRQATTR(thisContext.actualHandle,
                    thisContext.augmentedAttributes,&noLength)))
   goto fail;
   thisContext.nColumns = noLength/sizeof(struct Col_Attributes);
  } else goto fail;

  if (!thisContext.attributes)
  {thisContext.attributes = 
             malloc((sqlDAsize=SQLDASIZE(thisContext.nColumns)));
   memset(thisContext.attributes,'\0',sqlDAsize);

   if (RQ_OK!=(returnCode=
    EHNRQDESC(thisContext.actualHandle,thisContext.attributes,&sqlDAsize)))
   goto fail;

  }

  for(nthColumn=0;nthColumn < thisContext.nColumns;nthColumn++)
  {  nthAttribute.sqllen=nthAAttribute.Col_Width;
     nthAttribute.sqlname.data[nthAttribute.sqlname.length]=0;
  }

  done: thisContext.prepared = TRUE;
        bindCursor(thisCursor);
        return(OK);

  fail: return(NOT_OK);

}
 xType2 ehnCloseCursor(int thisCursor)
{
 int      thisConnect=thisContext.path;
 unsigned actualHandle,isPersistor;

 Free(thisContext.attributes);
 Free(thisContext.augmentedAttributes);
 if (thisContext.format && thisContext.format != nullFormat)
                                      free(thisContext.format);

 actualHandle  = thisContext.actualHandle;
 isPersistor   = thisContext.persisting;
 memset(&(thisContext),0,sizeof(CURSOR));
 thisState->sqlInProcess--;

 if (!isPersistor) return(EHNRQCLOSE(actualHandle));
 else return(EHNRQFREEPM(actualHandle));

}
int xType2 ehnOfflineCursor(int thisCursor)
{int thisConnect=thisContext.path;

 thisHit->cursor = 0;
 ehnCloseCursor(thisCursor);

}
int xType2 ehnInit(int arg) 
{
  execAdaptive(&state,&sql,&db);
  xsqlLink(&thisProcess,&other,&am,&cs);
}
/*          
 MODULE: REMOTSQL.C

 PURPOSE: Provide Remote SQL Function Interface for ADPTVSQL.DLL.

 DESCRIPTION:

  Backend module for ADAPTIVE.C.

 FUNCTION:

   ehnConnect(void)
  
    Connect to AS400.

   ehnDescribe(int thisCursor)

    Construct and fill in database manager style SQLDA with info obtained
    from EHNRQATTR.

   ehnEndSQL()

    Terminate connection.

   ehnExecSelect(int cursorHandle)
   
    Execute AS400 SQL (select)

   ehnExecSQL(void)

    Execute AS400 SQL (non-select)
    
   ehnFetchSQLERRD3(long *value)
 
    Set the argument to the ERRD3 value of the SQLCA.

   ehnSQLError(in wasCode,char *whichAPI)  

    Print message to log re SQL error.

       
 HISTORY:
 
  11/16/92 - Recovered from BACK2400.
  02/08/93 - Recovered from AS400PCS.DLL, ehn- mapping performed.
             Adaptive for operation in common interface.
  02/09/93 - Tested for operational eqivalence with AS400PCS.DLL
             using SQL.EXE.
  02/12/93 - Convert to XSQLPCS2.
  11/11/98 - Resurrection as interface to 'Peer.  
*/

