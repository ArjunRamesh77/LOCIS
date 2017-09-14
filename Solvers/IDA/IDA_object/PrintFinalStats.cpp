#include"IDAclass.h"

/*
* Print final integrator statistics
*/

void solver_IDA::PrintFinalStats()
{
	if (ESCAPE) return;
	int retval, returncode, klast, kcur;
	long int nst, nni, nje, nre, nreLS, netf, ncfn, nge,
		lenrw, leniw, nlinsetups, netfails;
	
	double	hlast, hcur, hinused, tcur, tolsfac;

	if (Useroptionsptr->finalstatsprintlevel == 0)
	{

	}
	else if (Useroptionsptr->finalstatsprintlevel == 1)
	{
		finalstats1.NEQ = problem_data.NEq;
		retval = IDAGetNumSteps(IDAmem, &nst);
		returncode = CheckFlag(retval, "IDAGetNumSteps");
		if (returncode == 0) return;
		finalstats1.nst = nst;

		retval = IDAGetNumResEvals(IDAmem, &nre);
		returncode = CheckFlag(retval, "IDAGetNumResEvals");
		if (returncode == 0) return;
		finalstats1.nre = nre;

		retval = IDADlsGetNumJacEvals(IDAmem, &nje);
		returncode = CheckFlag(retval, "IDADlsGetNumJacEvals");
		if (returncode == 0) return;
		finalstats1.nje = nje;

		retval = IDAGetNumNonlinSolvIters(IDAmem, &nni);
		returncode = CheckFlag(retval, "IDAGetNumNonlinSolvIters");
		if (returncode == 0) return;
		finalstats1.nni = nni;


		retval = IDAGetNumErrTestFails(IDAmem, &netf);
		returncode = CheckFlag(retval, "IDAGetNumErrTestFails");
		if (returncode == 0) return;
		finalstats1.netf = netf;

		retval = IDAGetNumNonlinSolvConvFails(IDAmem, &ncfn);
		returncode = CheckFlag(retval, "IDAGetNumNonlinSolvConvFails");
		if (returncode == 0) return;
		finalstats1.ncfn = ncfn;

		retval = IDADlsGetNumResEvals(IDAmem, &nreLS);
		returncode = CheckFlag(retval, "IDADlsGetNumResEvals");
		if (returncode == 0) return;
		finalstats1.nreLS = nreLS;

		retval = IDAGetNumGEvals(IDAmem, &nge);
		returncode = CheckFlag(retval, "IDAGetNumGEvals");
		if (returncode == 0) return;
		finalstats1.nge = nge;

		//retval = IDAGetConsistentIC(IDAmem, problem_data.yy, problem_data.yp);
		//returncode = CheckFlag(retval, "IDAGetConsistentIC");
		//if (returncode == 0) return;
		//finalstats1.Init_cond_y = NV_DATA_S(problem_data.yy);
		//finalstats1.Init_cond_yp = NV_DATA_S(problem_data.yp);

		std::cout << finalstats1.nni << "\n";
		std::cout << finalstats1.nge << "\n";
		returncode = UserInfoFuncptr(&finalstats1, 1);
 	}
	else if (Useroptionsptr->finalstatsprintlevel == 2)
	{
		finalstats2.NEQ = problem_data.NEq;
		retval = IDAGetNumSteps(IDAmem, &nst);
		returncode = CheckFlag(retval, "IDAGetNumSteps");
		if (returncode == 0) return;
		finalstats2.nst = nst;

		retval = IDAGetNumResEvals(IDAmem, &nre);
		returncode = CheckFlag(retval, "IDAGetNumResEvals");
		if (returncode == 0) return;
		finalstats2.nre = nre;

		retval = IDADlsGetNumJacEvals(IDAmem, &nje);
		returncode = CheckFlag(retval, "IDADlsGetNumJacEvals");
		if (returncode == 0) return;
		finalstats2.nje = nje;

		retval = IDAGetNumNonlinSolvIters(IDAmem, &nni);
		returncode = CheckFlag(retval, "IDAGetNumNonlinSolvIters");
		if (returncode == 0) return;
		finalstats2.nni = nni;

		retval = IDAGetNumErrTestFails(IDAmem, &netf);
		returncode = CheckFlag(retval, "IDAGetNumErrTestFails");
		if (returncode == 0) return;
		finalstats2.netf = netf;

		retval = IDAGetNumNonlinSolvConvFails(IDAmem, &ncfn);
		returncode = CheckFlag(retval, "IDAGetNumNonlinSolvConvFails");
		if (returncode == 0) return;
		finalstats2.ncfn = ncfn;

		retval = IDADlsGetNumResEvals(IDAmem, &nreLS);
		returncode = CheckFlag(retval, "IDADlsGetNumResEvals");
		if (returncode == 0) return;
		finalstats2.nreLS = nreLS;

		retval = IDAGetNumGEvals(IDAmem, &nge);
		returncode = CheckFlag(retval, "IDAGetNumGEvals");
		if (returncode == 0) return;
		finalstats2.nge = nge;

		retval = IDAGetWorkSpace(IDAmem, &lenrw, &leniw);
		returncode = CheckFlag(retval, "IDAGetWorkSpace");
		if (returncode == 0) return;
		finalstats2.lenrw = lenrw;
		finalstats2.leniw = leniw;

		retval = IDAGetNumLinSolvSetups(IDAmem, &nlinsetups);
		returncode = CheckFlag(retval, "IDAGetNumLinSolvSetups");
		if (returncode == 0) return;
		finalstats2.nlinsetups = nlinsetups;

		retval = IDAGetNumErrTestFails(IDAmem, &netfails);
		returncode = CheckFlag(retval, "IDAGetNumErrTestFails");
		if (returncode == 0) return;
		finalstats2.netfails = netfails;

		retval = IDAGetLastOrder(IDAmem, &klast);
		returncode = CheckFlag(retval, "IDAGetLastOrder");
		if (returncode == 0) return;
		finalstats2.klast = klast;

		retval = IDAGetCurrentOrder(IDAmem, &kcur);
		returncode = CheckFlag(retval, "IDAGetCurrentOrder");
		if (returncode == 0) return;
		finalstats2.kcur = kcur;

		retval = IDAGetLastStep(IDAmem, &hlast);
		returncode = CheckFlag(retval, "IDAGetLastStep");
		if (returncode == 0) return;
		finalstats2.hlast = hlast;

		retval = IDAGetCurrentStep(IDAmem, &hcur);
		returncode = CheckFlag(retval, "IDAGetCurrentStep");
		if (returncode == 0) return;
		finalstats2.hcur = hcur;

		retval = IDAGetActualInitStep(IDAmem, &hinused);
		returncode = CheckFlag(retval, "IDAGetActualInitStep");
		if (returncode == 0) return;
		finalstats2.hinused = hinused;

		
		retval = IDAGetCurrentTime(IDAmem, &tcur);
		returncode = CheckFlag(retval, "IDAGetCurrentTime");
		if (returncode == 0) return;
		finalstats2.tcur = tcur;

		
		retval = IDAGetTolScaleFactor(IDAmem, &tolsfac);
		returncode = CheckFlag(retval, "IDAGetTolScaleFactor");
		if (returncode == 0) return;
		finalstats2.tolsfac = tolsfac;

		//retval = IDAGetErrWeights(mem, &eweight);
		//returncode = CheckFlag(retval, "IDAGetErrWeights");
		//if (returncode == 0) return;
		//finalstats2.eweight = eweight;

		//IDAGetEstLocalErrors
		returncode = UserInfoFuncptr(&finalstats2, 2);
	}
		


}