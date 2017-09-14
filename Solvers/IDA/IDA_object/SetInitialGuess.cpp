#include"IDAclass.h"

void solver_IDA::SetInitialGuess()
{
	if (ESCAPE) return;
	realtype *yyval, *ypval;

	InitializeNVector(problem_data.yy, problem_data.NEq);
	InitializeNVector(problem_data.yp, problem_data.NEq);

	yyval = ypval = NULL;

	yyval = NV_DATA_S(problem_data.yy);
	ypval = NV_DATA_S(problem_data.yp);

	for (int i = 0; i <= problem_data.NEq - 1; i++)
	{
		yyval[i] = problem_data.initVal_y[i];
		ypval[i] = problem_data.initVal_yp[i];
	}

}