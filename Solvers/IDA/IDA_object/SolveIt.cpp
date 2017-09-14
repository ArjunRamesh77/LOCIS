#include"IDAclass.h"


int solver_IDA::SolveIt(double tpoint)
{
	if (ESCAPE) return 0;
	int flag, flagr, returncode;
	realtype tret;
	double tout, iout, hstep;
	error lastError;


	//------------SOLVE-------------------------------------

	if (Useroptionsptr->solvetype == 1) // User knows the evaluation points	
	{
		int len = Useroptionsptr->timevectorlength;
		for (int i = 0; i < len; i++)
		{
			tout = Useroptionsptr->timepointvector[i];


			try
			{
				flag = IDASolve(IDAmem, tout, &tret, problem_data.yy, problem_data.yp, IDA_NORMAL);
			}
			catch (...)
			{
				IDAErrors->SetErr(-100, "IDA function failed");
				IDAErrors->AddErrorLine("something went wrong");
				IDAErrors->AddErrortovector();
				lastError = IDAErrors->GetLastError();
				ESCAPE = true; // Exit everything else

				returncode = UsererrorFuncptr(&lastError, 2); // return code 2 to let user know that code escapes

				return -100;

			}


			returncode = CheckFlag(flag, "IDASolve");
			if (returncode == 0) return 0;

			if (flag == IDA_ROOT_RETURN)
			{
				InfoFuncdata.rootReturn = true;

				flagr = IDAGetRootInfo(IDAmem, InfoFuncdata.rootsfound);
				returncode = CheckFlag(flag, "IDAGetRootInfo");
				if (returncode == 0) return 0;
			}

			PrintOutput(tret);
			InfoFuncdata.rootReturn = false;
		}
		PrintFinalStats();
		
	}
	else if (Useroptionsptr->solvetype == 2) // User wants to solve just at one point
	{
		//tout = Useroptionsptr->timepoint;
		tout = tpoint;

		try
		{
			flag = IDASolve(IDAmem, tout, &tret, problem_data.yy, problem_data.yp, IDA_NORMAL);
		}
		catch (...)
		{
			IDAErrors->SetErr(-100, "IDA function failed");
			IDAErrors->AddErrorLine("something went wrong");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			ESCAPE = true; // Exit everything else

			returncode = UsererrorFuncptr(&lastError, 2); // return code 2 to let user know that code escapes

			return -100;

		}
		returncode = CheckFlag(flag, "IDASolve");
		if (returncode == 0) return 0;

		if (flag == IDA_ROOT_RETURN)
		{
			InfoFuncdata.rootReturn = true;

			flagr = IDAGetRootInfo(IDAmem, InfoFuncdata.rootsfound);
			returncode = CheckFlag(flag, "IDAGetRootInfo");
			if (returncode == 0) return 0;
		}

		PrintOutput(tret);
		InfoFuncdata.rootReturn = false;

		PrintFinalStats();
	}
	else if (Useroptionsptr->solvetype == 3) // User wants values at uniformly distributed points
	{
		iout = 0; 
			
		hstep = (Useroptionsptr->tfinal - Useroptionsptr->t0) / (Useroptionsptr->nout - 1);

		tout = Useroptionsptr->t0 + hstep;
		iout = 0;
		while (iout <= Useroptionsptr->nout - 1)
		{

			try
			{

				flag = IDASolve(IDAmem, tout, &tret, problem_data.yy, problem_data.yp, IDA_NORMAL);
			}
			catch (...)
			{
				IDAErrors->SetErr(-100, "IDA function failed");
				IDAErrors->AddErrorLine("something went wrong");
				IDAErrors->AddErrortovector();
				lastError = IDAErrors->GetLastError();
				ESCAPE = true; // Exit everything else

				returncode = UsererrorFuncptr(&lastError, 2); // return code 2 to let user know that code escapes

				return -100;

			}
			returncode = CheckFlag(flag, "IDASolve");
			if (returncode == 0) return 0;

			if (flag == IDA_ROOT_RETURN)
			{
				InfoFuncdata.rootReturn = true;
				
				flagr = IDAGetRootInfo(IDAmem, InfoFuncdata.rootsfound);
				returncode = CheckFlag(flag, "IDAGetRootInfo");
				if (returncode == 0) return 0;
			}

			PrintOutput(tret);
			InfoFuncdata.rootReturn = false;

			if (flag == IDA_SUCCESS)
			{
				iout++;
				tout += hstep; // time point increase by hstep
			}

		}
		PrintFinalStats();
	}
	return 1;
}