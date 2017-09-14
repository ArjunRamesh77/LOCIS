#include"Kinsol_Class.h"
solver_kinsol::solver_kinsol()
{
	ESCAPE = false;

}

solver_kinsol::~solver_kinsol()
{

	if (problem_data.InitVal != NULL)
	{
		delete problem_data.InitVal;
	}

	if (problem_data.x != NULL)
	{
		delete problem_data.x;
	}

	if (kmem != NULL)	
		KINFree(&kmem);

	if (userdataKIN.Jtrial != NULL)
		delete userdataKIN.Jtrial;

	if (problem_data.initvector != NULL)
	{
		N_VDestroy(problem_data.initvector);
	}
}