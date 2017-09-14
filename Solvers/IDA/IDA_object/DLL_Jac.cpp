#include"IDAclass.h"

/*
* Define the Jacobian function.
*/

int jac(long int Neq, realtype tt, realtype cj,
	N_Vector yy, N_Vector yp, N_Vector resvec,
	DlsMat JJ, void *user_data,
	N_Vector tempv1, N_Vector tempv2, N_Vector tempv3)
{
	CallbackJac jaclocal;
	realtype *yval, *ypval;
	IDASetData *data;

	int i, j;

	data = (IDASetData*)user_data;
	jaclocal = NULL;
	jaclocal = data->jacptr;

	yval = NV_DATA_S(yy);
	ypval = NV_DATA_S(yp);


	jaclocal(tt, yval, ypval, data->Jtrial, cj);

	for (i = 0; i < (JJ->M); i++)
	{
		for (j = 0; j < (JJ->N); j++)
		{
			DENSE_ELEM(JJ, i, j) = data->Jtrial[j*JJ->M + i];
			//data->Jtrial[i][j] = 0;
		}
	}

	return(0);
}
