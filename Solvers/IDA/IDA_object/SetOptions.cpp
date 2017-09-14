#include"IDAclass.h"
#include "root.h"
#include"jac.h"

int solver_IDA::SetOptions(IDAOptions *options)
{

	if (ESCAPE) return 0;
	int flag, solvetype, retval;
	IDAOptions defaultoptions;
	error lastError;
	int returncode;

	//***********************************
	// CHECK INPUT
	//***********************************
	if (options == NULL)
	{
		// print error message
		IDAErrors->SetErr(0000, "Invalid User input");
		IDAErrors->AddErrorLine("Uninitialized options");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		ESCAPE = true; // Exit everything else
		returncode = UsererrorFuncptr(&lastError, 2);

		return -1;

	}


	Useroptionsptr = options;
	
	defaultoptions.reltol = 1e-4;

	defaultoptions.abstol = new double[problem_data.NEq];
	for (int i = 0; i < problem_data.NEq; i++)
	{
		defaultoptions.abstol[i] = 1e-4;
	}

	if (Useroptionsptr->timepoint != NaN )
	{
		solvetype = 1;
	}
	else if (Useroptionsptr->timepointvector != NULL)
	{
		solvetype = 2;
	}
	else
	{
		solvetype = 3;
	}
	
	// Option check for t0 only if time points are distributed over a range
	if (solvetype == 3)
	{
		if (Useroptionsptr->t0 == NaN)
		{
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("Initial time point not provided");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		}

		if (Useroptionsptr->tfinal == NaN)
		{
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("Final time point not provided");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		}

		if (Useroptionsptr->nout == -1)
		{
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("Number of time points not provided");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		}
		else
		{
			if (Useroptionsptr->nout <= 0)
			{
				IDAErrors->SetErr(1000, "Option setup error:");
				IDAErrors->AddErrorLine("Number of time points should be positive");
				IDAErrors->AddErrortovector();
				lastError = IDAErrors->GetLastError();
				returncode = UsererrorFuncptr(&lastError, 1);
				if (returncode == 0) return(0);
			}
			else
			{

			}
		}

	};
	// ***********************************************************************************
	// ************************TOLERANCE CHECK********************************************
	// ***********************************************************************************


	// Option check for relative tolerance
	if ((*options).reltol == -1)
	{
		// if the user doesn't define it, set default
		(*options).reltol = defaultoptions.reltol;
	}
	else
	{
		// check if the function tolerance is positive
		if ((*options).reltol > 0)
		{

		}
		else
		{
			// print error message
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("Relative tolerance is negative");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		};
	};

	// Option check for absolute tolerance
	if (options->tolerancetype == -1)
	{
		IDAErrors->SetErr(1000, "Option setup error:");
		IDAErrors->AddErrorLine("tolerance type not provided");
		IDAErrors->AddErrorLine("scalar tolerance:1, vector tolerance:2");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		returncode = UsererrorFuncptr(&lastError, 1);
		if (returncode == 0) return(0);

	}
	else if (options->tolerancetype == 1)
	{
		if (options->abstolscalar = -1)
		{
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("scalar tolerance not provided");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		}
		else if (options->abstolscalar < 0)
		{
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("Absolute tolerance is negative");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		}
		else
		{

		}
	}
	else if (options->tolerancetype == 2)
	{
		if (options->tolerancesize == -1)
		{
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("tolerance vector size not provided");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		}
		else if (options->tolerancesize != problem_data.NEq)
		{
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("tolerance vector size must be equal to number of equations");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);

		}
		else
		{
			if ((*options).abstol == NULL) // user has defined tolerance type and size before reaching here
			{

				for (int i = 0; i <= problem_data.NEq - 1; i++)
				{
					(*options).abstol[i] = defaultoptions.abstol[i]; // if null take default
				}

			}
			else
			{

				for (int i = 0; i <= problem_data.NEq - 1; i++) // if not null check if any value is negative
				{
					if ((*options).abstol[i] >= 0)
					{

					}
					else
					{
						// print error message
						IDAErrors->SetErr(1000, "Option setup error:");
						IDAErrors->AddErrorLine("Absolute tolerance is negative");
						IDAErrors->AddErrortovector();
						lastError = IDAErrors->GetLastError();
						returncode = UsererrorFuncptr(&lastError, 1);
						if (returncode == 0) return(0);

					}
				}
				returncode = SetIntegrationTol();
				if (returncode == 0) return(0);
			}

		}

	}
	else
	{

	}

	// ***********************************************************************************
	// ************************LINEAR SOLVER  ********************************************
	// ***********************************************************************************


	// Option check for linear solver
	if ((*options).LinearSolver == "UNDEFINED")
	{
		IDAErrors->SetErr(1030, "Option setup error:");
		IDAErrors->AddErrorLine("Linear solver not defined");
		IDAErrors->AddErrorLine("Linear solver must be one of the following:");
		IDAErrors->AddErrorLine("IDADENSE, IDABAND, IDAKLU");
		IDAErrors->AddErrorLine("Input is case sensitive");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		returncode = UsererrorFuncptr(&lastError, 1);
		if (returncode == 0) return(0);
	}
	else if ((*options).LinearSolver == "IDADENSE")
	{
		flag = IDADense(IDAmem, problem_data.NEq);
		returncode = CheckFlag(flag, "IDADense");
		if (returncode == 0) return(0);

		// If the solver is DENSE and user wants to use jacobian then it has to be dense jacobian
		if (Useroptionsptr->if_jacobian)
		{
			flag = IDADlsSetDenseJacFn(IDAmem, jac);
			returncode = CheckFlag(flag, "IDARootInit");
			if (returncode == 0) return 0;
		}

	}
	else if ((*options).LinearSolver == "IDABAND")
	{

		if ((*options).mupper == -1)
		{
			// error saying define upper half band width
			IDAErrors->SetErr(1040, "Option setup error:");
			IDAErrors->AddErrorLine("IDABAND linear solver");
			IDAErrors->AddErrorLine("Upper half band width not defined");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		};

		if ((*options).mlower == -1)
		{
			// error saying define lower half band width
			IDAErrors->SetErr(1050, "Option setup error:");
			IDAErrors->AddErrorLine("IDABAND linear solver");
			IDAErrors->AddErrorLine("Lower half band width not defined");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		};

		if ((*options).mlower != -1 && (*options).mupper != -1)
		{
			if ((*options).mlower > 0 && (*options).mupper > 0)
			{

				//	flag = IDABand(kmem, problem_data.NEq, options.mupper, options.mlower);
			}
			else
			{
				if ((*options).mlower > 0)
				{
					IDAErrors->SetErr(1060, "Option setup error:");
					IDAErrors->AddErrorLine("IDABAND linear solver");
					IDAErrors->AddErrorLine("upper half band width must be positive");
					IDAErrors->AddErrortovector();
					lastError = IDAErrors->GetLastError();
					returncode = UsererrorFuncptr(&lastError, 1);
					if (returncode == 0) return(0);
				}
				else
				{
					IDAErrors->SetErr(1070, "Option setup error:");
					IDAErrors->AddErrorLine("IDABAND linear solver");
					IDAErrors->AddErrorLine("lower half band width must be positive");
					IDAErrors->AddErrortovector();
					lastError = IDAErrors->GetLastError();
					returncode = UsererrorFuncptr(&lastError, 1);
					if (returncode == 0) return(0);
				};
				// warning telling user to check the half bandwidth
			};
		};
	}
	else if ((*options).LinearSolver == "IDAKLU")
	{
		if ((*options).NNZ == -1)
		{
			// error saying NNZ is must
			IDAErrors->SetErr(1080, "Option setup error:");
			IDAErrors->AddErrorLine("IDAKLU linear solver");
			IDAErrors->AddErrorLine("Problem dimension not defined");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		}
		else
		{
			if ((*options).NNZ > 0)
			{
				//			flag = IDAKLU(kmem, problem_data.NEq, options.NNZ);
			}
			else
			{
				// error saying check NNZ
				IDAErrors->SetErr(1090, "Option setup error:");
				IDAErrors->AddErrorLine("IDAKLU linear solver");
				IDAErrors->AddErrorLine("problem dimension must be positive");
				IDAErrors->AddErrortovector();
				lastError = IDAErrors->GetLastError();
				returncode = UsererrorFuncptr(&lastError, 1);
				if (returncode == 0) return(0);
			};
		};
	}
	else
	{
		IDAErrors->SetErr(1100, "Option setup error:");
		IDAErrors->AddErrorLine("Incorrect Linear solver name");
		IDAErrors->AddErrorLine("Linear solver must be one of the following:");
		IDAErrors->AddErrorLine("IDADENSE, IDABAND, IDAKLU");
		IDAErrors->AddErrorLine("Input is case sensitive");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		returncode = UsererrorFuncptr(&lastError, 1);
		if (returncode == 0) return(0);
	};

	// ***********************************************************************************
	// ************************MAX ORD ***************************************************
	// ***********************************************************************************


	// option check for maximum order
	if ((*options).maxord == -1)
	{
	}
	else
	{
		// check if the function tolerance is positive
		if ((*options).maxord > 0)
		{

			flag = IDASetMaxOrd(IDAmem, (*options).maxord);
			returncode = CheckFlag(flag, "IDASetMaxOrd");
			if (returncode == 0) return(0);

		}
		else
		{
			// print error message
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("Maximum order of linear multistep method is negative");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		};
	};

	// Option check for maximum number of steps
	if ((*options).maxnumsteps == -1)
	{
	}
	else
	{
		// check if the function tolerance is positive
		if ((*options).maxnumsteps > 0)
		{

			flag = IDASetMaxNumSteps(IDAmem, (*options).maxnumsteps);
			returncode = CheckFlag(flag, "IDASetMaxNumSteps");
			if (returncode == 0) return(0);
		}
		else
		{
			// print error message
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("Maximum number of steps to reach next output time is negative");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		};
	};

	// Option check for initial step size
	if ((*options).initstep == -1)
	{
	}
	else
	{
		// check if the initial step size is positive
		if ((*options).initstep > 0)
		{

			flag = IDASetInitStep(IDAmem, (*options).initstep);
			returncode = CheckFlag(flag, "IDASetInitStep");
			if (returncode == 0) return(0);
		}
		else
		{
			// print error message
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("Initial step size is negative");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		};
	};

	// Option check for maximum step size
	if ((*options).maxstep == -1)
	{
	}
	else
	{
		// check if the function tolerance is positive
		if ((*options).maxstep > 0)
		{

			flag = IDASetMaxStep(IDAmem, (*options).maxstep);
			returncode = CheckFlag(flag, "IDASetMaxStep");
			if (returncode == 0) return(0);
		}
		else
		{
			// print error message
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("Maximum absolute step size is negative");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		};
	};


	if ((*options).stoptime == NaN)
	{
	}
	else
	{
			flag = IDASetStopTime(IDAmem, (*options).stoptime);
			returncode = CheckFlag(flag, "IDASetStopTime");
			if (returncode == 0) return(0);
	};

	if ((*options).maxerrtestfails == -1)
	{
	}
	else
	{
		// check if the function tolerance is positive
		if ((*options).maxerrtestfails > 0)
		{

			flag = IDASetMaxErrTestFails(IDAmem, (*options).maxerrtestfails);
			returncode = CheckFlag(flag, "IDASetMaxErrTestFails");
			if (returncode == 0) return(0);
		}
		else
		{
			// print error message
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("Maximum number of error test failures is negative");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		};
	};


	if ((*options).nonlineariters == -1)
	{
	}
	else
	{
		// check if the function tolerance is positive
		if ((*options).nonlineariters > 0)
		{

			flag = IDASetMaxNonlinIters(IDAmem, (*options).nonlineariters);
			returncode = CheckFlag(flag, "IDASetMaxNonlinIters");
			if (returncode == 0) return(0);
		}
		else
		{
			// print error message
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("Maximum number of nonlinear iterations is negative");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		};
	};

	if ((*options).maxconvfails == -1)
	{
	}
	else
	{
		// check if the function tolerance is positive
		if ((*options).maxconvfails > 0)
		{

			flag = IDASetMaxConvFails(IDAmem, (*options).maxconvfails);
			returncode = CheckFlag(flag, "IDASetMaxConvFails");
			if (returncode == 0) return(0);
		}
		else
		{
			// print error message
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine(
				"Maximum number of nonlinear solver convergence failures at one step is negative"
			);
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		};
	};

	if ((*options).nonlinearconvcoeff == -1)
	{
	}
	else
	{
		// check if the function tolerance is positive
		if ((*options).nonlinearconvcoeff > 0)
		{

			flag = IDASetNonlinConvCoef(IDAmem, (*options).nonlinearconvcoeff);
			returncode = CheckFlag(flag, "IDASetNonlinConvCoef");
			if (returncode == 0) return(0);
		}
		else
		{
			// print error message
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine(
				"Safety factor in nonlinear convergence test is negative"
			);
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		};
	};

	if ((*options).suppressalg == false)
	{

	}
	else
	{
		if ((*options).suppressalg == true)
		{
			// SET ID IS MUST AFTER

		}
		else
		{
			// print error message
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("suppressalg can either be true or false");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return(0);
		}
	}


	if (Useroptionsptr->callrootfn != NULL) // User wants to call root function
	{
		if (Useroptionsptr->NrootFn == -1) // if user hasn't defined Nrootfn
		{
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("Define Number of root functions before calling root function");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return 0;
		}
		else
		{
			if (Useroptionsptr->NrootFn < 0) 
			{
				IDAErrors->SetErr(1000, "Option setup error:");
				IDAErrors->AddErrorLine("Number of root functions should be positive");
				IDAErrors->AddErrortovector();
				lastError = IDAErrors->GetLastError();
				returncode = UsererrorFuncptr(&lastError, 1);
				if (returncode == 0) return 0;
			}
			else
			{
				InfoFuncdata.rootsfound = new int[Useroptionsptr->NrootFn];
				flag = IDARootInit(IDAmem, Useroptionsptr->NrootFn, root);
				returncode = CheckFlag(flag, "IDARootInit");
				if (returncode == 0) return 0;

			}
		}
	}


	if (Useroptionsptr->solvetype == -1)
	{
		IDAErrors->SetErr(1000, "Option setup error:");
		IDAErrors->AddErrorLine("Define a solvetype");
		IDAErrors->AddErrorLine("1: for user supplied set of time points");
		IDAErrors->AddErrorLine("2: for user supplied single time point");
		IDAErrors->AddErrorLine("3: for uniformly distributed time points within a range");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		returncode = UsererrorFuncptr(&lastError, 1);
		if (returncode == 0) return 0;
	}
	else if ((Useroptionsptr->solvetype != 1)  
		&&(Useroptionsptr->solvetype != 2) &&(Useroptionsptr->solvetype != 3))
	{
		IDAErrors->SetErr(1000, "Option setup error:");
		IDAErrors->AddErrorLine("Define a solvetype");
		IDAErrors->AddErrorLine("1: for user supplied set of time points");
		IDAErrors->AddErrorLine("2: for user supplied single time point");
		IDAErrors->AddErrorLine("3: for uniformly distributed time points within a range");
		IDAErrors->AddErrortovector();
		lastError = IDAErrors->GetLastError();
		returncode = UsererrorFuncptr(&lastError, 1);
		if (returncode == 0) return 0;
	}
	else if (Useroptionsptr->solvetype == 1) // User gives a vector
	{
		if (Useroptionsptr->timevectorlength == -1)
		{
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("Define length of time point vector");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return 0;
		}
		else
		{
			if (Useroptionsptr->timevectorlength < 0)
			{
				IDAErrors->SetErr(1000, "Option setup error:");
				IDAErrors->AddErrorLine("Time point vector length has to be positive");
				IDAErrors->AddErrortovector();
				lastError = IDAErrors->GetLastError();
				returncode = UsererrorFuncptr(&lastError, 1);
				if (returncode == 0) return 0;
			}
		}
	}
	else
	{

	}

	if (Useroptionsptr->callSetID == true)// User wants to call setID
	{
		if (Useroptionsptr->setidptr == NULL) // User hasn't defined setidptr
		{
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("Define SetID vector before calling root function");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return 0;
		}
		else
		{
			if (Useroptionsptr->setidlength == -1) // User has defined setidptr but not set id length
			{
				IDAErrors->SetErr(1000, "Option setup error:");
				IDAErrors->AddErrorLine("Define SetID vector length");
				IDAErrors->AddErrortovector();
				lastError = IDAErrors->GetLastError();
				returncode = UsererrorFuncptr(&lastError, 1);
				if (returncode == 0) return 0;
			}
			else if (Useroptionsptr->setidlength != problem_data.NEq)// Vector length is not equal to number of equations
			{
				IDAErrors->SetErr(1000, "Option setup error:");
				IDAErrors->AddErrorLine("Define SetID vector length");
				IDAErrors->AddErrortovector();
				lastError = IDAErrors->GetLastError();
				returncode = UsererrorFuncptr(&lastError, 1);
				if (returncode == 0) return 0;

			}
			else //setidptr and length both are correct
			{
			}
		}
	}


	if (Useroptionsptr->callCalcIC == true) // User wants to call CalcIC
	{
		if (Useroptionsptr->callSetID == true) // User wants to call SetID 
		{
			if (Useroptionsptr->setidptr == NULL) // User hasn't defined setidptr
			{
				IDAErrors->SetErr(1000, "Option setup error:");
				IDAErrors->AddErrorLine("Define SetID vector before calling root function");
				IDAErrors->AddErrortovector();
				lastError = IDAErrors->GetLastError();
				returncode = UsererrorFuncptr(&lastError, 1);
				if (returncode == 0) return 0;
			}
			else // set id ptr is correct
			{
				if (Useroptionsptr->setidlength == -1) // User has defined setidptr but not set id length
				{
					IDAErrors->SetErr(1000, "Option setup error:");
					IDAErrors->AddErrorLine("Define SetID vector length");
					IDAErrors->AddErrortovector();
					lastError = IDAErrors->GetLastError();
					returncode = UsererrorFuncptr(&lastError, 1);
					if (returncode == 0) return 0;
				}
				else if (Useroptionsptr->setidlength != problem_data.NEq)// Vector length is not equal to number of equations
				{
					IDAErrors->SetErr(1000, "Option setup error:");
					IDAErrors->AddErrorLine("SetID vector length must be equal to number of equations");
					IDAErrors->AddErrortovector();
					lastError = IDAErrors->GetLastError();
					returncode = UsererrorFuncptr(&lastError, 1);
					if (returncode == 0) return 0;

				}
				else if (Useroptionsptr->setidlength <= 0)// Vector length is not equal to number of equations
				{
					IDAErrors->SetErr(1000, "Option setup error:");
					IDAErrors->AddErrorLine("SetID vector length must be positive");
					IDAErrors->AddErrortovector();
					lastError = IDAErrors->GetLastError();
					returncode = UsererrorFuncptr(&lastError, 1);
					if (returncode == 0) return 0;

				}
				else //setidptr and length both are correct
				{
					N_Vector nvecSetID;
					double* nvecsetIDptr;
					nvecSetID = NULL;
					nvecSetID = N_VNew_Serial(Useroptionsptr->setidlength);
					nvecsetIDptr = NV_DATA_S(nvecSetID);

					for (int i = 0; i < Useroptionsptr->setidlength; i++)
					{
						nvecsetIDptr[i] = Useroptionsptr->setidptr[i];
					}
					flag = IDASetId(IDAmem, nvecSetID);
					returncode = CheckFlag(flag, "IDASolve");
					if (returncode == 0) return 0;
					N_VDestroy_Serial(nvecSetID);

					//******************
					try
					{
						flag = IDACalcIC(IDAmem, IDA_YA_YDP_INIT, Useroptionsptr->t0);
					}
					catch (...)
					{
						IDAErrors->SetErr(-100, "IDA calcIC failed");
						IDAErrors->AddErrorLine("something went wrong");
						IDAErrors->AddErrortovector();
						lastError = IDAErrors->GetLastError();
						ESCAPE = true; // Exit everything else

						returncode = UsererrorFuncptr(&lastError, 2); // return code 2 to let user know that code escapes

						return -100;
					}

					returncode = CheckFlag(flag, "IDACalcIC");
					if (returncode == -1) return 0;
					retval = IDAGetConsistentIC(IDAmem, problem_data.yy, problem_data.yp);
					returncode = CheckFlag(retval, "IDAGetConsistentIC");
					if (returncode == 0) return 0;
					finalstats1.NEQ = problem_data.NEq;
					finalstats1.Init_cond_y = NV_DATA_S(problem_data.yy);
					finalstats1.Init_cond_yp = NV_DATA_S(problem_data.yp);

					returncode = UserInfoFuncptr(&finalstats1, 3);
				}
			}

		}
		else
		{
			IDAErrors->SetErr(1000, "Option setup error:");
			IDAErrors->AddErrorLine("Must call setid before calculating initial conditions");
			IDAErrors->AddErrortovector();
			lastError = IDAErrors->GetLastError();
			returncode = UsererrorFuncptr(&lastError, 1);
			if (returncode == 0) return 0;
		}

	};

	return 1;

}