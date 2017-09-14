#include "NLSKinsol.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Residual Function
int residualFunctionKinsol(N_Vector y, N_Vector f, void *user_data)
{
	NLSKinsolUserData* localUserData = static_cast<NLSKinsolUserData*>(user_data);
	double *dr;
	double *dy;

	dy = NV_DATA_S(y);
	dr = NV_DATA_S(f);

	localUserData->pyFunctionsObject->fSSResidualFunction(dy, dr);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dense Jacobian Function
int denseJacobianFunctionKinsol(long int N, N_Vector y, N_Vector f, DlsMat J, void *user_data, N_Vector tmp1, N_Vector tmp2)
{
	NLSKinsolUserData* localUserData = static_cast<NLSKinsolUserData*>(user_data);
	double *dy;

	dy = NV_DATA_S(y);
	localUserData->pyFunctionsObject->fSSDenseJacobianFunction(dy, J->data);

	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup the problem
int NLSKinsol::setup(int iumberEquations_arg, double dfnormTolerance_arg, 
	PyFunctionServer* pyFunctionsObject, cerrors* errorVector)
{
	int retflag;

	if (errVector == NULL)
	{

	}
	else
	{
		errVector = errorVector;
	}
	stringOut = "";
	infoFuncData.InfoFormatFn = (outputInfo)NLSKinsolOutputInfoHandler;
	infoFuncData.outPutInfoHandler = cbInfo;
	infoFuncData.stringOut = &stringOut;

	errorFuncData.outPutErrorHandler = cbError;
	errorFuncData.errVector = errorVector;
	

	// Set number of equations and variables
	if (iumberEquations_arg > 0)
	{
		inumberEquations = iumberEquations_arg;
		y = N_VNew_Serial(inumberEquations);

		scale = N_VNew_Serial(inumberEquations);
		N_VConst_Serial(1, scale);
	}
	else
	{
		return NLS_KINSOL_FAIL;
	}


	// Set Tolerance
	if (dfnormTolerance > 0.0)
		dfnormTolerance = dfnormTolerance_arg;
	else
		return NLS_KINSOL_FAIL;

	// Basic Setup
	kmem = KINCreate();
	if (!kmem)
		return NLS_KINSOL_FAIL;

	// Pass Function pointers to User data
	userData.NEQ = inumberEquations;
	userData.pyFunctionsObject = pyFunctionsObject;

	retflag = KINSetErrHandlerFn(kmem, NLSKinsolERRORFUN, &errorFuncData);
	if (retflag != KIN_SUCCESS) return NLS_KINSOL_FAIL;

	retflag = KINSetUserData(kmem, &userData);
	if (retflag != KIN_SUCCESS) return NLS_KINSOL_FAIL;

	retflag = KINInit(kmem, residualFunctionKinsol, y);
	if (retflag != KIN_SUCCESS) return NLS_KINSOL_FAIL;

	retflag = KINSetInfoHandlerFn(kmem, NLSKinsolIHFUN, &infoFuncData);
	if (retflag != KIN_SUCCESS) return NLS_KINSOL_FAIL;

	retflag = KINSetPrintLevel(kmem, 1);
	if (retflag != KIN_SUCCESS) return NLS_KINSOL_FAIL;

	// OPTIONS
	retflag = KINSetNoInitSetup(kmem, TRUE);
	if (retflag != KIN_SUCCESS) return NLS_KINSOL_FAIL;

	retflag = KINSetFuncNormTol(kmem, dfnormTolerance);
	if (retflag != KIN_SUCCESS) return NLS_KINSOL_FAIL;

	retflag = KINDense(kmem, inumberEquations);
	if (retflag != KIN_SUCCESS) return NLS_KINSOL_FAIL;

	retflag = KINDlsSetDenseJacFn(kmem, denseJacobianFunctionKinsol);
	if (retflag != KIN_SUCCESS) return NLS_KINSOL_FAIL;

	retflag = KINSetMaxSetupCalls(kmem, 1);
	if (retflag != KIN_SUCCESS) return NLS_KINSOL_FAIL;

	return NLS_KINSOL_PASS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Solve the problem
int NLSKinsol::solve(double* guess)
{
	int ret;
	if (kmem && y && scale && guess)
	{
		double* temp = NV_DATA_S(y);
		std::stringstream cLines;
		std::string tmp;
		const char* cstr;

		for (int i = 0; i < inumberEquations; i++)
			temp[i] = guess[i];

		ret = KINSol(kmem, y, KIN_NONE, scale, scale);
		cLines << (stringOut) << "\n";
		//cLines << "\n";
		tmp = cLines.str();
		cstr = tmp.c_str();
		cbInfo(0, cstr);
		cLines.str("");
		cLines.clear();
		stringOut = "";


		for (int i = 0; i < inumberEquations; i++)
			guess[i] = temp[i];

		return ret;
	}

	return NLS_KINSOL_FAIL;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check KINSOL functions return flag
int NLSKinsol::checkFlag(int flagvalue, std::string funcname)
{
	if (flagvalue >= 0)
	{
		return KIN_SUCCESS;
	}
	else
	{
		return -1;
	}

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SETOPTIONS
// scaled step tol

int NLSKinsol::SetScaledStepTol(double stol)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRINT HEADER

void NLSKinsol::printHeader(double fnormtol)
{
	std::stringstream cLines;
	std::string tmp;
	const char* cstr;


	cLines << std::string(15 * 4, '-') + "\n";
	cLines << center("Iter", 13) + " | " + center("Evaluation", 13) + " | " + center("fnorm", 13) << " | "  + center("pnorm", 13) << "\n";

	cLines << std::string(15 * 4, '-') + "\n";
	tmp = cLines.str();
	cstr = tmp.c_str();
	cbInfo(0, cstr);
	cLines.str("");
	cLines.clear();
	return;
}

std::string NLSKinsol::center(const std::string s, const int w) 
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

void  NLSKinsol::printFinalStats()
{

	std::stringstream cLines;
	std::string tmp;
	const char* cstr;

	long int nni, nfe, nje, nfeD;
	int flag;

	flag = KINGetNumNonlinSolvIters(kmem, &nni);
	checkFlag(flag, "KINGetNumNonlinSolvIters");

	flag = KINGetNumFuncEvals(kmem, &nfe);
	checkFlag(flag, "KINGetNumFuncEvals");

	flag = KINDlsGetNumJacEvals(kmem, &nje);
	checkFlag(flag, "KINDlsGetNumJacEvals");

	flag = KINDlsGetNumFuncEvals(kmem, &nfeD);
	checkFlag(flag, "KINDlsGetNumFuncEvals");


	cLines <<  "\n";
	cLines <<  std::string(15 * 4, '-') << "\n";

	cLines << "Final Statistics:\n";
	cLines << "Number of nonlinear iterations = " + std::to_string(nni) + "\n";
	cLines <<"Number of function evaluations = " + std::to_string(nfe) + "\n";
	cLines << "Number of jacobian evaluations = " + std::to_string(nje) + "\n";
	tmp = cLines.str();
	cstr = tmp.c_str();
	cbInfo(0, cstr);
	cLines.str("");
	cLines.clear();


}
