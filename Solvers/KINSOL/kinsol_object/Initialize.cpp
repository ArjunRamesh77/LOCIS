#include "Kinsol_Class.h"

// This method initializes a new NVector 'u' of length 'size'.
void solver_kinsol::InitializeNVector(N_Vector &u, int size)
{
	if (ESCAPE) return;

	u = NULL;
	u = N_VNew_Serial(size);

	return;
};