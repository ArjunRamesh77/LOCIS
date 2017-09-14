#include "Kinsol_Class.h"
#include "Jac.h"

int solver_kinsol::SetOptions(KinsolOptions *options)
{
	if (ESCAPE) return 0;
	int flag;
	realtype fnormtol;
	KinsolOptions defaultoptions;
	error lastError;
	int returncode, returnflag;

	//***********************************
	// CHECK INPUT
	//***********************************
	if (options == NULL)
	{
		// print error message
		KinsolErrors->SetErr(0000, "Invalid User input");
		KinsolErrors->AddErrorLine("Uninitialized options");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UserErrFuncPtr(&lastError, 2);

		return -1;

	}


	//Save options as a part of kinsol
	KinsolOptionsObj = options;

	// Define default options
	defaultoptions.FuncNormTolerance = 1e-5;
	defaultoptions.ScaledStepTol = 1e-5;
	defaultoptions.if_jacobian = 1;
	defaultoptions.GlobalStrategy = KIN_NONE;
	defaultoptions.NoMinEps = FALSE;
	defaultoptions.MaxSetupCalls = 10;
	defaultoptions.MaxSubSetupCalls = 5;
	defaultoptions.finalstatsPrintLevel = 0;

	//***********************************
	// Function-norm stopping tolerance
	//***********************************

	if ((*options).FuncNormTolerance == -1)
	{
		// if the user doesn't define it, set default
		(*options).FuncNormTolerance = defaultoptions.FuncNormTolerance;
	}
	else
	{
		// check if the function tolerance is positive
		if ((*options).FuncNormTolerance > 0)
		{

			fnormtol = (*options).FuncNormTolerance;

			// Set tolerance
			flag = KINSetFuncNormTol(kmem, fnormtol);

			returnflag = CheckFlag(flag, "KINSETFUNCNORMTOL");
			if (returnflag == 0) return(0);

		}
		else
		{
			// print error message
			KinsolErrors->SetErr(1000, "Option setup error:");
			KinsolErrors->AddErrorLine("Function norm tolerance is negative");
			KinsolErrors->AddErrortovector();
			lastError = KinsolErrors->GetLastError();
			returncode = UserErrFuncPtr(&lastError, 1);
			if (returncode == 0) return(0);

		};
	};


	//***********************************
	// Scaled-step stopping tolerance
	//***********************************

	if ((*options).ScaledStepTol == -1)
	{

		// if the user doesn't define it, set default
		(*options).ScaledStepTol = defaultoptions.ScaledStepTol;
	}
	else
	{
		// check if the scaled-step tolerance is positive
		if ((*options).ScaledStepTol > 0)
		{
			// Set tolerance
			flag = KINSetScaledStepTol(kmem, (*options).ScaledStepTol);

			returnflag = CheckFlag(flag, "KINSETSCALEDSTEPTOL");
			if (returnflag == 0) return(0);
		}
		else
		{
			// print error message
			KinsolErrors->SetErr(1010, "Option setup error:");
			KinsolErrors->AddErrorLine("Scaled-step tolerance is negative");
			KinsolErrors->AddErrortovector();
			lastError = KinsolErrors->GetLastError();
			returncode = UserErrFuncPtr(&lastError, 1);
			if (returncode == 0) return(0);
		};
	};

	//***********************************
	// If jacobian is defined (0/1 option)
	//***********************************

	if ((*options).if_jacobian == -1)
	{
		(*options).if_jacobian = defaultoptions.if_jacobian; // 1 by default
	}
	else
	{
		// if user specifically wants to do it without jacobian
		if (((*options).if_jacobian != 0) && ((*options).if_jacobian != 1))
		{
		KinsolErrors->SetErr(1020, "Option setup error:");
		KinsolErrors->AddErrorLine("If_jacobian must be 0 or 1");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		returncode = UserErrFuncPtr(&lastError, 1);
		if (returncode == 0) return(0);
		};
	};

	//***********************************
	// Linear Solver
	//***********************************

	if ((*options).LinearSolver == "UNDEFINED")
	{
		KinsolErrors->SetErr(1030, "Option setup error:");
		KinsolErrors->AddErrorLine("Linear solver not defined");
		KinsolErrors->AddErrorLine("Linear solver must be one of the following:");
		KinsolErrors->AddErrorLine("KINDENSE, KINBAND, KINKLU");
		KinsolErrors->AddErrorLine("Input is case sensitive");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		returncode = UserErrFuncPtr(&lastError, 1);
		if (returncode == 0) return(0);
	}
	else if ((*options).LinearSolver == "KINDENSE")
	{
		flag = KINDense(kmem, problem_data.NEq);
		returnflag = CheckFlag(flag, "KINDENSE");
		if (returnflag == 0) return(0);

		// USE DENSE JACOBIAN ONLY WHEN USING DENSE LINEAR SOLVER
		if (KinsolOptionsObj->if_jacobian == 1)
		{
			flag = KINDlsSetDenseJacFn(kmem, jacfunc);
			returncode = CheckFlag(flag, "KINDLSSETDENSEJACFN");
			if (returncode == 0) return 0;
		}

	}
	else if ((*options).LinearSolver == "KINBAND")
	{

		if ((*options).mupper == -1)
		{
			// error saying define upper half band width
			KinsolErrors->SetErr(1040, "Option setup error:");
			KinsolErrors->AddErrorLine("KINBAND linear solver");
			KinsolErrors->AddErrorLine("Upper half band width not defined");
			KinsolErrors->AddErrortovector();
			lastError = KinsolErrors->GetLastError();
			returncode = UserErrFuncPtr(&lastError, 1);
			if (returncode == 0) return(0);
		}
		else if ((*options).mlower == -1)
		{
			// error saying define lower half band width
			KinsolErrors->SetErr(1050, "Option setup error:");
			KinsolErrors->AddErrorLine("KINBAND linear solver");
			KinsolErrors->AddErrorLine("Lower half band width not defined");
			KinsolErrors->AddErrortovector();
			lastError = KinsolErrors->GetLastError();
			returncode = UserErrFuncPtr(&lastError, 1);
			if (returncode == 0) return(0);
		}
		else // both are defined
		{
			if ((*options).mlower > 0 && (*options).mupper > 0)
			{

			//	flag = KINBand(kmem, problem_data.NEq, options.mupper, options.mlower);
			}
			else
			{
				if ((*options).mlower <= 0)
				{
					KinsolErrors->SetErr(1060, "Option setup error:");
					KinsolErrors->AddErrorLine("KINBAND linear solver");
					KinsolErrors->AddErrorLine("lower half band width must be positive");
					KinsolErrors->AddErrortovector();
					lastError = KinsolErrors->GetLastError();
					returncode = UserErrFuncPtr(&lastError, 1);
					if (returncode == 0) return(0);
				}
				else if ((*options).mlower <= 0)
				{
					KinsolErrors->SetErr(1070, "Option setup error:");
					KinsolErrors->AddErrorLine("KINBAND linear solver");
					KinsolErrors->AddErrorLine("upper half band width must be positive");
					KinsolErrors->AddErrortovector();
					lastError = KinsolErrors->GetLastError();
					returncode = UserErrFuncPtr(&lastError, 1);
					if (returncode == 0) return(0);
				}
				else
				{

				};
				// warning telling user to check the half bandwidth
			};
		};
	}
	else if ((*options).LinearSolver == "KINKLU")
	{
		if ((*options).NNZ == -1)
		{
			// error saying NNZ is must
			KinsolErrors->SetErr(1080, "Option setup error:");
			KinsolErrors->AddErrorLine("KINKLU linear solver");
			KinsolErrors->AddErrorLine("Problem dimension not defined");
			KinsolErrors->AddErrortovector();
			lastError = KinsolErrors->GetLastError();
			returncode = UserErrFuncPtr(&lastError, 1);
			if (returncode == 0) return(0);
		}
		else
		{
			if ((*options).NNZ > 0)
			{
	//			flag = KINKLU(kmem, problem_data.NEq, options.NNZ);
			}
			else
			{
				// error saying check NNZ
				KinsolErrors->SetErr(1090, "Option setup error:");
				KinsolErrors->AddErrorLine("KINKLU linear solver");
				KinsolErrors->AddErrorLine("problem dimension must be positive");
				KinsolErrors->AddErrortovector();
				lastError = KinsolErrors->GetLastError();
				returncode = UserErrFuncPtr(&lastError, 1);
				if (returncode == 0) return(0);
			};
		};
	}
	else
	{
		KinsolErrors->SetErr(1100, "Option setup error:");
		KinsolErrors->AddErrorLine("Incorrect Linear solver name");
		KinsolErrors->AddErrorLine("Linear solver must be one of the following:");
		KinsolErrors->AddErrorLine("KINDENSE, KINBAND, KINKLU");
		KinsolErrors->AddErrorLine("Input is case sensitive");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		returncode = UserErrFuncPtr(&lastError, 1);
		if (returncode == 0) return(0);
	};

	//***********************************
	// Global Strategy
	//***********************************

	if ((*options).GlobalStrategy == -1)
	{
		KinsolErrors->SetErr(1110, "Option setup error:");
		KinsolErrors->AddErrorLine("Global strategy is not defined");
		KinsolErrors->AddErrorLine("Global strategy must be one of the following:");
		KinsolErrors->AddErrorLine("KIN_NONE 0, KIN_LINESEARCH 1, KIN_PICARD 2, KIN_FP 3");
		KinsolErrors->AddErrorLine("Input is case sensitive");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		returncode = UserErrFuncPtr(&lastError, 1);
		if (returncode == 0) return(0);
	}
	else if ((*options).GlobalStrategy == KIN_NONE)
	{
		(*options).GlobalStrategy = KIN_NONE;
	}
	else if ((*options).GlobalStrategy == KIN_LINESEARCH)
	{
		(*options).GlobalStrategy = KIN_LINESEARCH;
	}
	else if ((*options).GlobalStrategy == KIN_FP)
	{
		(*options).GlobalStrategy = KIN_FP;
	}
	else if ((*options).GlobalStrategy == KIN_PICARD)
	{
		(*options).GlobalStrategy = KIN_PICARD;
	}
	else
	{
		KinsolErrors->SetErr(1120, "Option setup error:");
		KinsolErrors->AddErrorLine("Incorrect Global strategy");
		KinsolErrors->AddErrorLine("Global strategy must be one of the following:");
		KinsolErrors->AddErrorLine("KIN_NONE 0, KIN_LINESEARCH 1, KIN_PICARD 2, KIN_FP 3");
		KinsolErrors->AddErrorLine("Input is case sensitive");
		KinsolErrors->AddErrortovector();
		lastError = KinsolErrors->GetLastError();
		returncode = UserErrFuncPtr(&lastError, 1);
		if (returncode == 0) return(0);
	};

	//***********************************
	// No MIN Eps
	//***********************************

	if ((*options).NoMinEps == false)
	{
		(*options).NoMinEps = defaultoptions.NoMinEps;
	}

	//***********************************
	// Max Setup Calls
	//***********************************

	if ((*options).MaxSetupCalls == -1)
	{
		(*options).MaxSetupCalls = defaultoptions.MaxSetupCalls;
	}
	else
	{ 
		//check if user supplied value makes sense if 
		//it does keep it else print error message
		if ((*options).MaxSetupCalls > 0)
		{
			flag = KINSetMaxSetupCalls(kmem, (*options).MaxSetupCalls);
			returnflag = CheckFlag(flag, "KINSETMAXSETUPCALL");
			if (returnflag == 0) return(0);
		}
		else
		{
			KinsolErrors->SetErr(1130, "Option setup error:");
			KinsolErrors->AddErrorLine("maximum set up call number must be positive");
			KinsolErrors->AddErrortovector();
			lastError = KinsolErrors->GetLastError();
			returncode = UserErrFuncPtr(&lastError, 1);
			if (returncode == 0) return(0);
		};
		
	};

	//***********************************
	// Max Sub Setup Calls
	//***********************************

	if ((*options).MaxSubSetupCalls == -1)
	{
		(*options).MaxSubSetupCalls = defaultoptions.MaxSubSetupCalls;
	}
	else
	{
		//check if user supplied value makes sense if 
		//it does keep it else print error message
		if ((*options).MaxSubSetupCalls > 0)
		{
			flag = KINSetMaxSubSetupCalls(kmem, (*options).MaxSubSetupCalls);
			returnflag = CheckFlag(flag, "KINSETMAXSUBSETUPCALLS");
			if (returnflag == 0) return(0);

		}
		else
		{
			KinsolErrors->SetErr(1140, "Option setup error:");
			KinsolErrors->AddErrorLine("maximum sub set up call number must be positive");
			KinsolErrors->AddErrortovector();
			lastError = KinsolErrors->GetLastError();
			returncode = UserErrFuncPtr(&lastError, 1);
			if (returncode == 0) return(0);
		};

	};

	if ((*options).finalstatsPrintLevel == -1)
	{
		(*options).finalstatsPrintLevel = defaultoptions.finalstatsPrintLevel;
	}
	else
	{
		if ((*options).finalstatsPrintLevel != 0 && (*options).finalstatsPrintLevel != 1 && (*options).finalstatsPrintLevel != 2)
		{
			KinsolErrors->SetErr(1150, "Option setup error:");
			KinsolErrors->AddErrorLine("final statistics print level must be either 0, 1 or 2");
			KinsolErrors->AddErrortovector();
			lastError = KinsolErrors->GetLastError();
			returncode = UserErrFuncPtr(&lastError, 1);
			if (returncode == 0) return(0);
		}
	}
	return(1);
}