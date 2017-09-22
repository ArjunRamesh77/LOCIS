#ifndef NLSKINSOL_H
#define NLSKINSOL_H

#include "LOCIS_API_INCLUDE.h"
#include "PyFunctionServer.h"
#include "kinsol/kinsol.h"
#include "kinsol/kinsol_dense.h"
#include "nvector/nvector_serial.h"
#include "sundials/sundials_types.h"
#include "sundials/sundials_math.h"
#include "NLSKinsolHelper.h"
#include <iomanip>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Kinsol static functions
static int residualFunctionKinsol(N_Vector y, N_Vector f, void *user_data);
static int denseJacobianFunctionKinsol(long int N, N_Vector y, N_Vector f, DlsMat J, void *user_data, N_Vector tmp1, N_Vector tmp2);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Kinsol UserData
struct NLSKinsolUserData
{
	int NEQ;
	PyFunctionServer* pyFunctionsObject;
	
	// error handler
	NLSKinsolUserData()
	{
		NEQ = -1;
		pyFunctionsObject = NULL;
	}

	~NLSKinsolUserData()
	{

	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Kinsol return types
#define NLS_KINSOL_FAIL -1
#define NLS_KINSOL_PASS 0

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Kinsol Info handler
typedef void(*outputInfo)(void *s, int type);
typedef void(*outputError)(void *s, int type);

struct NLSKinsolinfomsgstruct
{
	std::string module;
	std::string function;
	std::string msg;
	LOCISInfoCallBack outPutInfoHandler;
	std::string* totalStringout;
};

struct NLSKinsolerrormsgstruct
{
	int code;
	std::string module;
	std::string function;
	std::string msg;

};

struct NLSKinsolInfoHandler
{
	outputInfo InfoFormatFn;

	LOCISInfoCallBack outPutInfoHandler;

	std::string* stringOut;
};

struct NLSKinsolErrorHandler
{

	LOCISErrorCallBack outPutErrorHandler;
	cerrors* errVector;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Kinsol Wrapper class
class NLSKinsol
{
	void* kmem;

	// input
	int inumberEquations;
	double dfnormTolerance;
	NLSKinsolUserData userData;
	N_Vector y, scale;

	LOCISErrorCallBack cbError;
	LOCISInfoCallBack cbInfo;
	
	// output
	NLSKinsolInfoHandler infoFuncData;
	NLSKinsolErrorHandler errorFuncData;

	// error
	cerrors* errVector;

	std::string stringOut;
public:
	NLSKinsol(LOCISInfoCallBack cbInfo_arg, LOCISErrorCallBack cbError_arg)
	{
		cbError = cbError_arg;
		cbInfo = cbInfo_arg;
		kmem = NULL;
		inumberEquations = -1;
		dfnormTolerance = 1.0e-5;
		y = NULL;
	}

	~NLSKinsol()
	{
		if(y)
			N_VDestroy_Serial(y);
		if(scale)
			N_VDestroy_Serial(scale);
		if(kmem)
			KINFree(&kmem);
	}

	// Error Handling
	int checkFlag(int flagvalue, std::string funcname);
	
	// Setup and solve
	int setup(int iumberEquations, double dfnormTolerance, PyFunctionServer* pyFunctionsObject, cerrors* errorVector);
	int solve(double* guess);

	// Set options
	int SetScaledStepTol(double stol);

	// print output
	void printHeader(double dfnormTolerance);

	// print final stats
	void printFinalStats();

	std::string center(const std::string s, const int w);
};

#endif