#ifndef KINSOL_CLASS_H
#define KINSOL_CLASS_H

#include <kinsol/kinsol.h>
#include <kinsol/kinsol_dense.h>
#include <nvector/nvector_serial.h>
#include <sundials/sundials_types.h>
#include <sundials/sundials_math.h>
#include "Kinsol_Error.h"
#include "Kinsol_Options.h"
#include "funcpointers.h"

using namespace KINSOL_HELPERS;
#define NaN -1.0e35;

struct KINSetData
{

	CallbackRes resptr;
	CallbackJac jacptr;
	CallbackInfo UserInfoFuncPtr;
	CallbackError UserErrFuncPtr;
	//double **Jtrial;
	double *Jtrial;
};

struct KINErrFuncData
{
	CallbackError UserErrFuncPtr;
	error_kinsol* ErrVecPtr;
};

struct KINInfoFuncData
{
	CallbackInfo UserInfoFuncPtr;
};


struct problemdata
{

	int NVar, NEq;
	double *InitVal, *x;
	N_Vector initvector;
};


class solver_kinsol 
{

	problemdata problem_data;

	KINSetData userdataKIN;

	KINErrFuncData ErrFuncData;

	KINInfoFuncData InfoFuncData;

	error_kinsol *KinsolErrors;

	KinsolOptions* KinsolOptionsObj;

	void *kmem;

	CallbackInfo UserInfoFuncPtr;

	CallbackError UserErrFuncPtr;

	finalstatsl1 finalstats1;

	finalstatsl2 finalstats2;

	bool ESCAPE;

public:

	void SetProblemdata(int NVAR, int NEQ,
		double *init_val);

	int ProblemSetup(int NVAR, int NEQ, double *init_val, 
		CallbackRes resptr, CallbackJac jacptr, 
		CallbackInfo userinfofunptr, CallbackError usererrfunptr,
		error_kinsol *);

	int SetOptions(KinsolOptions *options);

	int SolveIt(double *x);

	void SetInitialGuess(realtype *init_val, N_Vector &u);

	void InitializeNVector(N_Vector &u, int size);

	void PrintFinalStats(void *kmem, int printlevel);

	int CheckFlag(int flagvalue, std::string funcname);

	void allocateJtrial();

//************************************************
	int CheckUserInput(int NVAR, int NEQ,
		double *init_val, CallbackRes resptr, CallbackJac jacptr,
		CallbackInfo userinfofunptr);

	solver_kinsol();

	~solver_kinsol();
};


#endif