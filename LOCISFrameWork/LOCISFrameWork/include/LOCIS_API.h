#ifndef LOCIS_API_H
#define LOCIS_API_H

#include "LOCIS_API_INCLUDE.h"

#define LOCIS_API_EXPORT __declspec(dllexport)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return codes



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main API functions for using the LOCIS Framework
extern "C"
{
	LOCIS_API_EXPORT long LOCISCreate();

	LOCIS_API_EXPORT long LOCISExit(long lLMobjCounter,bool final);

	LOCIS_API_EXPORT long LOCISSetTraceFile(long lLMobjCounter, const char* cFileName);
	
	LOCIS_API_EXPORT long LOCISSetCallBacks(long lLMobjCounter, LOCISErrorCallBack cbError, LOCISInfoCallBack cbInfo);
	
	LOCIS_API_EXPORT long LOCISInit(long lLMobjCounter, const char* cSource, LOCISErrorCallBack cbError, LOCISInfoCallBack cbInfo, LOCISSteadyStateResults cbSSRes, LOCISSteadyDynamicResults cbDYRes);
	
	LOCIS_API_EXPORT long LOCISSolve(long lLMobjCounter);

	LOCIS_API_EXPORT long LOCISGetAllVarNames(long lLMobjCounter, long* lpNum, const char* cNames);
	
}
#endif // LOCIS_API_H
