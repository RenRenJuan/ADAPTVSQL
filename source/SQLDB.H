/* SQLDB.H - DB-Library header file for the Microsoft SQL Server.
**	     The SYBASE SQL Server database for PC networks.
**
** Copyright (c) 1989, 1991 by Microsoft Corp.	All rights reserved.
** Copyright (c) 1989, 1991 by Sybase, Inc.  All rights reserved.
**
*/

#define  DBPROCESS  void		/* dbprocess structure type */
#define  LOGINREC   void		/* login record type */
#define  DBCURSOR   void		/* cursor record type */

/* Macros for setting the loginrec */
#define DBSETLHOST(a,b) 	dbsetlname((a), (b), DBSETHOST)
#define DBSETLUSER(a,b)         dbsetlname((a), (b), DBSETUSER)
#define DBSETLPWD(a,b)          dbsetlname((a), (b), DBSETPWD)
#define DBSETLAPP(a,b)          dbsetlname((a), (b), DBSETAPP)
#define BCP_SETL(a,b)		bcp_setl((a), (b))
    /* The following is a version 4.2 addition */
#define DBSETLNATLANG(a,b)	dbsetlname((a), (b), DBSETLANG)


/* Macros for referencing the dbproc */
#define DBDEAD(a)		dbdead(a)
#define DBRBUF(a)               ((DBINT)dbdataready(a))
#define dbrbuf(a)               ((DBINT)dbdataready(a))
#define DBCOUNT(a)		dbcount(a)
#define DBCURCMD(a)		dbcurcmd(a)
#define DBCURROW(a)		dbcurrow(a)
#define DBFIRSTROW(a)		dbfirstrow(a)
#define DBLASTROW(a)		dblastrow(a)
#define DBNUMORDERS(a)		dbnumorders(a)
#define DBROWS(a)		dbrows(a)
#define DBMORECMDS(a)		dbmorecmds(a)
#define DBROWTYPE(a)		dbrowtype(a)
#define DBCMDROW(a)		dbcmdrow(a)
#define DBISAVAIL(a)		dbisavail(a)
#define DBGETTIME()		dbgettime()

#ifndef DBMSWIN

/* Macros for locking and unlocking Windows data segment (for compatibility) */
#define DBLOCKLIB()
#define DBUNLOCKLIB()

#else	/* #ifndef DBMSWIN; Windows compilation */

/* Macros for locking and unlocking the DBLIB data segment */
#define DBLOCKLIB()	dblocklib()
#define DBUNLOCKLIB()	dbunlocklib()

#endif	/* #ifndef DBMSWIN */

#ifndef DBNTWIN32
typedef long (far pascal *LGFARPROC)(); 	/* os/2/windows loadable driver fp*/
#endif

/* Bulk Copy Definitions (bcp) */

#define DB_IN	1		    /* transfer from client to server */
#define DB_OUT	2		    /* transfer from server to client */

#define BCPMAXERRS  1		    /* bcp_control parameter */
#define BCPFIRST    2		    /* bcp_control parameter */
#define BCPLAST     3		    /* bcp_control parameter */
#define BCPBATCH    4		    /* bcp_control parameter */


#ifdef DBMSDOS

/* error handler */
extern INT ( SQLAPI * DbErrHandler)(DBPROCESS *,INT,INT,INT,CHAR *,CHAR *);

/* message handler */
extern INT ( SQLAPI * DbMsgHandler)(DBPROCESS *,DBINT,INT,INT,CHAR *);

/* Dblib function prototypes */
extern	LOGINREC *  SQLAPI dblogin(void );
extern	DBPROCESS * SQLAPI dbopen(LOGINREC *,CHAR *);

extern void * SQLAPI dberrhandle(INT (SQLAPI * DbErrHandler)(DBPROCESS *,INT,INT,INT,CHAR *,CHAR *));
extern void * SQLAPI dbmsghandle(INT (SQLAPI * DbMsgHandler)(DBPROCESS *, DBINT,INT,INT,CHAR *));

extern	BYTE *	SQLAPI dbinit(void);
extern	RETCODE SQLAPI dbsetlname(LOGINREC *,CHAR *,INT);
extern	void	SQLAPI dbfreelogin(LOGINREC *);
extern	RETCODE SQLAPI dbresults(DBPROCESS *);
extern	STATUS	SQLAPI dbnextrow(DBPROCESS *);
extern	void	SQLAPI dbclose(DBPROCESS *);
extern	RETCODE SQLAPI dbcmd(DBPROCESS *,CHAR *);
extern	RETCODE SQLAPI dbfcmd(DBPROCESS *, CHAR *, ...);
extern	INT	SQLAPI dbstrlen(DBPROCESS *);
extern	RETCODE SQLAPI dbstrcpy(DBPROCESS *,INT,INT,CHAR *);
extern	CHAR *	SQLAPI dbgetchar(DBPROCESS *, INT);
extern	RETCODE SQLAPI dbsqlexec(DBPROCESS *);
extern	RETCODE SQLAPI dbsqlok(DBPROCESS *);
extern	RETCODE SQLAPI dbsqlsend(DBPROCESS *);
extern	RETCODE SQLAPI dbcanquery(DBPROCESS *);
extern	STATUS	SQLAPI dbgetrow(DBPROCESS *,DBINT);
extern	void	SQLAPI dbclrbuf(DBPROCESS *,DBINT);
extern	void	SQLAPI dbfreebuf(DBPROCESS *);
extern	RETCODE SQLAPI dbsettime(INT);
extern	INT	SQLAPI dbgettime(void);
extern	RETCODE SQLAPI dbsetlogintime(INT);
extern	DBUBOOL SQLAPI dbdataready(DBPROCESS *);
extern	RETCODE SQLAPI dbcancel(DBPROCESS *);
extern	void	SQLAPI dbexit(void);
extern	RETCODE SQLAPI dbsetopt(DBPROCESS *,INT,CHAR *);
extern	DBUBOOL SQLAPI dbisopt(DBPROCESS *, INT, CHAR *);
extern	RETCODE SQLAPI dbclropt(DBPROCESS *,INT,CHAR *);
extern	INT	SQLAPI dbordercol(DBPROCESS *,INT);
extern	INT	SQLAPI dbconvert(DBPROCESS *,INT,BYTE *,DBINT,INT,BYTE *,DBINT);
extern	DBINT	SQLAPI dbcollen(DBPROCESS *,INT);
extern	BYTE *	SQLAPI dbcolname(DBPROCESS *,INT);
extern	INT	SQLAPI dbcoltype(DBPROCESS *,INT);
extern	DBINT	SQLAPI dbcolutype(DBPROCESS *,INT);
extern	BYTE *	SQLAPI dbdata(DBPROCESS *,INT);
extern	DBINT	SQLAPI dbdatlen(DBPROCESS *,INT);
extern	INT	SQLAPI dbnumcols(DBPROCESS *);
extern	DBUBOOL SQLAPI dbcolbrowse(DBPROCESS *,INT);
extern	CHAR *	SQLAPI dbtabname(DBPROCESS *,INT);
extern	CHAR *	SQLAPI dbcolsource(DBPROCESS *,INT);
extern	DBUBOOL SQLAPI dbtabbrowse(DBPROCESS *,INT);
extern	CHAR *	SQLAPI dbtabsource(DBPROCESS *, INT, INT *);
extern	INT	SQLAPI dbtabcount(DBPROCESS *);
extern	INT	SQLAPI dbtsnewlen(DBPROCESS *);
extern	RETCODE SQLAPI dbtsput(DBPROCESS *,DBBINARY *,INT,INT,CHAR *);
extern	CHAR * SQLAPI dbqual(DBPROCESS *,INT,CHAR *);
extern	void	SQLAPI dbfreequal(CHAR *);
extern	void SQLAPI dbprhead(DBPROCESS *);
extern	RETCODE SQLAPI dbprrow(DBPROCESS *);
extern	RETCODE SQLAPI dbtxtsput(DBPROCESS *,DBBINARY *,INT);
extern	RETCODE SQLAPI dbwritetext(DBPROCESS *,CHAR *,DBBINARY *,DBTINYINT, DBBINARY *,DBBOOL,DBINT,BYTE *);
extern	RETCODE SQLAPI dbmoretext(DBPROCESS *,DBINT,BYTE *);
extern	RETCODE SQLAPI dbuse(DBPROCESS *,CHAR *);
extern	RETCODE SQLAPI dbbind(DBPROCESS *,INT,INT,DBINT,BYTE *);
extern	DBUBOOL SQLAPI dbwillconvert(INT,INT);
extern	RETCODE SQLAPI dbsetnull(DBPROCESS *, INT, INT, BYTE *);
extern	RETCODE SQLAPI dbaltbind(DBPROCESS *,INT,INT,INT,DBINT,BYTE *);
extern	DBINT	SQLAPI dbaltlen(DBPROCESS *,INT,INT);
extern	INT	SQLAPI dbalttype(DBPROCESS *,INT,INT);
extern	BYTE *	SQLAPI dbadata(DBPROCESS *,INT,INT);
extern	DBINT	SQLAPI dbadlen(DBPROCESS *,INT,INT);
extern	INT	SQLAPI dbnumalts(DBPROCESS *,INT);
extern	INT	SQLAPI dbaltcolid(DBPROCESS *,INT,INT);
extern	BYTE *	SQLAPI dbbylist(DBPROCESS *,INT,INT *);
extern	INT	SQLAPI dbaltop(DBPROCESS *,INT,INT);
extern	INT	SQLAPI dbnumcompute(DBPROCESS *);
extern	CHAR *	SQLAPI dbprtype(INT);
extern	CHAR *	SQLAPI dbchange(DBPROCESS *);
extern	CHAR *	SQLAPI dbname(DBPROCESS *);
extern	INT	SQLAPI dbgetoff(DBPROCESS *,DBUSMALLINT,INT);
extern	DBINT	SQLAPI dbreadpage(DBPROCESS *,CHAR far *,DBINT,BYTE *);
extern	RETCODE SQLAPI dbwritepage(DBPROCESS *,CHAR *,DBINT,DBINT,BYTE *);

extern	DBBINARY * SQLAPI dbtsnewval(DBPROCESS *);
extern	DBBINARY * SQLAPI dbtxptr(DBPROCESS *,INT);
extern	DBBINARY * SQLAPI dbtxtimestamp(DBPROCESS *,INT);
extern	DBBINARY * SQLAPI dbtxtsnewval(DBPROCESS *);
extern	RETCODE SQLAPI dbsetmaxprocs(SHORT);
extern	SHORT SQLAPI dbgetmaxprocs(void);
extern	BOOL  SQLAPI dbiscount(DBPROCESS *);

/* remote procedure call functions, these are version 4.2 extensions */
extern	DBBOOL	SQLAPI dbhasretstat(DBPROCESS *);
extern	INT	SQLAPI dbnumrets(DBPROCESS *);
extern	BYTE *	SQLAPI dbretdata(DBPROCESS *, INT);
extern	DBINT	SQLAPI dbretlen(DBPROCESS *, INT);
extern	CHAR *	SQLAPI dbretname(DBPROCESS *, INT);
extern	DBINT	SQLAPI dbretstatus(DBPROCESS *);
extern	INT	SQLAPI dbrettype(DBPROCESS *, INT);
extern	RETCODE SQLAPI dbrpcinit(DBPROCESS *, CHAR *, DBSMALLINT);
extern	RETCODE SQLAPI dbrpcparam(DBPROCESS *,CHAR *,BYTE,INT,DBINT,DBINT,BYTE *);
extern	RETCODE SQLAPI dbrpcsend(DBPROCESS *);
extern	void	SQLAPI dbrpwclr(LOGINREC *);
extern	RETCODE SQLAPI dbrpwset(LOGINREC *, CHAR *, CHAR *, INT);
extern	void SQLAPI dbsetuserdata(DBPROCESS *, void *);
extern	void * SQLAPI dbgetuserdata(DBPROCESS *);

/* two-phase commit functions */
extern DBPROCESS * SQLAPI open_commit(LOGINREC *,CHAR *);
extern void	   SQLAPI close_commit(DBPROCESS *);
extern void	   SQLAPI build_xact_string(CHAR *,CHAR *,DBINT,CHAR *);
extern DBINT	   SQLAPI start_xact(DBPROCESS *,CHAR *,CHAR *,INT);
extern RETCODE	   SQLAPI commit_xact(DBPROCESS *,DBINT);
extern RETCODE	   SQLAPI abort_xact(DBPROCESS *,DBINT);
extern RETCODE	   SQLAPI remove_xact(DBPROCESS *,DBINT,INT);
extern RETCODE	   SQLAPI scan_xact(DBPROCESS *,DBINT);
extern INT	   SQLAPI stat_xact(DBPROCESS *,DBINT);

/* bulk copy prototypes */
extern DBINT   SQLAPI bcp_batch(DBPROCESS *);
extern RETCODE SQLAPI bcp_bind(DBPROCESS *, BYTE *, INT, DBINT, BYTE *, INT, INT, INT);
extern RETCODE SQLAPI bcp_colfmt(DBPROCESS *, INT, BYTE, INT, DBINT, BYTE *, INT, INT);
extern RETCODE SQLAPI bcp_collen(DBPROCESS *, DBINT, INT);
extern RETCODE SQLAPI bcp_colptr(DBPROCESS *, BYTE *, INT);
extern RETCODE SQLAPI bcp_columns(DBPROCESS *, INT);
extern RETCODE SQLAPI bcp_control(DBPROCESS *, INT, DBINT);
extern DBINT   SQLAPI bcp_done(DBPROCESS *);
extern RETCODE SQLAPI bcp_exec(DBPROCESS *, DBINT *);
extern RETCODE SQLAPI bcp_init(DBPROCESS *, CHAR *, CHAR *, CHAR *, INT);
extern RETCODE SQLAPI bcp_moretext(DBPROCESS *, DBINT, BYTE *);
extern RETCODE SQLAPI bcp_sendrow(DBPROCESS *);
extern RETCODE SQLAPI bcp_setl(LOGINREC *, DBBOOL);
/* Version 4.2 additions */
extern RETCODE SQLAPI bcp_readfmt(DBPROCESS *, CHAR *);
extern RETCODE SQLAPI bcp_writefmt(DBPROCESS *, CHAR *);


/* Functions for referencing the dbproc */
RETCODE SQLAPI dbcmdrow(DBPROCESS *);
DBINT	SQLAPI dbcount(DBPROCESS *);
INT	SQLAPI dbcurcmd(DBPROCESS *);
DBINT	SQLAPI dbcurrow(DBPROCESS *);
DBUBOOL SQLAPI dbdead(DBPROCESS *);
DBINT	SQLAPI dbfirstrow(DBPROCESS *);
DBUBOOL SQLAPI dbisavail(DBPROCESS *);
void	SQLAPI dbsetavail(DBPROCESS *);
DBINT	SQLAPI dblastrow(DBPROCESS *);
RETCODE SQLAPI dbmorecmds(DBPROCESS *);
INT	SQLAPI dbnumorders(DBPROCESS *);
RETCODE SQLAPI dbrows(DBPROCESS *);
STATUS	SQLAPI dbrowtype(DBPROCESS *);

/* version 4.2 extensions */
extern	DBINT SQLAPI dbreadtext(DBPROCESS *,void *, DBINT);
extern	DBINT SQLAPI dbaltutype(DBPROCESS *,INT,INT);
extern	RETCODE SQLAPI dbanullbind(DBPROCESS *,INT,INT,DBINT *);
extern	RETCODE SQLAPI dbnullbind(DBPROCESS *,INT,DBINT *);
extern	RETCODE SQLAPI dbdatecrack(DBPROCESS *, DBDATEREC *, DBDATETIME *);
extern	DBBOOL SQLAPI  dbvarylen(DBPROCESS *, INT);

/* Cursor related functions */
extern	DBCURSOR * SQLAPI dbcursoropen(DBPROCESS *,CHAR *,INT,INT,UINT,DBINT *);
extern	RETCODE SQLAPI dbcursorinfo(DBCURSOR *, int *, DBINT *);
extern	RETCODE SQLAPI dbcursorcolinfo(DBCURSOR *, int, char *, int *,
	     DBINT *, int *);
extern	RETCODE SQLAPI dbcursorbind(DBCURSOR *, int, int, DBINT, DBINT *, CHAR *);
extern	RETCODE SQLAPI dbcursorfetch(DBCURSOR *,INT,INT);
extern	RETCODE SQLAPI dbcursor(DBCURSOR *, INT,INT,CHAR *,CHAR *);
extern	RETCODE SQLAPI dbcursorclose(DBCURSOR *);

#endif		    /* DBMSDOS */

#ifdef DBMSOS2
/* error handler */
extern INT ( SQLAPI * DbErrHandler)(DBPROCESS far *,INT,INT,INT,CHAR far *,CHAR far *);

/* message handler */
extern INT ( SQLAPI * DbMsgHandler)(DBPROCESS far *,DBINT,INT,INT,CHAR far *);

/* extern USHORT DbTimeOut;	/* time out period in seconds		*/

/* Dblib function prototypes */
extern LOGINREC far *  SQLAPI dblogin(void );
extern DBPROCESS far * SQLAPI dbopen(LOGINREC far *,CHAR far *);

extern void far * SQLAPI dberrhandle(INT (SQLAPI * DbErrHandler)(DBPROCESS far *,INT,INT,INT,CHAR far *,CHAR far *));
extern void far * SQLAPI dbmsghandle(INT (SQLAPI * DbMsgHandler)(DBPROCESS far *,DBINT,INT,INT,CHAR far *));

extern BYTE far * SQLAPI dbinit(void);
extern RETCODE	  SQLAPI dbsetlname(LOGINREC far *,CHAR far *,INT);
extern void	  SQLAPI dbfreelogin(LOGINREC far *);
extern RETCODE	  SQLAPI dbresults(DBPROCESS far *);
extern STATUS	  SQLAPI dbnextrow(DBPROCESS far *);
extern void	  SQLAPI dbclose(DBPROCESS far *);
extern RETCODE	  SQLAPI dbcmd(DBPROCESS far *,CHAR far *);
extern RETCODE	  SQLAPI dbfcmd(DBPROCESS far *, CHAR far *, ...);
extern INT	  SQLAPI dbstrlen(DBPROCESS far *);
extern RETCODE	  SQLAPI dbstrcpy(DBPROCESS far *,INT,INT,CHAR far *);
extern CHAR far * SQLAPI dbgetchar(DBPROCESS far *, INT);
extern RETCODE	  SQLAPI dbsqlexec(DBPROCESS far *);
extern RETCODE	  SQLAPI dbsqlok(DBPROCESS far *);
extern RETCODE	  SQLAPI dbsqlsend(DBPROCESS far *);
extern RETCODE	  SQLAPI dbcanquery(DBPROCESS far *);
extern STATUS	  SQLAPI dbgetrow(DBPROCESS far *,DBINT);
extern void	  SQLAPI dbclrbuf(DBPROCESS far *,DBINT);
extern void	  SQLAPI dbfreebuf(DBPROCESS far *);
extern RETCODE	  SQLAPI dbsettime(INT);
extern INT	  SQLAPI dbgettime(void);
extern RETCODE	  SQLAPI dbsetlogintime(INT);
extern DBUBOOL	  SQLAPI dbdataready(DBPROCESS far *);
extern RETCODE	  SQLAPI dbcancel(DBPROCESS far *);
extern void	  SQLAPI dbexit(void);
extern RETCODE	  SQLAPI dbsetopt(DBPROCESS far *,INT,CHAR far *);
extern DBUBOOL	  SQLAPI dbisopt(DBPROCESS far *, INT, CHAR far *);
extern RETCODE	  SQLAPI dbclropt(DBPROCESS far *,INT,CHAR far *);
extern INT	  SQLAPI dbordercol(DBPROCESS far *,INT);
extern INT	  SQLAPI dbconvert(DBPROCESS far *,INT,BYTE far *,DBINT,INT,BYTE far *,DBINT);
extern DBINT	  SQLAPI dbcollen(DBPROCESS far *,INT);
extern BYTE far * SQLAPI dbcolname(DBPROCESS far *,INT);
extern INT	  SQLAPI dbcoltype(DBPROCESS far *,INT);
extern DBINT	  SQLAPI dbcolutype(DBPROCESS *,INT);
extern BYTE far * SQLAPI dbdata(DBPROCESS far *,INT);
extern DBINT	  SQLAPI dbdatlen(DBPROCESS far *,INT);
extern INT	  SQLAPI dbnumcols(DBPROCESS far *);
extern DBUBOOL	  SQLAPI dbcolbrowse(DBPROCESS far *,INT);
extern CHAR far * SQLAPI dbtabname(DBPROCESS far *,INT);
extern CHAR far * SQLAPI dbcolsource(DBPROCESS far *,INT);
extern DBUBOOL	  SQLAPI dbtabbrowse(DBPROCESS far *,INT);
extern CHAR far * SQLAPI dbtabsource(DBPROCESS far *, INT, INT far *);
extern INT	  SQLAPI dbtabcount(DBPROCESS far *);
extern INT	  SQLAPI dbtsnewlen(DBPROCESS far *);
extern RETCODE	  SQLAPI dbtsput(DBPROCESS far *,DBBINARY far *,INT,INT,CHAR far *);
extern CHAR far * SQLAPI dbqual(DBPROCESS far *,INT,CHAR far *);
extern void	  SQLAPI dbfreequal(CHAR far *);
extern void	  SQLAPI dbprhead(DBPROCESS far *);
extern RETCODE	  SQLAPI dbprrow(DBPROCESS far *);
extern RETCODE	  SQLAPI dbtxtsput(DBPROCESS far *,DBBINARY far *,INT);
extern RETCODE	  SQLAPI dbwritetext(DBPROCESS far *,CHAR far *,DBBINARY far *,DBTINYINT, DBBINARY far *,DBBOOL,DBINT,BYTE far *);
extern RETCODE	  SQLAPI dbmoretext(DBPROCESS far *,DBINT,BYTE far *);
extern RETCODE	  SQLAPI dbuse(DBPROCESS far *,CHAR far *);
extern RETCODE	  SQLAPI dbbind(DBPROCESS far *,INT,INT,DBINT,BYTE far *);
extern DBUBOOL	  SQLAPI dbwillconvert(INT,INT);
extern RETCODE	  SQLAPI dbsetnull(DBPROCESS far *, INT, INT, BYTE far *);
extern RETCODE	  SQLAPI dbaltbind(DBPROCESS far *,INT,INT,INT,DBINT,BYTE far *);
extern DBINT	  SQLAPI dbaltlen(DBPROCESS far *,INT,INT);
extern INT	  SQLAPI dbalttype(DBPROCESS far *,INT,INT);
extern BYTE far * SQLAPI dbadata(DBPROCESS far *,INT,INT);
extern DBINT	  SQLAPI dbadlen(DBPROCESS far *,INT,INT);
extern INT	  SQLAPI dbnumalts(DBPROCESS far *,INT);
extern INT	  SQLAPI dbaltcolid(DBPROCESS far *,INT,INT);
extern BYTE far * SQLAPI dbbylist(DBPROCESS far *,INT,INT far *);
extern INT	  SQLAPI dbaltop(DBPROCESS far *,INT,INT);
extern INT	  SQLAPI dbnumcompute(DBPROCESS far *);
extern CHAR far * SQLAPI dbprtype(INT);
extern CHAR far * SQLAPI dbchange(DBPROCESS far *);
extern CHAR far * SQLAPI dbname(DBPROCESS far *);
extern INT	  SQLAPI dbgetoff(DBPROCESS far *,DBUSMALLINT,INT);
extern DBINT	  SQLAPI dbreadpage(DBPROCESS far *,CHAR far *,DBINT,BYTE far *);
extern RETCODE	  SQLAPI dbwritepage(DBPROCESS far *,CHAR far *,DBINT,DBINT,BYTE far *);

extern DBBOOL	  SQLAPI dbhasretstat(DBPROCESS far *);
extern INT	  SQLAPI dbnumrets(DBPROCESS far *);
extern BYTE far * SQLAPI dbretdata(DBPROCESS far *, INT);
extern DBINT	  SQLAPI dbretlen(DBPROCESS far *, INT);
extern CHAR far * SQLAPI dbretname(DBPROCESS far *, INT);
extern DBINT	  SQLAPI dbretstatus(DBPROCESS far *);
extern INT	  SQLAPI dbrettype(DBPROCESS far *, INT);
extern RETCODE	  SQLAPI dbrpcinit(DBPROCESS far *, CHAR far *, DBSMALLINT);
extern RETCODE	  SQLAPI dbrpcparam(DBPROCESS far *,CHAR far *,BYTE,INT,DBINT,DBINT,BYTE far*);
extern RETCODE	  SQLAPI dbrpcsend(DBPROCESS far *);
extern void	  SQLAPI dbrpwclr(LOGINREC far *);
extern RETCODE	  SQLAPI dbrpwset(LOGINREC far *, CHAR far *, CHAR far *, INT);
extern	void SQLAPI dbsetuserdata(DBPROCESS far *, void far *);
extern	void * SQLAPI dbgetuserdata(DBPROCESS far *);

extern DBBINARY far * SQLAPI dbtsnewval(DBPROCESS far *);
extern DBBINARY far * SQLAPI dbtxptr(DBPROCESS far *,INT);
extern DBBINARY far * SQLAPI dbtxtimestamp(DBPROCESS far *,INT);
extern DBBINARY far * SQLAPI dbtxtsnewval(DBPROCESS far *);
extern	RETCODE SQLAPI dbsetmaxprocs(SHORT);
extern	SHORT SQLAPI dbgetmaxprocs(void);
extern	BOOL  SQLAPI dbiscount(DBPROCESS far *);

/* two-phase commit functions */
extern DBPROCESS far * SQLAPI open_commit(LOGINREC far *,CHAR far *);
extern void	       SQLAPI close_commit(DBPROCESS far *);
extern void	       SQLAPI build_xact_string(CHAR far *,CHAR far *,DBINT,CHAR far *);
extern DBINT	       SQLAPI start_xact(DBPROCESS far *,CHAR far *,CHAR far *,INT);
extern RETCODE	       SQLAPI commit_xact(DBPROCESS far *,DBINT);
extern RETCODE	       SQLAPI abort_xact(DBPROCESS far *,DBINT);
extern RETCODE	       SQLAPI remove_xact(DBPROCESS far *,DBINT,INT);
extern RETCODE	       SQLAPI scan_xact(DBPROCESS far *,DBINT);
extern INT	       SQLAPI stat_xact(DBPROCESS far *,DBINT);

/* bulk copy prototypes */
extern DBINT   SQLAPI bcp_batch(DBPROCESS far *);
extern RETCODE SQLAPI bcp_bind(DBPROCESS far *, BYTE far *, INT, DBINT, BYTE far *, INT, INT, INT);
extern RETCODE SQLAPI bcp_colfmt(DBPROCESS far *, INT, BYTE, INT, DBINT, BYTE far *, INT, INT);
extern RETCODE SQLAPI bcp_collen(DBPROCESS far *, DBINT, INT);
extern RETCODE SQLAPI bcp_colptr(DBPROCESS far *, BYTE far *, INT);
extern RETCODE SQLAPI bcp_columns(DBPROCESS far *, INT);
extern RETCODE SQLAPI bcp_control(DBPROCESS far *, INT, DBINT);
extern DBINT   SQLAPI bcp_done(DBPROCESS far *);
extern RETCODE SQLAPI bcp_exec(DBPROCESS far *, DBINT far *);
extern RETCODE SQLAPI bcp_init(DBPROCESS far *, CHAR far *, CHAR far *, CHAR far *, INT);
extern RETCODE SQLAPI bcp_moretext(DBPROCESS far *, DBINT, BYTE far *);
extern RETCODE SQLAPI bcp_sendrow(DBPROCESS far *);
extern RETCODE SQLAPI bcp_setl(LOGINREC far *, DBBOOL);
/* Version 4.2 additions */
extern RETCODE SQLAPI bcp_readfmt(DBPROCESS far *, CHAR far *);
extern RETCODE SQLAPI bcp_writefmt(DBPROCESS far *, CHAR far *);

/* Functions for referencing the dbproc */
RETCODE SQLAPI dbcmdrow(DBPROCESS far *);
DBINT	SQLAPI dbcount(DBPROCESS far *);
INT	SQLAPI dbcurcmd(DBPROCESS far *);
DBINT	SQLAPI dbcurrow(DBPROCESS far *);
DBUBOOL SQLAPI dbdead(DBPROCESS far *);
DBINT	SQLAPI dbfirstrow(DBPROCESS far *);
DBUBOOL SQLAPI dbisavail(DBPROCESS far *);
void	SQLAPI dbsetavail(DBPROCESS far *);
DBINT	SQLAPI dblastrow(DBPROCESS far *);
RETCODE SQLAPI dbmorecmds(DBPROCESS far *);
INT	SQLAPI dbnumorders(DBPROCESS far *);
RETCODE SQLAPI dbrows(DBPROCESS far *);
STATUS	SQLAPI dbrowtype(DBPROCESS far *);

/* TDS version 4 extensions */
extern	DBINT SQLAPI dbreadtext(DBPROCESS far *,void far *, DBINT);
extern	DBINT SQLAPI dbaltutype(DBPROCESS far *,SHORT,SHORT);
extern	RETCODE SQLAPI dbanullbind(DBPROCESS far *,INT,INT,DBINT far *);
extern	RETCODE SQLAPI dbnullbind(DBPROCESS far *,INT,DBINT far *);
extern	RETCODE SQLAPI dbdatecrack(DBPROCESS far *, DBDATEREC far *, DBDATETIME far *);
extern	DBBOOL SQLAPI  dbvarylen(DBPROCESS far *, INT);

/* Cursor related functions */
extern	DBCURSOR * SQLAPI dbcursoropen(DBPROCESS far *, CHAR far *,
		      INT,INT,UINT, DBINT far *);
extern	RETCODE SQLAPI dbcursorinfo(DBCURSOR far *, int far *, DBINT far *);
extern	RETCODE SQLAPI dbcursorcolinfo(DBCURSOR far *, int, char far *, int far *,
	     DBINT far *, int far *);
extern	RETCODE SQLAPI dbcursorbind(DBCURSOR far *, int, int, DBINT, DBINT far *, CHAR far *);
extern	RETCODE SQLAPI dbcursorfetch(DBCURSOR far *,INT,INT);
extern	RETCODE SQLAPI dbcursor(DBCURSOR far *, INT,INT,CHAR far *,CHAR far *);
extern	RETCODE SQLAPI dbcursorclose(DBCURSOR far *);


/* server enumeration function */
extern int SQLAPI dbserverenum(USHORT,CHAR far *,USHORT,USHORT far *);

#endif		/* DBMSOS2 */

#ifdef DBMSWIN
#ifndef  NEAR
#define  NEAR
#endif
/* Dblib function prototypes */
extern LOGINREC NEAR *	SQLAPI dblogin(void);
extern DBPROCESS NEAR * SQLAPI dbopen(LOGINREC NEAR *,CHAR far *);

FARPROC dberrhandle(FARPROC);
FARPROC dbmsghandle(FARPROC);

extern BYTE far * SQLAPI dbinit(void);

extern void    SQLAPI dbwinexit(void);
extern RETCODE SQLAPI dbsetlname(LOGINREC NEAR *,CHAR far *,INT);
extern void    SQLAPI dbfreelogin(LOGINREC NEAR *);
extern RETCODE SQLAPI dbresults(DBPROCESS NEAR *);
extern STATUS  SQLAPI dbnextrow(DBPROCESS NEAR *);
extern void    SQLAPI dbclose(DBPROCESS NEAR *);
extern RETCODE SQLAPI dbcmd(DBPROCESS NEAR *,CHAR far *);
extern RETCODE SQLAPI dbfcmd(DBPROCESS NEAR *, CHAR far *, ...);
extern INT   SQLAPI dbstrlen(DBPROCESS NEAR *);
extern RETCODE SQLAPI dbstrcpy(DBPROCESS NEAR *,INT,INT,CHAR far *);
extern RETCODE SQLAPI dbsqlexec(DBPROCESS NEAR *);
extern RETCODE SQLAPI dbsqlok(DBPROCESS NEAR *);
extern RETCODE SQLAPI dbsqlsend(DBPROCESS NEAR *);
extern RETCODE SQLAPI dbcanquery(DBPROCESS NEAR *);
extern STATUS  SQLAPI dbgetrow(DBPROCESS NEAR *,DBINT);
extern void    SQLAPI dbclrbuf(DBPROCESS NEAR *,DBINT);
extern void    SQLAPI dbfreebuf(DBPROCESS NEAR *);
extern RETCODE SQLAPI dbsettime(INT);
extern INT     SQLAPI dbgettime(void);
extern RETCODE SQLAPI dbsetlogintime(INT);
extern DBUBOOL SQLAPI dbdataready(DBPROCESS NEAR *);
extern RETCODE SQLAPI dbcancel(DBPROCESS NEAR *);
extern void    SQLAPI dbexit(void);
extern RETCODE SQLAPI dbsetopt(DBPROCESS NEAR *,INT,CHAR far *);
extern DBUBOOL SQLAPI dbisopt(DBPROCESS NEAR *, INT, CHAR far *);
extern RETCODE SQLAPI dbclropt(DBPROCESS NEAR *,INT,CHAR far *);
extern INT   SQLAPI dbordercol(DBPROCESS NEAR *,INT);
extern INT   SQLAPI dbconvert(DBPROCESS NEAR *,INT,BYTE far *,DBINT,INT,BYTE far *,DBINT);
extern DBINT   SQLAPI dbcollen(DBPROCESS NEAR *,INT);
extern INT   SQLAPI dbcoltype(DBPROCESS NEAR *,INT);
extern DBINT   SQLAPI dbcolutype(DBPROCESS NEAR *,INT);
extern DBINT   SQLAPI dbdatlen(DBPROCESS NEAR *,INT);
extern INT   SQLAPI dbnumcols(DBPROCESS NEAR *);
extern DBUBOOL SQLAPI dbcolbrowse(DBPROCESS NEAR *,INT);
extern DBUBOOL SQLAPI dbtabbrowse(DBPROCESS NEAR *,INT);
extern INT   SQLAPI dbtabcount(DBPROCESS NEAR *);
extern INT   SQLAPI dbtsnewlen(DBPROCESS NEAR *);
extern RETCODE SQLAPI dbtsput(DBPROCESS NEAR *,DBBINARY far *,INT,INT,CHAR far *);
extern void    SQLAPI dbfreequal(CHAR far *);
extern void    SQLAPI dbprhead(DBPROCESS NEAR *);
extern RETCODE SQLAPI dbprrow(DBPROCESS NEAR *);
extern RETCODE SQLAPI dbtxtsput(DBPROCESS NEAR *,DBBINARY far *,INT);
extern RETCODE SQLAPI dbwritetext(DBPROCESS NEAR *,CHAR far *,DBBINARY far *,DBTINYINT, DBBINARY far *,DBBOOL,DBINT,BYTE far *);
extern RETCODE SQLAPI dbmoretext(DBPROCESS NEAR *,DBINT,BYTE far *);
extern RETCODE SQLAPI dbuse(DBPROCESS NEAR *,CHAR far *);
extern RETCODE SQLAPI dbbind(DBPROCESS NEAR *,INT,INT,DBINT,BYTE far *);
extern DBUBOOL SQLAPI dbwillconvert(INT,INT);
extern RETCODE SQLAPI dbsetnull(DBPROCESS NEAR *, INT, INT, BYTE far *);
extern RETCODE SQLAPI dbaltbind(DBPROCESS NEAR *,INT,INT,INT,DBINT,BYTE far *);
extern DBINT   SQLAPI dbaltlen(DBPROCESS NEAR *,INT,INT);
extern INT   SQLAPI dbalttype(DBPROCESS NEAR *,INT,INT);
extern DBINT   SQLAPI dbadlen(DBPROCESS NEAR *,INT,INT);
extern INT   SQLAPI dbnumalts(DBPROCESS NEAR *,INT);
extern INT   SQLAPI dbaltcolid(DBPROCESS NEAR *,INT,INT);
extern INT   SQLAPI dbaltop(DBPROCESS NEAR *,INT,INT);
extern INT   SQLAPI dbnumcompute(DBPROCESS NEAR *);
extern INT   SQLAPI dbgetoff(DBPROCESS NEAR *,DBUSMALLINT,INT);
extern DBINT   SQLAPI dbreadpage(DBPROCESS NEAR *,CHAR far *,DBINT,BYTE far *);
extern RETCODE SQLAPI dbwritepage(DBPROCESS NEAR *,CHAR far *,DBINT,DBINT,BYTE far *);

/* remote procedure call functions, these are version 4.2 extensions */
extern DBBOOL  SQLAPI dbhasretstat(DBPROCESS NEAR *);
extern INT   SQLAPI dbnumrets(DBPROCESS NEAR *);
extern DBINT   SQLAPI dbretlen(DBPROCESS NEAR *, INT);
extern DBINT   SQLAPI dbretstatus(DBPROCESS NEAR *);
extern INT   SQLAPI dbrettype(DBPROCESS NEAR *, INT);
extern RETCODE SQLAPI dbrpcinit(DBPROCESS NEAR *, CHAR far *, DBSMALLINT);
extern RETCODE SQLAPI dbrpcparam(DBPROCESS*,CHAR far *,BYTE,INT,DBINT,DBINT,BYTE far*);
extern RETCODE SQLAPI dbrpcsend(DBPROCESS NEAR *);
extern void    SQLAPI dbrpwclr(LOGINREC NEAR *);
extern RETCODE SQLAPI dbrpwset(LOGINREC NEAR *, CHAR far *, CHAR far *, INT);
extern CHAR far * SQLAPI dbretname(DBPROCESS NEAR *, INT);
extern BYTE far * SQLAPI dbretdata(DBPROCESS NEAR *, INT);
extern	void SQLAPI dbsetuserdata(DBPROCESS NEAR *, void far *);
extern	void * SQLAPI dbgetuserdata(DBPROCESS NEAR *);

extern CHAR far * SQLAPI dbprtype(INT);
extern CHAR far * SQLAPI dbchange(DBPROCESS NEAR *);
extern CHAR far * SQLAPI dbname(DBPROCESS NEAR *);
extern BYTE far * SQLAPI dbbylist(DBPROCESS NEAR *,INT,INT far *);
extern BYTE far * SQLAPI dbadata(DBPROCESS NEAR *,INT,INT);
extern CHAR far * SQLAPI dbqual(DBPROCESS NEAR *,INT,CHAR far *);
extern CHAR far * SQLAPI dbtabsource(DBPROCESS NEAR *, INT, INT far *);
extern CHAR far * SQLAPI dbtabname(DBPROCESS NEAR *,INT);
extern CHAR far * SQLAPI dbcolsource(DBPROCESS NEAR *,INT);
extern BYTE far * SQLAPI dbdata(DBPROCESS NEAR *,INT);
extern BYTE far * SQLAPI dbcolname(DBPROCESS NEAR *,INT);
extern CHAR far * SQLAPI dbgetchar(DBPROCESS NEAR *, INT);

extern DBBINARY far * SQLAPI dbtsnewval(DBPROCESS NEAR *);
extern DBBINARY far * SQLAPI dbtxptr(DBPROCESS NEAR *,INT);
extern DBBINARY far * SQLAPI dbtxtimestamp(DBPROCESS NEAR *,INT);
extern DBBINARY far * SQLAPI dbtxtsnewval(DBPROCESS NEAR *);
extern	RETCODE SQLAPI dbsetmaxprocs(SHORT);
extern	SHORT SQLAPI dbgetmaxprocs(void);
extern	BOOL  SQLAPI dbiscount(DBPROCESS NEAR *);

/* two-phase commit functions */
extern DBPROCESS NEAR * SQLAPI open_commit(LOGINREC NEAR *,CHAR far *);
extern void	   SQLAPI close_commit(DBPROCESS NEAR *);
extern void	   SQLAPI build_xact_string(CHAR far *,CHAR far *,DBINT,CHAR far *);
extern DBINT	   SQLAPI start_xact(DBPROCESS NEAR *,CHAR far *,CHAR far *,INT);
extern RETCODE	   SQLAPI commit_xact(DBPROCESS NEAR *,DBINT);
extern RETCODE	   SQLAPI abort_xact(DBPROCESS NEAR *,DBINT);
extern RETCODE	   SQLAPI remove_xact(DBPROCESS NEAR *,DBINT,INT);
extern RETCODE	   SQLAPI scan_xact(DBPROCESS NEAR *,DBINT);
extern INT	   SQLAPI stat_xact(DBPROCESS NEAR *,DBINT);

/* bulk copy prototypes */
extern DBINT   SQLAPI bcp_batch(DBPROCESS NEAR *);
extern RETCODE SQLAPI bcp_bind(DBPROCESS NEAR *, BYTE far *, INT, DBINT, BYTE far *, INT, INT, INT);
extern RETCODE SQLAPI bcp_colfmt(DBPROCESS NEAR *, INT, BYTE, INT, DBINT, BYTE far *, INT, INT);
extern RETCODE SQLAPI bcp_collen(DBPROCESS NEAR *, DBINT, INT);
extern RETCODE SQLAPI bcp_colptr(DBPROCESS NEAR *, BYTE far *, INT);
extern RETCODE SQLAPI bcp_columns(DBPROCESS NEAR *, INT);
extern RETCODE SQLAPI bcp_control(DBPROCESS NEAR *, INT, DBINT);
extern DBINT   SQLAPI bcp_done(DBPROCESS NEAR *);
extern RETCODE SQLAPI bcp_exec(DBPROCESS NEAR *, DBINT far *);
extern RETCODE SQLAPI bcp_init(DBPROCESS NEAR *, CHAR far *, CHAR far *, CHAR far *, INT);
extern RETCODE SQLAPI bcp_moretext(DBPROCESS NEAR *, DBINT, BYTE far *);
extern RETCODE SQLAPI bcp_sendrow(DBPROCESS NEAR *);
extern RETCODE SQLAPI bcp_setl(LOGINREC NEAR *, DBBOOL);
/* Version 4.2 additions */
extern RETCODE SQLAPI bcp_readfmt(DBPROCESS NEAR *, CHAR far *);
extern RETCODE SQLAPI bcp_writefmt(DBPROCESS NEAR *, CHAR far *);

/* Functions for referencing the dbproc */
RETCODE SQLAPI dbcmdrow(DBPROCESS NEAR *);
DBINT	SQLAPI dbcount(DBPROCESS NEAR *);
INT	SQLAPI dbcurcmd(DBPROCESS NEAR *);
DBINT	SQLAPI dbcurrow(DBPROCESS NEAR *);
DBUBOOL SQLAPI dbdead(DBPROCESS NEAR *);
DBINT	SQLAPI dbfirstrow(DBPROCESS NEAR *);
DBUBOOL SQLAPI dbisavail(DBPROCESS NEAR *);
void	SQLAPI dbsetavail(DBPROCESS NEAR *);
DBINT	SQLAPI dblastrow(DBPROCESS NEAR *);
void	SQLAPI dblocklib( void );
RETCODE SQLAPI dbmorecmds(DBPROCESS NEAR *);
INT	SQLAPI dbnumorders(DBPROCESS NEAR *);
RETCODE SQLAPI dbrows(DBPROCESS NEAR *);
STATUS	SQLAPI dbrowtype(DBPROCESS NEAR *);
void	SQLAPI dbunlocklib( void );
/* Function to return the absolute address of the dbproc */
DBPROCESS far * SQLAPI dbabsolute(DBPROCESS NEAR *);

/* TDS version 4 extensions */
extern	DBINT SQLAPI dbreadtext(DBPROCESS NEAR *,void far *, DBINT);
extern	DBINT SQLAPI dbaltutype(DBPROCESS NEAR *,SHORT,SHORT);
extern	RETCODE SQLAPI dbanullbind(DBPROCESS NEAR *,INT,INT,DBINT far *);
extern	RETCODE SQLAPI dbnullbind(DBPROCESS NEAR *,INT,DBINT far *);
extern	RETCODE SQLAPI dbdatecrack(DBPROCESS NEAR *, DBDATEREC far *, DBDATETIME far *);
extern	DBBOOL SQLAPI  dbvarylen(DBPROCESS NEAR *, INT);

/* Cursor related functions */
extern	DBCURSOR * SQLAPI dbcursoropen(DBPROCESS NEAR *, CHAR far *,
	   INT,INT,UINT, DBINT far *);
extern	RETCODE SQLAPI dbcursorinfo(DBCURSOR NEAR *, int far *, DBINT far *);
extern	RETCODE SQLAPI dbcursorcolinfo(DBCURSOR NEAR *, int, char far *, int far *,
	     DBINT far *, int far *);
extern	RETCODE SQLAPI dbcursorbind(DBCURSOR NEAR *, int, int, DBINT, DBINT far *,
	   CHAR far *);
extern	RETCODE SQLAPI dbcursorfetch(DBCURSOR NEAR *,INT,INT);
extern	RETCODE SQLAPI dbcursor(DBCURSOR NEAR *, INT,INT,CHAR far *,CHAR far *);
extern	RETCODE SQLAPI dbcursorclose(DBCURSOR NEAR *);

/* server enumeration function */
extern int SQLAPI dbserverenum(USHORT,CHAR far *,USHORT,USHORT far *);

#endif	/* #ifdef DBMSWIN */

#ifdef DBNTWIN32

#ifndef FARPROC
typedef  int (SQLAPI *FARPROC)();
#endif
/* Dblib function prototypes */
extern LOGINREC *  SQLAPI dblogin(void);
extern DBPROCESS * SQLAPI dbopen(LOGINREC *,CHAR *);

FARPROC dberrhandle(FARPROC);
FARPROC dbmsghandle(FARPROC);

extern BYTE * SQLAPI dbinit(void);

extern void    SQLAPI dbwinexit(void);
extern RETCODE SQLAPI dbsetlname(LOGINREC *,CHAR *,INT);
extern void    SQLAPI dbfreelogin(LOGINREC *);
extern RETCODE SQLAPI dbresults(DBPROCESS *);
extern STATUS  SQLAPI dbnextrow(DBPROCESS *);
extern void    SQLAPI dbclose(DBPROCESS  *);
extern RETCODE SQLAPI dbcmd(DBPROCESS *,CHAR *);
extern RETCODE SQLAPI dbfcmd(DBPROCESS *, CHAR *, ...);
extern INT   SQLAPI dbstrlen(DBPROCESS *);
extern RETCODE SQLAPI dbstrcpy(DBPROCESS *,INT,INT,CHAR *);
extern RETCODE SQLAPI dbsqlexec(DBPROCESS *);
extern RETCODE SQLAPI dbsqlok(DBPROCESS *);
extern RETCODE SQLAPI dbsqlsend(DBPROCESS *);
extern RETCODE SQLAPI dbcanquery(DBPROCESS *);
extern STATUS  SQLAPI dbgetrow(DBPROCESS *,DBINT);
extern void    SQLAPI dbclrbuf(DBPROCESS *,DBINT);
extern void    SQLAPI dbfreebuf(DBPROCESS *);
extern RETCODE SQLAPI dbsettime(INT);
extern INT     SQLAPI dbgettime(void);
extern RETCODE SQLAPI dbsetlogintime(INT);
extern DBUBOOL SQLAPI dbdataready(DBPROCESS *);
extern RETCODE SQLAPI dbcancel(DBPROCESS *);
extern void    SQLAPI dbexit(void);
extern RETCODE SQLAPI dbsetopt(DBPROCESS *,INT,CHAR *);
extern DBUBOOL SQLAPI dbisopt(DBPROCESS *, INT, CHAR *);
extern RETCODE SQLAPI dbclropt(DBPROCESS *,INT,CHAR *);
extern INT   SQLAPI dbordercol(DBPROCESS *,INT);
extern INT   SQLAPI dbconvert(DBPROCESS *,INT,BYTE *,DBINT,INT,BYTE *,DBINT);
extern DBINT   SQLAPI dbcollen(DBPROCESS *,INT);
extern INT   SQLAPI dbcoltype(DBPROCESS *,INT);
extern DBINT   SQLAPI dbcolutype(DBPROCESS *,INT);
extern DBINT   SQLAPI dbdatlen(DBPROCESS *,INT);
extern INT   SQLAPI dbnumcols(DBPROCESS *);
extern DBUBOOL SQLAPI dbcolbrowse(DBPROCESS *,INT);
extern DBUBOOL SQLAPI dbtabbrowse(DBPROCESS *,INT);
extern INT   SQLAPI dbtabcount(DBPROCESS *);
extern INT   SQLAPI dbtsnewlen(DBPROCESS *);
extern RETCODE SQLAPI dbtsput(DBPROCESS *,DBBINARY *,INT,INT,CHAR *);
extern void    SQLAPI dbfreequal(CHAR *);
extern void    SQLAPI dbprhead(DBPROCESS *);
extern RETCODE SQLAPI dbprrow(DBPROCESS *);
extern RETCODE SQLAPI dbtxtsput(DBPROCESS *,DBBINARY *,INT);
extern RETCODE SQLAPI dbwritetext(DBPROCESS *,CHAR *,DBBINARY *,DBTINYINT, DBBINARY *,DBBOOL,DBINT,BYTE *);
extern RETCODE SQLAPI dbmoretext(DBPROCESS *,DBINT,BYTE *);
extern RETCODE SQLAPI dbuse(DBPROCESS *,CHAR *);
extern RETCODE SQLAPI dbbind(DBPROCESS *,INT,INT,DBINT,BYTE *);
extern DBUBOOL SQLAPI dbwillconvert(INT,INT);
extern RETCODE SQLAPI dbsetnull(DBPROCESS *, INT, INT, BYTE *);
extern RETCODE SQLAPI dbaltbind(DBPROCESS *,INT,INT,INT,DBINT,BYTE *);
extern DBINT   SQLAPI dbaltlen(DBPROCESS *,INT,INT);
extern INT   SQLAPI dbalttype(DBPROCESS *,INT,INT);
extern DBINT   SQLAPI dbadlen(DBPROCESS *,INT,INT);
extern INT   SQLAPI dbnumalts(DBPROCESS *,INT);
extern INT   SQLAPI dbaltcolid(DBPROCESS *,INT,INT);
extern INT   SQLAPI dbaltop(DBPROCESS *,INT,INT);
extern INT   SQLAPI dbnumcompute(DBPROCESS *);
extern INT   SQLAPI dbgetoff(DBPROCESS *,DBUSMALLINT,INT);
extern DBINT   SQLAPI dbreadpage(DBPROCESS *,CHAR *,DBINT,BYTE *);
extern RETCODE SQLAPI dbwritepage(DBPROCESS *,CHAR *,DBINT,DBINT,BYTE *);

/* remote procedure call functions, these are version 4.2 extensions */
extern DBBOOL  SQLAPI dbhasretstat(DBPROCESS *);
extern INT   SQLAPI dbnumrets(DBPROCESS *);
extern DBINT   SQLAPI dbretlen(DBPROCESS *, INT);
extern DBINT   SQLAPI dbretstatus(DBPROCESS *);
extern INT   SQLAPI dbrettype(DBPROCESS *, INT);
extern RETCODE SQLAPI dbrpcinit(DBPROCESS *, CHAR *, DBSMALLINT);
extern RETCODE SQLAPI dbrpcparam(DBPROCESS*,CHAR *,BYTE,INT,DBINT,DBINT,BYTE *);
extern RETCODE SQLAPI dbrpcsend(DBPROCESS *);
extern void    SQLAPI dbrpwclr(LOGINREC *);
extern RETCODE SQLAPI dbrpwset(LOGINREC *, CHAR *, CHAR *, INT);
extern CHAR * SQLAPI dbretname(DBPROCESS *, INT);
extern BYTE * SQLAPI dbretdata(DBPROCESS *, INT);
extern	void SQLAPI dbsetuserdata(DBPROCESS *, void *);
extern	void * SQLAPI dbgetuserdata(DBPROCESS *);

extern CHAR * SQLAPI dbprtype(INT);
extern CHAR * SQLAPI dbchange(DBPROCESS *);
extern CHAR * SQLAPI dbname(DBPROCESS *);
extern BYTE * SQLAPI dbbylist(DBPROCESS *,INT,INT *);
extern BYTE * SQLAPI dbadata(DBPROCESS *,INT,INT);
extern CHAR * SQLAPI dbqual(DBPROCESS *,INT,CHAR *);
extern CHAR * SQLAPI dbtabsource(DBPROCESS *, INT, INT *);
extern CHAR * SQLAPI dbtabname(DBPROCESS *,INT);
extern CHAR * SQLAPI dbcolsource(DBPROCESS *,INT);
extern BYTE * SQLAPI dbdata(DBPROCESS *,INT);
extern BYTE * SQLAPI dbcolname(DBPROCESS *,INT);
extern CHAR * SQLAPI dbgetchar(DBPROCESS *, INT);

extern DBBINARY * SQLAPI dbtsnewval(DBPROCESS *);
extern DBBINARY * SQLAPI dbtxptr(DBPROCESS *,INT);
extern DBBINARY * SQLAPI dbtxtimestamp(DBPROCESS *,INT);
extern DBBINARY * SQLAPI dbtxtsnewval(DBPROCESS *);
extern	RETCODE SQLAPI dbsetmaxprocs(SHORT);
extern	SHORT SQLAPI dbgetmaxprocs(void);
extern	BOOL  SQLAPI dbiscount(DBPROCESS *);

/* two-phase commit functions */
extern DBPROCESS * SQLAPI open_commit(LOGINREC *,CHAR *);
extern void	   SQLAPI close_commit(DBPROCESS *);
extern void	   SQLAPI build_xact_string(CHAR *,CHAR *,DBINT,CHAR *);
extern DBINT	   SQLAPI start_xact(DBPROCESS *,CHAR *,CHAR *,INT);
extern RETCODE	   SQLAPI commit_xact(DBPROCESS *,DBINT);
extern RETCODE	   SQLAPI abort_xact(DBPROCESS *,DBINT);
extern RETCODE	   SQLAPI remove_xact(DBPROCESS *,DBINT,INT);
extern RETCODE	   SQLAPI scan_xact(DBPROCESS *,DBINT);
extern INT	   SQLAPI stat_xact(DBPROCESS *,DBINT);

/* bulk copy prototypes */
extern DBINT   SQLAPI bcp_batch(DBPROCESS *);
extern RETCODE SQLAPI bcp_bind(DBPROCESS *, BYTE *, INT, DBINT, BYTE *, INT, INT, INT);
extern RETCODE SQLAPI bcp_colfmt(DBPROCESS *, INT, BYTE, INT, DBINT, BYTE *, INT, INT);
extern RETCODE SQLAPI bcp_collen(DBPROCESS *, DBINT, INT);
extern RETCODE SQLAPI bcp_colptr(DBPROCESS *, BYTE *, INT);
extern RETCODE SQLAPI bcp_columns(DBPROCESS *, INT);
extern RETCODE SQLAPI bcp_control(DBPROCESS *, INT, DBINT);
extern DBINT   SQLAPI bcp_done(DBPROCESS *);
extern RETCODE SQLAPI bcp_exec(DBPROCESS *, DBINT *);
extern RETCODE SQLAPI bcp_init(DBPROCESS *, CHAR *, CHAR *, CHAR *, INT);
extern RETCODE SQLAPI bcp_moretext(DBPROCESS *, DBINT, BYTE *);
extern RETCODE SQLAPI bcp_sendrow(DBPROCESS *);
extern RETCODE SQLAPI bcp_setl(LOGINREC *, DBBOOL);
/* Version 4.2 additions */
extern RETCODE SQLAPI bcp_readfmt(DBPROCESS *, CHAR *);
extern RETCODE SQLAPI bcp_writefmt(DBPROCESS *, CHAR  *);

/* Functions for referencing the dbproc */
RETCODE SQLAPI dbcmdrow(DBPROCESS *);
DBINT	SQLAPI dbcount(DBPROCESS *);
INT	SQLAPI dbcurcmd(DBPROCESS *);
DBINT	SQLAPI dbcurrow(DBPROCESS *);
DBUBOOL SQLAPI dbdead(DBPROCESS *);
DBINT	SQLAPI dbfirstrow(DBPROCESS *);
DBUBOOL SQLAPI dbisavail(DBPROCESS *);
void	SQLAPI dbsetavail(DBPROCESS *);
DBINT	SQLAPI dblastrow(DBPROCESS *);
void	SQLAPI dblocklib( void );
RETCODE SQLAPI dbmorecmds(DBPROCESS *);
INT	SQLAPI dbnumorders(DBPROCESS *);
RETCODE SQLAPI dbrows(DBPROCESS *);
STATUS	SQLAPI dbrowtype(DBPROCESS *);
void	SQLAPI dbunlocklib( void );
/* Function to return the absolute address of the dbproc */
DBPROCESS * SQLAPI dbabsolute(DBPROCESS *);

/* TDS version 4 extensions */
extern	DBINT SQLAPI dbreadtext(DBPROCESS *,void *, DBINT);
extern	DBINT SQLAPI dbaltutype(DBPROCESS *,SHORT,SHORT);
extern	RETCODE SQLAPI dbanullbind(DBPROCESS *,SHORT,SHORT,DBINT  *);
extern	RETCODE SQLAPI dbnullbind(DBPROCESS *,SHORT,DBINT *);
extern	RETCODE SQLAPI dbdatecrack(DBPROCESS *, DBDATEREC *, DBDATETIME *);
extern	DBBOOL SQLAPI  dbvarylen(DBPROCESS *, INT);

/* Cursor related functions */
extern	DBCURSOR * SQLAPI dbcursoropen(DBPROCESS *, CHAR  *,
	   INT,INT,UINT, DBINT *);
extern	RETCODE SQLAPI dbcursorinfo(DBCURSOR *, int *, DBINT *);
extern	RETCODE SQLAPI dbcursorcolinfo(DBCURSOR *, int, char *, int *,
	     DBINT *, int *);
extern	RETCODE SQLAPI dbcursorbind(DBCURSOR *, int, int, DBINT, DBINT *,
	   CHAR *);
extern	RETCODE SQLAPI dbcursorfetch(DBCURSOR *,INT,INT);
extern	RETCODE SQLAPI dbcursor(DBCURSOR *, INT,INT,CHAR *,CHAR *);
extern	RETCODE SQLAPI dbcursorclose(DBCURSOR *);

/* server enumeration function */
extern int SQLAPI dbserverenum(USHORT,CHAR far *,USHORT,USHORT far *);
#endif	/* #ifdef DBNTWIN32 */
