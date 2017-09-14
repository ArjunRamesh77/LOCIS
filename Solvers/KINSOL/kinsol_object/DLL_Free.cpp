#include "Kinsol_Class.h"
#include "kinsol_dll.h"

void FREE(int objnum)
{
	solver_kinsol *obj;
	obj = map(objnum);
	if(obj != NULL)
		delete obj;
}
