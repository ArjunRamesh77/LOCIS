#include "Kinsol_Class.h"
#include "kinsol_dll.h"

int KINSOLBOSS(int newobj, int NVAR, int NEQ, double* init_val, CallbackRes ResFunc,
	CallbackJac JacFunc, CallbackInfo UserInfoHandler, CallbackError UserErrorHandler,
	KinsolOptions* options, double* x, error_kinsol* errorvector)
{
	int objnum, returncode;
	
	objnum = INIT(newobj);

	returncode = SETDATA(objnum, errorvector, NVAR, NEQ, init_val, ResFunc, 
		JacFunc, UserInfoHandler, UserErrorHandler);
	if (returncode < 0) return returncode;

	returncode = SETOPTIONS(objnum, options);
	if (returncode < 0) return returncode;
	if (returncode == 0) return 0; // If any error at this point, stop execution

	returncode = SOLVE(objnum,  x);
	if (returncode <= 0) return returncode;

	FREE(objnum);
	
	return 1;
}