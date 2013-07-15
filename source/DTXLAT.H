
/**********************************************************************/
/*                                                                    */
/* Name: dtxlat.h                                                     */
/*                                                                    */
/* Descriptive Name = Definition/declarations for dtxlate             */
/*                                                                    */
/* Function = TO define the structures, values, and functions of      */
/*            data transfer API                                       */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/* Constant                                                           */
/**********************************************************************/
#pragma pack(1)

#define MINEBCDIC               0x40    /* Low bound in EBCDIC char   */
#define MAXEBCDIC               0xFF    /* Low bound in EBCDIC char   */
#define SPACE                   0x30

/**********************************************************************/
/* Return code                                                        */
/**********************************************************************/
#define DTX_SUCCESS             0x0000
#define DTX_OVERFLOW            0x0001
#define DTX_NULL_SOURCE         0xFFFF
#define DTX_ROUTER_NOT_LOADED   0XFFFE

#pragma pack()

/**********************************************************************/
/* Function declaration                                               */
/**********************************************************************/

//ICC cant: extern unsigned far pascal itoasc(char far *,char far *);
extern unsigned itoasc(char *,char *);
extern unsigned ltoasc(char *,char *);
extern unsigned asctoi(char *,char *);
extern unsigned asctol(char *,char *);
extern unsigned hextoasc(char *, char *,int);
extern unsigned asctohex(char *, char *,int);
extern unsigned paktopak(char *, char *,int);
extern unsigned dostopak(char *, char *,int);
extern unsigned paktodos(char *, char *,int);
extern unsigned paktoasc(char *, char *,int,int);
extern unsigned asctopak(char *, char *,int,int);
extern unsigned zontozon(char *, char *,int);
extern unsigned zontodos(char *, char *,int);
extern unsigned dostozon(char *, char *,int);
extern unsigned zontoasc(char *, char *,int,int);
extern unsigned asctozon(char *, char *,int,int);
extern unsigned asctoebc(char *, char *,int);
extern unsigned ebctoasc(char *, char *,int);
extern unsigned ebctoebc(char *, char *,int);
extern char * geta2e(void);
extern char * gete2a(void);

