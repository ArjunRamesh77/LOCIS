#include"IDAclass.h"
#include"IDA_dll.h"
/*
* Define the system residual function.
*/

int res(realtype tres, N_Vector yy, N_Vector yp, N_Vector rr, void *user_data)
{
	CallbackRes reslocal;
	double *yval, *ypval, *rval;
	IDASetData *data;

	data = (IDASetData*)user_data;
	reslocal = data->resptr;

	yval = NV_DATA_S(yy);
	ypval = NV_DATA_S(yp);
	rval = NV_DATA_S(rr);

	reslocal(tres, yval, ypval, rval);

	return(0);
}
