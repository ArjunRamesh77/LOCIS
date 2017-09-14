#ifndef JAC_H
#define JAC_H

#include "Kinsol_Class.h"

int jacfunc(long int N,
	N_Vector y, N_Vector f,
	DlsMat J, void *user_data,
	N_Vector tmp1, N_Vector tmp2);

#define IJth(A,i,j) DENSE_ELEM(A,i-1,j-1)

#endif
