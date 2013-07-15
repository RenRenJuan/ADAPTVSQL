#define  INCL16_DOS
#include <286defs.h>
#include <286base.h>
                                                 
 USHORT DosCSemClear(void *s)
 {
  USHORT value = DOS16SEMCLEAR(s);

   return value;
 };
 USHORT DosCSemRequest(void *s,ULONG t)
 {                             
  USHORT value=DOS16SEMREQUEST(s,t);  

   return value;
 }             
 USHORT DosCSemSet(void *s)
 {                             
  USHORT value=DOS16SEMSET(s);  

   return value;
 }             

 USHORT DosCMakeNmPipe(void *pszName,
        void *php, USHORT fsOpenMode, USHORT fsPipeMode, USHORT cbOutBuf,
        USHORT cbInBuf, ULONG ulTimeOut)
 {
  USHORT value= DOS16MAKENMPIPE(pszName,php,fsOpenMode,fsPipeMode,
                                cbOutBuf,cbInBuf,ulTimeOut);


   return value;
 }
 USHORT DosCConnectNmPipe(HPIPE hp)
 {
  USHORT value=DOS16CONNECTNMPIPE(hp);

   return value;
 }
 USHORT DosCDisconnectNmPipe(HPIPE hp)
 {
  USHORT value=DOS16DISCONNECTNMPIPE(hp);

   return value;
 };

 USHORT DosCLoadModule(char *pszFailName, short cbFileName,
                       char *pszModName, void *phmod)
 {
  USHORT value=DOS16LOADMODULE(pszFailName,cbFileName,pszModName,phmod);

   return value;
 };
 USHORT DosCGetProcAddr(HMODULE hmod, PSZ pszProcName,
			       PPFN ppfnProcAddr)
 {
  USHORT value=DOS16GETPROCADDR(hmod,pszProcName,ppfnProcAddr);

   return value;
 };
