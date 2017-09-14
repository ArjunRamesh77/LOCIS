#include "IDAclass.h"

void solver_IDA::SetProblemdata(int NEQ, double* y_init, double* yp_init)
{
	if (ESCAPE) return;
	problem_data.NEq = NEQ;
	problem_data.initVal_y = new double[NEQ];
	problem_data.initVal_yp = new double[NEQ];
	problem_data.initVal_y = y_init;
	problem_data.initVal_yp = yp_init;
	return;
}