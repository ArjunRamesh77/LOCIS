#include "IDAClass.h"
#include "IDA_dll.h"

int SETOPTIONS(int objnum, IDAOptions *options)
{
	solver_IDA *obj;
	int returncode;

	obj = NULL;
	obj = map(objnum);
	if (obj == NULL) return -666;

	returncode = obj->SetOptions(options);
	if (returncode <= 0) return returncode;

	return 1;
}
