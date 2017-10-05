#include "DAESIda.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IDA Residual function
int residualFunctionIda(realtype tres, N_Vector yy, N_Vector yp, N_Vector resval, void *user_data)
{
	DAESIdaUserData* localUserData = static_cast<DAESIdaUserData*>(user_data);
	double *dyy;
	double *dyp;
	double *dr;

	dyy = NV_DATA_S(yy);
	dyp = NV_DATA_S(yp);
	dr = NV_DATA_S(resval);

	localUserData->pyFunctionsObject->fDAEResidualFunction(tres, dyy, dyp, dr);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IDA Dense Jacobian function
int denseJacobianFunctionIda(long Neq, realtype tt, realtype cj, N_Vector yy, N_Vector yp, N_Vector resvec,
	DlsMat JJ, void *user_data, N_Vector tempv1, N_Vector tempv2, N_Vector tempv3)
{
	DAESIdaUserData* localUserData = static_cast<DAESIdaUserData*>(user_data);
	double *dyy;
	double *dyp;

	dyy = NV_DATA_S(yy);
	dyp = NV_DATA_S(yp);

	localUserData->pyFunctionsObject->fDAEDenseJacobianFunction(tt, dyy, dyp, JJ->data, cj);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup the problem
int DAESIda::setup(int iNumberEquations, double dRelTolerance, 
	double* dAbsTolerance, PyFunctionServer* pyFunctionsObject,
	double* guessy, double* guessyp, double tbeg, cerrors* DAESErrorVector)
{
	int retval;
	if (iNumberEquations > 0)
	{
		iNumberEquations = iNumberEquations;
		yy = N_VNew_Serial(iNumberEquations);
		yp = N_VNew_Serial(iNumberEquations);
		avtol = N_VNew_Serial(iNumberEquations);
	}
	else
	{
		return DAES_IDA_FAIL;
	}

	// Set Initial guess
	double* yyg = NULL; 
	double* ypg = NULL; 
	yyg = NV_DATA_S(yy);
	ypg = NV_DATA_S(yp);

	if (!yyg || !ypg)
	{
		return DAES_IDA_FAIL;
	}

	// Set error handler function
	errorFuncData.outPutErrorHandler = cbError;
	errorFuncData.errVector = DAESErrorVector;


	for (int i = 0; i < iNumberEquations; i++)
	{
		yyg[i] = guessy[i];
		ypg[i] = guessyp[i];
	}

	//Set Relative Scalar Tolerance
	if (dRelTolerance > 0.0)
	{
		dRelTolerance = dRelTolerance;
	}
	else
	{
		return DAES_IDA_FAIL;
	}

	// Absolute tolerance
	if (dAbsTolerance == NULL)
	{
		return DAES_IDA_FAIL;
	}

	//Set Absolute Vector Tolerance
	double* absTol = NULL;
	absTol = NV_DATA_S(avtol);
	if (!absTol)
	{
		return DAES_IDA_FAIL;
	}
	for (int i = 0; i < iNumberEquations; i++)
	{
		if (dAbsTolerance[i] > 0.0)
			absTol[i] = dAbsTolerance[i];
		else
			return DAES_IDA_FAIL;
	}

	// Main Setup functions
	imem = IDACreate();
	if (imem == NULL)
	{
		return DAES_IDA_FAIL;
	}

	retval = IDASetErrHandlerFn(imem, DAESIdaERRORFUN, &errorFuncData);
	if (retval != IDA_SUCCESS) return DAES_IDA_FAIL;

	userData.pyFunctionsObject = pyFunctionsObject;

	retval = IDASetUserData(imem, &userData);
	if (retval != IDA_SUCCESS) return DAES_IDA_FAIL;

	retval = IDAInit(imem, residualFunctionIda, tbeg, yy, yp);
	if (retval != IDA_SUCCESS) return DAES_IDA_FAIL;


	// Options
	retval = IDASVtolerances(imem, dRelTolerance, avtol);
	if (retval != IDA_SUCCESS) return DAES_IDA_FAIL;

	retval = IDADense(imem, iNumberEquations);
	if (retval != IDA_SUCCESS) return DAES_IDA_FAIL;

	retval = IDADlsSetDenseJacFn(imem, denseJacobianFunctionIda);
	if (retval != IDA_SUCCESS) return DAES_IDA_FAIL;

	
	return DAES_IDA_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Solve at a time step
int DAESIda::solve(double t, stateVars* DAESimVars)
{
	int retval;
	if (imem && yy && yp)
	{
		retval = IDASolve(imem, t, &tret, yy, yp, IDA_NORMAL);
		// Copy solution
		double* yval = NV_DATA_S(yy);
		double* ypval = NV_DATA_S(yp);
		for (int i = 0; i < DAESimVars->NumVars; i++)
		{
			DAESimVars->Vars[i] = yval[i];
			DAESimVars->dVars[i] = ypval[i];
		}
		return retval;
	}

	return DAES_IDA_FAIL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check IDA functions return flag
int DAESIda::checkFlag(int flagvalue, std::string funcname)
{
	if (flagvalue >= 0)
	{
		return IDA_SUCCESS;
	}
	else
	{
		return -1;
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRINT HEADER

void DAESIda::printHeader()
{


	std::stringstream cLines;
	std::string tmp;
	const char* cstr;
	cLines << "\n";
	cLines << "Tolerance parameters:\n";
	cLines << "Relative Tolerance  = " + std::to_string(dRelTolerance);

//	cLines << "Absolute Tolerance  = " + std::to_string(dAbsTolerance);


	cLines << "\n";
	cLines << std::string(15 * 5 + 4, '-') << "\n";
	cLines << center("t", 13) << " | "
		<< center("tret", 13) << " | "
		<< center("NumSteps", 13) << " | "
		<< center("Last Order", 13) << " | "
		<< center("Last Step", 13) << "\n";
	cLines << std::string(15 * 5 + 4, '-') << "\n";
	tmp = cLines.str();
	cstr = tmp.c_str();
	cbInfo(0, cstr);

	return;
}

std::string DAESIda::center(const std::string s, const int w)
{
	std::stringstream ss, spaces;
	int padding = w - s.size();                 // count excess room to pad
	for (int i = 0; i<padding / 2; ++i)
		spaces << " ";
	ss << spaces.str() << s << spaces.str();    // format with padding
	if (padding>0 && padding % 2 != 0)               // if odd #, add 1 space
		ss << " ";
	return ss.str();
}

void  DAESIda::printFinalStats(double t)
{
    int retval, kused, flag;
	long int nst;
	realtype hused;
	std::string print_string;
	print_string = "";

	flag = IDAGetLastOrder(imem, &kused);
	checkFlag(flag, "KINDlsGetNumFuncEvals");
	flag = IDAGetNumSteps(imem, &nst);
	checkFlag(flag, "KINDlsGetNumFuncEvals");
	flag = IDAGetLastStep(imem, &hused);
	checkFlag(flag, "KINDlsGetNumFuncEvals");

	print_string += IDAprd1(std::to_string(t),13);
	print_string += "   " + IDAprd1(std::to_string(tret), 13) + " ";
	print_string += "   " + IDAprd1(std::to_string(kused), 13) + " ";
	print_string += "   " + IDAprd1(std::to_string(nst), 13) + " ";
	print_string += IDAprd1(std::to_string(hused), 13) + " ";
	print_string += "\n";

	std::stringstream cLines;
	std::string tmp;
	const char* cstr;

	cLines << print_string;
	tmp = cLines.str();
	cstr = tmp.c_str();
	cbInfo(0, cstr);
}


void DAESIda::printFooter()
{
	std::stringstream cLines;
	std::string tmp;
	const char* cstr;

	cLines << std::string(15 * 5 + 4, '-') << "\n";
	tmp = cLines.str();
	cstr = tmp.c_str();
	cbInfo(0, cstr);

	return;

}
