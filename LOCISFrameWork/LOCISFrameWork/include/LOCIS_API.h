#ifndef LOCIS_API_H
#define LOCIS_API_H

#include "LOCIS_API_INCLUDE.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return codes



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main API functions for using the LOCIS Framework
extern "C"
{
	long LOCISCreate();

	long LOCISExit(long lLMobjCounter,bool final);

	long LOCISSetTraceFile(long lLMobjCounter, const char* cFileName);
	
	long LOCISSetCallBacks(long lLMobjCounter, LOCISErrorCallBack cbError, LOCISInfoCallBack cbInfo);
	
	long LOCISInit(long lLMobjCounter, const char* cSource, LOCISErrorCallBack cbError, LOCISInfoCallBack cbInfo, LOCISSteadyStateResults cbSSRes, LOCISSteadyDynamicResults cbDYRes);
	
	long LOCISSolve(long lLMobjCounter);

	long LOCISGetAllVarNames(long lLMobjCounter, long* lpNum, const char* cNames);
	
}
#endif // LOCIS_API_H
