#include "LOCIS_API.h"
#include "LOCISCoreManager.h"
#include <map>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implementation of all LOCIS functions
long LMobjCounter = -1;
std::map<int, LOCISCoreManager*> LOCISDllObjectsMap;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// First function to be called that returns the pointer to the LOCIS object
long LOCISCreate()
{
	LOCISCoreManager* LMobj = NULL;
	LMobj = new  LOCISCoreManager;
	
	if (LMobj)
	{
		LMobjCounter++;
		LOCISDllObjectsMap[LMobjCounter] = LMobj;
		return LMobjCounter;
	}
	else
	{
		return -1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Last function to free LOCIS memory
long LOCISExit(long lLMobjCounter, bool final)
{
	if (final)
	{
		FinalizePythonEnv();
	}

	auto f = LOCISDllObjectsMap.find(lLMobjCounter);
	if (f != LOCISDllObjectsMap.end())
	{
		delete f->second;
		f->second = NULL;
		return 0;
	}
	else
	{
		return -1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set the trace file
long LOCISSetTraceFile(long lLMobjCounter, const char* cFileName)
{
	if (cFileName == NULL)
		return -2;

	auto f = LOCISDllObjectsMap.find(lLMobjCounter);
	if (f != LOCISDllObjectsMap.end())
	{
		LOCISCoreManager* LCMobj = f->second;
		
		if (!LCMobj->setTraceFile(cFileName))
		{
			return -3;
		}

		return 0;
	}
	else
	{
		return -1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set all the callbacks
long LOCISSetCallBacks(long lLMobjCounter, LOCISErrorCallBack cbError, LOCISInfoCallBack cbInfo)
{
	if (cbError == NULL)
		return -2;
	
	if (cbInfo == NULL)
		return -3;

	auto f = LOCISDllObjectsMap.find(lLMobjCounter);
	if (f != LOCISDllObjectsMap.end())
	{
		LOCISCoreManager* LCMobj = f->second;

		if (!LCMobj->setCallbacks(cbError, cbInfo))
		{
			return -4;
		}

		return 0;
	}
	else
	{
		return -1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize 
long LOCISInit(long lLMobjCounter, const char* cSource, LOCISErrorCallBack cbError, LOCISInfoCallBack cbInfo, LOCISSteadyStateResults cbSSRes, LOCISSteadyDynamicResults cbDYRes)
{
	auto f = LOCISDllObjectsMap.find(lLMobjCounter);
	if (f != LOCISDllObjectsMap.end())
	{
		LOCISCoreManager* LCMobj = f->second;

		if (!LCMobj->initManager(cSource, cbError, cbInfo, cbSSRes, cbDYRes))
		{
			return -2;
		}

		return 0;
	}
	else
	{
		return -1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Run!!
long LOCISSolve(long lLMobjCounter)
{
	auto f = LOCISDllObjectsMap.find(lLMobjCounter);
	if (f != LOCISDllObjectsMap.end())
	{
		LOCISCoreManager* LCMobj = f->second;

		if (!LCMobj->solveSystem())
		{
			return -2;
		}

		return 0;
	}
	else
	{
		return -1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get all the variable names
long LOCISGetAllVarNames(long lLMobjCounter, long* lpNum, const char* cNames)
{
	auto f = LOCISDllObjectsMap.find(lLMobjCounter);
	if (f != LOCISDllObjectsMap.end())
	{
		LOCISCoreManager* LCMobj = f->second;


		return 0;
	}
	else
	{
		return -1;
	}
}
