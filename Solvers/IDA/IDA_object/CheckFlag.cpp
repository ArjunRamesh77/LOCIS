#include "IDAClass.h"

// Returns 0 if user tells to stop

int solver_IDA::CheckFlag(int flagvalue, std::string funcname)
{

	if (ESCAPE) return 0;
	std::string strname;
	int returncode;
	error lastError;
	if (funcname == "IDACalcIC")
	{
		if (flagvalue < 0)
		{
			strname = funcname + " failed";
			IDAErrors->SetErr(0001, strname);
			IDAErrors->AddErrorLine("IDA memory pointer is NULL");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			ESCAPE = true; // Exit everything else

			returncode = UsererrorFuncptr(&lastError, -1); // return code -1 to let user know that initialization error

			return -1;
		}
	}


	if (flagvalue == IDA_MEM_NULL)
	{
		// print error message
		strname = funcname + " failed";
		IDAErrors->SetErr(0001, strname);
		IDAErrors->AddErrorLine("IDA memory pointer is NULL");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		ESCAPE = true; // Exit everything else

		returncode = UsererrorFuncptr(&lastError, 2); // return code 2 to let user know that code escapes

		return 0; // kmem is NULL, don't wait for return code just exit
	}
	else if (flagvalue == IDA_ILL_INPUT)
	{
		// print error message
		strname = funcname + " failed";
		IDAErrors->SetErr(0011, strname);
		IDAErrors->AddErrorLine("Input argument has illegal value");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UsererrorFuncptr(&lastError, 2);
		return 0;
	}
	else if (flagvalue == IDA_MEM_FAIL)
	{
		// print error message
		strname = funcname + " failed";
		IDAErrors->SetErr(0011, strname);
		IDAErrors->AddErrorLine("Initialization failed");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UsererrorFuncptr(&lastError, 2);
		return 0;
	}
	else if (flagvalue == IDA_NO_MALLOC)
	{
		// print error message
		strname = funcname + " failed";
		IDAErrors->SetErr(0001, strname);
		IDAErrors->AddErrorLine("IDA Init has not been called");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		ESCAPE = true; // Exit everything else

		returncode = UsererrorFuncptr(&lastError, 2); // return code 2 to let user know that code escapes

		return 0; // kmem is NULL, don't wait for return code just exit
	}
	else if (flagvalue == IDADLS_MEM_NULL)
	{
		// print error message
		strname = funcname + " failed";
		IDAErrors->SetErr(0001, strname);
		IDAErrors->AddErrorLine("IDA memory pointer is NULL");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		ESCAPE = true; // Exit everything else

		returncode = UsererrorFuncptr(&lastError, 2); // return code 2 to let user know that code escapes

		return 0; // kmem is NULL, don't wait for return code just exit
	}
	else if (flagvalue == IDADLS_MEM_FAIL)
	{
		// print error message
		strname = funcname + " failed";
		IDAErrors->SetErr(0011, strname);
		IDAErrors->AddErrorLine("Initialization failed");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UsererrorFuncptr(&lastError, 2);
		return 0;
	}
	else if (flagvalue == IDADLS_ILL_INPUT)
	{
		// print error message
		strname = funcname + " failed";
		IDAErrors->SetErr(0011, strname);
		IDAErrors->AddErrorLine("IDADENSE is not compatible with current nvector module");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UsererrorFuncptr(&lastError, 2);
		return 0;
	}
	else if (flagvalue < 0)
	{
		// print error message
		strname = funcname + " failed";
		IDAErrors->SetErr(0001, strname);
		strname = "FLAG VALUE" + std::to_string(flagvalue);
		IDAErrors->AddErrorLine(strname);
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		ESCAPE = true; // Exit everything else

		returncode = UsererrorFuncptr(&lastError, 2); // return code 2 to let user know that code escapes

		return 0; // kmem is NULL, don't wait for return code just exit

	}
	else
	{ 
	}

	return(1);
}