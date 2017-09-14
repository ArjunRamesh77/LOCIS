#include "Kinsol_Class.h"
#include "kinsol_dll.h"

int func(N_Vector y, N_Vector f, void *user_data)
{
	CallbackRes reslocal;
	double *yd, *fd;
	KINSetData *data;

	data = (KINSetData*)user_data;
	reslocal = NULL;
	yd = NV_DATA_S(y);
	fd = NV_DATA_S(f);

	reslocal = data->resptr;
	reslocal(yd, fd);
	return (0);
}
