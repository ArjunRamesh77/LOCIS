#include "Kinsol_Class.h"

void solver_kinsol::PrintFinalStats(void *kmem, int printlevel)
{
	if (ESCAPE) return;

	long int nni, nfe, nje, nfeD, lenrw, leniw, nbcfails,
		nbacktr, lenrwB, leniwB, lsflag;
	realtype fnorm, steplength;
	int flag, returncode;

	if (printlevel == 0)
	{

	}
	else if (printlevel == 1)
	{
		flag = KINGetNumNonlinSolvIters(kmem, &nni);
		returncode = CheckFlag(flag, "KINGetNumNonlinSolvIters");
		if (returncode == 0) return;
		finalstats1.nni = nni;

		flag = KINGetNumFuncEvals(kmem, &nfe);
		returncode = CheckFlag(flag, "KINGetNumFuncEvals");
		if (returncode == 0) return;
		finalstats1.nfe = nfe;

		flag = KINDlsGetNumJacEvals(kmem, &nje);
		returncode = CheckFlag(flag, "KINDlsGetNumJacEvals");
		if (returncode == 0) return;
		finalstats1.nje = nje;

		flag = KINDlsGetNumFuncEvals(kmem, &nfeD);
		returncode = CheckFlag(flag, "KINDlsGetNumFuncEvals");
		if (returncode == 0) return;
		finalstats1.nfeD = nfeD;

		UserInfoFuncPtr(&finalstats1, 1);
	}
	else if (printlevel == 2)
	{
		flag = KINGetNumNonlinSolvIters(kmem, &nni);
		returncode = CheckFlag(flag, "KINGetNumNonlinSolvIters");
		if (returncode == 0) return;
		finalstats2.nni = nni;

		flag = KINGetNumFuncEvals(kmem, &nfe);
		returncode = CheckFlag(flag, "KINGetNumFuncEvals");
		if (returncode == 0) return;
		finalstats2.nfe = nfe;

		flag = KINDlsGetNumJacEvals(kmem, &nje);
		returncode = CheckFlag(flag, "KINDlsGetNumJacEvals");
		if (returncode == 0) return;
		finalstats2.nje = nje;

		flag = KINDlsGetNumFuncEvals(kmem, &nfeD);
		returncode = CheckFlag(flag, "KINDlsGetNumFuncEvals");
		if (returncode == 0) return;
		finalstats2.nfeD = nfeD;

		flag = KINGetWorkSpace(kmem, &lenrw, &leniw);
		returncode = CheckFlag(flag, "KINGetWorkSpace");
		if (returncode == 0) return;
		finalstats2.leniw = leniw;
		finalstats2.lenrw = lenrw;

		flag = KINGetNumBetaCondFails(kmem, &nbcfails);
		returncode = CheckFlag(flag, "KINGetNumBetaCondFails");
		if (returncode == 0) return;
		finalstats2.nbcfails = nbcfails;

		flag = KINGetNumBacktrackOps(kmem, &nbacktr);
		returncode = CheckFlag(flag, "KINGetNumBacktrackOps");
		if (returncode == 0) return;
		finalstats2.nbacktr = nbacktr;

		flag = KINGetFuncNorm(kmem, &fnorm);
		returncode = CheckFlag(flag, "KINGetFuncNorm");
		if (returncode == 0) return;
		finalstats2.fnorm = fnorm;

		flag = KINGetStepLength(kmem, &steplength);
		returncode = CheckFlag(flag, "KINGetStepLength");
		if (returncode == 0) return;
		finalstats2.steplength = steplength;

		flag = KINDlsGetWorkSpace(kmem, &lenrwB, &leniwB);
		returncode = CheckFlag(flag, "KINDlsGetWorkSpace");
		if (returncode == 0) return;
		finalstats2.leniwB = leniwB;
		finalstats2.lenrwB = lenrwB;

		flag = KINDlsGetLastFlag(kmem, &lsflag);
		returncode = CheckFlag(flag, "KINDlsGetLastFlag");
		if (returncode == 0) return;
		finalstats2.lsflag = lsflag;
		UserInfoFuncPtr(&finalstats2, 2);

	}
}