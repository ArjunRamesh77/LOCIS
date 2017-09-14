#include "Kinsol_Class.h"

// Returns 0 if user tells to stop

int solver_kinsol::CheckFlag(int flagvalue, std::string funcname)
{
	if (ESCAPE) return 0;

	std::string strname;
	int returncode;
	error lastError;

	if (flagvalue == KIN_MEM_NULL)
	{
		// print error message
		strname = funcname + " failed";
		KinsolErrors->SetErr(0001, strname);
		KinsolErrors->AddErrorLine("KINSOL memory pointer is NULL");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		ESCAPE = true; // Exit everything else

		returncode = UserErrFuncPtr(&lastError, 2); // return code 2 to let user know that code escapes

		return 0; // kmem is NULL, don't wait for return code just exit
	}
	else if (flagvalue == KIN_ILL_INPUT)
	{
		// print error message
		strname = funcname + " failed";
		KinsolErrors->SetErr(0011, strname);
		KinsolErrors->AddErrorLine("Input argument has illegal value");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UserErrFuncPtr(&lastError, 2);
		return 0;
	}
	else if (flagvalue == KIN_MEM_FAIL)
	{
		// print error message
		strname = funcname + " failed";
		KinsolErrors->SetErr(0011, strname);
		KinsolErrors->AddErrorLine("Initialization failed");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UserErrFuncPtr(&lastError, 2);
		return 0;
	}
	else if (flagvalue == KINDLS_MEM_NULL)
	{
		// print error message
		strname = funcname + " failed";
		KinsolErrors->SetErr(0001, strname);
		KinsolErrors->AddErrorLine("KINSOL memory pointer is NULL");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		ESCAPE = true; // Exit everything else

		returncode = UserErrFuncPtr(&lastError, 2); // return code 2 to let user know that code escapes

		return 0; // kmem is NULL, don't wait for return code just exit
	}
	else if (flagvalue == KINDLS_MEM_FAIL)
	{
		// print error message
		strname = funcname + " failed";
		KinsolErrors->SetErr(0011, strname);
		KinsolErrors->AddErrorLine("Initialization failed");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UserErrFuncPtr(&lastError, 2);
		return 0;
	}
	else if (flagvalue == KINDLS_LMEM_NULL)
	{
		// print error message
		strname = funcname + " failed";
		KinsolErrors->SetErr(0001, strname);
		KinsolErrors->AddErrorLine("KINDENSE linear solver not initialized");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		ESCAPE = true; // Exit everything else

		returncode = UserErrFuncPtr(&lastError, 2); // return code 2 to let user know that code escapes

		return 0; // kmem is NULL, don't wait for return code just exit
	}
	else if (flagvalue < 0)
	{
		// print error message
		strname = funcname + " failed";
		KinsolErrors->SetErr(0001, strname);
		strname = "FLAG VALUE" + std::to_string(flagvalue);
		KinsolErrors->AddErrorLine(strname);
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		ESCAPE = true; // Exit everything else

		returncode = UserErrFuncPtr(&lastError, 2); // return code 2 to let user know that code escapes

		return 0; // kmem is NULL, don't wait for return code just exit
	}

	return(1);
}