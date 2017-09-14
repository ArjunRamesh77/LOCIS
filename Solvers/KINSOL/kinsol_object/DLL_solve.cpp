#include "Kinsol_Class.h"
#include "kinsol_dll.h"

int SOLVE(int objnum,  double *x)
{
	int returncode;
	solver_kinsol *obj;
	obj = map(objnum);
	if (obj == NULL) return -666;

	returncode = obj->SolveIt(x);
	if (returncode <= 0) return returncode;

	return 1;
}
