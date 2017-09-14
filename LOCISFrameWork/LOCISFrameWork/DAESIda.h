#ifndef DAES_IDA_H
#define DAES_IDA_H

#include "LOCIS_API_INCLUDE.h"
#include "PyFunctionServer.h"
#include "ida/ida.h"
#include "ida/ida_dense.h"
#include "nvector/nvector_serial.h"
#include "sundials/sundials_math.h"
#include "sundials/sundials_types.h"

#include "DAESIdaHelper.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IDA static functions
static int residualFunctionIda(realtype tres, N_Vector yy, N_Vector yp, N_Vector resval, void *user_data);
static int denseJacobianFunctionIda(long Neq, realtype tt, realtype cj, N_Vector yy, N_Vector yp, N_Vector resvec,
									DlsMat JJ, void *user_data, N_Vector tempv1, N_Vector tempv2, N_Vector tempv3);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ida UserData
struct DAESIdaUserData
{
	int NEQ;
	PyFunctionServer* pyFunctionsObject;

	DAESIdaUserData()
	{
		NEQ = -1;
		pyFunctionsObject = NULL;
	}

	~DAESIdaUserData()
	{

	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ida return types
#define DAES_IDA_FAIL -1
#define DAES_IDA_PASS 0

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IDA Info handler
typedef void(*outputError)(void *s, int type);

struct DAESIdaerrormsgstruct
{
	int code;
	std::string module;
	std::string function;
	std::string msg;
};

struct DAESIdaErrorHandler
{
	LOCISErrorCallBack outPutErrorHandler;
	cerrors* errVector;
};

struct DAESIdainfomsgstruct
{
	int NEQ;
	int LastOrder;
	long int Numsteps;
	double LastStep, t;
	double* yval;
	double* yp_val;
	bool rootReturn;
	int *rootsfound;
	int numroots;

};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ida Wrapper class
class DAESIda
{
	void* imem;

	// input
	int iNumberEquations;
	double dRelTolerance;
	double tret;
	double* dAbsTolerance;
	DAESIdaUserData userData;
	N_Vector yy, yp, avtol;
	LOCISErrorCallBack cbError;
	LOCISInfoCallBack cbInfo;
	// Error function
	DAESIdaErrorHandler errorFuncData;

	// error
	cerrors* errVector;
public:
	DAESIda(LOCISInfoCallBack cbInfo_arg, LOCISErrorCallBack cbError_arg)
	{
		cbError = cbError_arg;
		cbInfo = cbInfo_arg;
		imem = NULL;
		iNumberEquations = -1;
		dRelTolerance = 1.0e-5;
		dAbsTolerance = NULL;
		yy = NULL;
		yp = NULL;
		avtol = NULL;
	}

	~DAESIda()
	{
		N_VDestroy_Serial(yy);
		N_VDestroy_Serial(yp);
		N_VDestroy_Serial(avtol);
		IDAFree(&imem);
	}
	// Error Handling
	int checkFlag(int flagvalue, std::string funcname);


	int setup(int iNumberEquations, double dRelTolerance, double* dAbsTolerance, 
		PyFunctionServer* pyFunctionsObject, double* guessy, 
		double* guessyp, double tbeg, cerrors* DAESErrorVector);
	int solve(double t, stateVars* DAESimVars);

	// output
	void printHeader();

	// print final stats
	void printFinalStats(double t);

	// print footer
	void printFooter();

	std::string center(const std::string s, const int w);
};


#endif