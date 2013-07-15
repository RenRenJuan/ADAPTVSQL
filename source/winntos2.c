#define  INCL16_DOS
#include <xsqlc.h>
#include <286defs.h>
#include <286base.h>
                                                 
 USHORT DosCSemClear(void *s)
 {
  USHORT value; // = DOS16SEMCLEAR(s);

   return value;
 };
 USHORT DosCSemRequest(void *s,ULONG t)
 {                             
  USHORT value;// =DOS16SEMREQUEST(s,t);  

   return value;
 }             
 USHORT DosCSemSet(void *s)
 {                             
  USHORT value; // =DOS16SEMSET(s);  

   return value;
 }             
 USHORT DosCMakeNmPipe(void *pszName,
        void *php, USHORT fsOpenMode, USHORT fsPipeMode, USHORT cbOutBuf,
        USHORT cbInBuf, ULONG ulTimeOut)
 {
  USHORT value;   // = DOS16MAKENMPIPE(pszName,php,fsOpenMode,fsPipeMode,
                  //             cbOutBuf,cbInBuf,ulTimeOut);


   return value;
 }
 USHORT DosCConnectNmPipe(HPIPE hp)
 {
  USHORT value; // =DOS16CONNECTNMPIPE(hp);

   return value;
 }
 USHORT DosCDisconnectNmPipe(HPIPE hp)
 {
  USHORT value; // =DOS16DISCONNECTNMPIPE(hp);

   return value;
 };

 USHORT DosCLoadModule(char *pszFailName, short cbFileName,
                       char *pszModName, void *phmod)
 {
  USHORT value; // =DOS16LOADMODULE(pszFailName,cbFileName,pszModName,phmod);

   return value;
 };
 USHORT DosCGetProcAddr(HMODULE hmod, PSZ pszProcName,
			       PPFN ppfnProcAddr)
 {
  USHORT value; // =DOS16GETPROCADDR(hmod,pszProcName,ppfnProcAddr);

   return value;
 }
 USHORT DosGetNamedSharedMem(void **gotten,char * name,unsigned long flags)
 {
 }
 USHORT DosAllocSharedMem(void **gotten,char *name,unsigned long size,unsigned flags)
 {
 }
 USHORT DosQueryProcAddr(HMODULE hmod,unsigned long ordinal,char * name,void *pF)
 {
 }
 USHORT DosLoadModule(char *name, unsigned long nameSize,char *moduleName,void *module)
 {
 }
 USHORT DosWrite(int hFile,void *buffer,unsigned long writeN,unsigned long *wroteN)
 {
 }
 USHORT paktoasc()
 {
 }
 void checkTheHeap(char *where)
 {
 }





