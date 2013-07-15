/* SQLFRONT.H - DB-Library header file for the Microsoft SQL Server.
**	     The SYBASE SQL Server database for PC networks.
**
** All constant and macro definitions for DB-Library applications
** programming are contained in this file.  This file MUST be included
** before SQLDB.H and one of the following #defines made: DBMSDOS, DBMSOS2,
** DBNTWIN32 and DBMSWIN (for the appropriate operating platform). When
** compiling a WLO 1.0 application (using Windows libraries for os/2),
** use DBMSWIN.
**
** Copyright (c) 1989, 1991 by Microsoft Corp.	All rights reserved.
** Copyright (c) 1989, 1991 by Sybase, Inc.  All rights reserved.
**
*/

/* Note this has changed because Windows 3.1 defines API as 'pascal far' */

#if !defined(M_I86SM)  && !defined(DBNTWIN32)
#define SQLAPI			cdecl far
#else
#define SQLAPI			cdecl
#endif

#ifndef  API
#define  API  SQLAPI
#endif

/* define datatypes used by dblib for portability */

#define DBUBOOL 	       unsigned int

#ifndef SHORT
#define SHORT			short
#endif

#ifndef INT
#define INT			int
#endif

#ifndef UINT
#define UINT			unsigned int
#endif

#ifndef USHORT
#define USHORT 			unsigned short
#endif

#ifndef ULONG
#define ULONG 			unsigned long
#endif

#ifndef CHAR
#define CHAR			char
#endif

#ifndef DOUBLE
#define DOUBLE			double
#endif

#ifndef OS2_INCLUDED
typedef unsigned char           BYTE;
#endif

#if !defined(OS2_INCLUDED) && !defined(DBMSWIN) && !defined(DBNTWIN32)
typedef DBUBOOL 		BOOL;
#endif

#ifndef TRUE
#define TRUE                    1
#endif

#ifndef FALSE
#define FALSE                   0
#endif

#define TINYBIND	1
#define SMALLBIND       2
#define INTBIND         3
#define CHARBIND        4
#define BINARYBIND	5
#define BITBIND 	6
#define DATETIMEBIND	7
#define MONEYBIND	8
#define FLT8BIND	9
#define STRINGBIND	10
#define NTBSTRINGBIND	11
#define VARYCHARBIND	12
#define VARYBINBIND	13

/* These are version 4.2 additions */
#define FLT4BIND	14
#define SMALLMONEYBIND	15
#define SMALLDATETIBIND 16
#define  MAXBIND    SMALLDATETIBIND
/* #define MAXBIND	   VARYBINBIND	*/


#define DBSAVE          1
#define DBNOSAVE        0

#define DBNOERR         -1
#define DBFINDONE       0x04            /* definately done		*/
#define DBMORE          0x10            /* maybe more commands waiting  */
#define DBMORE_ROWS     0x20            /* this command returned rows   */

#define MAXNAME                 31

/* Timestamp length */
#define  DBTXTSLEN              8       /* length of text timestamp	*/

/* Text pointer length */
#define  DBTXPLEN               16

/* Error code returns */
#define INT_EXIT                0
#define INT_CONTINUE            1
#define INT_CANCEL              2

/* Server buffer size */
#define SERVBSIZE               512
#define DBNETCOUNT		10

/* Dboptions */
#define DBMAXOPTS		14

/* dboptions */
#define DBBUFFER                0
#define DBOFFSET                1
#define DBROWCOUNT              2
#define DBSTAT                  3
#define DBTEXTLIMIT             4
#define DBTEXTSIZE              5
#define DBARITHABORT            6
#define DBARITHIGNORE           7
#define DBNOAUTOFREE            8
#define DBNOCOUNT               9
#define DBNOEXEC                10
#define DBPARSEONLY             11
#define DBSHOWPLAN              12
#define DBSTORPROCID		13
    /* The following is a version 4.2 enhancement */
#define DBANSItoOEM		14	/* Windows only option */

/* Data Type Tokens. */
# define SQLTEXT		0x23
# define SQLARRAY		0x24
# define SQLVARBINARY		0x25
# define SQLINTN		0x26
# define SQLVARCHAR		0x27
# define SQLBINARY		0x2d
# define SQLIMAGE		0x22
# define SQLCHAR		0x2f
# define SQLINT1		0x30
# define SQLBIT 		0x32
# define SQLINT2		0x34
# define SQLINT4		0x38
# define SQLMONEY		0x3c
# define SQLDATETIME		0x3d
# define SQLFLT8		0x3e
# define SQLFLTN		0x6d
# define SQLMONEYN		0x6e
# define SQLDATETIMN		0x6f

/* These are version 4.2 additions */
# define SQLFLT4		0x3b
# define SQLMONEY4		0x7a
# define SQLDATETIM4		0x3a


/* Data stream tokens */
# define SQLCOLFMT		0xa1
# define OLD_SQLCOLFMT		0x2a
# define SQLPROCID		0x7c
# define SQLCOLNAME		0xa0
# define SQLTABNAME		0xa4
# define SQLCOLINFO		0xa5
# define SQLALTNAME		0xa7
# define SQLALTFMT		0xa8
# define SQLERROR		0xaa
# define SQLINFO		0xab
# define SQLRETURNVALUE 	0xac
# define SQLRETURNSTATUS        0x79
# define SQLRETURN		0xdb
# define SQLCONTROL		0xae
# define SQLALTCONTROL		0xaf
# define SQLROW 		0xd1
# define SQLALTROW		0xd3
# define SQLDONE		0xfd
# define SQLDONEPROC		0xfe
# define SQLDONEINPROC		0xff
# define SQLOFFSET		0x78
# define SQLORDER               0xa9
# define SQLLOGINACK		0xad	/* >>> NOTICE: change to real value */

/* Ag op tokens */
# define SQLAOPCNT		0x4b
# define SQLAOPSUM		0x4d
# define SQLAOPAVG		0x4f
# define SQLAOPMIN		0x51
# define SQLAOPMAX		0x52
# define SQLAOPANY		0x53
# define SQLAOPNOOP		0x56

/* error numbers (dberrs) DB-Library error codes */
#define SQLEMEM         10000
#define SQLENULL        10001
#define SQLENLOG        10002
#define SQLEPWD         10003
#define SQLECONN        10004
#define SQLEDDNE        10005
#define SQLENULLO       10006
#define SQLESMSG        10007
#define SQLEBTOK        10008
#define SQLENSPE        10009
#define SQLEREAD        10010
#define SQLECNOR        10011
#define SQLETSIT        10012
#define SQLEPARM        10013
#define SQLEAUTN        10014
#define SQLECOFL        10015
#define SQLERDCN        10016
#define SQLEICN         10017
#define SQLECLOS        10018
#define SQLENTXT        10019  /* new error number/message */
#define SQLEDNTI        10020
#define SQLETMTD        10021
#define SQLEASEC        10022
#define SQLENTLL        10023
#define SQLETIME        10024
#define SQLEWRIT        10025
#define SQLEMODE        10026
#define SQLEOOB         10027
#define SQLEITIM        10028
#define SQLEDBPS        10029
#define SQLEIOPT        10030
#define SQLEASNL        10031
#define SQLEASUL        10032
#define SQLENPRM        10033
#define SQLEDBOP        10034
#define SQLENSIP        10035
#define SQLECNULL       10036
#define SQLESEOF	10037
#define SQLERPND	10038
#define SQLECSYN	10039
#define SQLENONET	10040
#define SQLEBTYP	10041
#define SQLEABNC	10042
#define SQLEABMT	10043
#define SQLEABNP	10044
#define SQLEBNCR	10045
#define SQLEAAMT	10046
#define SQLENXID	10047
#define SQLEIFNB	10048
#define SQLEKBCO	10049
#define SQLEBBCI	10050
#define SQLEKBCI	10051
#define SQLEBCWE	10052
#define SQLEBCNN	10053
#define SQLEBCOR	10054
#define SQLEBCPI	10055
#define SQLEBCPN	10056
#define SQLEBCPB	10057
#define SQLEVDPT	10058
#define SQLEBIVI	10059
#define SQLEBCBC	10060
#define SQLEBCFO	10061
#define SQLEBCVH	10062
#define SQLEBCUO	10063
#define SQLEBUOE	10064
#define SQLEBWEF	10065
#define SQLEBTMT	10066
#define SQLEBEOF	10067
#define SQLEBCSI	10068
#define SQLEPNUL	10069
#define SQLEBSKERR	10070
#define SQLEBDIO	10071
#define SQLEBCNT	10072
#define SQLEMDBP	10073
#define SQLINIT 	10074
#define SQLCRSINV	10075
#define SQLCRSCMD	10076
#define SQLCRSNOIND	10077
#define SQLCRSDIS	10078
#define SQLCRSAGR	10079
#define SQLCRSORD	10080
#define SQLCRSMEM	10081
#define SQLCRSBSKEY	10082
#define SQLCRSNORES	10083
#define SQLCRSVIEW	10084
#define SQLCRSBUFR	10085
#define SQLCRSFROWN	10086
#define SQLCRSBROL	10087
#define SQLCRSFRAND	10088
#define SQLCRSFLAST	10089
#define SQLCRSRO	10090
#define SQLCRSTAB	10091
#define SQLCRSUPDTAB	10092
#define SQLCRSUPDNB	10093
#define SQLCRSVIIND	10094
#define SQLCRSNOUPD	10095
#define SQLCRSOS2	10096
#define SQLEBCSA        10097
#define SQLEBCRO	10098
#define SQLEBCNE	10099
#define SQLEBCSK	10100
#define SQLEUVBF	10101
#define SQLEBIHC	10102
#define SQLEBWFF	10103

/* The severity levels are defined here */
#define	EXINFO		1	/* informational, non-error */
#define	EXUSER		2	/* user error */
#define	EXNONFATAL	3	/* non-fatal error */
#define EXCONVERSION	4	/* Error in DB-LIBRARY data conversion. */
#define EXSERVER	5	/* The Server has returned an error flag. */
#define EXTIME		6	/* We have exceeded our timeout period while
				 * waiting for a response from the Server -
				 * the DBPROCESS is still alive.
				 */
#define EXPROGRAM	7	/* coding error in user program */
#define	EXRESOURCE	8	/* running out of resources - the DBPROCESS
				 * may be dead.
				 */
#define	EXCOMM		9	/* failure in communication with Server -
				 * the DBPROCESS is dead.
				 */
#define	EXFATAL		10	/* fatal error - the DBPROCESS is dead. */
#define	EXCONSISTENCY	11	/* internal software error  - notify MS
				 * Technical Supprt.
				 */


/* offset identifiers */
#define OFF_SELECT      0x16d
#define OFF_FROM        0x14f
#define OFF_ORDER       0x165
#define OFF_COMPUTE     0x139
#define OFF_TABLE       0x173
#define OFF_PROCEDURE   0x16a
#define OFF_STATEMENT   0x1cb
#define OFF_PARAM       0x1c4
#define OFF_EXEC	0x12c

/* print lengths for certain fixed length data types. */
#define PRINT4  	11
#define PRINT2  	6
#define PRINT1  	3
#define PRFLT8		20
#define PRMONEY 	26
#define PRBIT		3
#define PRDATETIME	27


#define MAXTYPES	11
#define DBMAXCHAR       256     /* max length of DBVARBINARY and DBVARCHAR,etc */

#ifndef DBTYPEDEFS	/* srv.h (Open Server include) not already included*/

#define DBTYPEDEFS

/* dblib datatypes */
typedef char            DBCHAR;
typedef unsigned char   DBBINARY;
typedef unsigned char   DBTINYINT;
typedef short           DBSMALLINT;
typedef unsigned short  DBUSMALLINT;
typedef long            DBINT;
typedef double          DBFLT8;
typedef unsigned char   DBBIT;
typedef unsigned char   DBBOOL;

/* These are version 4.2 additions */
typedef float  DBFLT4;
typedef long   DBMONEY4;

#define DBREAL	DBFLT4

typedef struct dbdatetime4
{
    unsigned short numdays;		 /* No of days since Jan-1-1900 */
    unsigned short nummins;		 /* No. of minutes since midnight */
}DBDATETIM4;



typedef struct dbvarychar
{
    DBSMALLINT  len;
    DBCHAR      str[DBMAXCHAR];
}DBVARYCHAR;

typedef struct dbvarybin
{
    DBSMALLINT  len;
    BYTE        array[DBMAXCHAR];
}DBVARYBIN;

typedef struct dbmoney
{
    DBINT mnyhigh;
    ULONG mnylow;
}DBMONEY;

typedef struct dbdatetime
{
    DBINT dtdays;
    ULONG dttime;
}DBDATETIME;

/* DBDATEREC structure used by dbdatecrack. This is a version 4.2 addition */
typedef struct dbdaterec
{
    int     year;	    /* 1753 - 9999 */
    int     quarter;	    /* 1 - 4 */
    int     month;	    /* 1 - 12 */
    int     dayofyear;	    /* 1 - 366 */
    int     day;	    /* 1 - 31 */
    int     week;	    /* 1 - 54 (for leap years) */
    int     weekday;	    /* 1 - 7  (Mon - Sun) */
    int     hour;	    /* 0 - 23 */
    int     minute;	    /* 0 - 59 */
    int     second;	    /* 0 - 59 */
    int     millisecond;    /* 0 - 999 */
} DBDATEREC;

#endif		/* end DBTYPEDEFS */

/* various datatype and return codes */
#define RETCODE		INT
#define STATUS          INT
#define SUCCEED		1
#define FAIL		0


#define MORE_ROWS	-1
#define NO_MORE_ROWS	-2
#define REG_ROW         MORE_ROWS
#define BUF_FULL	-3

/*
**  Status code for dbresults(). Possible return values are
**  SUCCEED, FAIL, and NO_MORE_RESULTS.
*/
#define NO_MORE_RESULTS 2

/* macros for dbsetlname() */
#define DBSETHOST	1
#define DBSETUSER	2
#define DBSETPWD	3
#define DBSETAPP	4
#define DBSETID         5
    /* The following is a version 4.2 addition */
#define DBSETLANG	6

/* standard exit and error values */
#define STDEXIT		0
#define ERREXIT		-1

/* The following are version 4.2 additions */

#define  DBRPCRECOMPILE     1
#define  DBRPCRETURN	    1


/* Cursor related constants */

/*
 ** Following flags are used in the concuropt parameter in the
 ** dbcursoropen function
 */
#define CUR_READONLY	    1	/* Read only cursor, no data modifications*/
#define CUR_LOCKCC	    2	/* Intent to update, all fetched data locked
				 **  when dbcursorfetch is called inside a
				 **  transaction block
				 */
#define CUR_OPTCC	    3	/* Optimistic concurrency control, data
				 **  modifications succeed only if the row
				 **  hasn't been updated since the last fetch
				 */
#define CUR_OPTCCVAL	    4	/* Optimistic concurrency control based on
				 ** selected column values
				 */


/*
 ** Following flags are used in the scrollopt parameter in the
 ** dbcursoropen function
 */
#define CUR_FORWARD	    0	/* Forward only scrolling */
#define CUR_KEYSET	   -1	/* Keyset driven scrolling */
#define CUR_DYNAMIC	    1	/* Fully dynamic */
/* Any other number indicates mixed scrolling. (Keyset driven within the given
 **  number, dynamic outside)
 */

/*
 ** Following flags define the fetchtype in the dbcursorfetch function
 */
#define  FETCH_FIRST	1	/* Fetch first n rows */
#define  FETCH_NEXT	2	/* Fetch next n rows */
#define  FETCH_PREV	3	/* Fetch previous n rows */
#define  FETCH_RANDOM	4	/* Fetch n rows beginning with given row# */
#define  FETCH_RELATIVE 5	/* Fetch relative to previous fetch row # */
#define  FETCH_LAST	6	/* Fetch the last n rows */

/*
 ** Following flags define the per row status as filled by dbcursorfetch
 */
#define FTC_SUCCEED	0x01	/* Fetch succeeded, (failed if not set) */
#define FTC_MISSING	0x02	/* The row is missing */
#define FTC_ENDOFKEYSET 0x04	/* End of the keyset reached */
#define FTC_ENDOFRESULTS 0x08	/* End of results set reached */

/*
 ** Following flags define the operator types for the dbcursor function
 */
#define CRS_UPDATE	1	/* Update operation */
#define CRS_DELETE	2	/* Delete operation */
#define CRS_INSERT	3	/* Insert operation */
#define CRS_REFRESH	4	/* Refetch given row */
#define CRS_LOCKCC	5	/* Lock given row
				 ** (if only inside a transaction)
				 */


/*
 ** Following value can be passed to the dbcursorbind function for NOBIND
 **   type.
 */
#define NOBIND	    -2		/* Return length and pointer to data */


/*
** The following values are passed to dbserverenum for searching criteria,
** under all platforms except DOS were dbserverenum is not supported.
*/

#define NET_SEARCH		0x0001
#define LOC_SEARCH		0x0002

/*
** These constansts are the possible return values from dbserverenum.
*/

#define ENUM_SUCCESS		0x0000
#define MORE_DATA		0x0001
#define NET_NOT_AVAIL		0x0002
#define OUT_OF_MEMORY		0x0004


