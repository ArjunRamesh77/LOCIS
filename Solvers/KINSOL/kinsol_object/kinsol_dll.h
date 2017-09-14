#pragma once 

#include "Kinsol_Class.h"

solver_kinsol* map(int num);

extern "C"

{

	_declspec(dllexport) int INIT(int newobj);
	
	_declspec(dllexport) int SETDATA(int , error_kinsol *, int &,
		int &, double *, CallbackRes, CallbackJac, CallbackInfo, CallbackError);

	_declspec(dllexport) int SETOPTIONS(int, KinsolOptions *);
	
	_declspec(dllexport) int SOLVE(int, double *);

	_declspec(dllexport) void FREE(int);

	_declspec(dllexport) int KINSOLBOSS(int, int, int, double*, CallbackRes, 
		CallbackJac, CallbackInfo, CallbackError, KinsolOptions*, double*, error_kinsol*);
}