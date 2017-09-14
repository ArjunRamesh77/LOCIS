#ifndef LOCIS_API_INCLUDE
#define LOCIS_API_INCLUDE

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Callbacks
typedef long(*LOCISErrorCallBack)(long lErrorType, const char* cMessage);
typedef long(*LOCISInfoCallBack)(long lInfoType, const char* cMessage);
typedef long(*LOCISSteadyStateResults)(long lpNum, const char* cNames, double* dpValues);
typedef long(*LOCISSteadyDynamicResults)(long lColIndex, long lpNum, const char* cNames, double* dpTime, double* dpValues);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// function pointers (.dll)
typedef long(*LOCISDLL_LOCISCreate)();
typedef long(*LOCISDLL_LOCISExit)(long, bool);
typedef long(*LOCISDLL_LOCISSetTraceFile)(long, const char*);
typedef long(*LOCISDLL_LOCISSetCallBacks)(long, LOCISErrorCallBack, LOCISInfoCallBack);
typedef long(*LOCISDLL_LOCISInit)(long, const char*, LOCISErrorCallBack, LOCISInfoCallBack, LOCISSteadyStateResults, LOCISSteadyDynamicResults);
typedef long(*LOCISDLL_LOCISSolve)(long);

#endif // LOCIS_API_INCLUDE
