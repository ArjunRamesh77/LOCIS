#pragma once

#include "IDAclass.h"

solver_IDA* map(int num);


extern "C"

{
	_declspec(dllexport) int IDABOSS(int, CallbackJac, CallbackRes,
		CallbackError, CallbackInfo, CallbackRoot, error_IDA *, int &, double *, double *, IDAOptions *);

	_declspec(dllexport) int INIT(int);

	_declspec(dllexport) int SETOPTIONS(int, IDAOptions *);

	_declspec(dllexport) int SETDATA(int, CallbackJac, CallbackRes,
		CallbackError, CallbackInfo, CallbackRoot, error_IDA *, int &,	double *, double *);

	_declspec(dllexport) int SOLVE(int,double);

	_declspec(dllexport) void FREE(int);
}