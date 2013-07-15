/************************************************************************/
/* ReMoTe SQL Return Codes                                              */
/************************************************************************/

#define RQ_OK 0

/* EXECSQL errors */
#define RQ_MSG_NOTF     0x3             /* No message found to retrieve       */
#define RQ_PREP_ERROR   0x4             /* Error during stmnt prep, see SQLCA */
#define RQ_NON_SELECT   0x6             /* Non-select SQL stmnt to EHNRQSELECT*/
                                        /* or select stmnt to EHNRQEXEC       */
#define RQ_NON_CONNECT  0x6             /* SQL statement for EHNRQCONNECT     */
                                        /* must be a CONNECT statement        */
#define RQ_LONG_STMNT   0x7             /* SQL Statement larger than buffer   */
                                        /* size                               */
#define RQ_EXEC_ERROR   0x8             /* Error durring SQL stmnt execution  */
#define RQ_NVLD_OPTION  0X9             /* Option structure contains at least */
                                        /* one invalid option                 */

/* FETCH/UPDATe/DELETe Codes   */
#define RQ_NO_FETCH     0xA             /* Update/delete w/o previous fetch   */
#define RQ_NOT_UPDATE   0xB             /* Update/delete require update mode  */
#define RQ_FETCH_ERR    0xC             /* Error occurred on SQL FETCH        */
#define RQ_NOT_ACT      0xD             /* Cursor specified is not active     */
#define RQ_OPEN_ERROR   0xE             /* SQL Open Cursor failed             */
#define RQ_LASTROW      0xF            /* Last selected row has been retrieved*/
#define RQ_MAX_SEL      0x10            /* Maximum # SQL stmnts already active*/
#define RQ_NVLD_STATE   0x11           /* Operation not valid in current state*/

/* STARTSQL/ENDSQL errors */
#define RQ_NOT_INIT     0x14            /* EHNRQSTART/EHNRQACCEPT not done    */
#define RQ_ALRDY_INIT   0x14            /* Already initialized                */

/* Remote Program Comm. Codes */
#define RQ_NVLD_COMMIT  0x15            /* Commit mode specified not valid  */
#define RQ_RMT_ACTIVE   0x16            /* A remote program is already active */
#define RQ_NO_RMT_PGM   0x17            /* There is no active remote program  */
#define RQ_RMT_TERM     0x18            /* Remote program has terminated      */
#define RQ_FORCE_TERM   0x19     /* Forced termination.  The remote appl. that*/
                                 /* initiated this application has terminated.*/
#define RQ_NVLD_RW      0x1A           /* SEND, RECV, or SQL op. not valid now*/
#define RQ_NVLD_LEN     0x1B            /* Send data length too large or <= 0 */
#define RQ_DATA_TRUNC   0x1C            /* Data truncation. Buffer too small  */
#define RQ_EOD          0x1D   /* End of data (Host appl. is finished sending)*/
#define RQ_INVOC_FAIL   0x1E            /* Invocation of rmt appl. pgm failed */

/* Abondon Ship Errors  */
#define RQ_COMM_ERROR   0x20            /* Comm. link to remote system lost   */
#define RQ_LEVEL        0x24       /* Server & IWS API levels are incompatible*/
#define RQ_XI_UNAVAIL   0x26            /* PCSXI program has not been started */
#define RQ_LOAD_FAIL    0x27 /*PCSXI unable to load .OVL into protected memory*/
#define RQ_CIM_UNAVAIL  0x28            /* PCS comm intf mgr not loaded       */
#define RQ_DBCSXlateErr 0x2A       /* 8580 Sys1IBM.table not found (DBCS only)*/
#define RQ_XlateOverfl  0x2C  /* Translation buffer size too small (DBCS only)*/
#define RQ_Xlt_Setup    0x2E            /* Error durint DBCS translate init.  */
#define RQ_NO_MEMORY    0x32            /* Insufficient memory                */
#define RQ_MEMORY       0x32            /* Memory not freed                   */

#define RQ_NO_USERID      0x40  /* Password specified but no userid specified */
#define RQ_USERID_INVLGTH 0x41  /* Userid exceeds maximum length of 10        */
#define RQ_PWORD_INVLGTH  0x42  /* Password exceeds maximum length of 10      */
#define RQ_INV_SECURITY   0x43  /* Host rejected userid/password combination  */
#define RQ_INV_CHAR       0x44  /* Invalid character in userid or password    */

typedef unsigned char hcursor;          /* Handle for select cursor           */

/**************************************************************/
/* Defines for valid commitment control values to be used on  */
/* the EHNRQSTART and EHNRQSTARTSEC calls                     */
/**************************************************************/

   /*     Native AS/400 naming convention           */

#define RQ_NTV_COMMIT_NONE 0            /* Commit = *NONE     */
#define RQ_NTV_COMMIT_ALL  1            /* Commit = *ALL      */
#define RQ_NTV_COMMIT_CHG  2            /* Commit = *CHG      */
#define RQ_NTV_COMMIT_CS   3            /* Commit = *CS       */

   /*     SQL naming convention                     */

#define RQ_SQL_COMMIT_NONE 256          /* Commit = *NONE     */
#define RQ_SQL_COMMIT_ALL  257          /* Commit = *ALL      */
#define RQ_SQL_COMMIT_CHG  258          /* Commit = *CHG      */
#define RQ_SQL_COMMIT_CS   259          /* Commit = *CS       */


/**************************************************************/
/*                                                            */
/* Declares for querying and setting SQL parser options       */
/*                                                            */
/**************************************************************/

struct Options_Struct
  {
   char name_conven_indicator;          /* Indicator for naming conv          */
   char name_conven_value;              /* Value for naming convention        */

   char comm_cntrl_indicator;           /* Indicator for commitment ctl       */
   char comm_cntrl_value;               /* Value for commitment ctl           */

   char date_format_indicator;          /* Indicator for date format          */
   char date_format_value;              /* Value for date format              */

   char date_separt_indicator;          /* Indicator for date separator       */
   char date_separt_value;              /* Value for date separator           */

   char time_format_indicator;          /* Indicator for time format          */
   char time_format_value;              /* Value for time format              */

   char time_separt_indicator;          /* Indicator for time separator       */
   char time_separt_value;              /* Value for time separator           */

   char decimal_point_indicator;        /* Indicator for decimal point        */
   char decimal_point_value;            /* Value for decimal point            */
  };

#define RQ_OPTION_QUERY '0'             /* Indicator when option value        */
                                        /* is not to be set                   */
#define RQ_OPTION_SET   '1'             /* Indicator when option value        */
                                        /* is to be set                       */

/*  Commitment control values   */

#define RQ_COMMIT_NONE  '0'             /* *NONE                              */
#define RQ_COMMIT_ALL   '1'             /* *ALL                               */
#define RQ_COMMIT_CHG   '2'             /* *CHG - change                      */
#define RQ_COMMIT_CS    '3'             /* *CS  - cursor stability            */

/*  Naming convention values   */

#define RQ_NAME_SYS     '0'             /* *SYS - native AS/400 names         */
#define RQ_NAME_SQL     '1'             /* *SQL - SQL naming convention       */

/*  Date format values         */

#define RQ_DATEFMT_USA  '0'             /* *USA - US format                   */
#define RQ_DATEFMT_ISO  '1'             /* *ISO -                             */
#define RQ_DATEFMT_EUR  '2'             /* *EUR -                             */
#define RQ_DATEFMT_JIS  '3'             /* *JIS -                             */
#define RQ_DATEFMT_MDY  '4'             /* *MDY - month/day/year              */
#define RQ_DATEFMT_DMY  '5'             /* *DMY - day/month/year              */
#define RQ_DATEFMT_YMD  '6'             /* *YMD - year/month/day              */
#define RQ_DATEFMT_JUL  '7'             /* *JUL - julian                      */

/*  Date separator character   */

#define RQ_DATESEP_PERIOD '0'           /* '.'                                */
#define RQ_DATESEP_COMMA  '1'           /* ','                                */
#define RQ_DATESEP_BLANK  '2'           /* ' '                                */
#define RQ_DATESEP_SLASH  '3'           /* '/'                                */
#define RQ_DATESEP_DASH   '4'           /* '-'                                */

/*  Time format values         */

#define RQ_TIMEFMT_USA  '0'             /* *USA -                             */
#define RQ_TIMEFMT_ISO  '1'             /* *ISO -                             */
#define RQ_TIMEFMT_EUR  '2'             /* *EUR -                             */
#define RQ_TIMEFMT_JIS  '3'             /* *JIS -                             */
#define RQ_TIMEFMT_HMS  '4'             /* *HMS - hours/minutes/seconds       */

/*  Time separator character   */

#define RQ_TIMESEP_PERIOD '0'           /* '.'                                */
#define RQ_TIMESEP_COMMA  '1'           /* ','                                */
#define RQ_TIMESEP_BLANK  '2'           /* ' '                                */
#define RQ_TIMESEP_COLON  '3'           /* ':'                                */

/*  Decimal point character    */

#define RQ_DECIMAL_PERIOD '0'           /* '.'                                */
#define RQ_DECIMAL_COMMA  '1'           /* ','                                */

/**********************************************************************/
/*                                                                     /
/* The following copyright statement applies to the succeeding lines   /
/* up to the "end of SQLCA" banner lines                               /
/*                                                                     /
/*                                                                     /
/**********************************************************************/

/**************************************************************************

   Module Name    = SQLCA.H

   Descriptive Name = SQLCA External Include File

   Copyright = nnnnnnnn (C) Copyright IBM Corp. 1987
               Licensed Material - Program Property of IBM
               Refer to Copyright Instructions Form Number G120-3083

   Function = Include File defining
                SQLCA

   Operating System = OS/2 Extended Edition

**************************************************************************/
/* SQL Communication Area - SQLCA - structures and constants */

/* SQL Communication Area - SQLCA */
struct sqlca
{
   unsigned char  sqlcaid[8];           /* Eyecatcher = 'SQLCA   ' */
   long           sqlcabc;              /* SQLCA size in bytes = 136 */
   long           sqlcode;              /* SQL return code */
   short          sqlerrml;             /* Length for SQLERRMC */
   unsigned char  sqlerrmc[70];         /* Error message tokens */
   unsigned char  sqlerrp[8];           /* Diagnostic information */
   long           sqlerrd[6];           /* Diagnostic information */
   unsigned char  sqlwarn[8];           /* Warning flags */
   unsigned char  sqlext[8];            /* Reserved */
};

#define   SQLCODE        sqlca.sqlcode
#define   SQLWARN0       sqlca.sqlwarn[0]
#define   SQLWARN1       sqlca.sqlwarn[1]
#define   SQLWARN2       sqlca.sqlwarn[2]
#define   SQLWARN3       sqlca.sqlwarn[3]
#define   SQLWARN4       sqlca.sqlwarn[4]
#define   SQLWARN5       sqlca.sqlwarn[5]
#define   SQLWARN6       sqlca.sqlwarn[6]
#define   SQLWARN7       sqlca.sqlwarn[7]

/**********************************************************************/
/*                                                                     /
/* end of SQLCA                                                        /
/*                                                                     /
/**********************************************************************/


/**********************************************************************/
/*                                                                     /
/* The following copyright statement applies to the succeeding lines   /
/* up to the "end of SQLDA" banner lines                               /
/*                                                                     /
/*                                                                     /
/**********************************************************************/

/**************************************************************************

   Module Name    = SQLDA.H

   Descriptive Name = SQLDA External Include File

   Copyright = nnnnnnnn (C) Copyright IBM Corp. 1987
               Licensed Material - Program Property of IBM
               Refer to Copyright Instructions Form Number G120-3083

   Function = Include File defining
                SQLDA

   Operating System = OS/2 Extended Edition

**************************************************************************/
/* SQL Descriptor Area - Variable descriptor */

/* SQL Descriptor Area - SQLDA */
struct sqlda
{
   unsigned char  sqldaid[8];           /* Eye catcher = 'SQLDA   ' */
   long           sqldabc;              /* SQLDA size in bytes = 16+44*SQLN */
   short          sqln;                 /* Number of SQLVAR elements */
   short          sqld;                 /* # of used SQLVAR elements */
   struct sqlvar
   {
      short          sqltype;           /* Variable data type */
      short          sqllen;            /* Variable data length */
      unsigned char *sqldata;           /* Pointer to variable data value */
      short         *sqlind;            /* Pointer to Null indicator */
      struct sqlname                    /* Variable Name */
      {
         short          length;         /* Name length [1..30] */
         unsigned char  data[30];       /* Variable or Column name */
      }  sqlname;
   } sqlvar[1];
};

/* macro for allocating SQLDA */
#define   SQLDASIZE(n) (sizeof(struct sqlda) + (n-1)*sizeof(struct sqlvar))

/**********************************************************************/
/*                                                                     /
/* end of SQLDA                                                        /
/*                                                                     /
/**********************************************************************/


struct Col_Attributes
  {
   short Col_Type;                      /* SQL Column type                    */
   unsigned char Col_Prec;              /* Column Precision (Arith types only)*/
   unsigned char Col_Scale;             /* Column Scale           "           */
   char  CCSID[2];                      /* Reserved                           */
   char rsvd2[4];                       /* Reserved                           */
   short Col_Width;                     /* Source data width of column        */
   short Col_Data_Off;                  /* Offset to data in result area      */
   short Col_Ind_Off;                   /* Offset to indicator (zero if none) */
   char Col_Name[32];                   /* Column name (null terminated)      */
  };


//#define extentry _saveregs pascal far
#define extentry 

extern int  extentry EHNRQACCEPT(char  *,unsigned short);
extern int  extentry EHNRQATTR(hcursor, struct Col_Attributes  *,
                                        short  *);
extern int  extentry EHNRQCLOSE(hcursor);
extern int  extentry EHNRQCONNECT(char  *, struct Options_Struct  *,
                                   short  *);
extern int  extentry EHNRQDELETE(hcursor);
extern int  extentry EHNRQDESC(hcursor, struct sqlda  *, short  *);
extern int  extentry EHNRQEND(void);
extern void extentry EHNRQERROR(char  x[4], char  y[8]);
extern int  extentry EHNRQEXEC(char  *);
extern int  extentry EHNRQEXECPM(char  *, hcursor  *);
extern int  extentry EHNRQEXECVAL(hcursor, char  *, void  *);
extern int  extentry EHNRQFETCH(hcursor, char  *, short  *);
extern int  extentry EHNRQFREEPM(hcursor);
extern int  extentry EHNRQGETF(hcursor, char  *, void  *);
extern int  extentry EHNRQINVOKE(char  *);
extern int  extentry EHNRQOPTIONS(short,struct Options_Struct  *,
                                  short  *);
extern int  extentry EHNRQRECV(int, char  *, int  *);
extern int  extentry EHNRQRTVMSG(char  *, short  *);
extern int  extentry EHNRQSELECT(char  *, int, hcursor  *);
extern int  extentry EHNRQSELECTPM(char  *, hcursor  *);
extern int  extentry EHNRQSELECTVAL(hcursor, char  *, void  *, int);
extern int  extentry EHNRQSEND(int, char  *);
extern int  extentry EHNRQSETROWS(unsigned short);
extern int  extentry EHNRQSQLCA(struct sqlca  *, short  *);
extern int  extentry EHNRQSTART(char  *, short, unsigned short);
extern int  extentry EHNRQSTARTSEC(char  *,short,unsigned short,
                                   char  *,char  *,char  *);
extern int  extentry EHNRQUPCUR(hcursor, char  *, void  *);