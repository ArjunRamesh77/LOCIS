#include "Kinsol_Class.h"

void solver_kinsol::SetProblemdata(int NVAR, int NEQ 
		, double *init_val)
{
	if (ESCAPE) return;

	problem_data.NVar = NVAR;
	problem_data.NEq = NEQ;

	problem_data.InitVal = new double [NVAR];		

	for (int i = 0; i < problem_data.NVar; i++)
	{
		problem_data.InitVal[i] = init_val[i];
	}

	InitializeNVector(problem_data.initvector, problem_data.NEq);
	SetInitialGuess(problem_data.InitVal, problem_data.initvector);

	return;
}