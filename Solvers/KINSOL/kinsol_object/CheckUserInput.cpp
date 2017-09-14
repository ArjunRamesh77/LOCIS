#include "Kinsol_Class.h"
#include "func.h"
#include"error.h"
#include"info.h"

int solver_kinsol::CheckUserInput(int NVAR, int NEQ,
	double *init_val, CallbackRes resptr, CallbackJac jacptr,
	CallbackInfo userinfofunptr)
{
	int returncode;
	error lastError;

	if (NVAR <= 0)
	{
		// print error message
		KinsolErrors->SetErr(0000, "Invalid User input");
		KinsolErrors->AddErrorLine("Number of variables must be positive");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UserErrFuncPtr(&lastError, 2);

		return -1;
	}

	if (NEQ <= 0)
	{
		// print error message
		KinsolErrors->SetErr(0000, "Invalid User input");
		KinsolErrors->AddErrorLine("Number of Equations must be positive");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UserErrFuncPtr(&lastError, 2);

		return -1;
	}

	if (NVAR != NEQ)
	{
		// print error message
		KinsolErrors->SetErr(0000, "Invalid User input");
		KinsolErrors->AddErrorLine("Number of variables must be equal to number of equations");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UserErrFuncPtr(&lastError, 2);

		return -1;
	}

	if (init_val == NULL)
	{
		// print error message
		KinsolErrors->SetErr(0000, "Invalid User input");
		KinsolErrors->AddErrorLine("Uninitialized initial value");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UserErrFuncPtr(&lastError, 2);

		return -1;
	}

	if (resptr == NULL)
	{
		// print error message
		KinsolErrors->SetErr(0000, "Invalid User input");
		KinsolErrors->AddErrorLine("Uninitialized residual function");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UserErrFuncPtr(&lastError, 2);

		return -1;
	}

	if (jacptr == NULL)
	{
		// print error message
		KinsolErrors->SetErr(0000, "Invalid User input");
		KinsolErrors->AddErrorLine("Uninitialized jacobian function");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UserErrFuncPtr(&lastError, 2);

		return -1;
	}

	if (userinfofunptr == NULL)
	{
		// print error message
		KinsolErrors->SetErr(0000, "Invalid User input");
		KinsolErrors->AddErrorLine("Uninitialized User info handler function");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UserErrFuncPtr(&lastError, 2);

		return -1;
	}


	return 1;
}