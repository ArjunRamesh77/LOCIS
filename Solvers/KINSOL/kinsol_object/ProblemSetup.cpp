#include "Kinsol_Class.h"
#include "func.h"
#include"error.h"
#include"info.h"

int solver_kinsol::ProblemSetup(int NVAR, int NEQ, 
	double *init_val, CallbackRes resptr, CallbackJac jacptr, 
	CallbackInfo userinfofunptr, CallbackError usererrfunptr, error_kinsol *usererrvecptr)
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
		KinsolErrors = usererrvecptr;
	}
	if (usererrfunptr == NULL) // if error function pointer is null exit immediately
	{
		return -1;
	}
	else
	{
		UserErrFuncPtr = usererrfunptr;
	}
	// CHECK if all inputs are valid
	returncode = CheckUserInput(NVAR, NEQ, init_val, resptr, jacptr, 
		userinfofunptr);
	if (returncode == -1) return -1; // User input error

	UserInfoFuncPtr = userinfofunptr;

	//Copy Userdata to local object
	userdataKIN.resptr = resptr;
	userdataKIN.jacptr = jacptr;
	userdataKIN.UserInfoFuncPtr = userinfofunptr;
	userdataKIN.UserErrFuncPtr = usererrfunptr;

	UserErrFuncPtr = usererrfunptr;

	//Contruct dataerrorpointer
	ErrFuncData.UserErrFuncPtr = UserErrFuncPtr;
	ErrFuncData.ErrVecPtr = KinsolErrors;

	problem_data.x = new double[NVAR];

	// Set problem data
	SetProblemdata(NVAR, NEQ, init_val);

	// Set info func data
	InfoFuncData.UserInfoFuncPtr = UserInfoFuncPtr;


	// Allocate Jacboan Jtrial
	allocateJtrial();


	kmem = NULL;
	kmem = KINCreate();
	if (kmem == NULL)
	{
		// print error message
		KinsolErrors->SetErr(0000, "KINCREATE Error");
		KinsolErrors->AddErrorLine("KINSOL memory pointer is NULL");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UserErrFuncPtr(&lastError, 2);

		return 0; // memory is NULL, don't wait for return code just exit
	}

	flag = KINSetErrHandlerFn(kmem, ERRORFUN, &ErrFuncData);
	returncode = CheckFlag(flag, "KINSETERRHANDLERFN");
	if (returncode == 0) return 0;

	flag = KINSetUserData(kmem, &userdataKIN);
	returncode = CheckFlag(flag, "KINSETUSERDATA");
	if (returncode == 0) return 0;

	flag = KINInit(kmem, func, problem_data.initvector);
	returncode = CheckFlag(flag, "KINInit");
	if (returncode == 0) return 0;

	// SET info func data
	flag = KINSetInfoHandlerFn(kmem, IHFUN, &InfoFuncData);
	returncode = CheckFlag(flag, "KINSETINFOHANDLERFN");
	if (returncode == 0) return 0;

	flag = KINSetPrintLevel(kmem, 2);
	returncode = CheckFlag(flag, "KINSETPRINTLEVEL");
	if (returncode == 0) return 0;


	return 1;
};


