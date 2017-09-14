#include "IDAClass.h"
#include "IDA_dll.h"

int SETDATA(int objnum, CallbackJac jacptr, CallbackRes resptr,
	CallbackError errorptr, CallbackInfo infoptr, CallbackRoot rootptr, error_IDA *usererrvecptr,
	int &NEQ, double *y_init, double *yp_init)
{
	solver_IDA *obj;
	int returncode;
	obj = map(objnum);
	if (obj == NULL) return -666;

	returncode = (*obj).ProblemSetup(NEQ, y_init, yp_init, 
		jacptr, resptr, errorptr, infoptr, rootptr, usererrvecptr);
	if (returncode <= 0) return returncode;

	return 1;
}
