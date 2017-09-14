#pragma once
#include "InforError.h"

//Contains function pointers to be used by both kinsol_class anb kinsol_dll

namespace KINSOL_HELPERS
{

	typedef void(*CallbackRes)(double *x, double *y);

	//typedef void(*CallbackJac)(double *x, double **y);
	typedef void(*CallbackJac)(double *x, double *y);

	typedef void(*CallbackInfo)(void *s, int type);

	typedef int(*CallbackError)(void *s, int type);
}