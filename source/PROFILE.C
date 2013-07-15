#include <xsqlmods.h>
#define SOURCE_GROUP   XSQLDBMS_DLL
#define SOURCE_MODULE  PROFILE_C
#include <xsqlc.h>


#if (PLATFORM == XSQL_OS2)

 int           factoryCIs(char *);
 XSQL_RECORD * fileCI2Memory(XSQL_RECORD_HEADER *fileCI,
                            FILE *from,void *requester,int mode);
 void xType2   freeCIname(CI_NAME *lost);
 int           makePresent(XSQL_RECORD *thisRecord,FILE *opened);
 int  xType2   name2NodeRelation(XSQL_RECORD *node,CI_NAME *parsedName);
 CI_HEADER * xType2 namedCI(char *ciName,XSQL_RECORD **arg);
 CI_HEADER   * leafIn(XSQL_RECORD *container,CI_NAME *ciName);
 XSQL_RECORD * dfsCIClass(XSQL_RECORD *search,CI_NAME *parsedName);

#endif

#if (PLATFORM == XSQL_WINNT)

 int                factoryCIs(char *);
 XSQL_RECORD *      fileCI2Memory(XSQL_RECORD_HEADER *fileCI,
                            FILE *from,void *requester,int mode);
 xType2 void        freeCIname(CI_NAME *lost);
 int                makePresent(XSQL_RECORD *thisRecord,FILE *opened);
 xType2 int         name2NodeRelation(XSQL_RECORD *node,CI_NAME *parsedName);
 xType2 CI_HEADER * namedCI(char *ciName,XSQL_RECORD **arg);
 CI_HEADER   *      leafIn(XSQL_RECORD *container,CI_NAME *ciName);
 XSQL_RECORD *      dfsCIClass(XSQL_RECORD *search,CI_NAME *parsedName);

#endif

#define heapWalk(X) checkTheHeap(X)

#define newlyVisitedNode(X) if (X->marking != search->marking)  \
                            {X->prevMark  = 0; X->superMark = 0;\
                             X->nextMark  = 0; X->subMark   = 0;\
                             X->marking   = search->marking;    \
                             search->body = (void *)X; isNew = TRUE;}

long          passage=0; // For fileCI2Memory debugging

#if (PLATFORM == XSQL_OS2) 
 int xType2 checkTheHeap(char *where)
#endif
#if (PLATFORM == XSQL_WINNT) 
 xType2 int checkTheHeap(char *where)
#endif
{
 int  heapCode;

 if ( !state->pointerCheckingDisabled )
 {if (state->debugCode < 100)
  {heapCode = _heapchk();
   if (heapCode != _HEAPOK)
       xsqlLogPrint("Heap check(%d) in %s",heapCode,where);
  }
   else
   {
    heapCode = _heap_walk(NULL);
    if (heapCode != _HEAPOK)
        xsqlLogPrint("Heap check(%d) in %s",heapCode,where);
   }
 }

}
int bindCI(XSQL_RECORD *toBind)
{//
 //Check all the CI's in the record for corresponding bind CI's. \\
 //As an assist to the interactive state, the named CI

}
//======================================================================
int xType2 xsqlBindProfile(char *ciName) 
{
 int          validRef;
 XSQL_RECORD *thisXSQLRecord,*searchCursor,*container;
 CI_HEADER   *thisCI;

 if (ciName)
 {//Specific record binding requested.
  checkReference( ciName, validRef );
  if ( !validRef )
  {
    xsqlLogPrint( "CI name parse failure: 1" ); 
    return NOT_OK;
  }
  if (!strlen(ciName)) goto fail;
  if (thisCI=namedCI( ciName, &thisXSQLRecord) )
      bindCI(thisXSQLRecord);
 }
  else {//Scan CI class XSQL_PROFILE and perform ciBind at each node.
        xsqlLogPrint("Binding %s to session ...",state->proFileName);
        searchCursor = malloc(sizeof(XSQL_RECORD));
        memset(searchCursor,0,sizeof(XSQL_RECORD));
        searchCursor->recordType = XSQL_PROFILE;
        while(container=dfsCIClass(searchCursor,NULL))                  
              bindCI(container);
       }


 done: return OK;
 fail: xsqlLogPrint("%s caused profile binding to fail.",ciName);
       return NOT_OK;

}
//======================================================================
#if (PLATFORM == XSQL_OS2)
 CI_NAME * xType2 ciNameFromString(char *ciName)
#endif
#if (PLATFORM == XSQL_WINNT)
 xType2 CI_NAME * ciNameFromString(char *ciName)
#endif
{
 char    *cursor=ciName,*thisMember,*nameStart=ciName;
 int      memberSize=0,thisLevel=0,scanning=TRUE,validRef;
 CI_NAME *value;

 value      = malloc(sizeof(CI_NAME));
 memset(value,0,sizeof(CI_NAME));

 thisMember = malloc(64);

 checkReference( ciName, validRef );
 if ( !validRef )
 {
    xsqlLogPrint( "CI name parse failure: 1" ); 
    goto fail;
 }

 if (!strlen(ciName)) goto fail;

 while(scanning)
 {if (*cursor == '.' || !scanning)
  {finishMember:
   if (!memberSize) goto fail;
   thisMember[memberSize]        = 0;
   value->values[thisLevel]      = malloc(memberSize+1);
   strncpy(value->values[thisLevel],nameStart,memberSize);
   value->values[thisLevel][memberSize] = 0;
   nameStart                    += memberSize + 1;
   memberSize                    = 0;
   value->levels                 = ++thisLevel;        
   if (!scanning) goto done;
  }
  else
      thisMember[memberSize++] = *cursor;
  cursor++;     
  if (!*cursor) 
    {scanning = FALSE; goto finishMember;}
 }

 done: Free(thisMember);
       return value;

 fail: Free(thisMember);
       freeCIname(value);
       return NULL;

}
//======================================================================
int classFromName(CI_NAME *parsedName )
{
 int            nthClass,found=FALSE,value=0;
 XSQL_RECORD  **profile=state->proFile;
 CI_HEADER     *classDef;
 
 if (!profile)            goto done;
 if (!parsedName->levels) goto done;

 for (nthClass=0;nthClass<MAX_PROFREC_TYPE && !found;nthClass++)
   {classDef = profile[nthClass]->body;
    if (!stricmp(parsedName->values[0],classDef->ciName))
       {value = nthClass + 1; found = TRUE;}
   }

 done: return value;

}
//======================================================================
XSQL_RECORD * dfsCIClass(XSQL_RECORD *search,CI_NAME *parsedName)
{
 int          done=FALSE, isNew=FALSE,
              lexRelation, stop, thisClass, thisLevel;
 XSQL_RECORD *thisRecord=search,*value=NULL,**profile=state->proFile;
 CI_HEADER   *thisCI;

 if (!profile) return value;

 heapWalk("dfsCIClass 0");

 if (parsedName)
 {thisClass  = classFromName(parsedName);
  thisLevel  = 1;
  stop       = FALSE;
  thisRecord = profile[thisClass-1];
  thisCI     = thisRecord->body; 
  while(thisCI && !stop)
  {lexRelation=stricmp(thisCI->ciName,parsedName->values[thisLevel-1]);
   if (!lexRelation)
   {if (thisLevel == parsedName->levels)
          stop = TRUE;
    else {if (thisRecord->sub)
          {doSub:
            thisRecord = thisRecord->sub;
            thisCI     = thisRecord->body;
            thisLevel++;
          }
          else {if (thisRecord->subAbsent)
                    {makePresent(thisRecord,NULL);
                     goto doSub;
                    }
                else
                     stop = TRUE;
               }
         }
   }
    else if (lexRelation < 0)
         {if (thisRecord->next)
          {doNext:
            thisRecord = thisRecord->next;
            thisCI     = thisRecord->body;
          }
          else {if (thisRecord->subAbsent)
                    {makePresent(thisRecord,NULL);
                     goto doNext;
                    }
                else
                    stop = TRUE;
               }
         }
          else 
              stop = TRUE;
  }
  return thisRecord;
 }

 if (!search->body)
 {thisRecord        = ((XSQL_RECORD **)state->proFile)[search->recordType-1];
  search->marking   = ++state->lastMark; 
  newlyVisitedNode(thisRecord);
  return(thisRecord);
 }

 thisRecord = (void *)search->body; //ICC
 newlyVisitedNode(thisRecord);

 if (isNew) {thisRecord->subMark   = 0;
             thisRecord->superMark = 0;
             thisRecord->nextMark  = 0;
             thisRecord->prevMark  = 0;}

tremauxPredicate: 

 if ((thisRecord->next && !thisRecord->nextMark
                       &&  thisRecord->level > 1)
     || (thisRecord->nextAbsent && thisRecord->level > 1))
        goto chooseNext;
 if ((thisRecord->sub  && !thisRecord->subMark)
     || thisRecord->subAbsent)
        goto chooseSub;

 if (thisRecord->prev && thisRecord->prevMark == 2)
 {thisRecord = thisRecord->prev; goto tremauxPredicate;}
 if (thisRecord->super && thisRecord->superMark == 2)
 {thisRecord = thisRecord->super; goto tremauxPredicate;}

 return NULL;

chooseNext:

 if (thisRecord->nextAbsent)
     makePresent(thisRecord,NULL);
 thisRecord->nextMark = 1;
 thisRecord           = thisRecord->next;
 newlyVisitedNode(thisRecord);
 if (isNew) {thisRecord->prevMark = 2;
             return(thisRecord);
            }
 thisRecord->prevMark = 1;
 thisRecord           = thisRecord->prev;
 goto tremauxPredicate;

chooseSub:

 if (thisRecord->subAbsent)
     makePresent(thisRecord,NULL);
 thisRecord->subMark  = 1;
 thisRecord           = thisRecord->sub;
 newlyVisitedNode(thisRecord);
 if (isNew) {thisRecord->superMark = 2;
             return(thisRecord);
            }
 thisRecord->superMark= 1;
 thisRecord           = thisRecord->super;
 goto tremauxPredicate;

}
//======================================================================
XSQL_RECORD * fileCI2Memory(XSQL_RECORD_HEADER *fileCI,FILE *opened,
                           void *node,int modeIsClass)
{

 char                *dataCursor,*fileBuffer,**fieldArray=NULL;
 int                  didOpen=FALSE,nthCI,newRecordSize,fetchSize,baseType,
                      nFields,nthField,dataSize,dataIndex;
 long                 fileCursor, tale;
 FILE                *profileFile;
 CI_HEADER           *thisCI,*recCI=NULL;
 CI_FILE_HEADER      *thisFileAtom;
 XSQL_RECORD         *newObject,*requester=NULL;
 XSQL_RECORD         *value=NULL;
 XSQL_RECORD_HEADER  *theFileHeader;
 XSQL_HEADER_ELEMENT *class=NULL;
 STORED_STRING       *ssArray=NULL;

 passage++;

 if (!opened)    
 {if (!state->proFileName) return(value);
  if (!(profileFile = fopen(state->proFileName,"rb")))
  {xsqlLogPrint("Couldn't open profile (42) %s!", state->proFileName);
   return value;
  }
  didOpen              = TRUE;
 }
  else profileFile     = opened;

 if (modeIsClass)
      {class         = node;
       fileCursor    = class->offset;
       theFileHeader = malloc(sizeof(XSQL_RECORD_HEADER));
       memset(theFileHeader,0,sizeof(XSQL_RECORD_HEADER));
       if (_lseek(_fileno(profileFile),fileCursor,SEEK_SET)!=fileCursor)
       {xsqlLogPrint("Error locating class header (43) from %s!",
                      state->proFileName);
        goto done;
       }
       tale          = _tell(_fileno(profileFile));
       if (_read(_fileno(profileFile),theFileHeader,sizeof(XSQL_RECORD_HEADER))
           !=sizeof(XSQL_RECORD_HEADER))
       {xsqlLogPrint("Error reading class header (44) from %s!",
                      state->proFileName);
        goto done;
       }
       if (class->recordType != theFileHeader->recordType)
       {xsqlLogPrint("Class record type error (45): %d:%d",
                      class->recordType, theFileHeader->recordType );
        goto done;
       }
       if (class->size       != theFileHeader->size )
       {xsqlLogPrint("Class record size error (46): %d:%d",
                      class->size, theFileHeader->size );
        goto done;
       }
       fileCursor    = class->offset + sizeof(XSQL_RECORD_HEADER);
       fetchSize     = class->size - sizeof(XSQL_RECORD_HEADER);
       newRecordSize = sizeof(XSQL_RECORD) 
       + (fetchSize-((theFileHeader->nItems) * sizeof(CI_FILE_HEADER)))
       + ((theFileHeader->nItems) * sizeof(CI_HEADER));
       dataSize      = fetchSize 
                        - ((theFileHeader->nItems) * sizeof(CI_FILE_HEADER));
      }
 else {requester     = node;
       theFileHeader = fileCI;
       if (requester->subAbsent)
           fileCursor = requester->toGo->sub + sizeof(XSQL_RECORD_HEADER);
       else if (requester->nextAbsent)
            fileCursor = requester->toGo->next + sizeof(XSQL_RECORD_HEADER);
       fetchSize     = fileCI->size - sizeof(XSQL_RECORD_HEADER);
       newRecordSize = sizeof(XSQL_RECORD) 
       + (fetchSize-((theFileHeader->nItems) * sizeof(CI_FILE_HEADER)))
       + ((theFileHeader->nItems) * sizeof(CI_HEADER));
       dataSize      = fetchSize  
                        - ((theFileHeader->nItems) * sizeof(CI_FILE_HEADER));
      }

 newObject = malloc(newRecordSize);
 memset(newObject,0,newRecordSize);

 fileBuffer          = malloc(fetchSize + 1);

 //Read the body of or full XSQL_RECORD from disk, then reconstitute the
 //     (less compact) memory form.

 if (_lseek(_fileno(profileFile),fileCursor,SEEK_SET)!=fileCursor)
 {xsqlLogPrint("Error reading profile rec (40) from %s!",
               state->proFileName);
   Free(fileBuffer);
   goto done;
 }
 if (_read(_fileno(profileFile),fileBuffer,fetchSize)!=fetchSize)
 {xsqlLogPrint("Error reading profile rec (41) from %s!",
                 state->proFileName);
   Free(fileBuffer);
   goto done;
 }

 newObject->recordType = theFileHeader->recordType;
 newObject->level      = theFileHeader->level;
 newObject->nItems     = theFileHeader->nItems;
 newObject->size       = newRecordSize - sizeof(XSQL_RECORD);
 newObject->body       = (CI_HEADER *)(newObject + 1);
 newObject->toGo       = theFileHeader;

 if (theFileHeader->next)
     newObject->nextAbsent = TRUE;

 if (theFileHeader->sub)
     newObject->subAbsent  = TRUE;

 newObject->offset     = fileCursor;

 thisFileAtom          = (CI_FILE_HEADER *)fileBuffer;
                
 dataCursor            = (char *)&thisFileAtom[theFileHeader->nItems];

 dataIndex             = 0;

 recCI                 = newObject->body;

 for(nthCI=0,thisCI=newObject->body ;nthCI<theFileHeader->nItems;
     nthCI++, thisCI++, thisFileAtom++ )
 {thisCI->xsqlType = thisFileAtom->xsqlType;
  if (thisFileAtom->ciName.size)
  {thisCI->ciName = malloc(thisFileAtom->ciName.size + 1);
   memcpy(thisCI->ciName,
          (dataCursor+thisFileAtom->ciName.offset),
          thisFileAtom->ciName.size);
   thisCI->ciName[thisFileAtom->ciName.size] = 0;
   dataIndex += thisFileAtom->ciName.size;
  }
  if (thisFileAtom->ciTitle.size)
  {thisCI->ciTitle = malloc(thisFileAtom->ciTitle.size + 1);
   memcpy(thisCI->ciTitle,
          (dataCursor+thisFileAtom->ciTitle.offset),
          thisFileAtom->ciTitle.size);
   thisCI->ciTitle[thisFileAtom->ciTitle.size] = 0;
   dataIndex += thisFileAtom->ciTitle.size;
  }
  if (thisFileAtom->ciDescription.size)
  {thisCI->ciDescription = malloc(thisFileAtom->ciDescription.size + 1);
   memcpy(thisCI->ciDescription,
          (dataCursor+thisFileAtom->ciDescription.offset),
          thisFileAtom->ciDescription.size);
   thisCI->ciDescription[thisFileAtom->ciDescription.size] = 0;
   dataIndex += thisFileAtom->ciDescription.size;
  }
  if (thisCI->xsqlType > XSQL_SELF_DEFINED)
       {nFields      = thisFileAtom->value.size;
        baseType     = thisCI->xsqlType % XSQL_SELF_DEFINED;
        fieldArray   = malloc(sizeof(char *) * nFields);
        memset(fieldArray,0,sizeof(char *) * nFields);
        ssArray      = (STORED_STRING *)
                       (dataCursor+thisFileAtom->value.offset);
        dataIndex   += nFields * sizeof(char *);
        thisCI->value=fieldArray;
        thisCI->size = thisFileAtom->value.size;
       }
  else {nFields      = 1;
        fieldArray   = NULL;
        baseType     = thisCI->xsqlType;
       }
  for (nthField=0;nthField<nFields;nthField++)
   switch(baseType)
   {case 0:
         if (thisFileAtom->value.size)
             xsqlLogPrint("0 type with a size");
         break;
    case SQL_TYP_SMALL:
    case SQL_TYP_INTEGER:
         memcpy(&thisCI->value,&thisFileAtom->value,4);
         break;
    case SQL_TYP_CSTR:
         if (!fieldArray)
         {thisCI->value = malloc(thisFileAtom->value.size+1);
          memcpy(thisCI->value,(dataCursor+thisFileAtom->value.offset),
                 thisFileAtom->value.size);
          ((char *)thisCI->value)[thisFileAtom->value.size] = 0;
          dataIndex += thisFileAtom->value.size;
         } 
          else {fieldArray[nthField] = malloc(ssArray[nthField].size+1);
                memcpy(fieldArray[nthField],(dataCursor+ssArray[nthField].offset),
                       ssArray[nthField].size);
                fieldArray[nthField][ssArray[nthField].size] = 0;
                dataIndex += ssArray[nthField].size;
               } 
         break;
    default:;
          xsqlLogPrint("Unknown type %d",baseType);
         break;
  }
 }

 if (dataIndex != dataSize)
 {if (recCI->ciName)
    xsqlLogPrint("%s size error %d(%ld)",
      recCI->ciName,dataSize-dataIndex,passage);
  else
    xsqlLogPrint("Size error: %d(%ld)",dataSize-dataIndex,passage);
 }

 value = newObject;

 done: if (didOpen)
           fclose(profileFile);

       return value;
      
}
//======================================================================
#if (PLATFORM == XSQL_OS2)
 void xType2 freeCIname(CI_NAME *lost)
#endif
#if (PLATFORM == XSQL_WINNT)
 xType void freeCIname(CI_NAME *lost)
#endif
{
 int i;

 for (i=0;i<(MAX_CI_LEVELS-1);i++)
     Free(lost->values[i]);
 
 Free(lost);

}
//======================================================================
#if (PLATFORM == XSQL_OS2)
 char * xType2 fullName(XSQL_RECORD *edgeNode)
#endif
#if (PLATFORM == XSQL_WINNT)
 xType2 char * fullName(XSQL_RECORD *edgeNode)
#endif
{
 XSQL_RECORD *thisNode;
 CI_HEADER   *thisCI;
 int          valueSize=0, thisLevel, validRef;
 char        *value=NULL, *decompiledName[MAX_CI_LEVELS];


 checkReference( edgeNode, validRef);
 if ( !validRef )
 {
  xsqlLogPrint("Invalid XSQL reference sent fullName.");
  return value;
 }

 checkReference( edgeNode->body, validRef); 
 if ( !validRef )
 {
  xsqlLogPrint("Malformed XSQL reference sent fullName.");
  return value;
 }

 thisCI=edgeNode->body;

 checkReference( edgeNode->super, validRef); 
 if ( !validRef )
 {
  if (edgeNode->level == 1)
       {value = malloc(strlen(thisCI->ciName)+1);
        strcpy(value,thisCI->ciName);
        return value;
       }
  else
       xsqlLogPrint("Orphaned CI sent fullName");
       return  value;
 }

 checkReference( thisCI->ciName, validRef); 
 if ( !validRef )
 {
  xsqlLogPrint("Malformed CI name sent fullName.");
  return value;
 }

 if (edgeNode->level > MAX_CI_LEVELS)
 {
  xsqlLogPrint("Impossibly complex CI name sent fullName.");
  return value;
 }

 if (!edgeNode->level)
 {
  xsqlLogPrint("Impossibly simple CI name sent fullName.");
  return value;
 }

 for( thisNode=edgeNode,thisLevel=edgeNode->level-1;
      thisNode;
      thisNode=thisNode->super,
      thisCI= (!thisNode ? NULL : thisNode->body),
      thisLevel--
      )
      {decompiledName[thisLevel] = thisCI->ciName;
       valueSize += strlen(thisCI->ciName) + 2;
      }

 value  = malloc(valueSize);
 *value = 0;

 for(thisLevel=0;thisLevel<edgeNode->level;thisLevel++)
  if(thisLevel < (edgeNode->level - 1))
    {strcat(value,decompiledName[thisLevel]);
     strcat(value,".");
    }
     else
         strcat(value,decompiledName[thisLevel]);

 return value;
  
}
//======================================================================
#if (PLATFORM == XSQL_OS2)
 XSQL_RECORD * xType2 isInstance(CI_NAME *wouldBe)
#endif
#if (PLATFORM == XSQL_WINNT)
 xType2 XSQL_RECORD * isInstance(CI_NAME *wouldBe)
#endif
{
 char        *instanceName=wouldBe->values[wouldBe->levels-1];
 int          situation;
 XSQL_RECORD *metadata=NULL,*found;

 wouldBe->values[wouldBe->levels-1] = " DATA";
 found                              = dfsCIClass( NULL, wouldBe );
 situation                          = name2NodeRelation(found,wouldBe);
 wouldBe->values[wouldBe->levels-1] = instanceName;
 if (situation == FOUND)
     metadata  = found;

 return metadata;

}
//======================================================================
CI_HEADER *leafIn(XSQL_RECORD *container,CI_NAME *ciName)
{
 int         nthCI=1;
 CI_HEADER *thisCI,*value=NULL;

 for (thisCI=container->body; nthCI<=container->nItems && !value;
      nthCI++,thisCI++
      ) if (!stricmp(thisCI->ciName,ciName->values[ciName->levels-1]))
        value = thisCI; 

 return value;

}
//======================================================================
#if (PLATFORM == XSQL_OS2)
  int xType2 loadProfile()
#endif
#if (PLATFORM == XSQL_WINNT)
  xType2 int loadProfile()
#endif
{
 int                 i,value=NOT_OK,nthClass,newClass;
 long                fileCursor,tale;
 CI_HEADER           *thisCI;
 FILE                *profileFile;
 XSQL_HEADER_ELEMENT *classTable;
 XSQL_RECORD        **proFile,*thisProfileRec,*lastProfileRec=NULL;

 if (!state->proFileName) goto nullProfile;
 heapWalk("loadProfile 0");

 if (!(profileFile = fopen(state->proFileName,"rb")))
 {xsqlLogPrint("Couldn't open XSQL profile %s!", state->proFileName);
  goto nullProfile;
 }
  else {tale = _tell(_fileno(profileFile)); goto loadProfile;}

 nullProfile:

  if (state->proFile) {
    xsqlLogPrint("Attempt to dangle a profile.");
    return value;
                      };
 
  proFile = malloc( sizeof(void *) * MAX_PROFREC_TYPE );

  for (i=0;i<MAX_PROFREC_TYPE;i++) 
  {thisProfileRec = malloc( sizeof(XSQL_RECORD) );
   memset(thisProfileRec, 0, sizeof(XSQL_RECORD));
   proFile[i] = thisProfileRec;
   thisProfileRec->recordType = i + 1;
   if (!lastProfileRec)
         lastProfileRec       = thisProfileRec;
   else {lastProfileRec->next = thisProfileRec;
         thisProfileRec->prev = lastProfileRec;
         lastProfileRec       = thisProfileRec;
        }
   thisCI       = malloc(sizeof(CI_HEADER));
   memset(thisCI, 0, sizeof(CI_HEADER));
   thisProfileRec->body  = thisCI;
   thisProfileRec->level = 1;
   switch(i)
   {
    case RESERVED_CLASS_0:
         thisCI->ciName = "CLASS_0";
         break;
    case CONFIGURATION_CONTROLS:
         thisCI->ciName = "CONFIG";
         break;
    case RESERVED_CLASS_1:
         thisCI->ciName = "CLASS_1";
         break;
    case XSQL_PROFILE:
         thisCI->ciName = "VARIABLE";
         break;
    case GUI_CONTROLS:
         thisCI->ciName = "CONTROL";
         break;
   }
  }
 
  state->proFile = proFile;
  theProfile     = proFile;
  return OK;

 // Load the file class table.

 loadProfile:

 heapWalk("loadProfile 1");
 classTable = malloc(sizeof(XSQL_HEADER_ELEMENT) * (MAX_PROFREC_TYPE));
 memset(classTable,0,sizeof(XSQL_HEADER_ELEMENT) * (MAX_PROFREC_TYPE));

 for( nthClass=0; nthClass < MAX_PROFREC_TYPE; nthClass++ )
 {tale = _tell(_fileno(profileFile));
  if (_read(_fileno(profileFile),&classTable[nthClass],
           sizeof(XSQL_HEADER_ELEMENT)) != sizeof(XSQL_HEADER_ELEMENT))
  {xsqlLogPrint("Error reading XSQL class table from %s!", state->proFileName);
   goto done;
  }
 }

 // Load the roots of the file form of the profile, converting to the
 // memory form, completing preliminary load. Operation and implicit
 // invocation of makePresent do the rest of the loading.

 heapWalk("loadProfile 2");
 proFile = malloc( sizeof(void *) * (MAX_PROFREC_TYPE) );
 memset(proFile, 0, sizeof(void *) * (MAX_PROFREC_TYPE) );

 for( nthClass=0; nthClass < MAX_PROFREC_TYPE; nthClass++ )
 {tale = _tell(_fileno(profileFile));
  if (!(proFile[nthClass]=thisProfileRec=
    fileCI2Memory(NULL,profileFile,&classTable[nthClass],TRUE)))
  {xsqlLogPrint("Error loading profile from %s!", state->proFileName);
   goto done;
  }
  if (lastProfileRec)
  {lastProfileRec->next = thisProfileRec;
   thisProfileRec->prev = lastProfileRec;
  } 
  lastProfileRec = thisProfileRec;
  tale = _tell(_fileno(profileFile));
 }

 xsqlLogPrint("The profile %s has been opened.",state->proFileName);
 xsqlBindProfile( NULL );

 value          = OK;
 state->proFile = proFile;

 done: if (profileFile)
           fclose(profileFile);
       Free(classTable);
       return value;

}
int makePresent(XSQL_RECORD *thisRecord,FILE *opened)
{
 int                 didOpen=FALSE;
 long                fileCursor,tale;
 FILE                *profileFile;
 XSQL_RECORD         *new;
 XSQL_RECORD_HEADER  *requester,*loaded;

 if (!opened)    
 {if (!state->proFileName) return(NOT_OK);
  if (!(profileFile = fopen(state->proFileName,"rb")))
  {xsqlLogPrint("Couldn't open profile (50) %s!", state->proFileName);
   return NOT_OK;
  }
  didOpen              = TRUE;
 }
  else {tale = _tell(_fileno(opened)); profileFile     = opened;}

 requester = thisRecord->toGo;
 
 if (thisRecord->subAbsent)
 {tale = _tell(_fileno(profileFile));
  fileCursor = requester->sub;
  loaded     = malloc(sizeof(XSQL_RECORD_HEADER));
  memset(loaded,0,sizeof(XSQL_RECORD_HEADER));
  if (_lseek(_fileno(profileFile),fileCursor,SEEK_SET)!=fileCursor)
  {xsqlLogPrint("Error locating record header (32) from %s!",
                 state->proFileName);
   return NOT_OK;
  }
  if (_read(_fileno(profileFile),loaded,sizeof(XSQL_RECORD_HEADER))
      !=sizeof(XSQL_RECORD_HEADER))
  {xsqlLogPrint("Error reading profile rec (31) from %s!",state->proFileName);
   return NOT_OK;
  }
  if (!(new=fileCI2Memory(loaded,profileFile,thisRecord,FALSE)))
      return NOT_OK;
  thisRecord->sub       = new;        
  new->super            = thisRecord;
  thisRecord->subAbsent = FALSE;
 }

 if (thisRecord->nextAbsent)
 {tale = _tell(_fileno(profileFile));
  fileCursor = requester->next;
  loaded     = malloc(sizeof(XSQL_RECORD_HEADER));
  memset(loaded,0,sizeof(XSQL_RECORD_HEADER));
  if (_lseek(_fileno(profileFile),fileCursor,SEEK_SET)!=fileCursor)
  {xsqlLogPrint("Error locating record header (33) from %s!",
                 state->proFileName);
   return NOT_OK;
  }
  if (_read(_fileno(profileFile),loaded,sizeof(XSQL_RECORD_HEADER))
      != sizeof(XSQL_RECORD_HEADER))
  {xsqlLogPrint("Error reading profile rec (30) from %s!",state->proFileName);
   return NOT_OK;
  }
  if (!(new=fileCI2Memory(loaded,profileFile,thisRecord,FALSE)))
      return NOT_OK;
  thisRecord->next       = new;
  new->prev              = thisRecord;
  new->super             = thisRecord->super;
  thisRecord->nextAbsent = FALSE;
 }

 if (didOpen)
     fclose(profileFile);

 return OK;

}
//======================================================================
XSQL_RECORD_HEADER *memoryCI2File(XSQL_RECORD *memoryCI)
{// Returned object is full record, ready to be written.

 char               *recordCursor, *dataCursor, *ciRecFullName;
 short               nthCI,newDataSize=0,newRecordSize,thisItemSize,
                     dataIndex,thisAtomSize,nFields,baseType,nthField,
                   **fieldArray=NULL;
 CI_HEADER          *thisCI;
 CI_FILE_HEADER     *thisFileAtom;
 XSQL_RECORD_HEADER *value=NULL,*newFileObject;
 STORED_STRING      *ssArray; 

 if (!memoryCI) return value;
 thisCI=memoryCI->body;
 ciRecFullName=fullName(memoryCI); //ICC

 if (!memoryCI->nItems) memoryCI->nItems++;

 for(nthCI=1;nthCI<=memoryCI->nItems && thisCI;nthCI++,thisCI=thisCI->next)
  { nFields=thisItemSize=0;
    fieldArray=NULL;
    if (thisCI->xsqlType > XSQL_SELF_DEFINED)
         {nFields      = thisCI->size;
          baseType     = thisCI->xsqlType % XSQL_SELF_DEFINED;
          fieldArray   = thisCI->value;
          thisItemSize = nFields * sizeof(char *);
         }
    else {baseType   = thisCI->xsqlType;
          nFields    = 1;
         }
    for (nthField=0;nthField<nFields;nthField++)
    {switch(baseType)
     {case 0:;
           if (thisCI->value)
           {xsqlLogPrint("%s.%s, value but zero type.",
                          ciRecFullName,thisCI->ciName);
            exit(1);
           }
           break;
      case SQL_TYP_INTEGER:
      case SQL_TYP_SMALL:
           break;
      case SQL_TYP_CSTR:
           if (fieldArray && fieldArray[nthField])
                 thisItemSize += strlen((char *)fieldArray[nthField]);
            else{if (!thisCI->value)
                 thisItemSize = 0;
                 else
                 thisItemSize += thisCI->value ? strlen(thisCI->value) : 0;
                }
           break;
      default:
           xsqlLogPrint("%s.%s type %d, value but unknown type.",
             ciRecFullName,thisCI->ciName,thisCI->xsqlType);
           exit(1);
     }
    }
    if (thisCI->ciName && *thisCI->ciName)
        thisItemSize += strlen(thisCI->ciName);
    if (thisCI->ciTitle && *thisCI->ciTitle)
        thisItemSize += strlen(thisCI->ciTitle);
    if (thisCI->ciDescription && *thisCI->ciDescription)
        thisItemSize += strlen(thisCI->ciDescription);
    newDataSize += thisItemSize;
  }
      
 newRecordSize = sizeof(XSQL_RECORD_HEADER)  
                 + ((memoryCI->nItems) * sizeof(CI_FILE_HEADER))
                 + newDataSize;

 memoryCI->size= ((memoryCI->nItems) * sizeof(CI_HEADER))
                 + newDataSize;

 newFileObject = malloc(newRecordSize);
 memset(newFileObject,0,newRecordSize);

 newFileObject->recordType = memoryCI->recordType;
 newFileObject->level      = memoryCI->level;
 newFileObject->size       = newRecordSize;
 newFileObject->nItems     = memoryCI->nItems;

 recordCursor  = (char *)(newFileObject + 1);
 thisFileAtom  = (CI_FILE_HEADER *)recordCursor;
 dataCursor    = (char *)&thisFileAtom[memoryCI->nItems];
 dataIndex     = 0;

 for(nthCI=0,thisCI=memoryCI->body ;nthCI<memoryCI->nItems && thisCI;
     nthCI++,thisCI=thisCI->next, thisFileAtom++ )
 {thisFileAtom->xsqlType        = thisCI->xsqlType;
  thisFileAtom->ciName.offset   = dataIndex;
  thisAtomSize                  = strlen(thisCI->ciName);
  memcpy(dataCursor,thisCI->ciName,thisAtomSize);
  dataCursor                   += thisAtomSize;
  thisFileAtom->ciName.size     = thisAtomSize;
  dataIndex                    += thisAtomSize;
  if (thisCI->ciTitle)
  {thisFileAtom->ciTitle.offset = dataIndex;
   thisAtomSize                 = strlen(thisCI->ciTitle);
   memcpy(dataCursor,thisCI->ciTitle,thisAtomSize);
   dataCursor                  += thisAtomSize;
   thisFileAtom->ciTitle.size   = thisAtomSize;
   dataIndex                   += thisAtomSize;
  }
  if (thisCI->ciDescription)
  {thisFileAtom->ciDescription.offset= dataIndex;
   thisAtomSize                      = strlen(thisCI->ciDescription);
   memcpy(dataCursor,thisCI->ciDescription,thisAtomSize);
   dataCursor                       += thisAtomSize;
   thisFileAtom->ciDescription.size  = thisAtomSize;
   dataIndex                        += thisAtomSize;
  }
  nFields    = 0;
  fieldArray = NULL;
  ssArray    = (void *)dataCursor; //ICC was STORED_STRING **
  if (thisCI->xsqlType > XSQL_SELF_DEFINED)
  {nFields      = thisCI->size;
   baseType     = thisCI->xsqlType % XSQL_SELF_DEFINED;
   fieldArray   = thisCI->value;
   if (fieldArray)
   {thisFileAtom->value.offset = dataIndex;
    thisFileAtom->value.size   = nFields;
   }
    else if (nFields)
         {xsqlLogPrint("%s.%s %d fields but no value.",ciRecFullName,
                        thisCI->ciName,nFields );
          exit(1);
         }
   dataCursor  += nFields * sizeof(STORED_STRING);
   dataIndex   += nFields * sizeof(STORED_STRING);
  }
  else {baseType   = thisCI->xsqlType;
        nFields    = 1;
        fieldArray = NULL;
       }
  for (nthField=0;nthField<nFields;nthField++)
   switch(baseType)
   {case 0:   
          if (thisCI->value)
          {xsqlLogPrint("%s.%s, value but zero type!",
                        ciRecFullName,thisCI->ciName);
           exit(1);
          }
          break;
    case SQL_TYP_CSTR:
          if (fieldArray)
             {ssArray[nthField].offset= dataIndex;
              if (fieldArray[nthField])
              {thisAtomSize            = strlen((char *)fieldArray[nthField]);
               if (thisAtomSize)   
                memcpy(dataCursor,fieldArray[nthField],thisAtomSize);
              }
               else thisAtomSize = 0;
             }
          else
              thisAtomSize            = thisCI->value ? 
                                         strlen(thisCI->value) : 0;
          if (!fieldArray && thisCI->value)
          {thisFileAtom->value.offset = dataIndex;
           thisFileAtom->value.size   = thisAtomSize;
           if (thisAtomSize)
               memcpy(dataCursor,thisCI->value,thisAtomSize);
          }
          if (fieldArray)
              ssArray[nthField].size  = thisAtomSize;
          dataCursor                 += thisAtomSize;
          dataIndex                  += thisAtomSize;
          break;
    case SQL_TYP_INTEGER:
    case SQL_TYP_SMALL:
          memcpy( &thisFileAtom->value, &thisCI->value, 4);
          if (fieldArray)
          {}
          break;
   }
 }

 value = memoryCI->toGo = newFileObject;

 done: if (dataIndex!=newDataSize)
          xsqlLogPrint("%s size error.",fullName(memoryCI));
       if (memoryCI->toGo)
         {unsigned size1=0,size2=0;
          size1 = memoryCI->size  
                   - ((memoryCI->nItems) * sizeof(CI_HEADER));
          size2 = memoryCI->toGo->size -
                  ((memoryCI->toGo->nItems) * sizeof(CI_FILE_HEADER))
                   - sizeof(XSQL_RECORD_HEADER);
          if (size1!=size2)
            xsqlLogPrint("%s ~ %d",ciRecFullName,size2-size1);
         }

       Free(ciRecFullName);
       return(value);
      
}
//======================================================================
#if (PLATFORM == XSQL_OS2)
 CI_HEADER * xType2 namedCI(char *ciName,XSQL_RECORD **anchor)
#endif
#if (PLATFORM == XSQL_WINNT)
 xType2 CI_HEADER * namedCI(char *ciName,XSQL_RECORD **anchor)
#endif
{
 char          *leafName,*isMeta=NULL;
 int            ciClass,nullAnchor,noAnchor,addLeaf=FALSE,nthLeaf;
 XSQL_RECORD   *container,*searchCursor,**profile,*new=NULL,
               *super,*definition;
 CI_HEADER     *value=NULL,*nthCI,*mthCI,*lastCI=NULL;
 CI_NAME       *parsedName=NULL;
 int            situation,validRef;

 checkTheHeap("namedCI 0");

 checkReference( ciName, validRef );
 if ( !validRef )
 {
    xsqlLogPrint( "CI name ref invalid!" ); 
    return NULL;
 }

 parsedName=ciNameFromString(ciName);
 if (!parsedName)
 {  xsqlLogPrint( "CI name invalid!" ); 
    return NULL;
 }

 if (!state->proFile)
 {if (loadProfile()) 
   return(value);
 }

 profile=state->proFile;

 if (!(ciClass=classFromName(parsedName))) 
 {xsqlLogPrint("Unrecognized CI class: %s",ciName);
  return NULL;
 }

 searchCursor=profile[ciClass-1];

 isMeta = strchr(ciName,' ');

 if (anchor)
  {checkReference( anchor, validRef );
   if ( !validRef )
   {//if (container->level == 1 || situation != FOUND)
    xsqlLogPrint( "%s: record indirection invalid!", ciName ); 
    return NULL;
   }
   if (*anchor)
   {checkReference( *anchor, validRef );
    if ( !validRef )
    {xsqlLogPrint( "%s: record reference invalid!", ciName ); 
     return NULL;
    }
    addLeaf = TRUE; 
    leafName=parsedName->values[parsedName->levels-1];
    parsedName->values[parsedName->levels-1] = NULL;
    parsedName->levels--;
   }
  }

 if (!(container=dfsCIClass(NULL,parsedName)))
 {xsqlLogPrint("%s: containment failure!",ciName);
  return NULL;
 }

 if (container->body && !container->nItems) container->nItems++;

 situation = name2NodeRelation(container,parsedName);

 if (situation <= NODE_ERROR)
 {xsqlLogPrint("%s: graph failure: %d!",ciName,situation);
  return NULL;
 }

 if ( !anchor && situation != FOUND && situation != FOUND_LEAF)
 {xsqlLogPrint("Couldn't anchor %s!",ciName);
  return NULL;
 }
  else if (addLeaf && situation != FOUND)
       {xsqlLogPrint("%s inappropriate for %s at this point.",
                      nodeSituation[situation],ciName);     
        goto fail;
       }

 // Now have valid CI name region in which to operate. If referencing
 // and not defining, leave with the found reference.

 heapWalk("named CI 1");

 if (anchor && container->level == 1 && situation == FOUND)
 {value   = container->body;
  *anchor       = container;
  searchCursor = malloc(sizeof(XSQL_RECORD));
  memset(searchCursor,0,sizeof(XSQL_RECORD));
  searchCursor->recordType = ciClass;
  while(container=dfsCIClass(searchCursor,NULL));                  
  goto done;
 }

 if (!anchor)
  {if (situation != FOUND && situation != FOUND_LEAF)
   {xsqlLogPrint("can't find %s",ciName); goto fail;}
   deReference:
    if (situation == FOUND)
         value = container->body;
    else value = leafIn(container,parsedName); 
    if (anchor)
       *anchor = container;
    goto done;
  }
  else if ((situation == FOUND && !addLeaf) || situation == FOUND_LEAF)
       goto deReference;

 definition=isInstance(parsedName);

 if (definition && !isMeta)
 {if (definition->nItems < 2)
  {xsqlLogPrint("%s definition NULL!",fullName(definition));
   goto fail;
  }
  new             = malloc(sizeof(XSQL_RECORD));
  memset(new,0,sizeof(XSQL_RECORD));
  new->super      = definition->super;
  new->nItems     = definition->nItems;
  new->level      = definition->level;
  *anchor         = new;
  value           = new->body = malloc(sizeof(CI_HEADER)*definition->nItems);
  nthCI           = value;
  mthCI           = definition->body;
  memset(value,0,sizeof(CI_HEADER)*definition->nItems);
  for (nthLeaf=0;nthLeaf<definition->nItems;nthLeaf++,nthCI++,mthCI++)
  {nthCI->value         = NULL;
   nthCI->ciName        = mthCI->ciName;
   nthCI->ciTitle       = mthCI->ciTitle;       // These can be
   nthCI->ciDescription = mthCI->ciDescription; // overridden
   nthCI->xsqlType= mthCI->xsqlType;
   if (lastCI) 
     lastCI->next = nthCI;
   lastCI         = nthCI;
   if (!nthLeaf)
   {nthCI->ciName = malloc(strlen(parsedName->values[(parsedName->levels)-1])+1);
    strcpy(nthCI->ciName,parsedName->values[(parsedName->levels)-1]);
   } 
  }
 }

 if ((situation > NODE_ERROR && situation < NEW_RECORD) && !new)
 {if (situation != FOUND && situation != FOUND_LEAF)
  {new            = malloc(sizeof(XSQL_RECORD));
   memset(new,0,sizeof(XSQL_RECORD));
   new->super     = container;
   new->nItems    = 1;
   *anchor        = new;
   value          = new->body  = malloc(sizeof(CI_HEADER));
   memset(value,0,sizeof(CI_HEADER));
   value->ciName  = malloc(strlen(parsedName->values[parsedName->levels-1])+1);
   strcpy(value->ciName,parsedName->values[parsedName->levels-1]);
   new->level     = parsedName->levels;
   if (isMeta)
   {if (!stricmp(value->ciName," DATA"))
         new->recordType = DATA_DEF;
    else if (!stricmp(value->ciName," CONTROL"))
              new->recordType = CONTROL_DEF;
         else xsqlLogPrint("%s is an unknown definition type",value->ciName);
   }
   checkTheHeap("namedCI newRecord");
  }
  else {if (situation == FOUND_LEAF)
             value=leafIn(container,parsedName);
         else
             value=container->body;
         if (anchor)
             *anchor=container;
         checkTheHeap("namedCI oldLeaf");
        }
 }

 treeMaintenance:

 switch(situation)
 {case FOUND:
  case FOUND_LEAF:
  case NEW_SUB:
    if (container->sub && situation == NEW_SUB)
    {xsqlLogPrint("%s can't replace %s",ciName,fullName(container->sub));
     goto fail;
    }
    else
    if (situation == NEW_SUB)
        container->sub = new;
    if (addLeaf) 
     {CI_HEADER *oldBody=container->body,*this,
      *newBody=malloc((container->nItems+1) * sizeof(CI_HEADER));
      char      *recordCursor=(char *)oldBody;
      int        nthItem;
      CI_HEADER *new=&(newBody[container->nItems]),*last;

      memcpy(newBody,oldBody,container->nItems*sizeof(CI_HEADER));
      memset(new,0,sizeof(CI_HEADER));
      new->ciName = malloc(strlen(leafName)+1);
      strcpy(new->ciName,leafName);
      container->nItems++;
      container->body = newBody;
      Free(oldBody);
      value = new;
      for (nthItem=0,this=last=newBody;
           nthItem<container->nItems;nthItem++,this++)
           if (nthItem) {last->next = this; last=this;}
      checkTheHeap("namedCI newLeaf");
     }
                break;

  case PRE_SUB: //Stub.
    xsqlLogPrint("%s doesn't implement PRE_SUB",ciName);  
                break;
  case POST_SUB://Stub.
    xsqlLogPrint("%s doesn't implement POST_SUB",ciName);  
                break;
  case PRE_NEXT:
                if (!container->prev)
                {container->super->sub = new;
                 new->prev             = container->prev;
                 container->prev       = new;
                 new->next             = container;
                }
                else {container->prev->next = new;
                      new->prev             = container->prev;
                      container->prev       = new;
                      new->next             = container;
                     }
                new->super             = container->super;
                break;
  case POST_NEXT:
                new->super      = container->super;
                container->next = new;
                new->prev       = container;
                break;
 }
        
 goto done;

 fail: *anchor = NULL;

 done: 
       if (parsedName)
           freeCIname(parsedName);
 
       return value;

}
//======================================================================
#if (PLATFORM == XSQL_OS2)
 int xType2 name2NodeRelation(XSQL_RECORD *node,CI_NAME *parsedName)
#endif
#if (PLATFORM == XSQL_WINNT)
 xType2 int name2NodeRelation(XSQL_RECORD *node,CI_NAME *parsedName)
#endif
{// Only called at end of search for specific node, so can assume caller
 // guarantees ordering. 
  
 int            lexRelation,nodeLevel=node->level,nthItem,found=FALSE,
                            nameLevel=parsedName->levels;
 CI_HEADER     *thisCI=node->body;
 int            value=NODE_ERROR, eigenNode=nameLevel-nodeLevel; 
 XSQL_RECORD   *sub;

 eigenNode = eigenNode > 0 ? eigenNode : (eigenNode < 0 ? -1 : eigenNode);

 if (!thisCI)         goto done;
 if (eigenNode > 1)   {value--; goto done;}

 switch(eigenNode)
 {case -1: break;
  case  0: lexRelation  =
              stricmp(parsedName->values[nameLevel-1],thisCI->ciName);
           if (!lexRelation)
                value = FOUND;
           else if (lexRelation < 0) value = PRE_NEXT;
                else                 value = POST_NEXT;
           break;
  case  1: if (node->nItems > 1)
           {for(nthItem=node->nItems-1,thisCI++;
                     nthItem && !found;nthItem--,thisCI++)
            found=!stricmp(thisCI->ciName,parsedName->values[nameLevel-1]);
            if (found)
            {value = FOUND_LEAF; break;}
           }
           if (!node->sub)
             {value = NEW_SUB; break;}
           sub          = node->sub;
           if (!(thisCI = sub->body)) break;
           lexRelation  = 
              stricmp(parsedName->values[nameLevel-1],thisCI->ciName);
           if (!lexRelation) goto done;
           else if (lexRelation < 0) value = PRE_SUB;
                else             value = POST_SUB;
 }

 done: return value;
}
//======================================================================
#if (PLATFORM == XSQL_OS2)
 CI_HEADER * xType2 nextLeaf(XSQL_RECORD *container,CI_HEADER **cursor)
#endif
#if (PLATFORM == XSQL_WINNT)
 xType2 CI_HEADER * nextLeaf(XSQL_RECORD *container,CI_HEADER **cursor)
#endif
{         
 CI_HEADER *value;          

 int lastCI = ((char *)(*cursor) - (char *)container->body) / sizeof(CI_HEADER);

 if ((lastCI +1) == container->nItems) return(NULL);

 value   = *cursor;
 value++;
 *cursor = value;
 return  value;

}
//======================================================================
#if (PLATFORM == XSQL_OS2)
 void * xType2 nthAtom(CI_HEADER *thisCI, int short which)
#endif
#if (PLATFORM == XSQL_WINNT)
 xType2 void * nthAtom(CI_HEADER *thisCI, int short which)
#endif
{
 char **fieldArray=thisCI->value;

 if (which > thisCI->size) 
 {xsqlLogPrint("No %d-th atom.",which);
    return NULL;
 }

 if ( !which )
 {xsqlLogPrint("Atom indices are 1 based.");
    return NULL;
 }

 return fieldArray[(which - 1)];

}
//======================================================================
#if (PLATFORM == XSQL_OS2)
 int xType2 storeProfile()
#endif
#if (PLATFORM == XSQL_WINNT)
 xType2 int storeProfile()
#endif
{
 int                 value=NOT_OK,nthClass,newClass;
 long                fileCursor,classTableSize,headerSize,tale;
 FILE                *profileFile=NULL;
 XSQL_HEADER_ELEMENT *classTable=NULL;
 XSQL_RECORD_HEADER  *profile=NULL,*toGo=NULL;
 XSQL_RECORD         *proFile=NULL,*thisRecord=NULL,*searchCursor;

 if (!state->proFile) return value;
 if (!state->proFileName) state->proFileName = "xaccess.ini";

 if (!(profileFile = fopen(state->proFileName,"wb")))
 {xsqlLogPrint("Couldn't open XSQL profile %s!", state->proFileName);
  goto done;
 }

 tale = _tell(_fileno(profileFile));

 fileCursor = classTableSize = headerSize 
            = MAX_PROFREC_TYPE * sizeof(XSQL_HEADER_ELEMENT);

 searchCursor = malloc(sizeof(XSQL_RECORD));

 classTable = malloc(sizeof(XSQL_HEADER_ELEMENT) * MAX_PROFREC_TYPE);
 memset(classTable,0,sizeof(XSQL_HEADER_ELEMENT) * MAX_PROFREC_TYPE);

 // Walk the profile and convert each record to static, zero address form.

 for( nthClass=0; nthClass < MAX_PROFREC_TYPE; nthClass++ )
 {thisRecord                      =((XSQL_RECORD**)state->proFile)[nthClass];
  memset(searchCursor,0,sizeof(XSQL_RECORD));
  searchCursor->recordType = nthClass+1;
  while((toGo=memoryCI2File((thisRecord=dfsCIClass(searchCursor,NULL)))))
       {thisRecord->offset = fileCursor;
        fileCursor        += toGo->size;
       }
 }

 fileCursor = classTableSize = headerSize 
            = MAX_PROFREC_TYPE * sizeof(XSQL_HEADER_ELEMENT);

 // Recreate class table header, rewalk the profile storing and deleting
 // static form of the class graph. 

 for( nthClass=0; nthClass < MAX_PROFREC_TYPE; nthClass++ )
 {thisRecord = ((XSQL_RECORD **)state->proFile)[nthClass];
  classTable[nthClass].recordType = nthClass + 1;
  classTable[nthClass].size       = thisRecord->toGo->size;
  classTable[nthClass].offset     = thisRecord->offset;
  tale = _tell(_fileno(profileFile));
  if (_write(_fileno(profileFile),&classTable[nthClass],
            sizeof(XSQL_HEADER_ELEMENT)) != sizeof(XSQL_HEADER_ELEMENT))
  {xsqlLogPrint("Error creating XSQL class table on %s!", state->proFileName);
   goto done;
  }
  tale = _tell(_fileno(profileFile));
 }

 fileCursor = _tell(_fileno(profileFile));

 if (fileCursor != headerSize)
 {xsqlLogPrint("Header sync error on %s!",state->proFileName); goto done;}

 for( nthClass=0; nthClass < MAX_PROFREC_TYPE; nthClass++ )
 {thisRecord = ((XSQL_RECORD **)state->proFile)[nthClass];
  memset(searchCursor,0,sizeof(XSQL_RECORD));
  searchCursor->recordType = nthClass+1;
  while(thisRecord=dfsCIClass(searchCursor,NULL))
       {tale = _tell(_fileno(profileFile));
          if (tale != thisRecord->offset)
            xsqlLogPrint("Offset error: %s level %d %ld:%ld",
                         thisRecord->body->ciName,
                         thisRecord->level, thisRecord->offset, tale );
         {unsigned size1=0,size2=0;
          size1 = (thisRecord->size  
                   - ((thisRecord->nItems) * sizeof(CI_HEADER)));
          size2 = thisRecord->toGo->size -
                  ((thisRecord->toGo->nItems) * sizeof(CI_FILE_HEADER))
                   - sizeof(XSQL_RECORD_HEADER);
          if (size1!=size2)
            xsqlLogPrint("%s : %d",fullName(thisRecord),size2-size1);
         }
          if (thisRecord->level != thisRecord->toGo->level)
            xsqlLogPrint("Level error: %s %d:%d",
                         thisRecord->body->ciName, thisRecord->level,
                         thisRecord->toGo->level );
          if (thisRecord->recordType != thisRecord->toGo->recordType)
            xsqlLogPrint("Record type error: %s %d:%d",
                         thisRecord->body->ciName, thisRecord->recordType,
                         thisRecord->toGo->recordType );
        if (thisRecord->level == 1)
        {if (classTable[nthClass].recordType != thisRecord->toGo->recordType)
            xsqlLogPrint("Class record type error: %s %d:%d",
                       thisRecord->body->ciName, thisRecord->toGo->recordType,
                       classTable[nthClass].recordType );
         if (classTable[nthClass].size != thisRecord->toGo->size )
            xsqlLogPrint("Class record size error: %s %d:%d",
                       thisRecord->body->ciName, thisRecord->toGo->size,
                       classTable[nthClass].size );
         if (classTable[nthClass].offset != thisRecord->offset)
            xsqlLogPrint("Class record loc error: %s %ld:%ld",
                       thisRecord->body->ciName, thisRecord->offset,
                       classTable[nthClass].offset );
        }
        if (thisRecord->next)
            thisRecord->toGo->next = thisRecord->next->offset;
        if (thisRecord->sub)
            thisRecord->toGo->sub  = thisRecord->sub->offset;
        if (_write(_fileno(profileFile),thisRecord->toGo,thisRecord->toGo->size)
            !=thisRecord->toGo->size)
        {xsqlLogPrint("Error creating XSQL CI Record on %s!",
                       state->proFileName);
         goto done;
        }
        fileCursor = fileCursor + thisRecord->toGo->size;
        tale = _tell(_fileno(profileFile));
        //Release static form.
        Free(thisRecord->toGo);
       }
 }

 value = OK;

 fclose(profileFile);

 done: if (profileFile)
           fclose(profileFile);
       Free(profile);
       Free(classTable);
       return value;
}
/*          
  MODULE:      PROFILE.C

  PURPOSE:     Process an XSQL configuration file for an XSQL process.

  DESCRIPTION:

   Information specific to a given use of XSQL is stored in a binary
   file specified by the application. Application must place the
   path specification of this file in the STATE structure prior to
   invocation of xsqlLink, otherwise the default name in the current
   directory is used.

   The file structure is that of a table and a heap. The table contains
   information on the heap and the heap contains all the records. All
   accesses to the file are performed via functions in this module, it's
   internal structure is invisible to other modules.

   The table is an array of one 8-byte structure (XSQL_HEADER_ELEMENT)
   per record "class" locating the origins of the record graphs in the heap.
   Each record in a graph consists of a header followed by the record body,
   whose size is indicated in the header. The records are linked in an
   acyclic graph structure by file offset pointers. All file offsets
   stored in the profile are relative to it's origin.

   Configuration Items (CI's) are stored in the above format. CI's are
   grouped by type (class) with records for a given class in the same
   graph. Each record in a graph begins with a set of CI headers equal the 
   number of CI's in the record, except that the first record in the
   graph also has a header for the class. These headers locate the
   CI data which is in the body of the record.

   CI names beginning with a space are for profile internal processing,
   or "meta' records. Such records are taken as templates for other records
   with the same leading qualification (but without the leading space on
   the last name member). This records CI's are definitions of other CI
   records. An intrinsic meta type ' DATA' is recognized by the basic profile
   mechanism. When a CI name is presented, if it's level has this name
   defined then it can be instantiated and the internal file and live form
   of the CI are different. CI records created in this way are 'instance
   records' which do not need to have CI headers for each data value.
   Instead, an array of STORED_STRINGs corresponding to instance variables
   replaces the CI headers and prefixes a mini-heap of the instance values
   in the body of the CI, defined by the CI's (thier XSQL types) in the
   definition.

   Using the scheme above, the basic i/o routines in this module can
   maintain a structured name space of arbitrary data structures. Records   
   are demand loaded as regions of the profile graph are accessed, but are
   written en-masse only, there is no "makeNonPresent", only saveProfile. 

   Any further name space semantics is supplied by the application. 

   A profile is required to exist in the distributed version. 

   See also XSQL1.H.

  FUNCTION:

   int bindCI(XSQL_RECORD *ciName)

     Binds values for each CI in a record.

   int bindProfile( char *ciName )

     If ciName is NULL, causes the CI class XSQL_PROFILE to be scanned,
     depth first and binding action performed at each record as indicated
     there. If ciName is not NULL, then it specifies a ciName that is to
     have it's values bound.

   int classFromName(CI_NAME *parsedName )

     Get numerical class from parsed form.

   CI_NAME * ciNameFromString(char *ciName)

     Get parsed form of name.

   XSQL_RECORD * dfsCIClass(XSQL_RECORD *search,CI_NAME *parsedName)

     Starting at beginning of class specfied by recordType in search, does
     scan of a class graph of RECORDS. Any records  on disk in the search
     path are made present. Descent follows established ordering of graph. 

     If parsedName is not NULL, then DFS is unnecessary, and the function
     returns when it reaches the node which determines the relation of the
     graph to the name (one that can be evaluated by name2NodeRelation).
     In this case the search is NULL.

     If parsedName is NULL, search is to scan a class graph and a DFS of the
     graph is conducted starting at the class root which is indicated using
     the initial search argument. Simply visits each record once using DFS,
     one node per call. The next node to be written is returned as value until
     the graph is traversed when NULL becomes the value. Argument search is
     an XSQL_RECORD initialized to zero and which has it's xsqlType set to
     the class to be scanned. 
 
   XSQL_RECORD * fileCI2Memory(XSQL_RECORD_HEADER *fileCI,FILE *opened,
                           void *node,int modeIsClass)

     Convert CI record from file to live form.

   freeCIname(CI_NAME *lost)

     Loose a CI record.

   void * nthAtom( CI_HEADER *theCI, unsigned n )
 
     Returns the nth ( 1 based ) atom from a non-atomic CI. If n is outside
     the existing range or theCI is not an array, returns NULL. If theCI
     has a non-string base type, returns the value not a reference to the
     value.

   CI_HEADER * nextLeaf(XSQL_RECORD *container,CI_HEADER **last)

     This function returns the address of the next leaf CI in an XSQL_RECORD.
     The first call should use the CI_HEADER obtained from the namedCI
     used to find the record. Subsequent calls return the non-record CI's.
     When the last in the record has been retrieved, subsequent calls return
     NULL. If the record has no field level CIs, NULL is returned on the
     first call.

   XSQL_RECORD * isInstance( CI_NAME wouldBe )

     Check name wouldBe's leading qualification to see if a ' DATA' record
     is defined at same level. Return record if found, NULL otherwise.

   int xType2 loadProfile()

     Reads the class table and initializes for further access to individual
     records. Returns OK/NOT_OK. If no file specified, initializes
     profile structures and returns NOT_OK.

   XSQL_RECORD_HEADER *memoryCI2File(XSQL_RECORD *memoryCI)

     Convert CI record from live to file form.

   int makePresent(XSQL_RECORD *thisRecord,FILE *opened)

     Causes a CI record to be made live.

   CI_HEADER *namedCI(char *ciName,XSQL_RECORD **x)

     Per-CI store/retrieve API.

     CI's are identified by class and name. Within a CI class, CI graphs
     are formed based on a fully qualified CI name with members seperated
     by '.'. This hierarchial organization gives rise to "levels" or
     subtypes of the basic classes, called name regions. 
     
     The second argument x is optionally supplied by the application and
     upon successful completion of the function can point to the containing
     XSQL_RECORD record of the CI.

     If x is NULL the function will find the CI and return a pointer to it,
     otherwise it returns NULL. In that case if the CI name refers to a
     leaf the leaf will always be the returned CI. If x refers to NULL the
     same thing happens but x is made to refer to the containing XSQL_RECORD.
     In that case, the function will only return a NULL if the containing
     record could not be created, and if a leaf of the same name exists,
     it will NOT be the returned CI. x can refer to an XSQL_RECORD in which
     case, the call is taken as a request to add a named leaf CI to a record.
     Any other use of x is invalid.

     If the named CI is level 1, i.e. a class and x points to NULL, then
     the entire class is made present, and X points to it's root.

     If x refers to NULL and the ciName is not found but a record format
     (' DATA') record is found in the same name region, an instance of 
     that format is created and bound to ciName.

     Applications can use the record or the CI pointer to access CI's.
     Modified CI's are saved when the entire profile is.

   int name2NodeRelation(XSQL_RECORD *node,CI_NAME *parsedName)

     Determines graph relation between two nodes in the profile.

   int xType2 storeProfile()

     Causes the entire profile to be rewritten. Records may be added
     or deleted and offsets changed. Writes to temporary file then
     backs up old profile, if any. Returns OK/NOT_OK.


 HISTORY:

  12/07/93 - In-line design begun. 
  12/09/93 - First In-line design and rough coding complete.
  12/10/93 - Realized DFS inappropriate to name space scheme.
             Changed to keep graphs in lexicograhic order both in memory
             and file forms.
  12/10/93 - In-line design complete.
  12/14/93 - Redesigned external protocol, refined internal.
  01/12/94 - Second level effort, adding field level processing.
  05/08/94 - Resumed after clear bugs in base mechanism. Closure of 
             scheme with quasi GUI independent processing in XBC.
  06/02/94 - Instantiation logic with i/o coded.
  06/09/94 - Operation with XSQL for OS/2 verified.
  01/08/95 - Converted to 32-bit.
  01/27/95 - Final major feature, binding of profile variables to
             variables in live environment.
  02/01/96 - Resume completion of profile debugging. Mechanism
             will operate as required X property management under Unix.
             A common file format will be maintained and profiles will
             be portable across platforms.
  11/11/98 - Windows version initiated for real. Windows version will 
             retain concept above modified as follows:

               - C++ maps are used over the name strings. C++
                 code lives in profile.cpp.
            
               - File I/O is eliminated, MFC serialization functions
                 invoked from profile.cpp.

*/

