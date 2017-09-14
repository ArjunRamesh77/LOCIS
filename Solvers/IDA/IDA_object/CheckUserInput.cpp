#include "IDAclass.h"
#include"res.h"
#include"Error.h"

int solver_IDA::CheckUserInput(int NEQ, double* y_init, double* yp_init,
	CallbackJac jacptr, CallbackRes resptr,	CallbackInfo infofuncptr, CallbackRoot rootptr)
{
	int returncode;
	error lastError;

	if (NEQ <= 0)
	{
		// print error message
		IDAErrors->SetErr(0000, "Invalid User input");
		IDAErrors->AddErrorLine("Number of Equations must be positive");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UsererrorFuncptr(&lastError, 2);

		return -1;
	}

	if (y_init == NULL)
	{
		// print error message
		IDAErrors->SetErr(0000, "Invalid User input");
		IDAErrors->AddErrorLine("Uninitialized initial value");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UsererrorFuncptr(&lastError, 2);

		return -1;
	}

	if (yp_init == NULL)
	{
		// print error message
		IDAErrors->SetErr(0000, "Invalid User input");
		IDAErrors->AddErrorLine("Uninitialized initial value");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UsererrorFuncptr(&lastError, 2);

		return -1;
	}

	if (jacptr == NULL)
	{
		// print error message
		IDAErrors->SetErr(0000, "Invalid User input");
		IDAErrors->AddErrorLine("Uninitialized jacobian function");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UsererrorFuncptr(&lastError, 2);

		return -1;
	}


	if (resptr == NULL)
	{
		// print error message
		IDAErrors->SetErr(0000, "Invalid User input");
		IDAErrors->AddErrorLine("Uninitialized residual function");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UsererrorFuncptr(&lastError, 2);

		return -1;
	}

	if (infofuncptr == NULL)
	{
		// print error message
		IDAErrors->SetErr(0000, "Invalid User input");
		IDAErrors->AddErrorLine("Uninitialized User info handler function");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UsererrorFuncptr(&lastError, 2);

		return -1;
	}

	return 1;
}