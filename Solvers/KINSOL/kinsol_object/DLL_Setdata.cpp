#include "kinsol_dll.h"

int SETDATA(int objnum, error_kinsol *usererrvecptr, int &NVAR,
	int &NEQ, double *init_val, CallbackRes resptr,
	CallbackJac jacptr, CallbackInfo userinfofunptr, CallbackError usererrfunptr)
{
	solver_kinsol *obj;
	int returncode;

	obj = map(objnum);
	if (obj == NULL) return -666;

	returncode = obj->ProblemSetup(NVAR, NEQ, init_val, resptr, jacptr, 
		userinfofunptr, usererrfunptr, usererrvecptr);
	if (returncode < 0) return returncode;


	return 1;
}
