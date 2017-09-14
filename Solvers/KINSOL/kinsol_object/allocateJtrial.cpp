#include "Kinsol_Class.h"

void solver_kinsol::allocateJtrial()
{
	
	//userdataKIN.Jtrial = new double *[problem_data.NVar];
	userdataKIN.Jtrial = new double [problem_data.NVar*problem_data.NVar];
	/*
	for (int i = 0; i < (problem_data.NVar); i++)
	{
		userdataKIN.Jtrial[i] = new double[problem_data.NVar];
	}
	*/

}