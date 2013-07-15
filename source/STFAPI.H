/***************************************************************************/
/*                                                                         */
/* Name STFAPI.H                                                           */
/*                                                                         */
/* Descriptive name = Definitions/declarations for  STFAPI                 */
/*                                                                         */
/* Function  = This module defines the structures,values,and               */
/*             functions of STFAPI                                         */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
/***************************************************************************/
/* Function ID  and  constant                                              */
/***************************************************************************/
#pragma pack(1)

#define STF_OPEN_TRANSFER       0X01   /* Open a Transfer Request Function */
#define STF_RETRIEVE_TEMPLATE   0X02   /* Retrieve the Templates Function  */
#define STF_RETRIEVE_RECORD     0X03   /* Retrieve the Records Function    */
#define STF_CLOSE_TRANSFER      0X04   /* Close the Transfer               */
                                       /*  Request Function                */
#define STF_END_ALL_REQUEST     0X05   /* End all Transfer Request         */
                                       /* Conversations Function           */
#define STF_SEND_RECORD         0X06   /* Send Records Function            */
#define STF_END_REQUEST         0X07   /* End the Transfer Request         */
                                       /*  Conversation                    */

/*  Define conversion   constant for function 03 */
#define STF_NO_CHANGE           0X00   /* Not change                       */
#define STF_CHG_TO_DOS_RAND     0X01   /* Change when writing to a DOS     */
                                       /*  random file                     */
#define STF_CHG_TO_DOS_RAND2    0X11   /* Change when writing to a DOS     */
#define STF_CHG_DOS_RAND        0X02   /* The records change to DOS random */
                                       /* file format,with the following   */
                                       /* exceptions:                      */
                                       /* When an untranslatable EBCDIC    */
                                       /* zoned decimal byte is found, the */
                                       /* byte changes to an X'3F'         */
                                       /* When an untranslatable EBCDIC    */
                                       /* packed decimal byte is found,the */
                                       /* byte changes to an X'FF'         */
#define STF_CHG_DOS_RAND2       0X12   /* The records change to DOS random */
                                       /* type 2 file format,with following*/
                                       /* exceptions:                      */
                                       /* When an untranslatable EBCDIC    */
                                       /* zoned decimal byte is found,the  */
                                       /* byte is changed to an X'3F'.     */
                                       /* When an untranslatable EBCDIC    */
                                       /* packed decimal byte is found,the */
                                       /* byte is changed to an X'FF'.     */

/*  Define constant for function 06              */
#define STF_DOS_TO_SYSF        0X01    /* The records are translated from  */
                                       /* personal computer DOS random     */
                                       /* format to system format.         */
#define STF_DOS2_TO_SYSF       0x11    /* The records are translated from  */
                                       /* personal computer DOS random type*/
                                       /* 2 format to system format.       */


/***************************************************************************/
/*      Return Code                                                        */
/***************************************************************************/

#define STF_SUCCESS             0X0000 /* Successful Completion            */
#define STF_NOT_LOADED          0X00FE /* Stf not loaded                   */
#define STF_REQUEST_ENDED       0X0103 /* Transfer request is already ended*/
#define STF_UNTRANS_DATA_FOUND  0X0300 /* Untranslatable data found in     */
                                       /* transferred record.              */
#define STF_UNTRANS_DATA_SENT   0X0302 /* Untranslatable data in record to */
                                       /*  be transferred to AS/400.       */
#define STF_WARN_DETECTED_BY_AS400  0X0400 /* Warning detected by the      */
                                       /* AS/400    system.                */
#define STF_END_OF_FILE         0X1FFF /* End-of-file.                     */
#define STF_MAX_TRANS_ACTIVE    0X2000 /* Maximum number of transfer       */
                                       /* requests active.                 */
#define STF_INVALID_REQUEST_LENGTH 0X2001 /* Invalid transfer request      */
                                       /*  length.                         */
#define STF_CHAR_NOT_TRANSLATED 0X2002 /* Transfer request character cannot*/
                                       /* be translated.                   */
#define STF_REC_NOT_TRANSFERED  0X2003 /* Record not transferred to AS/400 */
                                       /* system due to untranslatable     */
                                       /*                     numeric data.*/
#define STF_NUMERIC_NOT_FIT     0X2004 /* Numeric data does not fit into   */
                                       /* range   specified by digits.     */
#define STF_INVALID_REC_LENGTH  0X2005 /* Record length given by your      */
                                       /* application program is incorrect.*/
#define STF_STACK_TOO_SMALL     0X2006 /* Program stack is not large enough*/
#define STF_INCORR_NULLMAP_OFFSET  0X2007 /* Incorrect Null Map Offset     */
#define STF_REC_LEN_OVER_4096   0X2008 /* Record length is greater than4096*/
#define STF_REQUEST_NOT_OPENED  0X2010 /* Transfer request not opened.     */
#define STF_TEMPLETE_NOT_RETRIEVE   0X2011 /* Template cannot be retrieved */
                                       /* now.                             */
#define STF_GET_REC_FAIL        0X2012 /* Cannot get records on a          */
                                       /* Person-Computer-to-AS/400 request*/
#define STF_SEND_REC_FAIL       0X2013 /* Cannot send records on a         */
                                       /* AS/400-to-Person-Computer request*/
#define STF_INVALID_REQUEST     0X2020 /* Incorrect function requested.    */
#define STF_INVALID_BUF_LENGTH  0X2021 /* Incorrect buffer length.         */
#define STF_INVALID_BUF_ADDR    0X2022 /* Incorrect buffer address.        */
#define STF_REQUEST_BUF_OVERLAPED 0X2023 /* Transfer request buffer        */
                                       /* overlaps a previous transfer     */
                                       /* request buffer                   */
#define STF_INCORRECT_BUFADDR   0X2024 /* Incorrect address specified for  */
                                       /* the buffer address parameter.    */
#define STF_INVALID_RETURN_ADDR 0X2025 /* Incorrect address specified for  */
                                       /* the return address parameter.    */
#define STF_INVALID_RETURN_LENGTH_ADDR 0X2026 /* Incorrect address         */
                                       /* specified for the return length  */
                                       /*  parameter.                      */
#define STF_INVALID_2ND_RETURN_ADDR 0X2027 /* Incorrect address specified  */
                                       /* for the  secondary return code   */
                                       /*    parameter.                    */
#define STF_INVALID_SYSTEM_ADDR 0X2028 /* Incorrect address specified for  */
                                       /* the system address parameter.    */
#define STF_OPTIONS_NOT_SUPPORTED  0X2030 /*Options statement not supported*/
                                       /* on the host system               */
#define STF_ERROR_W_STATE       0X3000 /*                                  */
                                       /* Error detected by                */
                                       /* AS/400- statement offset provided*/
#define STF_ERROR_WO_STATE      0X3100 /* Error detected by AS/400-no      */
                                       /* statement offset provided        */
#define STF_CONNECT_FAILED      0X5048 /* Connection failed to system &&&&&*/
#define STF_INVALID_SYSTEM_NAME 0X5050 /* System name is incorrect or      */
                                       /* inactive                         */
#define STF_PROGRAM_NOT_FOUND   0X5052 /* System &&&&&&&&.                 */
                                       /* &&&&&&&&&&&&&&& program not found*/
#define STF_PROGRAM_ENDED_ABNORMALLY 0X5054 /* System &&&&&&&&             */
                                       /* &&& program ended unexpectedly.  */
#define STF_SECURITY_ERROR      0X5056 /*Security error occurred for system*/
                                       /* &&&&&&&&.                        */
                                       /*                                  */
#define STF_SYSTEM_NOT_SUPPORTED 0X5058 /* System &&&&&&&& is not a        */
                                       /*   supported system.              */
#define STF_INCORRECT_RELEASE   0X5060 /* Cannot continue: &&&&&&&& must be*/
                                       /* Release &.& Level &&.            */
#define STF_CONTACT_ENDED       0X5062 /* Contact with system &&&&&&&&     */
                                       /* ended                            */
#define STF_CONTACT_INTERRUPTED 0X5064 /* Contact with system &&&&&&&&     */
                                       /*  temporarily interrupted.        */
#define STF_RESOURCE_FAILED     0X5066 /* Resource failure on system  &&&  */
#define STF_CONVERSATION_ENDED_ABNOR  0X5067 /* Conversation with          */
                                       /* &&&&&&&& was ended unexpectedly. */
#define STF_OS_ERROR            0X5400 /* Operation System returned error  */
                                       /* code &&&&                        */
#define STF_UNDEFINED_ERROR     0X9999 /* Undefined error.  Call your IBM  */
                                       /*  service  representative.        */


/******************************************************************************
* Parameter block
******************************************************************************/


struct stf{
          char far *buffaddr;    /* Far address pointing to the transfer   */
                                 /* request buffer                         */
          unsigned int bufflen;  /* The length of the transfer request     */
                                 /* buffer - must be 4096                  */
          unsigned int reqlen;   /* The length of data being sent          */
          char far *sysaddr;     /* Far address pointing to name of the    */
                                 /* system in which your request to run    */
          unsigned char conver;  /* Type of translation                    */
          char far *retaddr;     /* Far address pointing to the address    */
                                 /* within the transfer request buffer     */
          unsigned int retlen;   /* The length of data returned, or        */
                                 /* a count of the number of templates that*/
                                 /* can be retrieved                       */
          unsigned int retcode2; /* A secondary return code, or            */
                                 /* the number of templates that may be    */
                                 /* retrieved if a warning message occurred*/
                                 /* during the open of a REPLACE transfer  */
                                 /* request.                               */
};
#pragma pack()

int far pascal stfapi(char,struct stf far *); /* declare function                       */
