#include"IDAclass.h"
#include"IDA_dll.h"
/*
* Root function routine. Compute functions g_i(t,y) for i = 0,1.
*/

int root(realtype t, N_Vector yy, N_Vector yp, realtype *gout,
	void *user_data)
{

	double *yval, *ypval;

	IDASetData *data;

	data = (IDASetData*)user_data;
	if (data->rootptr != NULL) // if root function is defined continue
	{
		yval = NV_DATA_S(yy);
		ypval = NV_DATA_S(yp);
		data->rootptr(yval, ypval, gout);
	}
	else
	{
		// This function won't be called anyway. This function is needed to give user an option to choose
	}
	return(0);
}
