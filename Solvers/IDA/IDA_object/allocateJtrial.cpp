#include "IDAClass.h"

void solver_IDA::allocateJtrial()
{

	userdataIDA.Jtrial = new double [problem_data.NEq*problem_data.NEq];
	/*
	for (int i = 0; i < (problem_data.NEq); i++)
	{
		userdataIDA.Jtrial[i] = new double[problem_data.NEq];
	}
	*/

}