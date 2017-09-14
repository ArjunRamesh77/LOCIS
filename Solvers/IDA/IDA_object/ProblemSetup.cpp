#include "IDAclass.h"
#include"res.h"
#include"Error.h"
int solver_IDA::ProblemSetup(int NEQ, double* y_init, double* yp_init, 
	CallbackJac jacptr, CallbackRes resptr, CallbackError errorfuncptr,
	CallbackInfo infofuncptr, CallbackRoot rootptr, error_IDA *usererrvecptr)
{
	if (ESCAPE) return 0;
	int flag, returncode;
	error lastError;

	if (usererrvecptr == NULL) // if error vector is null exit immediately
	{
		return -1;
	}
	else
	{
		IDAErrors = usererrvecptr;
	}
	if (errorfuncptr == NULL) // if error function pointer is null exit immediately
	{
		return -1;
	}
	else
	{
		UsererrorFuncptr = errorfuncptr;
	}

	// CHECK if all inputs are valid
	returncode = CheckUserInput(NEQ, y_init, yp_init, jacptr, resptr,
		infofuncptr, rootptr);
	if (returncode == -1) return -1; // User input error


	UserInfoFuncptr = infofuncptr;

	UserRootPtr = rootptr;

	//	IDA error handler function userdata
	ErrFuncdata.Usererrorfuncptr = UsererrorFuncptr;
	ErrFuncdata.ErrorVecPtr = IDAErrors;


	// userdataIDA is used for setuserdata
	userdataIDA.jacptr = jacptr;
	userdataIDA.resptr = resptr;

	userdataIDA.rootptr = rootptr; // if it is null it will be taken care of inside dll_root.cpp

	SetProblemdata(NEQ, y_init, yp_init);

	// Allocate Jacboan Jtrial
	allocateJtrial();


	SetInitialGuess();
	
	// NEW PART--- ALLOCATE IDAMEM IN OPTIONS
	IDAmem = NULL;

	IDAmem = IDACreate();
	if (IDAmem == NULL)
	{
		IDAErrors->SetErr(1000, "IDACREATE Error");
		IDAErrors->AddErrorLine("IDA memory pointer is NULL");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		ESCAPE = true;
		returncode = UsererrorFuncptr(&lastError, 1);
		if (returncode == 0) return 0;
	}

	flag = IDASetErrHandlerFn(IDAmem, ERRORFUN, &ErrFuncdata);
	returncode = CheckFlag(flag, "IDASetErrHandlerFn");
	if (returncode == 0) return 0;

	flag = IDASetUserData(IDAmem, &userdataIDA);
	returncode = CheckFlag(flag, "IDASetUserData");
	if (returncode == 0) return 0;

	flag = IDAInit(IDAmem, res, 0, problem_data.yy, problem_data.yp);
	returncode = CheckFlag(flag, "IDAInit");
	if (returncode == 0) return 0;
	

	return 1;
}