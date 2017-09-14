#include "IDAClass.h"
#include "IDA_dll.h"

void FREE(int objnum)
{
	solver_IDA *obj;
	obj = map(objnum);

	if(obj != NULL)
		delete obj;
}
