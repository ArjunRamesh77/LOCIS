#pragma once
#include "Kinsol_Options.h"
#include "funcpointers.h"

namespace KINSOL_HELPERS
{
	typedef int(*f_Init)(int newobj);

	typedef int(*f_setdata) (int, error_kinsol *, int &,
		int &, double *, KINSOL_HELPERS::CallbackRes, KINSOL_HELPERS::CallbackJac, KINSOL_HELPERS::CallbackInfo, KINSOL_HELPERS::CallbackError);

	typedef int(*f_setoptions)(int, KINSOL_HELPERS::KinsolOptions *);

	typedef int(*f_funcisolve)(int, double *);

	typedef void(*f_free)(int objnum);

	typedef int(*f_boss)(int newobj, int NVAR, int NEQ, double* init_val, KINSOL_HELPERS::CallbackRes ResFunc, KINSOL_HELPERS::CallbackJac JacFunc, KINSOL_HELPERS::CallbackInfo UserInfoHandler, KINSOL_HELPERS::CallbackError UserErrorHandler,
		KINSOL_HELPERS::KinsolOptions* options, double* x, void* errorvector);
}