#include <xsqlmods.h>

#define  SOURCE_GROUP   XSQLDBMS_DLL
#define  SOURCE_MODULE  SQLKB_C
#define  INCL_DOSPROCESS 
#define  INCL_16DOSSEMAPHORES

#include <xsqlc.h>

//ICC can't: #pragma  intrinsic (memcpy, memset, strcmp, strcpy, strlen)

 xdclModeArities;

KNOWN_SQL *cacheFromCI(CACHE_ELEMENT *ce);

int xType2 bindCursor(int thisCursor)
{
 char          *bufCursor,*value;
 int            isNullable,thisType,thisConnect=thisContext.path;
 short          nthColumn,thisColumn,columnSize,*length;

  if (!thisContext.cache)
	  return(NOT_OK);

  if (!state->pointerCheckingDisabled && state->debugCode > 100 )
      checkTheHeap("'bindParameters@0");

  if (!thisContext.prepared) goto bindParameters;

  if (thisContext.sqlNow == SQL_SELECT)
  {if (!thisContext.bindings)
   {// ICC can't: thisContext.bindings = (void *)bufCursor =
    //               malloc(thisContext.nColumns*sizeof(BINDING));
    bufCursor =  malloc(thisContext.nColumns*sizeof(BINDING));
    thisContext.bindings = (void *)bufCursor;
    memset(bufCursor,'\0',thisContext.nColumns*sizeof(BINDING));
   }

   if (thisContext.nBindings != thisContext.nColumns)
   {for(thisColumn=0;thisColumn<thisContext.nColumns;thisColumn++)
    {if (!thisBinding.isBound)
     {columnSize                  = thisAttribute.sqllen;
      value                       = malloc(columnSize+1);
      length                      = &thisAttribute.sqllen; 
      allocThisBinding;
      if (*length) 
       thisBinding.internalLength = *length;
      thisBinding.isInternal      = TRUE;
     }
    }
   }
   
   if (!thisContext.bindingsCopied)
   {for(nthColumn=0;nthColumn<thisContext.nColumns;nthColumn++)
    {if (nthBinding.length)  *(nthBinding.length) =
          (long) nthAttribute.sqllen;
     nthAttribute.sqldata = nthBinding.value;
    }
    thisContext.bindingsCopied = TRUE;
   }
  }

 bindParameters:
 
  if (thisDB.accessMode >= DATABASE_MANAGER)
  for (nthColumn=0;nthColumn<thisHit->nParms;nthColumn++)
  {nthParameter.sqltype = thisHit->parmType[nthColumn];
   nthParameter.sqllen  = thisHit->parmLength[nthColumn];
   nthParameter.sqldata = thisHit->parms[nthColumn];
   if (!state->pointerCheckingDisabled && state->debugCode > 100 )
       checkTheHeap("'bindParameters@1");
  }
  else {if (!thisContext.format)
        {if (thisHit->nParms)
         {thisContext.format=bufCursor=malloc((thisHit->nParms*2)+2);
          for(nthColumn=0;nthColumn<thisHit->nParms;nthColumn++)
          {thisType = thisHit->parmType[nthColumn];
           setFormat:
            switch(thisType)
            {case SQL_TYP_NDATE:	  ; case SQL_TYP_NTIME:	   ;
             case SQL_TYP_NSTAMP:  ; case SQL_TYP_NVARCHAR: ; 
             case SQL_TYP_NCHAR:	  ; case SQL_TYP_NLONG:	   ;
             case SQL_TYP_NCSTR:	  ; case SQL_TYP_NVARGRAPH:; 
             case SQL_TYP_NGRAPHIC:; case SQL_TYP_NLONGRAPH:; 
             case SQL_TYP_NLSTR:	  ; case SQL_TYP_NFLOAT:   ; 
             case SQL_TYP_NDECIMAL:; case SQL_TYP_NINTEGER: ; 
             case SQL_TYP_NSMALL:  ; case SQL_TYP_NZONED:   ;
                isNullable = TRUE;
                thisType--;
                goto setFormat;
             case SQL_TYP_DATE:	  ; case SQL_TYP_CHAR:	   ;
             case SQL_TYP_VARCHAR: ; case SQL_TYP_CSTR:	   ;
                goto recognizedAsByteStringType;
             case SQL_TYP_LONG:	  ;
                goto recognizedAsLargeIntegerType;
            default:
             xsqlLogPrint("SQL type %d defaulted to 'A'",thisType);
            recognizedAsByteStringType:
                 *(bufCursor++) = 'A'; *(bufCursor++) = ' ';  break;
             case SQL_TYP_DECIMAL: ;
             case SQL_TYP_ZONED:   ;
             case SQL_TYP_FLOAT:   ; 
                  *(bufCursor++) = 'F'; *(bufCursor++) = ' '; break;
            recognizedAsLargeIntegerType:
             case SQL_TYP_INTEGER: ; 
                  *(bufCursor++) = 'L'; *(bufCursor++) = ' '; break;
             case SQL_TYP_SMALL:   ;
                  *(bufCursor++) = 'S'; *(bufCursor++) = ' '; break;
             case SQL_TYP_VARGRAPH:; 
             case SQL_TYP_GRAPHIC: ; 
                  *(bufCursor++) = 'G'; *(bufCursor++) = ' '; break;
             case SQL_TYP_LONGRAPH:; 
             case SQL_TYP_LSTR:	 ;
            }
          }
         *bufCursor = '\0';
         }
         else thisContext.format = nullFormat;
        }
        thisContext.parameters = thisHit->parms;
       }
      
  return(OK);

}
long isReducibleSQL(char *sqlText,KNOWN_SQL **hit,int sqlNow)
{
 char       *cursor=sqlText,*proto,*stringCursor,Work[50],partHashC,
            *theDatum;
 KNOWN_SQL  *residue;
 long        fullHash=0L,partHash;
 unsigned    endString, i, tokenSize, protoTokenSize, theType, theLength,
             seenFromClause=FALSE, inByClause=FALSE, doubleApostrophe,
             hasApostrophes, decrement, actualSize;

  if (!state->pointerCheckingDisabled && state->debugCode > 100 )
      checkTheHeap("'isReducible@0");

  residue            = malloc(sizeof(KNOWN_SQL)); 
  memset(residue,'\0',sizeof(KNOWN_SQL));     
  residue->prototype = malloc(strlen(cursor)+4);
  proto              = residue->prototype;
  memset(proto,'\0',strlen(cursor)+4);
  *hit               = residue;
  residue->sqlType   = sqlNow;

  while(*cursor)
  {hasApostrophes = FALSE;
   switch(*cursor)
   {case '\t':; case '\n':; case '\r':;
    case ' ' : tokenSize      = strspn(cursor," \t\r\n"); 
               partHash       = 32;
               protoTokenSize = 1;
               *proto         = ' ';
               goto hash;
    case '(':; case ')':;
               tokenSize      = 1;
               partHash       = *cursor;
               protoTokenSize = 1;
               break;
    case '\'': if (sqlNow == SQL_SELECT && !seenFromClause) 
               {tokenSize = 2 + strcspn(cursor+1,"'");
                goto inSelectList2;
               }
               for(doubleApostrophe=endString=FALSE,tokenSize=1;
                   !endString && *(cursor + tokenSize);tokenSize++)
                if (*(cursor+tokenSize) == '\'')
                  {if ( doubleApostrophe )
                     doubleApostrophe = FALSE;
                   else
                     {if ( *(cursor + tokenSize + 1) != '\'' ) 
                          endString        = TRUE;
                      else
                         {
                         doubleApostrophe = TRUE;
                         hasApostrophes   = TRUE;
                         }
                     }
                  }
               if (!endString) goto fail;
               theType = SQL_TYP_CHAR;
     parameterization:
               if (!residue->parms)
               {residue->parms=malloc(sizeof(char *) * MAX_PARMS_PER_SQL);
                memset(residue->parms,0,sizeof(char *) * MAX_PARMS_PER_SQL);
               }
               if (!residue->parmType)
               {residue->parmType=malloc(MAX_PARMS_PER_SQL * sizeof(unsigned));
                memset(residue->parmType,0,
                       sizeof(unsigned short) * MAX_PARMS_PER_SQL);
               }
               if (!residue->parmLength)
               {residue->parmLength=malloc(MAX_PARMS_PER_SQL * sizeof(unsigned));
                memset(residue->parmType,0,
                       sizeof(unsigned short) * MAX_PARMS_PER_SQL);
               }
               residue->parmType[residue->nParms] = theType;
               partHash       = 63;
               protoTokenSize = 2;
               strcpy(proto,"? ");
               switch(theType)
               {default:
                if (debug) 
                    xsqlLogPrint("Parameter type %d defaulted to SQL_TYP_CHAR",
                                theType);
                case SQL_TYP_CHAR:
                  // To force word allocation:
                  // actualSize  = tokenSize + ((tokenSize -1) % 4);
                  actualSize  = tokenSize - 1;
                  theDatum    = malloc(actualSize);
                  memset(theDatum,0,actualSize);
                  decrement                  = 2;
                  if (tokenSize > 2)
                    {if ( !hasApostrophes )
                           memcpy(theDatum,cursor + 1,tokenSize-2);
                     else {int j,k;

                           for (j=0,k=1,doubleApostrophe=FALSE;
                                k<tokenSize-1;k++)
                            if (*(cursor+k) == '\'')
                              {if ( doubleApostrophe )
                                  doubleApostrophe = FALSE;
                                else
                                  {if ( *(cursor + k) == '\'' ) 
                                     {theDatum[j++] = cursor[k];
                                      decrement++;
                                      doubleApostrophe = TRUE;
                                     }
                                   else
                                      theDatum[j] = 0;
                                  }
                              }
                            else
                               theDatum[j++] = cursor[k];
                          }
                    }      
                  *(theDatum+(tokenSize-decrement))
                                             = 0;
                  theLength                  = tokenSize - decrement;
                 break;
                case SQL_TYP_INTEGER:
                  theDatum                   = malloc(sizeof(long));
                  theLength                  = sizeof(long);
                  if (!sscanf(Work,"%ld",theDatum)) goto fail;
                 break;
               }
               residue->parms[residue->nParms]      = theDatum;
               residue->parmLength[residue->nParms] = theLength;
               residue->nParms++;
               goto hash;
    case '0':; case '1':; case '2':; case '3':; case '4':;
    case '5':; case '6':; case '7':; case '8':; case '9':;
    case '+':; case '-':; 
               if (sqlNow == SQL_SELECT &&
                   (!seenFromClause || inByClause))
                  goto inSelectList;
               tokenSize      = strcspn(cursor," ,\t\r\n");     
               memcpy(Work,cursor,tokenSize);
               Work[tokenSize]='\0';
               if (strcspn(Work,"0123456789"))
               {goto fail; // Later handle other types.
               } else theType = SQL_TYP_INTEGER;
               goto parameterization;
    default:  
     inSelectList:;
               tokenSize      = strcspn(cursor," ()\t\r\n");
     inSelectList2:;
               if (sqlNow == SQL_SELECT)
                 {if (seenFromClause && 
                      tokenSize == 2 && !strnicmp(cursor,"BY",2))
                      inByClause = TRUE;
                  else
                   if (inByClause && *(cursor + (tokenSize - 1)) != ',')
                       inByClause = FALSE;
                  if (!seenFromClause && 
                      tokenSize == 4 && !strnicmp(cursor,"FROM",4))
                       seenFromClause = TRUE;
                  if (seenFromClause &&
                     tokenSize == 6 && !strnicmp(cursor,"SELECT",6))
                      seenFromClause = FALSE;
                 }
               for (i=0,partHash=0L;i<tokenSize;i++)
                  {partHashC  = *(cursor + i);
                   toupper(partHashC);
                   partHash  += partHashC;
                  }
               protoTokenSize  = tokenSize;
   }
   memcpy(proto,cursor,protoTokenSize);
   hash: fullHash += partHash;
   next: cursor   += tokenSize;
         proto    += protoTokenSize;
  }

  done: if (!residue->parms) residue->parms=&nullParms;
        return(residue->fullHash=fullHash);

  fail: if (residue->parms)      Free(residue->parms);
        if (residue->parmType)   Free(residue->parmType);
        if (residue->parmLength) Free(residue->parmLength);
        Free(residue->prototype);
        Free(residue);
        return(0L);

} 
KNOWN_SQL * knownSQL(int thisConnect,char *SQLstring,int sqlNow)
{
 long          fullHash=0L,partHash;
 int           newEntry=FALSE,cacheDepth,collision=FALSE;
 CACHE_ELEMENT *probe;
 KNOWN_SQL     *residue=NULL,*hit=NULL,*try;
 
  if (!state->pointerCheckingDisabled && state->debugCode > 100 )
      checkTheHeap("'isKnownSQL@0");

  if (!(fullHash=isReducibleSQL(SQLstring,&residue,sqlNow))) goto done;

  partHash = fullHash % HASH_TABLE_SIZE;

  if (!thisApp->cache)
  {thisApp->cache = malloc(HASH_TABLE_SIZE * sizeof(CACHE_ELEMENT));
   memset(thisApp->cache,'\0',(HASH_TABLE_SIZE * sizeof(CACHE_ELEMENT)));
  }
 
  probe      = &(thisApp->cache[partHash]);
  cacheDepth = 1;
  if (!(try=probe->entry))
  {if (probe->fileOffset&&(try=probe->entry=cacheFromCI(probe)))
        goto testKey;
   else goto notFound;
  }
 testKey:
  if (try->sqlType  == sqlNow    &&
      try->fullHash == fullHash && try->nParms == residue->nParms) 
   {hit = try;
    Free(residue->prototype);
    if (residue->parms && residue->parms != &nullParms)    
    {memcpy(try->parms,residue->parms,try->nParms*sizeof(char *));
     Free(residue->parms);
    }
    if (residue->parmLength)    
    {memcpy(try->parmLength,residue->parmLength,try->nParms*sizeof(unsigned short));
     Free(residue->parmLength);
    }
    if (residue->parmType) Free(residue->parmType);
    Free(residue);
    goto done;
   }
  if (try->next)
  {if (try->nextAbsent
       && !(try->next=cacheFromCI((CACHE_ELEMENT *)try->next)))
       goto done;
   try=try->next;
   if (thisConnect)
   {if ((connection.forceReport == 2)||
        (connection.forceReport == 1 && !try->printed))
        xsqlLogPrint("SQL #%ld->#%ld @ cache depth: %d",
                     try->fullHash,try->next->fullHash,++cacheDepth);
   }
   goto testKey;
  }      
 
 notFound:

  if   (!try) hit = probe->entry = residue;
  else        hit = try->next    = residue;

  done: return(hit);

}
freeCursor(int thisConnect)
{
  char     losers[MAX_CURSORS];
  double   maxDiff=0.0;
  long     leastActive=10000000L;
  unsigned nthCursor,nLosers=0,loser,nthLoser,kavorkian=0; 
  int      modeSpecific = FALSE;

  DosSemRequest(&thisDB.gate,SEM_INDEFINITE_WAIT);

  modeSpecific = maxCursors[connection.accessMode][1] < MAX_SQL_IN_PROCESS
                 ? FALSE : TRUE;

  for(nthCursor=0;nthCursor < MAX_CURSORS;nthCursor++)
   {if (!nthHit && !nthContext.active)
       {if (!modeSpecific || 
            db[nthContext.path].accessMode == connection.accessMode )
           {loser = nthCursor; goto doit;}
       }
    if (!nthContext.active &&
        (nthHit->frequency + nthHit->hits <= leastActive))
    {if (!modeSpecific || 
         db[nthContext.path].accessMode == connection.accessMode )
     {loser=nthCursor; 
      if (nthHit->frequency + nthHit->hits < leastActive) 
          nLosers      = 1;
      else nLosers++;
      leastActive       = nthHit->frequency + nthHit->hits;
      losers[nLosers-1] = nthCursor;
     }
    }
   }
   if (nLosers > 1)
   {for (nthLoser=0;nthLoser<nLosers;nthLoser++)
    {nthCursor=losers[nthLoser];
     if (difftime(nthContext.stamp,0L) > maxDiff)
     {maxDiff   = difftime(nthContext.stamp,0L);
      kavorkian = nthLoser+1;
     }
    }
    if (!kavorkian) loser = losers[0];
    else            loser = losers[kavorkian-1];
   }

  doit: execOfflineCursor(loser);
 
  DosSemClear(&thisDB.gate);
  
}
KNOWN_SQL *cacheFromCI(CACHE_ELEMENT *ce)
{
 // Stub till level 2 optimization added.
 return(NULL);
}
/*       
  MODULE:      SQLKB.C
 
  PURPOSE:     Support automatic persisting cursors.

  DESCRIPTION:

    An efficiency intermediate between that of dynamic and static SQL 
    can be obtained without sacrificing the flexibility of dynamic. 
    This efficiency is transparent to the application programmer except
    for the setting of the degree of use of the capability.

    This is achieved by first recognizing that an application has presented
    SQL which can benefit from persistence of a cursor and then binding
    that cursor to arguments when it is subsequently presented.

    The application programmer controls the capability by setting the
    value of the state parameter optimizationLevel as indicated in
    APPSTATE.H and by setting the state parameter thresholdFrequency to
    the mimimum figure of merit a statement must have to get a
    persisting cursor. If thresholdFrequency is zero (the default) then
    all hits get persisting cursors.

    The actual operation of the capability is distributed across this 
    module and XSQLDBMS.C.

  FUNCTION:

      int bindCursor(int thisCursor,KNOWN_SQL *instance);

        Bind instance variables to SQLDA appropos statement context. 

      KNOWN_SQL *knownSQL(char *databaseName,char *sqlString)

        Perform recognition process. Returns NULL if SQL not eligible for
        optimization, not in cache, or current configuration doesn't permit
        optimization of this statement. Otherwise returns the structure.

      long isReducibleSQL(char *sqlText,KNOWN_SQL **hit) 

        Scan the source statement, obtaining the literals as the list of
        instance variable bindings and the parameterized form of the
        statement. Return 0 if the parameterization fails, its hash if it
        succeeds. Partially fill hit with other results of processing. This
        function makes a minimum number of calls as it embodies initial
        recognition.

      KNOWN_SQL *cacheFromCI(CACHE_ELEMENT *ce);

        The profile is a heap of records referenced as a name space via
        the CI_HEADER structure (see PROFILE.C). The class XSQL_PROFILE
        contains in it's first level all XSQL statement profiles. Each
        profile in turn has subsequent levels of qualification for the 
        statements themselves, identified by their hashes. 

        This feature is not complete and will require refinement of 
        existing support, e.g. each XSQL profile will have the following
        structure:
        
          1) The first HASH_TABLE_SIZE entries are those saved with 
             last store profile. This is a third level next chain
             under the profile name, named CACHE, i.e. 
             PROFILE.X.CACHE.44793 identifies a stored first level
             cache element.

          2) Subsequent elements of the original stored cache are
             stored in sub chains of the collided element. Using the
             above, PROFILE.X.CACHE.44793.3022 could be such an
             element. 

          3) Each cache element will be stored as a CI with

              ciName  - string form of the fullHash of the statement
              ciTitle - string form of the number of parameters
              ciDesc  - the parameterized form
              value   - structure with

                        - app name 
                        - cumulative hits
                        - parameter types
                        - sql statement type
      
  HISTORY:

   2/4/93 - In-line design begun. DDCS.SQC recovered as this module.
   5/1/93 - Profile processing clarified.
  10/8/93 - Correction to bug in processing numeric (integral parameters).
            Operation with SELECT only cursor eligible in production for
            some time but INSERT and UPDATE not working if accessMode
            < DATABASE_MANAGER.
  2/16/94 - Allow recognition to occur after parse, before execution
            phase. 
  8/31/96 - Recovery for integration in XSQLCORE.
 11/11/98 - SQLKB is here to stay!
*/

