#pragma once
#include "InforError.h"

namespace IDA_HELPERS
{
	typedef void(*CallbackJac)(double t, double *y, double *yp, double *J, double cj);

	typedef void(*CallbackRes)(double t, double *y, double *yp, double *r);

	typedef void(*CallbackRoot)(double *y, double *yp, double *gout);

	typedef int(*CallbackError)(void *s, int type);

	typedef int(*CallbackInfo)(void *s, int type);
}