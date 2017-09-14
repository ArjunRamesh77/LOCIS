#include "Kinsol_Class.h"

void solver_kinsol::SetInitialGuess(realtype *init_val, N_Vector &u)
{
	if (ESCAPE) return;

	realtype *udata;
	udata = NV_DATA_S(u);

	for (int i = 0; i < problem_data.NVar; i ++)
	{

		udata[i] = init_val[i];
	}
};
