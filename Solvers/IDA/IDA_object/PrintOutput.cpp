#include"IDAclass.h"

/*
* Print Output
*/

void solver_IDA::PrintOutput(realtype t)
{
	realtype *yval, *yp_val;
	int retval, kused, code;
	long int nst;
	realtype hused;

	yval = NV_DATA_S(problem_data.yy);
	yp_val = NV_DATA_S(problem_data.yp);

	retval = IDAGetLastOrder(IDAmem, &kused);
	code = CheckFlag(retval, "IDAGetLastOrder");
	retval = IDAGetNumSteps(IDAmem, &nst);
	code = CheckFlag(retval, "IDAGetNumSteps");
	retval = IDAGetLastStep(IDAmem, &hused);
	code = CheckFlag(retval, "IDAGetLastStep");

	InfoFuncdata.NEQ = problem_data.NEq;
	InfoFuncdata.LastOrder = kused;
	InfoFuncdata.Numsteps = nst;
	InfoFuncdata.LastStep = nst;
	InfoFuncdata.t = t;
	InfoFuncdata.yval = yval;
	InfoFuncdata.yp_val = yp_val;
	InfoFuncdata.numroots = Useroptionsptr->NrootFn; // use only if positive
	code = UserInfoFuncptr(&InfoFuncdata, 0);

}