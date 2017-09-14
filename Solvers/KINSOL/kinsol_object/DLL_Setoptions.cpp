#include "Kinsol_Class.h"
#include "kinsol_dll.h"

int SETOPTIONS(int objnum,  KinsolOptions *options)
{
	solver_kinsol *obj;
	int returncode;

	obj = NULL;
	obj = map(objnum);
	if (obj == NULL) return -666;

	returncode = obj->SetOptions(options);
	if (returncode <= 0) return returncode;

	return 1;
}
