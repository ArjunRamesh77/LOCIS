#include"IDAclass.h"
#include"IDA_dll.h"

int IDABOSS(int newobj, CallbackJac JacFunc, CallbackRes ResFunc,
	CallbackError UserErrorHandler, CallbackInfo UserInfoHandler, CallbackRoot RootFunc, error_IDA *errorvector, int &NEQ,
	double *y_init, double *yp_init, IDAOptions *options)
{
	double tpoint = 0.0;
	int objnum, returncode;
	objnum = INIT(newobj);

	returncode = SETDATA(objnum, JacFunc, ResFunc, UserErrorHandler, UserInfoHandler, RootFunc,
		errorvector, NEQ, y_init, yp_init);
	if (returncode < 0) return returncode;

	returncode = SETOPTIONS(objnum, options);
	if (returncode <= 0) return returncode;

	returncode = SOLVE(objnum, tpoint);
	if (returncode <= 0) return returncode;

	FREE(objnum);
	return 1;
}