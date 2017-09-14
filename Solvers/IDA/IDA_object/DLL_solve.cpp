#include "IDAClass.h"
#include "IDA_dll.h"

int SOLVE(int objnum, double tpoint)
{
	solver_IDA *obj;
	int returncode;

	obj = map(objnum);
	if (obj == NULL) return -666;

	returncode = (*obj).SolveIt(tpoint);
	if (returncode <= 0) return returncode;

	return 1;
}
