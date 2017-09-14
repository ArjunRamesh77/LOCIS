#include "Kinsol_Class.h"

int jacfunc(long int N,
	N_Vector y, N_Vector f,
	DlsMat J, void *user_data,
	N_Vector tmp1, N_Vector tmp2)
{
	CallbackJac jaclocal;
	KINSetData *data;
	int i, j;
	double *yd;// , **Jtrial;

	data = (KINSetData*)user_data;
	jaclocal = NULL;

	jaclocal = data->jacptr;

	yd = NV_DATA_S(y);
	
	// jac takes input yd and returns Jtrial		
	jaclocal(yd, data->Jtrial);


	for (i = 0; i < (J->M); i++)
	{
		for (j = 0; j < (J->N); j++)
		{
			DENSE_ELEM(J, i, j) = data->Jtrial[i*J->M + j];
			//data->Jtrial[i][j] = 0; // nullify Jtrial
		}
	}

	//J->data = data->Jtrial;

	return(0);
}
