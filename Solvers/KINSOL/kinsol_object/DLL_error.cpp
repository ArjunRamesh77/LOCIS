#include "kinsol_dll.h"

void ERRORFUN(int error_code, const char *module,
	const char *function, char *msg,
	void *user_data)	
{

	KINErrFuncData *data;
	error errtemp;
	int returncode;

	data = (KINErrFuncData*)user_data;

	(data->ErrVecPtr)->SetErr(error_code, "Kinsol internal error");
	(data->ErrVecPtr)->AddErrorLine(module);
	(data->ErrVecPtr)->AddErrorLine(function);
	(data->ErrVecPtr)->AddErrorLine(msg);
	(data->ErrVecPtr)->AddErrortovector();
	errtemp = (data->ErrVecPtr)->GetLastError();

	returncode = data->UserErrFuncPtr(&errtemp, 0);
};