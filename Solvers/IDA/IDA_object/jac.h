#pragma once

#include "IDAClass.h"

int jac(long int Neq, realtype tt, realtype cj,
	N_Vector yy, N_Vector yp, N_Vector resvec,
	DlsMat JJ, void *user_data,
	N_Vector tempv1, N_Vector tempv2, N_Vector tempv3);
