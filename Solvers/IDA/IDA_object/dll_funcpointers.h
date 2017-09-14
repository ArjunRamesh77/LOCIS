#pragma once

#include "IDAOptions.h"
#include "IDA_error.h"
#include "funcpointers.h"

namespace IDA_HELPERS
{
	typedef int(*f_Init)(int newobj);

	typedef int(*f_setoptions)(int objnum, IDA_HELPERS::IDAOptions *options);

	typedef int(*f_setdata)(int, IDA_HELPERS::CallbackJac, IDA_HELPERS::CallbackRes,
		IDA_HELPERS::CallbackError, IDA_HELPERS::CallbackInfo, IDA_HELPERS::CallbackRoot, IDA_HELPERS::error_IDA *, int &, double *, double *);

	typedef int(*f_funcisolve)(int objnum, double tpoint);

	typedef void(*f_free)(int objnum);

	typedef void(*f_boss)(int newobj, IDA_HELPERS::CallbackJac JacFunc, IDA_HELPERS::CallbackRes ResFunc,
		IDA_HELPERS::CallbackError ErrorFunc, IDA_HELPERS::CallbackInfo InfoFunc, IDA_HELPERS::CallbackRoot RootFunc, IDA_HELPERS::error_IDA *errobj, int &NEQ,
		double *y_init, double *yp_init, IDA_HELPERS::IDAOptions *options);
}
