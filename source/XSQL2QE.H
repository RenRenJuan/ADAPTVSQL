 // Helper Macros

 #ifdef XSQLDBMS

  #define qeBindCol(a,b,c,d)       xsqlBindCol(a,b,c,d)
  #define qeColName(a,b)           xsqlColName(a,b)
  #define qeConnect(a,b,c)         xsqlConnect(a,b,c)
  #define qeDataLength(a,b)        xsqlColWidth(a,b)
  #define qeDBErr()                xsqlDBErr()
  #define qeDisconnect(a)          xsqlDisconnect(a)
  #define qeEndSQL(a)              xsqlEndSQL(a)
  #define qeErr()                  xsqlErr()
  #define qeErrMsg()               xsqlErrMsg()
  #define qeExecSQL(a,b)           xsqlExecSQL(a,b)
  #define qeFetchNext(a)           xsqlFetchNext(a)
  #define qeFetchNumRecs(a)        xsqlFetchNumRows(a)
  #define qeFetchRandom(a,b)       xsqlFetchRandom(a,b)
  #define qeFetchSetOptions(a,b)   xsqlFetchSetOptions(a,b)
  #define qeNumCols(a)             xsqlNumCols(a)
  #define qeNumModRecs(a)          xsqlNumModRecs(a)
  #define qeSetDB(a,b)             xsqlSetDB(a,b)

  #define qeValChar(a,b,c,d)      (char *)osColumnValue4(AS_CHAR_TYPE,a,b,c,d)
  #define qeValLong(a,b)           xsqlColumnValue4(AS_LONGINT_TYPE,a,b)
  #define qeValFloat(a,b)          xsqlColumnValueFloat(a,b)
  #define qeValInt(a,b)            xsqlColumnValue2(AS_SMALLINT_TYPE,a,b)
  #define qeValDouble(a,b)         xsqlColumnValue8(a,b)
  #define qeValDecimal(a,b)       (char *)osColumnValue4(AS_DECIMAL_TYPE,a,b)

  #define qeValMultiCharBuf(a,b,c,d,e,f) \
            xsqlColumnValue2(AS_MULTIB_CHAR,a,c,b,d,e,f,g)

  #define qeTraceOn(a,b)           xsqlTraceOn(a,b)
  #define qeTraceOff(a)            xsqlTraceOff()

  #define qeColType(a,b)           xsqlColType(a,b)
  #define qeColWidth(a,b)          xsqlColWidth(a,b)
  #define qeColScale(a,b)          xsqlColScale(a,b)
  #define qeColPrecision(a,b)      xsqlColPrecision(a,b)
  #define qeWhere()                xsqlWhere() 

 #endif

