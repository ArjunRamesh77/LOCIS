#include "Kinsol_Class.h"

int solver_kinsol::SolveIt(double *x)
{
	if (ESCAPE) return 0;

	int flag, returncode;
	N_Vector s;
	double *ud;
	error lastError;


	InitializeNVector(s, problem_data.NEq);

	N_VConst_Serial(1, s); /* no scaling */
	try
	{
		flag = KINSol(kmem, problem_data.initvector, KinsolOptionsObj->GlobalStrategy, s, s);
	}
	catch (...)
	{
		KinsolErrors->SetErr(-100, "KINSOL function failed");
		KinsolErrors->AddErrorLine("something went wrong");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		ESCAPE = true; // Exit everything else

		returncode = UserErrFuncPtr(&lastError, 2); // return code 2 to let user know that code escapes

		return -100;
	}
	flag = CheckFlag(flag, "KINSol");
	if (flag == 0) return 0;

	ud = NV_DATA_S(problem_data.initvector);

	for (int i = 0; i < problem_data.NVar; i++)
	{
		problem_data.x[i] = ud[i];
		x[i] = problem_data.x[i];
	}

	PrintFinalStats(kmem, KinsolOptionsObj->finalstatsPrintLevel);
	N_VDestroy(s);
	return 1;
};
