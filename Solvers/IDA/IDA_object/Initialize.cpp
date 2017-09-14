#include "IDAclass.h"

// This method initializes a new NVector 'u' of length 'size'.
void solver_IDA::InitializeNVector(N_Vector &u, int size)
{
	if (ESCAPE) return;
	u = NULL;
	u = N_VNew_Serial(size);

	return;
};