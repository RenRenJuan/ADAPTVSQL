/***************************************************************************\
* 286BASE.h - Modified from:
* Module Name: BSEDOS.H
*
* OS/2 Base Include File
* =======================================================================
*
*   #define:		To include:
*
*  INCL16_DOSPROCESS	    Process and thread support
*  INCL16_DOSINFOSEG	    InfoSeg support
*  INCL16_DOSFILEMGR 	 File Management
*  INCL16_DOSMEMMGR	    Memory Management
*  INCL16_DOSSEMAPHORES  Semaphore support
*  INCL16_DOSDATETIME	 Date/Time and Timer support
*  INCL16_DOSMODULEMGR	 Module manager
*  INCL16_DOSRESOURCES	 Resource support
*  INCL16_DOSNLS 	       National Language Support
*  INCL16_DOSSIGNALS	    Signals
*  INCL16_DOSMISC	       Miscellaneous
*  INCL16_DOSMONITORS	 Monitors
*  INCL16_DOSQUEUES	    Queues
*  INCL16_DOSSESMGR	    Session Manager Support
*  INCL16_DOSDEVICES	    Device specific, ring 2 support
*  INCL16_DOSNMPIPES	    Named Pipes Support
*  INCL16_DOSTRACE  	    Trace support
*  INCL16_DOS            All of Above
*
\***************************************************************************/

#define INCL16_DOSINCLUDED

#ifdef INCL16_DOS

#define INCL16_DOSPROCESS
#define INCL16_DOSINFOSEG
#define INCL16_DOSFILEMGR
#define INCL16_DOSMEMMGR
#define INCL16_DOSSEMAPHORES
#define INCL16_DOSDATETIME
#define INCL16_DOSMODULEMGR
#define INCL16_DOSRESOURCES
#define INCL16_DOSNLS
#define INCL16_DOSSIGNALS
#define INCL16_DOSMISC
#define INCL16_DOSMONITORS
#define INCL16_DOSQUEUES
#define INCL16_DOSSESMGR
#define INCL16_DOSDEVICES
#define INCL16_DOSNMPIPES
#define INCL16_DOSTRACE

#endif /* INCL16_DOS */

#ifdef INCL16_ERRORS
#define INCL16_DOSERRORS
#endif /* INCL16_ERRORS */

#if (defined(INCL16_DOSPROCESS) || !defined(INCL16_NOCOMMON))

/*** General services */

USHORT APIENTRY DOS16BEEP(USHORT usFrequency, USHORT usDuration);

/*** Process and Thread support */

VOID APIENTRY DOS16EXIT(USHORT fTerminate, USHORT usExitCode);

/* Dos16Exit fTerminate */
#define EXIT_THREAD		0
#define EXIT_PROCESS		1

#endif /* common INCL16_DOSPROCESS definitions */

#if (defined(INCL16_DOSPROCESS) && !defined(INCL16_DOSPROCESS_INCLUDE))
#define INCL16_DOSPROCESS_INCLUDE

typedef struct _PIDINFO {	/* pidi */
	PID pid;
	TID tid;
	PID pidParent;
} PIDINFO;
typedef PIDINFO * FAR PPIDINFO;

typedef VOID (* FAR PFNTHREAD)(VOID);

USHORT APIENTRY DOS16CREATETHREAD(PFNTHREAD pfnFun, PTID pTid, PBYTE pbStack);
USHORT APIENTRY DOS16RESUMETHREAD(TID tid);
USHORT APIENTRY DOS16SUSPENDTHREAD(TID tid);

/* Dos16Cwait fScope */
#define DCWA_PROCESS		0
#define DCWA_PROCESSTREE	1

/* Dos16Cwait ffWait */
#define DCWW_WAIT		0
#define DCWW_NOWAIT		1

typedef struct _RESULTCODES {	/* resc */
	USHORT codeTerminate;
	USHORT codeResult;
} RESULTCODES;
typedef RESULTCODES * FAR  *PRESULTCODES;

USHORT APIENTRY DOS16CWAIT(USHORT fScope, USHORT fWait, PRESULTCODES prescResults,
			 PPID ppidProcess, PID pidWaitProcess);
USHORT APIENTRY DOS16SLEEP(unsigned long ulTime);

/* codeTerminate values (also passed to ExitList routines) */
#define TC_EXIT 	0
#define TC_HARDERROR	1
#define TC_TRAP 	2
#define TC_KILLPROCESS	3

typedef VOID (* _Far16 PASCAL PFNEXITLIST)(USHORT);

USHORT APIENTRY DOS16ENTERCRITSEC(VOID);
USHORT APIENTRY DOS16EXITCRITSEC(VOID);
USHORT APIENTRY DOS16EXITLIST(USHORT fFnCode, PFNEXITLIST pfnFunction);

/* Dos16ExitList fFnCode	*/
#define EXLST_ADD	1
#define EXLST_REMOVE	2
#define EXLST_EXIT	3

USHORT APIENTRY DOS16EXECPGM(PCHAR pchFailName, SHORT cbFailName,
			   USHORT fExecFlags, PSZ pszArgs, PSZ pszEnv,
			   PRESULTCODES prescResults, PSZ pszPgmName);

/* Dos16ExecPgm fExecFlags */
#define EXEC_SYNC		0
#define EXEC_ASYNC		1
#define EXEC_ASYNCRESULT	2
#define EXEC_TRACE		3
#define EXEC_BACKGROUND 	4

USHORT APIENTRY DOS16GETPID(PPIDINFO ppidInfo);
USHORT APIENTRY DOS16GETPPID(unsigned short pidChild, unsigned short * ppidParent);

USHORT APIENTRY DOS16GETPRTY(unsigned short  usScope, unsigned short * pusPriority, USHORT pid);
USHORT APIENTRY DOS16SETPRTY(unsigned short  usScope, unsigned short  fPrtyClass, SHORT sChange,
			   USHORT id);

/* Dos16GetPrty/Dos16SetPrty usScope */
#define PRTYS_PROCESS		0
#define PRTYS_PROCESSTREE	1
#define PRTYS_THREAD		2

/* Dos16GetPrty/Dos16SetPrty priority classes */
#define PRTYC_NOCHANGE		0
#define PRTYC_IDLETIME		1
#define PRTYC_REGULAR		2
#define PRTYC_TIMECRITICAL	3
#define PRTYC_FOREGROUNDSERVER	4

/* Dos16SetPrty priority deltas (sChange) */
#define PRTYD_MINIMUM		(-31)
#define PRTYD_MAXIMUM		31

USHORT APIENTRY Dos16KillProcess(USHORT usScope, PID pidProcess);

/* Dos16KillProcess usScope */
#define DKP_PROCESSTREE 	0
#define DKP_PROCESS		1

#endif /* INCL16_DOSPROCESS */


/*** InfoSeg support */

#if (defined(INCL16_DOSINFOSEG) && !defined(INCL16_DOSINFOSEG_INCLUDE))
#define INCL16_DOSINFOSEG_INCLUDE

/* Global Information Segment */

typedef struct _GINFOSEG {	/* gis */
	ULONG	time;
	ULONG	msecs;
	UCHAR	hour;
	UCHAR	minutes;
	UCHAR	seconds;
	UCHAR	hundredths;
	USHORT	timezone;
	USHORT	cusecTimerInterval;
	UCHAR	day;
	UCHAR	month;
	USHORT	year;
	UCHAR	weekday;
	UCHAR	uchMajorVersion;
	UCHAR	uchMinorVersion;
	UCHAR	chRevisionLetter;
	UCHAR	sgCurrent;
	UCHAR	sgMax;
	UCHAR	cHugeShift;
	UCHAR	fProtectModeOnly;
	USHORT	pidForeground;
	UCHAR	fDynamicSched;
	UCHAR	csecMaxWait;
	USHORT	cmsecMinSlice;
	USHORT	cmsecMaxSlice;
	USHORT	bootdrive;
	UCHAR	amecRAS[32];
	UCHAR	csgWindowableVioMax;
	UCHAR	csgPMMax;
} GINFOSEG;
typedef GINFOSEG * FAR PGINFOSEG;

/* Local Information Segment */

typedef struct _LINFOSEG {	/* lis */
	PID	pidCurrent;
	PID	pidParent;
	USHORT	prtyCurrent;
	TID	tidCurrent;
	USHORT	sgCurrent;
	UCHAR	rfProcStatus;
	UCHAR	dummy1;
	BOOL	fForeground;
	UCHAR	typeProcess;
	UCHAR	dummy2;
	SEL	selEnvironment;
	USHORT	offCmdLine;
	USHORT	cbDataSegment;
	USHORT	cbStack;
	USHORT	cbHeap;
	HMODULE hmod;
	SEL	selDS;
} LINFOSEG;
typedef LINFOSEG * FAR PLINFOSEG;

/* Process Type codes (local information segment typeProcess field)	*/

#define PT_FULLSCREEN		0 /* Full screen application		*/
#define PT_REALMODE		1 /* Real mode process			*/
#define PT_WINDOWABLEVIO	2 /* VIO windowable application 	*/
#define PT_PM			3 /* Presentation Manager application	*/
#define PT_DETACHED		4 /* Detached application		*/

/* Process Status Flag definitions (local info seg rfProcStatus field)	*/

#define PS_EXITLIST		1 /* Thread is in exitlist routine	*/


USHORT APIENTRY DOS16GETINFOSEG(PSEL pselGlobal, PSEL pselLocal);

/* Helper macros used to convert selector to PINFOSEG or LINFOSEG	*/

#define MAKEPGINFOSEG(sel)  ((PGINFOSEG)MAKEP(sel, 0))
#define MAKEPLINFOSEG(sel)  ((PLINFOSEG)MAKEP(sel, 0))

#endif /* INCL16_DOSINFOSEG */

#ifndef INCL16_SAADEFS

/*
 * CCHMAXPATH is the maximum fully qualified path name length including
 * the drive letter, colon, backslashes and terminating NULL.
 */
#define CCHMAXPATH	260

/*
 * CCHMAXPATHCOMP is the maximum individual path component name length
 * including a terminating NULL.
 */
#define CCHMAXPATHCOMP	256

#endif  /* !INCL16_SAADEFS */

#if ((defined(INCL16_DOSFILEMGR) || !defined(INCL16_NOCOMMON)) && !defined(INCL16_DOSFILEMGR_INCLUDE))
#define INCL16_DOSFILEMGR_INCLUDE

/*** File manager */

/* Dos16ChgFilePtr() file position codes */

#define FILE_BEGIN		0x0000 /* relative to beginning of file    */
#define FILE_CURRENT		0x0001 /* relative to current fptr position*/
#define FILE_END		0x0002 /* relative to end of file	   */

/* Dos16FindFirst/Next Directory handle types */

#define HDIR_SYSTEM		0x0001	/* Use system handle (1)	   */
#define HDIR_CREATE		0xFFFF	/* Allocate a new, unused handle   */

/* Dos16Copy control bits - may be or'ed together */
#define DCPY_EXISTING		0x00001 /* Copy even if target exists	   */
#define DCPY_APPEND		0x00002 /* Append to existing file, don't replace*/

/* Dos16open/Dos16QFHandState/Dos16QueryFileInfo et al file attributes; also        */
/* known as Dso File Mode bits...                                             */
#define FILE_NORMAL		0x0000
#define FILE_READONLY		0x0001
#define FILE_HIDDEN		0x0002
#define FILE_SYSTEM		0x0004
#define FILE_DIRECTORY		0x0010
#define FILE_ARCHIVED		0x0020

/* Dos16Open() actions */
#define FILE_EXISTED		0x0001
#define FILE_CREATED		0x0002
#define FILE_TRUNCATED		0x0003

/* Dos16Open() open flags */
#define FILE_OPEN		0x0001
#define FILE_TRUNCATE		0x0002
#define FILE_CREATE		0x0010

/* applies if file already exists */

#define OPEN_ACTION_FAIL_IF_EXISTS     0x0000  /* ---- ---- ---- 0000	*/
#define OPEN_ACTION_OPEN_IF_EXISTS     0x0001  /* ---- ---- ---- 0001	*/
#define OPEN_ACTION_REPLACE_IF_EXISTS  0x0002  /* ---- ---- ---- 0010	*/

/* applies if file does not exist */

#define OPEN_ACTION_FAIL_IF_NEW        0x0000  /* ---- ---- 0000 ----	*/
#define OPEN_ACTION_CREATE_IF_NEW      0x0010  /* ---- ---- 0001 ----	*/

/* Dos16Open/Dos16SetFHandState flags */

#define OPEN_ACCESS_READONLY		0x0000	/* ---- ---- ---- -000	*/
#define OPEN_ACCESS_WRITEONLY		0x0001	/* ---- ---- ---- -001	*/
#define OPEN_ACCESS_READWRITE		0x0002	/* ---- ---- ---- -010	*/
#define OPEN_SHARE_DENYREADWRITE	0x0010	/* ---- ---- -001 ----	*/
#define OPEN_SHARE_DENYWRITE		0x0020	/* ---- ---- -010 ----	*/
#define OPEN_SHARE_DENYREAD		0x0030	/* ---- ---- -011 ----	*/
#define OPEN_SHARE_DENYNONE		0x0040	/* ---- ---- -100 ----	*/
#define OPEN_FLAGS_NOINHERIT		0x0080	/* ---- ---- 1--- ----	*/
#define OPEN_FLAGS_NO_LOCALITY		0x0000	/* ---- -000 ---- ----	*/
#define OPEN_FLAGS_SEQUENTIAL		0x0100	/* ---- -001 ---- ----	*/
#define OPEN_FLAGS_RANDOM		0x0200	/* ---- -010 ---- ----	*/
#define OPEN_FLAGS_RANDOMSEQUENTIAL	0x0300	/* ---- -011 ---- ----	*/
#define OPEN_FLAGS_NO_CACHE		0x1000	/* ---1 ---- ---- ----	*/
#define OPEN_FLAGS_FAIL_ON_ERROR	0x2000	/* --1- ---- ---- ----	*/
#define OPEN_FLAGS_WRITE_THROUGH	0x4000	/* -1-- ---- ---- ----	*/
#define OPEN_FLAGS_DASD 		0x8000	/* 1--- ---- ---- ----	*/


/* Dos16SearchPath() constants from 1.1 (backword compatability) */

#define SEARCH_PATH		0x0000
#define SEARCH_CUR_DIRECTORY	0x0001
#define SEARCH_ENVIRONMENT	0x0002
#define SEARCH_IGNORENETERRS	0x0004

/* Dos16SearchPath() constants for 1.2 and later */
#define DSP_PATH		0 /* path is specified in parameter	*/
#define DSP_CUR_DIRECTORY	1 /* current dir will be searched first */
#define DSP_ENVIRONMENT 	2 /* from envirnoment variable		*/
#define DSP_IGNORENETERR	4 /* ignore net errs & continue search	*/

USHORT APIENTRY Dos16SearchPath(USHORT fsSearch, PSZ pszPath, PSZ pszFName,
			      PBYTE pBuf, USHORT cbBuf);

/*
 * Dos16FileIO
 */
/* File IO command words */
#define FIO_LOCK		0	/* Lock Files			*/
#define FIO_UNLOCK		1	/* Unlock Files 		*/
#define FIO_SEEK		2	/* Seek (set file ptr)		*/
#define FIO_READ		3	/* File Read			*/
#define FIO_WRITE		4	/* File Write			*/

/* Lock Sharing Modes */
#define FIO_NOSHARE		0	/* None 			*/
#define FIO_SHAREREAD		1	/* Read-Only			*/

typedef struct	_FIOLOCKCMD {	/* FLC	FileLockCmd prefix	*/
	USHORT	usCmd;		/* Cmd = FIO_LOCK		*/
	USHORT	cLockCnt;	/* Lock records that follow	*/
	ULONG	cTimeOut;	/* in Msec			*/
} FIOLOCKCMD;
typedef FIOLOCKCMD * FAR  PFIOLOCKCMD;

typedef struct	_FIOLOCKREC {	/* FLR FileLockRecord		   */
	USHORT fShare;		/* FIO_NOSHARE or FIO_SHAREREAD    */
	ULONG  cbStart; 	/* Starting offset for lock region */
	ULONG  cbLength;	/* Length of lock region	   */
} FIOLOCKREC;
typedef FIOLOCKREC * FAR  PFIOLOCKREC;

typedef struct	_FIOUNLOCKCMD { /* FUC FileUnlockCmd prefix	*/
	USHORT	usCmd;		/* Cmd = FIO_UNLOCK		*/
	USHORT	cUnlockCnt;	/* Unlock records that follow	*/
} FIOUNLOCKCMD;
typedef FIOUNLOCKCMD * FAR  PFIOUNLOCKCMD;

typedef struct	_FIOUNLOCKREC { /* FUR FileUnlockRecord 	     */
	ULONG	cbStart;	/* Starting offset for unlock region */
	ULONG	cbLength;	/* Length of unlock region	     */
} FIOUNLOCKREC;
typedef FIOUNLOCKREC * FAR  PFIOUNLOCKREC;

typedef struct	_FIOSEEKCMD {	/* FSC Seek command structure		  */
	USHORT	usCmd;		/* Cmd = FIO_SEEK			  */
	USHORT	fsMethod;	/* One of&gml FPM_BEGINNING, FPM_CURRENT, */
				/* or FPM_END				  */
	ULONG	cbDistance;	/* Byte offset for seek 		  */
	ULONG	cbNewPosition;	/* Bytes from start of file after seek	  */
} FIOSEEKCMD;
typedef FIOSEEKCMD * FAR  PFIOSEEKCMD;

typedef struct	_FIOREADWRITE { /* FRWC Read&Write command structure	 */
	USHORT	usCmd;		/* Cmd = FIO_READ or FIO_WRITE		*/
	PVOID	pbBuffer;	/* Pointer to data buffer		*/
	USHORT	cbBufferLen;	/* Bytes in buffer or max size		*/
	USHORT	cbActualLen;	/* Bytes actually read/written		*/
} FIOREADWRITE;
typedef FIOREADWRITE * FAR  PFIOREADWRITE;

/***
 * EA Info Levels & Find First/Next
 * API's: Dos16FindFirst, Dos16QueryFileInfo, Dos16QueryPathInfo, Dos16SetFileInfo,
 *      Dos16SetPathInfo
 */

/* File info levels : All listed API's */
#define FIL_STANDARD		1   /* Info level 1, standard file info */
#define FIL_QUERYEASIZE 	2   /* Level 2, return Full EA size	*/
#define FIL_QUERYEASFROMLIST	3   /* Level 3, return requested EA's   */

/* File info levels: Dos16...PathInfo only */
#define FIL_QUERYFULLNAME	5   /* Level 5, return fully qualified	*/
				    /*	 name of file			*/
#define FIL_NAMEISVALID 	6   /* Level 6, check validity of	*/
				    /* file/path name for this FSD	*/

/* Dos16FindNotifyFirst() */
#define FNOTIL_STANDARD 	1   /* Find-Notify Info level 1&gml Return */
				    /* standard directory change info	   */

/* Dos16FsAttach() */
/* Attact or detach */
#define FSATTACH		0	/* Attach file server	*/
#define FSDETACH		1	/* Detach file server	*/

/* Dos16FsCtl() */
/* Routing type */
#define FSCTL_HANDLE		1	/* File Handle directs req routing */
#define FSCTL_PATHNAME		2	/* Path Name directs req routing   */
#define FSCTL_FSDNAME		3	/* FSD Name directs req routing    */

/* Dos16QueryFSAttach() */
/* Information level types (defines method of query) */
#define FSAIL_QUERYNAME 	1	/* Return data for a Drive or Device */
#define FSAIL_DEVNUMBER 	2	/* Return data for Ordinal Device #  */
#define FSAIL_DRVNUMBER 	3	/* Return data for Ordinal Drive #   */

/* Item types (from data structure item "iType") */
#define FSAT_CHARDEV		1	/* Resident character device	*/
#define FSAT_PSEUDODEV		2	/* Pusedu-character device	*/
#define FSAT_LOCALDRV		3	/* Local drive			*/
#define FSAT_REMOTEDRV		4	/* Remote drive attached to FSD */

/* Data structure for QFSAttach       */
typedef struct	_FSQBUFFER {	/* fsqbf */
	USHORT	iType;		/* Item type			      */
	USHORT	cbName; 	/* Length of item name, sans NULL     */
	UCHAR	szName[1];	/* ASCIIZ item name		      */
	USHORT	cbFSDName;	/* Length of FSD name, sans NULL      */
	UCHAR	szFSDName[1];	/* ASCIIZ FSD name		      */
	USHORT	cbFSAData;	/* Length of FSD Attach data returned */
	UCHAR	rgFSAData[1];	/* FSD Attach data from FSD	      */
} FSQBUFFER;
typedef FSQBUFFER * FAR  PFSQBUFFER;

/*
 * File System Drive Information&gml Dos16QFSInfo Dos16SetFSInfo
 */

/* FS Drive Info Levels 						*/
#define FSIL_ALLOC		1 /* Drive allocation info (Query only) */
#define FSIL_VOLSER		2 /* Drive Volume/Serial information	*/

/* HANDTYPE values */

#define HANDTYPE_FILE		0x0000
#define HANDTYPE_DEVICE 	0x0001
#define HANDTYPE_PIPE		0x0002
#define HANDTYPE_NETWORK	0x8000

/* Dos16QHandType() */
/* Handle classes (low 8 bits of Handle Type)				*/
#define FHT_DISKFILE		0x0000	/* Disk file handle		*/
#define FHT_CHRDEV		0x0001	/* Character device handle	*/
#define FHT_PIPE		0x0002	/* Pipe handle			*/

/* Handle bits (high 8 bits of Handle Type)				*/
#define FHB_DSKREMOTE		0x8000	/* Remote disk			*/
#define FHB_CHRDEVREMOTE	0x8001	/* Remote character device	*/
#define FHB_PIPEREMOTE		0x8002	/* Remote pipe			*/


#ifndef INCL16_SAADEFS

/* File time and date types */

typedef struct _FTIME { 	/* ftime */
	unsigned twosecs : 5;
	unsigned minutes : 6;
	unsigned hours	 : 5;
} FTIME;
typedef FTIME * FAR PFTIME;

typedef struct _FDATE { 	/* fdate */
	unsigned day	 : 5;
	unsigned month	 : 4;
	unsigned year	 : 7;
} FDATE;
typedef FDATE * FAR PFDATE;

typedef struct _FILEFINDBUF {	/* findbuf */
	FDATE	fdateCreation;
	FTIME	ftimeCreation;
	FDATE	fdateLastAccess;
	FTIME	ftimeLastAccess;
	FDATE	fdateLastWrite;
	FTIME	ftimeLastWrite;
	ULONG	cbFile;
	ULONG	cbFileAlloc;
	USHORT	attrFile;
	UCHAR	cchName;
	CHAR	achName[CCHMAXPATHCOMP];
} FILEFINDBUF;
typedef FILEFINDBUF * FAR PFILEFINDBUF;

typedef struct _FILEFINDBUF2 {	/* findbuf2 */
	FDATE	fdateCreation;
	FTIME	ftimeCreation;
	FDATE	fdateLastAccess;
	FTIME	ftimeLastAccess;
	FDATE	fdateLastWrite;
	FTIME	ftimeLastWrite;
	ULONG	cbFile;
	ULONG	cbFileAlloc;
	USHORT	attrFile;
	ULONG	cbList;
	UCHAR	cchName;
	CHAR	achName[CCHMAXPATHCOMP];
} FILEFINDBUF2;
typedef FILEFINDBUF2 * FAR PFILEFINDBUF2;

/* extended attribute structures */
typedef struct _GEA {		/* gea */
	BYTE	cbName; 	/* name length not including NULL */
	CHAR	szName[1];	/* attribute name		  */
} GEA;
typedef GEA * FAR PGEA;

typedef struct _GEALIST {	/* geal */
	ULONG  cbList;		/* total bytes of structure inc full list */
	GEA    list[1]; 	/* variable length GEA structures	  */
} GEALIST;
typedef GEALIST * FAR PGEALIST;

typedef struct _FEA {		/* fea */
	BYTE	fEA;		/* flags				*/
	BYTE	cbName; 	/* name length not including NULL	*/
	USHORT	cbValue;	/* value length 			*/
} FEA;
typedef FEA * FAR PFEA;

/* flags for _FEA.fEA */

#define FEA_NEEDEA 0x80 	/* need EA bit */

typedef struct _FEALIST {	/* feal */
	ULONG  cbList;		/* total bytes of structure inc full list */
	FEA    list[1]; 	/* variable length FEA structures	  */
} FEALIST;
typedef FEALIST * FAR PFEALIST;

typedef struct _EAOP {		/* eaop */
	PGEALIST fpGEAList;	/* general EA list */
	PFEALIST fpFEAList;	/* full EA list    */
	ULONG	 oError;
} EAOP;
typedef EAOP * FAR  PEAOP;


/*
 * Equates for EA types
 *
 * Values 0xFFFE thru 0x8000 are reserved.
 * Values 0x0000 thru 0x7fff are user definable.
 * Value  0xFFFC is not used
 */

#define EAT_BINARY	0xFFFE /* length preceeded binary	   */
#define EAT_ASCII	0xFFFD /* length preceeded ASCII	   */
#define EAT_BITMAP	0xFFFB /* length preceeded bitmap	   */
#define EAT_METAFILE	0xFFFA /* length preceeded metafile	   */
#define EAT_ICON	0xFFF9 /* length preceeded icon 	   */
#define EAT_EA		0xFFEE /* length preceeded ASCII extended attribute */
			       /* name of associated data (#include)*/
#define EAT_MVMT	0xFFDF /* multi-valued, multi-typed field  */
#define EAT_MVST	0xFFDE /* multi-valued, single-typed field */
#define EAT_ASN1	0xFFDD /* ASN.1 field			   */


#endif  /* !INCL16_SAADEFS */

USHORT APIENTRY DOS16OPEN(PSZ pszFname, PHFILE phfOpen, PUSHORT pusAction,
			ULONG ulFSize, USHORT usAttr, USHORT fsOpenFlags,
			USHORT fsOpenMode, ULONG ulReserved);
USHORT APIENTRY DOS16OPEN2(PSZ pszFname, PHFILE phf, PUSHORT pusAction,
			 ULONG ulFSize, USHORT usAttr, USHORT usOpenFlags,
			 ULONG usOpenMode, PEAOP pvEABuf, ULONG ulReserved);
USHORT APIENTRY DOS16CLOSE(HFILE hf);
USHORT APIENTRY DOS16READ(HFILE hf, PVOID pBuf, USHORT cbBuf,
			PUSHORT pcbBytesRead);
USHORT APIENTRY DOS16WRITE(HFILE hf, PVOID bBuf, USHORT cbBuf,
			 PUSHORT pcbBytesWritten);
USHORT APIENTRY DOS16OPLOCKRELEASE(ULONG cookie, USHORT procBlkKey);
USHORT APIENTRY DOS16OPLOCKWAIT(PULONG pcookie, PULONG procBlkKey);

/* File system shutdown */

USHORT APIENTRY DOS16SHUTDOWN(ULONG ulReserved);

/* File time and date types */

typedef struct _FILESTATUS {	/* fsts */
	FDATE	fdateCreation;
	FTIME	ftimeCreation;
	FDATE	fdateLastAccess;
	FTIME	ftimeLastAccess;
	FDATE	fdateLastWrite;
	FTIME	ftimeLastWrite;
	ULONG	cbFile;
	ULONG	cbFileAlloc;
	USHORT	attrFile;
} FILESTATUS;
typedef FILESTATUS * FAR     PFILESTATUS;

typedef struct _FILESTATUS2 {	 /* fsts2 */
	FDATE	fdateCreation;
	FTIME	ftimeCreation;
	FDATE	fdateLastAccess;
	FTIME	ftimeLastAccess;
	FDATE	fdateLastWrite;
	FTIME	ftimeLastWrite;
	ULONG	cbFile;
	ULONG	cbFileAlloc;
	USHORT	attrFile;
	ULONG	cbList;
} FILESTATUS2;
typedef FILESTATUS2 * FAR     PFILESTATUS2;

typedef struct _FSALLOCATE {	/* fsalloc */
	ULONG	ulReserved;
	ULONG	cSectorUnit;
	ULONG	cUnit;
	ULONG	cUnitAvail;
	USHORT	cbSector;
} FSALLOCATE;
typedef FSALLOCATE * FAR     PFSALLOCATE;

typedef struct _VOLUMELABEL {	/* vol */
	BYTE	cch;
	CHAR	szVolLabel[12];
} VOLUMELABEL;
typedef VOLUMELABEL * FAR     PVOLUMELABEL;

typedef struct _FSINFO {	/* fsinf */
	ULONG ulVSN;
	VOLUMELABEL vol;
} FSINFO;
typedef FSINFO * FAR     PFSINFO;

typedef struct _FILELOCK {	/* flock */
	LONG	lOffset;
	LONG	lRange;
} FILELOCK;
typedef FILELOCK * FAR     PFILELOCK;

typedef SHANDLE HDIR;		/* hdir */
typedef HDIR * FAR     PHDIR;

USHORT APIENTRY DOS16DELETE(PSZ pszFName, ULONG ulReserverd);
USHORT APIENTRY DOS16DUPHANDLE(HFILE hfOld, PHFILE phfNew);

USHORT APIENTRY DOS16QFHANDSTATE(HFILE hf, PUSHORT pfsOpenMode);
USHORT APIENTRY DOS16SETFHANDSTATE(HFILE hf, USHORT fsState);
USHORT APIENTRY DOS16QHANDTYPE(HFILE hf, PUSHORT pfsType, PUSHORT pusDevAttr);

USHORT APIENTRY DOS16READASYNC (HFILE hf, PULONG hsemRam, PUSHORT pusErrCode,
			      PVOID pBuf, USHORT cbBuf, PUSHORT pcbBytesRead);
USHORT APIENTRY DOS16WRITEASYNC(HFILE hf, PULONG hsemRam, PUSHORT pusErrCode,
			      PVOID pBuf, USHORT cbBuf, PUSHORT pcbBytesWritten);

USHORT APIENTRY DOS16FINDFIRST(PSZ pszFSpec, PHDIR phdir, USHORT usAttr,
			     PFILEFINDBUF pffb, USHORT cbBuf, PUSHORT pcSearch,
			     ULONG ulReserved);
USHORT APIENTRY DOS16FINDFIRST2(PSZ pszFSpec, PHDIR phdir, USHORT usAttr,
			      PVOID pBuf, USHORT cbBuf, PUSHORT pcSearch,
			      USHORT usInfoLevel, ULONG ulReserved);
USHORT APIENTRY DOS16FINDNEXT(HDIR hdir, PFILEFINDBUF pffb, USHORT cbBuf,
			    PUSHORT pcSearch);
USHORT APIENTRY DOS16FINDCLOSE(HDIR hdir);

USHORT APIENTRY DOS16FSATTACH(PSZ pszDevName, PSZ pszFSD, PBYTE pData,
			    USHORT cbData, USHORT fsOp, ULONG ulReserved);
USHORT APIENTRY DOS16QFSATTACH(PSZ pszDev, USHORT usOrdinal, USHORT usInfoLevel,
			     PBYTE pFSAttBuf, PUSHORT cbBuf, ULONG ulReserved);
USHORT APIENTRY DOS16FSCTL(PBYTE pData, USHORT cbData, PUSHORT pcbData,
			 PBYTE pParms, USHORT cbParms, PUSHORT pcbParms,
			 USHORT usFunCode, PSZ pszRoute, HFILE hf,
			 USHORT usRouteMethod, ULONG ulReserved);

USHORT APIENTRY DOS16NEWSIZE(HFILE hf, ULONG ulNewSize);
USHORT APIENTRY DOS16BUFRESET(HFILE hf);

USHORT APIENTRY DOS16CHGFILEPTR(HFILE hf, LONG lOffset, USHORT fsMethod,
			      PULONG pulNewOffset);

USHORT APIENTRY DOS16SETFILEPTR(HFILE hf, LONG lOffset, USHORT fsMethod,
			      PULONG pulNewOffset);

USHORT APIENTRY DOS16FILELOCKS(HFILE hf, PLONG pUnlock, PLONG pLock);

USHORT APIENTRY DOS16MOVE(PSZ pszOld, PSZ pszNew, ULONG ulReserved);
USHORT APIENTRY DOS16COPY(PSZ pszSrc, PSZ pszDst, USHORT usOpt, ULONG ulReserved);
USHORT APIENTRY DOS16EDITNAME(USHORT usEditLevel, PSZ pszSrc, PSZ pszEdit,
			    PBYTE pszDst, USHORT cbDst);

USHORT APIENTRY DOS16FILEIO(HFILE hf, PBYTE pbCmd, USHORT cbCmd, PUSHORT pulErr);
USHORT APIENTRY DOS16MKDIR(PSZ pszDirName, ULONG usReserved);
USHORT APIENTRY DOS16MKDIR2(PSZ pszDir, PEAOP pBuf, ULONG ulReserved);
USHORT APIENTRY DOS16RMDIR(PSZ pszDir, ULONG ulReserved);
USHORT APIENTRY DOS16SELECTDISK(USHORT usDrvNum);
USHORT APIENTRY DOS16QCURDISK(PUSHORT pusDrvNum, PULONG pulLogDrvMap);

USHORT APIENTRY DOS16CHDIR(PSZ pszDir, ULONG ulReserved);
USHORT APIENTRY DOS16QCURDIR(USHORT usDrvNum, PBYTE pszPathBuf,
			   PUSHORT pcbPathBuf);

USHORT APIENTRY DOS16QFSINFO(USHORT usDrvNum, USHORT usInfoLevel, PBYTE pbInfo,
			   USHORT cbInfo);
USHORT APIENTRY DOS16SETFSINFO(USHORT usDrvNum, USHORT usInfoLevel, PBYTE pBuf,
			     USHORT cbBuf);
USHORT APIENTRY DOS16QVERIFY(PBOOL pfVerifyOn);
USHORT APIENTRY DOS16SETVERIFY(USHORT fVerify);
USHORT APIENTRY DOS16SETMAXFH(USHORT usHandles);

USHORT APIENTRY DOS16QFILEINFO(HFILE hf, USHORT usInfoLevel, PVOID pInfoBuf,
			     USHORT cbInfoBuf);
USHORT APIENTRY DOS16SETFILEINFO(HFILE hf, USHORT usInfoLevel, PBYTE pInfoBuf,
			       USHORT cbInfoBuf);
USHORT APIENTRY DOS16QPATHINFO(PSZ pszPath, USHORT usInfoLevel, PBYTE pInfoBuf,
			     USHORT cbInfoBuf, ULONG ulReserved);
USHORT APIENTRY DOS16SETPATHINFO(PSZ pszPath, USHORT usInfoLevel, PBYTE pInfoBuf,
			       USHORT cbInfoBuf, USHORT usFlags,
			       ULONG ulReserved);

/* defines for Dos16SetPathInfo -fsOptions flag */
#define DSPI_WRTTHRU    0x10    /* write through */

USHORT APIENTRY DOS16QFILEMODE(PSZ pszFName, PUSHORT pusAttr, ULONG ulReserved);
USHORT APIENTRY DOS16SETFILEMODE(PSZ pszFName, USHORT usAttr, ULONG ulReserved);

/* constants for Dos16EnumAttribute() */
#define ENUMEA_REFTYPE_FHANDLE 0
#define ENUMEA_REFTYPE_PATH    1
#define ENUMEA_LEVEL_NO_VALUE  1L

USHORT APIENTRY DOS16ENUMATTRIBUTE(USHORT, PVOID, ULONG, PVOID, ULONG, PULONG,
				 ULONG, ULONG);
/* ENUMEA_LEVEL_NO_VALUE info returned from Dos16EnumAttribute */
typedef struct _DENA1 {    /* dena */
	UCHAR	reserved;  /* 0 					  */
	UCHAR	cbName;    /* length of name exculding NULL		  */
	USHORT	cbValue;   /* length of value				  */
	UCHAR	szName[1]; /* variable length asciiz name		  */
} DENA1;
typedef DENA1 * FAR  PDENA1;

#endif /* COMMON INCL16_DOSFILEMGR */

#if (defined(INCL16_DOSMEMMGR) || !defined(INCL16_NOCOMMON))
/*** Memory management */

USHORT APIENTRY DOS16ALLOCSEG(USHORT cbSize, PSEL pSel, USHORT fsAlloc);
USHORT APIENTRY DOS16REALLOCSEG(USHORT cbNewSize, SEL sel);
USHORT APIENTRY DOS16FREESEG(SEL sel);
USHORT APIENTRY DOS16GIVESEG(SEL sel, PID pid, PSEL pSelRecipient);
USHORT APIENTRY DOS16GETSEG(SEL sel);
USHORT APIENTRY DOS16SIZESEG(SEL sel, PULONG pcbSize);

/* Segment attribute flags (used with Dos16AllocSeg) */

#define SEG_NONSHARED		0x0000
#define SEG_GIVEABLE		0x0001
#define SEG_GETTABLE		0x0002
#define SEG_DISCARDABLE 	0x0004
#define SEG_SIZEABLE		0x0008

#endif /* common INCL16_DOSMEMMGR */

#if (defined(INCL16_DOSMEMMGR) && !defined(INCL16_DOSMEMMGR_INCLUDE))
#define INCL16_DOSMEMMGR_INCLUDE

USHORT APIENTRY DOS16ALLOCHUGE(USHORT cSegs, USHORT cbPartialSeg, PSEL psel,
			     USHORT cMaxSegs, USHORT fsAlloc);
USHORT APIENTRY DOS16REALLOCHUGE(USHORT cSegs, USHORT cbPartialSeg, SEL sel);
USHORT APIENTRY DOS16GETHUGESHIFT(PUSHORT pusShiftCount);

USHORT APIENTRY DOS16ALLOCSHRSEG(USHORT cbSeg, PSZ pszSegName, PSEL psel);

USHORT APIENTRY DOS16LOCKSEG(SEL sel);
USHORT APIENTRY DOS16UNLOCKSEG(SEL sel);

USHORT APIENTRY DOS16GETSHRSEG(PSZ pszSegName, PSEL psel);

USHORT APIENTRY DOS16MEMAVAIL(PULONG pcbFree);
USHORT APIENTRY DOS16CREATECSALIAS(SEL selDS, PSEL pselCS);

USHORT APIENTRY DOS16SUBALLOC(SEL sel, PUSHORT pusOffset, USHORT cb);
USHORT APIENTRY DOS16SUBFREE(SEL sel, USHORT offBlock, USHORT cb);
USHORT APIENTRY DOS16SUBSET(SEL sel, USHORT fFlags, USHORT cbNew);

#endif /* INCL16_DOSMEMMGR */

#if (defined(INCL16_DOSSEMAPHORES) || !defined(INCL16_NOCOMMON))

/*** Semaphore support */

#define SEM_INDEFINITE_WAIT	(-1L)
#define SEM_IMMEDIATE_RETURN	0L

// Use for RAM semaphores only

USHORT APIENTRY DOS16SEMREQUEST(void * _Seg16 *hsem, LONG lTimeOut);
USHORT APIENTRY DOS16SEMCLEAR(void * _Seg16 *sem);
USHORT APIENTRY DOS16SEMSET(void * _Seg16 *hsem);
USHORT APIENTRY DOS16SEMWAIT(void * _Seg16 *hsem, LONG LTIMEOUT);
USHORT APIENTRY DOS16SEMSETWAIT(void * _Seg16 *hsem, LONG lTimeOut);

#endif /* common INCL16_DOSSEMAPHORES */

#if (defined(INCL16_DOSSEMAPHORES) && !defined(INCL16_DOSSEMAPHORES_INCLUDE))
#define INCL16_DOSSEMAPHORES_INCLUDE

typedef LHANDLE HSYSSEM;	/* hssm */
typedef HSYSSEM * FAR     PHSYSSEM;

USHORT APIENTRY DOS16CREATESEM(USHORT fExclusive, PHSYSSEM phsem, PSZ pszSemName);

#define CSEM_PRIVATE		0
#define CSEM_PUBLIC		1

USHORT APIENTRY DOS16OPENSEM(PHSEM phsem, PSZ pszSemName);
USHORT APIENTRY DOS16CLOSESEM(HSEM hsem);

typedef struct _MUXSEM {	/* mxs */
	USHORT	zero;
	HSEM	hsem;
} MUXSEM;
typedef MUXSEM * FAR     PMUXSEM;

typedef struct _MUXSEMLIST {	/* mxsl */
	USHORT	cmxs;
	MUXSEM	amxs[16];
} MUXSEMLIST;
typedef MUXSEMLIST * FAR     PMUXSEMLIST;

/*
 * Since a MUXSEMLIST structure is actually a variable length
 * structure, the following macro may be used to define a MUXSEMLIST
 * structure having size elements, named "name".
 */
#define DEFINEMUXSEMLIST(name, size) \
    struct {			     \
	USHORT cmxs;		     \
	MUXSEM amxs[size];	     \
    } name;

/*
 * This function actually takes a * FAR  pointer to a MUXSEMLIST structure
 * as its second parameter, but in order to allow its use with the
 * DEFINEMUXSEMLIST macro, it is declared here as PVOID.
 */
USHORT APIENTRY DOS16MUXSEMWAIT(PUSHORT pisemCleared, PVOID pmsxl, LONG lTimeOut);


/*** Fast safe ram semaphores */

typedef struct _DOSFSRSEM {	/* dosfsrs */
	USHORT	cb;
	PID	pid;
	TID	tid;
	USHORT	cUsage;
	USHORT	client;
	ULONG	sem;
} DOSFSRSEM;
typedef DOSFSRSEM * FAR     PDOSFSRSEM;

USHORT APIENTRY DOS16FSRAMSEMREQUEST(PDOSFSRSEM pdosfsrs, LONG lTimeOut);
USHORT APIENTRY DOS16FSRAMSEMCLEAR(PDOSFSRSEM pdosfsrs);

#endif /* INCL16_DOSSEMAPHORES */

#if ((defined(INCL16_DOSDATETIME) || !defined(INCL16_NOCOMMON)) && !defined(INCL16_DOSDATETIME_INCLUDE))

/*** Time support */

typedef struct _DATETIME {	/* date */
	UCHAR	hours;
	UCHAR	minutes;
	UCHAR	seconds;
	UCHAR	hundredths;
	UCHAR	day;
	UCHAR	month;
	USHORT	year;
	SHORT	timezone;
	UCHAR	weekday;
} DATETIME;
typedef DATETIME * FAR     PDATETIME;

USHORT APIENTRY DOS16GETDATETIME(PDATETIME pdatetime);
USHORT APIENTRY DOS16SETDATETIME(PDATETIME pdatetime);

#endif /* common INCL16_DOSDATETIME */

#if (defined(INCL16_DOSDATETIME) && !defined(INCL16_DOSDATETIME_INCLUDE))
#define INCL16_DOSDATETIME_INCLUDE

typedef SHANDLE HTIMER;
typedef HTIMER * FAR     PHTIMER;

USHORT APIENTRY DOS16TIMERASYNC(ULONG ulTime, HSEM hsem, PHTIMER phtimer);
USHORT APIENTRY DOS16TIMERSTART(ULONG ulTime, HSEM hsem, PHTIMER phtimer);
USHORT APIENTRY DOS16TIMERSTOP(HTIMER htimer);

#endif /* INCL16_DOSDATETIME */


/*** Module manager */

#if (defined(INCL16_DOSMODULEMGR) && !defined(INCL16_DOSMODULEMGR_INCLUDE))
#define INCL16_DOSMODULEMGR_INCLUDE

USHORT APIENTRY DOS16LOADMODULE(PSZ pszFailName, USHORT cbFileName,
                              PSZ pszModName, PHMODULE phmod);
USHORT APIENTRY DOS16FREEMODULE(HMODULE hmod);
USHORT APIENTRY DOS16GETPROCADDR(HMODULE hmod, PSZ pszProcName,
			       PPFN ppfnProcAddr);
USHORT APIENTRY DOS16GETMODHANDLE(PSZ pszModName, PHMODULE phMod);
USHORT APIENTRY DOS16GETMODNAME(HMODULE hmod, USHORT cbBuf, PCHAR pchBuf);

#endif /* INCL16_DOSMODULEMGR */

#if (defined(INCL16_DOSRESOURCES) || !defined(INCL16_NOCOMMON))

/*** Resource support */

/* Predefined resource types */

#define RT_POINTER		1   /* mouse pointer shape		*/
#define RT_BITMAP		2   /* bitmap				*/
#define RT_MENU 		3   /* menu template			*/
#define RT_DIALOG		4   /* dialog template			*/
#define RT_STRING		5   /* string tables			*/
#define RT_FONTDIR		6   /* font directory			*/
#define RT_FONT 		7   /* font				*/
#define RT_ACCELTABLE		8   /* accelerator tables		*/
#define RT_RCDATA		9   /* binary data			*/
#define RT_MESSAGE		10  /* error mesage tables		*/
#define RT_DLGINCL16UDE		11  /* dialog include file name 	*/
#define RT_VKEYTBL		12  /* key to vkey tables		*/
#define RT_KEYTBL		13  /* key to UGL tables		*/
#define RT_CHARTBL		14
#define RT_DISPLAYINFO		15  /* screen display information	*/

#define RT_FKASHORT		16  /* function key area short form	*/
#define RT_FKALONG		17  /* function key area long form	*/

#define RT_HELPTABLE		18
#define RT_HELPSUBTABLE 	19

#define RT_FDDIR		20
#define RT_FD			21

#define RT_MAX			22  /* 1st unused Resource Type 	*/


#endif /* common INCL16_DOSRESOURCES */

#if (defined(INCL16_DOSRESOURCES) && !defined(INCL16_DOSRESOURCES_INCLUDE))
#define INCL16_DOSRESOURCES_INCLUDE

USHORT APIENTRY DOS16GETRESOURCE(HMODULE hmod, USHORT idType, USHORT idName,
			       PSEL psel);
USHORT APIENTRY DOS16GETRESOURCE2(HMODULE hmod, USHORT idType, USHORT idName,
				PVOID * FAR      ppData);
USHORT APIENTRY DOS16FREERESOURCE(PVOID pData);

#endif /* INCL16_DOSRESOURCES */


/*** NLS Support */

#if (defined(INCL16_DOSNLS) && !defined(INCL16_DOSNLS_INCLUDE))
#define INCL16_DOSNLS_INCLUDE

typedef struct _COUNTRYCODE {	/* ctryc */
	USHORT	country;
	USHORT	codepage;
} COUNTRYCODE;
typedef COUNTRYCODE * FAR     PCOUNTRYCODE;

/* fsDateFmt */

#define DATEFMT_MM_DD_YY  0x0000
#define DATEFMT_DD_MM_YY  0x0001
#define DATEFMT_YY_MM_DD  0x0002

/* fsCurrencyFmt */

#define CURRENCY_FOLLOW   0x0001
#define CURRENCY_SPACE    0x0002
#define CURRENCY_DECIMAL  0x0004

typedef struct _COUNTRYINFO {	/* ctryi */
	USHORT	country;
	USHORT	codepage;
	USHORT	fsDateFmt;
	CHAR	szCurrency[5];
	CHAR	szThousandsSeparator[2];
	CHAR	szDecimal[2];
	CHAR	szDateSeparator[2];
	CHAR	szTimeSeparator[2];
	UCHAR	fsCurrencyFmt;
	UCHAR	cDecimalPlace;
	UCHAR	fsTimeFmt;
	USHORT	abReserved1[2];
	CHAR	szDataSeparator[2];
	USHORT	abReserved2[5];
} COUNTRYINFO;
typedef COUNTRYINFO * FAR     PCOUNTRYINFO;

USHORT APIENTRY DOS16GETCTRYINFO(USHORT cbBuf, PCOUNTRYCODE pctryc,
			       PCOUNTRYINFO pctryi, PUSHORT pcbCtryInfo);
USHORT APIENTRY DOS16GETDBCSEV(USHORT cbBuf, PCOUNTRYCODE pctryc, PCHAR pchBuf);
USHORT APIENTRY DOS16CASEMAP(USHORT usLen, PCOUNTRYCODE pctryc, PCHAR pchStr);
USHORT APIENTRY DOS16GETCOLLATE(USHORT cbBuf, PCOUNTRYCODE pctryc, PCHAR pchBuf,
			      PUSHORT pcbTable);
USHORT APIENTRY DOS16GETCP(USHORT cbBuf, PUSHORT pBuf, PUSHORT pcbCodePgLst);
USHORT APIENTRY DOS16SETCP(USHORT usCodePage, USHORT usReserved);
USHORT APIENTRY DOS16SETPROCCP(USHORT usCodePage, USHORT usReserved);

#endif /* INCL16_DOSNLS */


/*** Signal support */

#if (defined(INCL16_DOSSIGNALS) && !defined(INCL16_DOSSIGNALS_INCLUDE))
#define INCL16_DOSSIGNALS_INCLUDE

/* Signal Numbers for Dos16SetSigHandler  */

#define SIG_CTRLC		1	/* Control C		      */
/*	(UNDEFINED)		2	   reserved for future use    */
#define SIG_KILLPROCESS 	3	/* Program Termination	      */
#define SIG_CTRLBREAK		4	/* Control Break	      */
#define SIG_PFLG_A		5	/* Process Flag A	      */
#define SIG_PFLG_B		6	/* Process Flag B	      */
#define SIG_PFLG_C		7	/* Process Flag C	      */
#define SIG_CSIGNALS		8	/* number of signals plus one */

/* Flag Numbers for Dos16FlagProcess */

#define PFLG_A			0	/* Process Flag A	      */
#define PFLG_B			1	/* Process Flag B	      */
#define PFLG_C			2	/* Process Flag C	      */

/* Signal actions */

#define SIGA_KILL		0
#define SIGA_IGNORE		1
#define SIGA_ACCEPT		2
#define SIGA_ERROR		3
#define SIGA_ACKNOWLEDGE	4

/* Dos16HoldSignal constants */

#define HLDSIG_ENABLE		0
#define HLDSIG_DISABLE		1

/* Dos16FlagProcess codes */

#define FLGP_SUBTREE		0
#define FLGP_PID		1

typedef VOID (* _Far16 PASCAL PFNSIGHANDLER)(USHORT, USHORT);

USHORT APIENTRY DOS16SETSIGHANDLER(PFNSIGHANDLER pfnSigHandler,
				 PFNSIGHANDLER * FAR      ppfnPrev, PUSHORT pfAction,
				 USHORT fAction, USHORT usSigNum);
USHORT APIENTRY DOS16FLAGPROCESS(PID PID, USHORT FSCOPE, USHORT USFLAGNUM,
			       USHORT usFlagArg);
USHORT APIENTRY DOS16HOLDSIGNAL(USHORT fDisable);
USHORT APIENTRY DOS16SENDSIGNAL(USHORT idProcess, USHORT usSigNumber);

#endif /* INCL16_DOSSIGNALS */


/*** Monitor support */

#if (defined(INCL16_DOSMONITORS) && !defined(INCL16_DOSMONITORS_INCLUDE))
#define INCL16_DOSMONITORS_INCLUDE

#define MONITOR_DEFAULT 0x0000
#define MONITOR_BEGIN   0x0001
#define MONITOR_END     0x0002

typedef SHANDLE HMONITOR;	/* hmon */
typedef HMONITOR * FAR     PHMONITOR;

typedef struct _MONIN { 	/* mnin */
	USHORT cb;
	BYTE abReserved[18];
	BYTE abBuffer[108];
} MONIN;
typedef MONIN * FAR     PMONIN;

typedef struct _MONOUT {	/* mnout */
	USHORT cb;
	UCHAR buffer[18];
	BYTE abBuf[108];
} MONOUT;
typedef MONOUT * FAR     PMONOUT;

USHORT APIENTRY DOS16MONOPEN(PSZ pszDevName, PHMONITOR phmon);
USHORT APIENTRY DOS16MONCLOSE(HMONITOR hmon);
USHORT APIENTRY DOS16MONREG(HMONITOR hmon, PBYTE pbInBuf, PBYTE pbOutBuf,
			  USHORT fPosition, USHORT usIndex);
USHORT APIENTRY DOS16MONREAD(PBYTE pbInBuf, USHORT fWait, PBYTE pbDataBuf,
			   PUSHORT pcbData);
USHORT APIENTRY DOS16MONWRITE(PBYTE pbOutBuf, PBYTE pbDataBuf, USHORT cbData);

#endif /* INCL16_DOSMONITORS */


/*** Pipe and queue support */

#if (defined(INCL16_DOSQUEUES) && !defined(INCL16_DOSQUEUES_INCLUDE))
#define INCL16_DOSQUEUES_INCLUDE

/* Dos16CreateQueue() priority */

#define QUE_FIFO       0x0000
#define QUE_LIFO       0x0001
#define QUE_PRIORITY   0x0002

typedef SHANDLE HQUEUE; 	/* hq */
typedef HQUEUE * FAR     PHQUEUE;

typedef struct _QUEUERESULT {	/* qresc */
	PID pidProcess;
	USHORT usEventCode;
} QUEUERESULT;
typedef QUEUERESULT * FAR     PQUEUERESULT;

USHORT APIENTRY DOS16MAKEPIPE(PHFILE phfRead, PHFILE phfWrite, USHORT cb);
USHORT APIENTRY DOS16CLOSEQUEUE(HQUEUE hqueue);
USHORT APIENTRY DOS16CREATEQUEUE(PHQUEUE phqueue, USHORT fQueueOrder,
			       PSZ pszQueueName);
USHORT APIENTRY DOS16OPENQUEUE(PUSHORT ppidOwner, PHQUEUE phqueue,
			     PSZ pszQueueName);
USHORT APIENTRY DOS16PEEKQUEUE(HQUEUE hqueue, PQUEUERESULT pqresc,
			     PUSHORT pcbElement, PULONG ppBuf,
			     PUSHORT pElemCode, UCHAR fWait,
			     PBYTE pbElemPrty, ULONG hsem);
USHORT APIENTRY DOS16PURGEQUEUE(HQUEUE hqueue);
USHORT APIENTRY DOS16QUERYQUEUE(HQUEUE hqueue, PUSHORT pcElem);
USHORT APIENTRY DOS16READQUEUE(HQUEUE hqueue, PQUEUERESULT pqresc,
			     PUSHORT pcbElem, PVOID * FAR     ppv, USHORT usElem,
			     UCHAR fWait, PBYTE pbElemPrty, HSEM hsem);
USHORT APIENTRY DOS16WRITEQUEUE(HQUEUE hqueue, USHORT usRequest, USHORT cbBuf,
			      PBYTE pBuf, UCHAR fPriority);

#endif /* INCL16_DOSQUEUES */

#if (defined(INCL16_DOSMISC) && !defined(INCL16_DOSMISC_INCLUDE))
#define INCL16_DOSMISC_INCLUDE

/* Dos16QSysInfo index */
#define Q_MAX_PATH_LENGTH	0 /* index for query max path length  */

/* Dos16Error() error-handling types */

#define HARDERROR_ENABLE	0x0001
#define HARDERROR_DISABLE	0x0000
#define EXCEPTION_ENABLE	0x0000
#define EXCEPTION_DISABLE	0x0002

/* Dos16SetVec vectors */

#define VECTOR_DIVIDE_BY_ZERO	0x0000
#define VECTOR_OVERFLOW 	0x0004
#define VECTOR_OUTOFBOUNDS	0x0005
#define VECTOR_INVALIDOPCODE	0x0006
#define VECTOR_NO_EXTENSION	0x0007
#define VECTOR_EXTENSION_ERROR	0x0010

/* Dos16GetMachineMode() machine modes */

#define MODE_REAL	0x0000
#define MODE_PROTECTED	0x0001

USHORT APIENTRY DOS16ERROR(USHORT fEnable);
USHORT APIENTRY DOS16SETVEC(USHORT usVecNum, PFN pfnFun, PPFN ppfnPrev);
USHORT APIENTRY DOS16GETMESSAGE(PCHAR * FAR      ppchVTable, USHORT usVCount,
			      PCHAR pchBuf, USHORT cbBuf, USHORT usMsgNum,
			      PSZ pszFileName, PUSHORT pcbMsg);
USHORT APIENTRY DOS16ERRCLASS(USHORT usErrCode, PUSHORT pusClass,
			    PUSHORT pfsAction, PUSHORT pusLocus);
USHORT APIENTRY DOS16INSMESSAGE(PCHAR * FAR      ppchVTable, USHORT usVCount,
			      PSZ pszMsg, USHORT cbMsg, PCHAR pchBuf,
			      USHORT cbBuf, PUSHORT pcbMsg);
USHORT APIENTRY DOS16PUTMESSAGE(HFILE hf, USHORT cbMsg, PCHAR pchMsg);
USHORT APIENTRY DOS16QSYSINFO(USHORT index, PBYTE pBuf, USHORT cbBuf);
USHORT APIENTRY DOS16GETENV(PUSHORT pselEnv, PUSHORT pOffsetCmd);
USHORT APIENTRY DOS16SCANENV(PSZ pszVarName, PSZ	* FAR      ppszResult);
USHORT APIENTRY DOS16GETVERSION(PUSHORT pVer);
USHORT APIENTRY DOS16GETMACHINEMODE(PBYTE pMachMode);

#endif /* INCL16_DOSMISC */


/*** Session manager support */

#if (defined(INCL16_DOSSESMGR) && !defined(INCL16_DOSSESMGR_INCLUDE))
#define INCL16_DOSSESMGR_INCLUDE

/* Dos16QAppType() application types */

#define NOTSPECIFIED	0x0000
#define NOTWINDOCOMPAT	0x0001
#define WINDOWCOMPAT	0x0002
#define WINDOWAPI	0x0003
#define BOUND		0x0008
#define DYNAMICLINK	0x0010
#define DOSFORMAT	0x0020

typedef struct _STARTDATA {	/* stdata */
	USHORT	Length;
	USHORT	Related;
	USHORT	FgBg;
	USHORT	TraceOpt;
	PSZ	PgmTitle;
	PSZ	PgmName;
	PBYTE	PgmInputs;
	PBYTE	TermQ;
	PBYTE	Environment;
	USHORT	InheritOpt;
	USHORT	SessionType;
	PSZ	IconFile;
	ULONG	PgmHandle;
	USHORT	PgmControl;
	USHORT	InitXPos;
	USHORT	InitYPos;
	USHORT	InitXSize;
	USHORT	InitYSize;
} STARTDATA;
typedef STARTDATA * FAR     PSTARTDATA;

/* STATUSDATA.SelectInd constants */

#define TARGET_UNCHANGED       0x0000
#define TARGET_SELECTABLE      0x0001
#define TARGET_NOT_SELECTABLE  0x0002

/* STATUSDATA.BondInd constants */

#define BOND_UNCHANGED  0x0000
#define BOND_CHILD      0x0001
#define BOND_NONE       0x0002

typedef struct _STATUSDATA {	/* stsdata */
	USHORT	Length;
	USHORT	SelectInd;
	USHORT	BondInd;
} STATUSDATA;
typedef STATUSDATA * FAR     PSTATUSDATA;

USHORT APIENTRY DOS16STARTSESSION(PSTARTDATA pstdata, PUSHORT pidSession,
				PUSHORT ppid);
USHORT APIENTRY DOS16SETSESSION(USHORT idSession, PSTATUSDATA pstsdata);
USHORT APIENTRY DOS16SELECTSESSION(USHORT idSession, ULONG ulReserved);
USHORT APIENTRY DOS16STOPSESSION(USHORT fScope, USHORT idSession,
			       ULONG ulReserved);
USHORT APIENTRY DOS16QAPPTYPE(PSZ pszPrgName, PUSHORT pusType);

#endif /* INCL16_DOSSESMGR */


/*** Device support */

#if (defined(INCL16_DOSDEVICES) && !defined(INCL16_DOSDEVICES_INCLUDE))
#define INCL16_DOSDEVICES_INCLUDE

/* Dos16PhysicalDisk() physical disk information */

#define INFO_COUNT_PARTITIONABLE_DISKS	0x0001
#define INFO_GETIOCTLHANDLE		0x0002
#define INFO_FREEIOCTLHANDLE		0x0003

/* Dos16DevConfig information requests */

#define DEVINFO_PRINTER      0x0000
#define DEVINFO_RS232        0x0001
#define DEVINFO_FLOPPY       0x0002
#define DEVINFO_COPROCESSOR  0x0003
#define DEVINFO_SUBMODEL     0x0004
#define DEVINFO_MODEL        0x0005
#define DEVINFO_ADAPTER      0x0006

USHORT APIENTRY DOS16DEVCONFIG(PVOID pDevInfo, USHORT usItem, USHORT usReserved);
USHORT APIENTRY DOS16DEVIOCTL(PVOID pData, PVOID pParms, USHORT usFun,
			    USHORT usCategory, HFILE hDev);
USHORT APIENTRY DOS16DEVIOCTL2(PVOID pData, USHORT cbData, PVOID pParm,
			     USHORT cbParm, USHORT usFun, USHORT usCategory,
			     HFILE hDev);

USHORT APIENTRY DOS16CLIACCESS(VOID);
USHORT APIENTRY DOS16PORTACCESS(USHORT usReserved, USHORT fRelease,
			      USHORT usFirstPort, USHORT usLastPort);
USHORT APIENTRY DOS16PHYSICALDISK(USHORT usFun, PBYTE pOut, USHORT cbOut,
				PBYTE pParm, USHORT cbParm);

USHORT APIENTRY DOS16R2STACKREALLOC(USHORT cbStack);
VOID   APIENTRY DOS16CALLBACK(PFN pfn);

#endif /* INCL16_DOSDEVICES */


/*** Dos16NamedPipes API Support */

#if (defined(INCL16_DOSNMPIPES) && !defined(INCL16_DOSNMPIPES_INCLUDE))
#define INCL16_DOSNMPIPES_INCLUDE

typedef SHANDLE HPIPE;		/* hp */
typedef HPIPE * FAR     PHPIPE;

/*** Data structures and equates used with named pipes ***/

typedef struct _PIPEINFO { /* nmpinf */
	USHORT cbOut;
	USHORT cbIn;
	BYTE   cbMaxInst;
	BYTE   cbCurInst;
	BYTE   cbName;
	CHAR   szName[1];
} PIPEINFO;
typedef PIPEINFO * FAR     PPIPEINFO;

/* The following structure for backward compatability to nmpipe.h */

struct	npi_data1 {	/* PipeInfo data block (returned, level 1) */
	unsigned short	npi_obuflen;	/* length of outgoing I/O buffer */
	unsigned short	npi_ibuflen;	/* length of incoming I/O buffer */
	unsigned char	npi_maxicnt;	/* maximum number of instances	 */
	unsigned char	npi_curicnt;	/* current number of instances	 */
	unsigned char	npi_namlen;	/* length of pipe name		 */
	char	npi_name[1];		/* start of name		 */
};	/* npi_data1 */

typedef struct _PIPESEMSTATE {	/* nmpsmst */
	BYTE   fStatus;
	BYTE   fFlag;
	USHORT usKey;
	USHORT usAvail;
} PIPESEMSTATE;
typedef PIPESEMSTATE * FAR     PPIPESEMSTATE;

/* The following structure for backward compatability to nmpipe.h */

struct  npss    {       /* QNmPipeSemState information record */
	unsigned char	npss_status; /* type of record, 0 = EOI, 1 = read ok, */
                                     /*   2 = write ok, 3 = pipe closed       */
	unsigned char	npss_flag;   /* additional info, 01 = waiting thread  */
	unsigned short	npss_key;    /* user's key value                      */
	unsigned short	npss_avail;  /* available data/space if status = 1/2  */
};	/* npss */

/* values in npss_status */
#define NPSS_EOI		0 /* End Of Information    */
#define NPSS_RDATA		1 /* read data available   */
#define NPSS_WSPACE		2 /* write space available */
#define NPSS_CLOSE		3 /* pipe in CLOSING state */

/* values in npss_flag */
#define NPSS_WAIT		0x01  /* waiting thread on end of pipe */

/* defined bits in pipe mode */
#define NP_NBLK 		0x8000 /* non-blocking read/write */
#define NP_SERVER		0x4000 /* set if server end	  */
#define NP_WMESG		0x0400 /* write messages	  */
#define NP_RMESG		0x0100 /* read as messages	  */
#define NP_ICOUNT		0x00FF /* instance count field	  */


/*	Named pipes may be in one of several states depending on the actions
 *	that have been taken on it by the server end and client end.  The
 *	following state/action table summarizes the valid state transitions:
 *
 *	Current state		Action			Next state
 *
 *	 <none> 	    server Dos16MakeNmPipe	DISCONNECTED
 *	 DISCONNECTED	    server connect		LISTENING
 *	 LISTENING	    client open 		CONNECTED
 *	 CONNECTED	    server disconn		DISCONNECTED
 *	 CONNECTED	    client close		CLOSING
 *	 CLOSING	    server disconn		DISCONNECTED
 *	 CONNECTED	    server close		CLOSING
 *	 <any other>	    server close		<pipe deallocated>
 *
 *	If a server disconnects his end of the pipe, the client end will enter a
 *	special state in which any future operations (except close) on the file
 *	descriptor associated with the pipe will return an error.
 */

/*
 *	Values for named pipe state
 */

#define NP_DISCONNECTED 	1 /* after pipe creation or Disconnect */
#define NP_LISTENING		2 /* after Dos16NmPipeConnect	       */
#define NP_CONNECTED		3 /* after Client open		       */
#define NP_CLOSING		4 /* after Client or Server close      */

/* Dos16MakeNmPipe open modes */

#define NP_ACCESS_INBOUND	0x0000
#define NP_ACCESS_OUTBOUND	0x0001
#define NP_ACCESS_DUPLEX	0x0002
#define NP_INHERIT		0x0000
#define NP_NOINHERIT		0x0080
#define NP_WRITEBEHIND		0x0000
#define NP_NOWRITEBEHIND	0x4000

/* Dos16MakeNmPipe and Dos16QNmPHandState state */

#define NP_READMODE_BYTE	0x0000
#define NP_READMODE_MESSAGE	0x0100
#define NP_TYPE_BYTE		0x0000
#define NP_TYPE_MESSAGE 	0x0400
#define NP_END_CLIENT		0x0000
#define NP_END_SERVER		0x4000
#define NP_WAIT 		0x0000
#define NP_NOWAIT		0x8000
#define NP_UNLIMITED_INSTANCES	0x00FF

typedef struct _AVAILDATA   {	/* PeekNMPipe Bytes Available record */
	USHORT	cbpipe; 	/* bytes left in the pipe	     */
	USHORT	cbmessage;	/* bytes left in current message     */
} AVAILDATA;
typedef AVAILDATA * FAR PAVAILDATA;

USHORT APIENTRY DOS16CALLNMPIPE(PSZ pszName, PBYTE pInBuf, USHORT cbInBuf,
			      PBYTE pbOutBuf, USHORT cbOutBuf, PUSHORT pcbRead,
			      ULONG ulTimeOut);
USHORT APIENTRY DOS16CONNECTNMPIPE(HPIPE hp);
USHORT APIENTRY DOS16DISCONNECTNMPIPE(HPIPE hp);
USHORT APIENTRY DOS16MAKENMPIPE(PSZ pszName, PHPIPE php, USHORT fsOpenMode,
			      USHORT fsPipeMode, USHORT cbOutBuf,
			      USHORT cbInBuf, ULONG ulTimeOut);
USHORT APIENTRY DOS16PEEKNMPIPE(HPIPE hp, PBYTE pBuf, USHORT cbBuf,
			      PUSHORT pcbRead, PAVAILDATA pAvail,
			      PUSHORT pfsState);
USHORT APIENTRY DOS16QNMPHANDSTATE(HPIPE hp, PUSHORT pfsState);
USHORT APIENTRY DOS16QNMPIPEINFO(HPIPE hp, USHORT usInfoLevel, PBYTE pBuf,
			       USHORT cb);
USHORT APIENTRY DOS16QNMPIPESEMSTATE(HSEM hsem, PPIPESEMSTATE pnmpsmst, USHORT cb);
USHORT APIENTRY DOS16SETNMPHANDSTATE(HPIPE hp, USHORT fsState);
USHORT APIENTRY DOS16SETNMPIPESEM(HPIPE hp, HSEM hsem, USHORT usKeyVal);
USHORT APIENTRY DOS16TRANSACTNMPIPE(HPIPE hp, PBYTE bOutBuf, USHORT cbOut,
				  PBYTE pInBuf, USHORT cbIn, PUSHORT pcbRead);
USHORT APIENTRY DOS16WAITNMPIPE(PSZ pszName, ULONG ulTimeOut);

#endif /* INCL16_DOSNMPIPES */


/*** Trace support */

#if (defined(INCL16_DOSTRACE) && !defined(INCL16_DOSTRACE_INCLUDE))
#define INCL16_DOSTRACE_INCLUDE

typedef struct PTRACEBUF {	/* ptrcbf */
	PID    pid;
	TID    tid;
	USHORT cmd;
	USHORT value;
	USHORT offv;
	USHORT segv;
	USHORT mte;
	USHORT rAX;
	USHORT rBX;
	USHORT rCX;
	USHORT rDX;
	USHORT rSI;
	USHORT rDI;
	USHORT rBP;
	USHORT rDS;
	USHORT rES;
	USHORT rIP;
	USHORT rCS;
	USHORT rF;
	USHORT rSP;
	USHORT rSS;
} PTRACEBUF;
typedef PTRACEBUF * FAR PPTRACEBUF;

USHORT APIENTRY DOS16PTRACE(PBYTE pPtraceBuf);

#endif /* INCL16_DOSTRACE */
