#include "IDA_dll.h"

void ERRORFUN(int error_code, const char *module,
	const char *function, char *msg,
	void *user_data)
{

	CallbackError errorlocal;
	IDAErrFuncData *data;
	error errtemp;
	int returncode;

	data = (IDAErrFuncData*)user_data;
	errorlocal = NULL;

	// ErrorVecptr points to the error vector to the class IDA
	
	errorlocal = data->Usererrorfuncptr;
	(data->ErrorVecPtr)->SetErr(error_code, "Kinsol internal error");
	(data->ErrorVecPtr)->AddErrorLine(module);
	(data->ErrorVecPtr)->AddErrorLine(function);
	(data->ErrorVecPtr)->AddErrorLine(msg);
	(data->ErrorVecPtr)->AddErrortovector();
	errtemp = (data->ErrorVecPtr)->GetLastError();

	returncode = errorlocal(&errtemp, 1);
};