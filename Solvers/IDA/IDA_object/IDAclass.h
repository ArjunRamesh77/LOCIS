#pragma once

#include "stdafx.h"
#include <stdio.h>

#include <ida/ida.h>
#include "ida/ida_dense.h"
#include <nvector/nvector_serial.h>
#include <sundials/sundials_math.h>
#include <math.h>
#include"IDAOptions.h"
#include"IDA_error.h"
#include "funcpointers.h"


using namespace IDA_HELPERS;


/* Macro to define dense matrix elements, indexed from 1. */

#define IJth(A,i,j) DENSE_ELEM(A,i-1,j-1)

int jac(long int Neq, realtype tt, realtype cj,
	N_Vector yy, N_Vector yp, N_Vector resvec,
	DlsMat JJ, void *user_data,
	N_Vector tempv1, N_Vector tempv2, N_Vector tempv3);

struct IDASetData
{

	CallbackRes resptr;
	CallbackJac jacptr;
	CallbackRoot rootptr;
	CallbackError UserErrorFuncPtr;
	double *Jtrial;
};

struct IDAErrFuncData
{
	CallbackError Usererrorfuncptr;
	error_IDA* ErrorVecPtr;
};



struct problemdata
{

	int NEq;

	double *initVal_y, *initVal_yp;

	N_Vector yy, yp;
};

class solver_IDA
{
	problemdata problem_data;

	IDASetData userdataIDA;

	IDAErrFuncData ErrFuncdata;

	IDAInfoFuncData InfoFuncdata;
	
	error_IDA *IDAErrors;

	IDAOptions *Useroptionsptr;

	void *IDAmem;

	CallbackError UsererrorFuncptr;

	CallbackInfo UserInfoFuncptr;

	CallbackRoot UserRootPtr;

	bool ESCAPE;

	finalstatsl1 finalstats1;

	finalstatsl2 finalstats2;


public:
	void SetProblemdata(int NEQ, double* y_init, double* yp_init);

	int ProblemSetup(int NEQ, double* y_init, double* yp_init, 
		CallbackJac, CallbackRes, CallbackError, CallbackInfo infoptr,
		CallbackRoot rootptr, error_IDA *usererrvecptr);

	int SetOptions(IDAOptions *);

	void InitializeNVector(N_Vector &u, int size);

	int SetIntegrationTol();

	void SetInitialGuess();

	int SolveIt(double tpoint);

	int CheckFlag(int flagvalue, std::string funcname);

	void PrintOutput(realtype t);

	void PrintFinalStats();

	solver_IDA();

	~solver_IDA();

	void allocateJtrial();

	int CheckUserInput(int NEQ, double* y_init, double* yp_init,
		CallbackJac jacptr, CallbackRes resptr,
		CallbackInfo infofuncptr, CallbackRoot rootptr);
};