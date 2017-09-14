#include "LOCISCoreManager.h"
#include "NLSKinsol.h"
#include "DAESIda.h"
#include <sstream>
// Atharv
#include "error.h"
#pragma optimize( "", off )

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialized the LOCIS Core
bool LOCISCoreManager::initManager(const char* LOCISSourceFile_arg, LOCISErrorCallBack cbErr_arg, LOCISInfoCallBack cbInfo_arg, LOCISSteadyStateResults cbSSRes_arg, LOCISSteadyDynamicResults cbDYRes_arg)
{

	//Declare heap based error vector here
	LOCISerror = NULL;
	cbError = cbErr_arg;
	cbInfo = cbInfo_arg;
	cbSSRes = cbSSRes_arg;
	cbDYRes = cbDYRes_arg;

	LOCISerror = new  cerrors(cbErr_arg);
	assert(LOCISerror);

	//Print Header
	printLOCISErrorInfo(LM_LOCIS_SIMPLE_HEADER);

	// Check if stream valid
	assert(LOCISSourceFile_arg);

	//Initialize lexer
	LOCISlexer = NULL;
	LOCISlexer = new lexer(LOCISSourceFile_arg, LOCISerror);
	assert(LOCISlexer);

	//Initialize parser
	LOCISparser = NULL;
	LOCISparser = new parser(LOCISlexer, LOCISerror);
	assert(LOCISparser);

	//Get Full AST
	LOCISAST = NULL;
	LOCISAST = LOCISparser->startRule(0);
	if (!LOCISAST)
	{
		printLOCISErrorInfo(LM_LOCIS_ERROR_IN_INPUT);
		return false;
	}

	//Python Environment
	if (InitializePythonEnv() == 0)
	{
		printLOCISErrorInfo(LM_LOCIS_FAILED_INIT_PYTHON_ENV);
	}

	//Initialize the interpreter
	LOCISinterpreter = NULL;
	LOCISinterpreter = new interpreter(LOCISAST, LOCISerror);
	assert(LOCISinterpreter);

	// Start the interpreter
	try
	{
		LOCISinterpreter->Start();
	}
	catch (...)
	{
		//All exceptions will be handled here
		// this is terminal
		printLOCISErrorInfo(LM_LOCIS_SEMANTIC_FAIL);
		return false;
	}
	printLOCISErrorInfo(LM_LOCIS_INPUT_PROCESSING);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sets the trace file
bool LOCISCoreManager::setTraceFile(const char* cTraceFileName_arg)
{
	if (cTraceFileName_arg == NULL)
		return false;

	cTraceFileName = cTraceFileName_arg;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sets the trace file
bool LOCISCoreManager::setCallbacks(LOCISErrorCallBack cbError_arg, LOCISInfoCallBack cbInfo_arg)
{
	if (cbError_arg == NULL)
		return false;
	if (cbInfo_arg == NULL)
		return false;

	cbError = cbError_arg;
	cbInfo = cbInfo_arg;
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main Function to run the simplified LOCISFw model
int LOCISCoreManager::solve(int simulationMode, double tStart, double tEnd, int numtSteps, std::string solverName, double tolerance, double reltol)
{	

	// Setup Equation server
	LOCISEquation = NULL;
	LOCISEquation = new equation(LOCISinterpreter);
	assert(LOCISEquation);

	// Set Equation mode for interpreter
	LOCISEquation->setEquationModeForInterpreter(true);

	std::vector<EquationUnit>* Equations = NULL; 
	Equations = new std::vector<EquationUnit>;

	std::vector<EquationUnit>* InitEquations = NULL;
	InitEquations = new std::vector<EquationUnit>;

	std::vector<EquationUnit>* AllEquations = NULL;
	AllEquations = new std::vector<EquationUnit>;

	assert(Equations);
	assert(InitEquations);
	assert(AllEquations);

	LOCISEquation->setSimulationType(simulationMode);
	LOCISEquation->setEquations(Equations, InitEquations);

	///////////////////////////////////////////////////////////////////
	// Performs the dynamic simulation
	if (simulationMode == EQ_STEADY_STATE)
	{
		printLOCISErrorInfo(LM_LOCIS_STEADY_STATE_START);

		int NumEqInit = 0;
		int NumTotalVars = 0;
		stateVars* DAESimVars = NULL;

		// Get first equation set
		printLOCISErrorInfo(LM_LOCIS_GENERATE_EQUATIONS);
		assert(LOCISEquation->getNewEquationsFromState());

		// Get total number of Equations for Initialization
		NumEqInit = Equations->size();

		// Get State vectors
		DAESimVars = LOCISEquation->MapToStateVector();
		assert(DAESimVars);

		// Get total number of Variables
		NumTotalVars = DAESimVars->NumVars;

		// Check if system is consistent
		printLOCISSteadyStateNumbers(DAESimVars->NumVars, NumEqInit);
		if (NumEqInit != NumTotalVars)
		{
			printLOCISErrorInfo(LM_LOCIS_VARIABLES_EQUATION_MISMATCH);
			return LM_INCONSISTENT_VARIABLE_NUMBER;
		}

		// Create Python Functions
		PyFunctionServer PySSServer;
		assert(PySSServer.setAllVariableData(DAESimVars));
		assert(PySSServer.setEquationsList(Equations));
		assert(PySSServer.inputDataInit(false));

		std::stringstream* ss = NULL;
		ss = PySSServer.generateSSResidualFunction();
		assert(ss);

		PySSServer.registerSSResidualFunction(ss);
		delete ss;
		ss = NULL;

		ss = PySSServer.generateSSDenseJacobianFunction();
		assert(ss);

		PySSServer.registerSSDenseJacobianFunction(ss);
		delete ss;

		// KINSOL
		NLSKinsol NLSKinsolObj(cbInfo, cbError);
		int retflag = 0;

		retflag = NLSKinsolObj.setup(NumTotalVars, tolerance, &PySSServer, LOCISerror);
		if (retflag < 0)
		{
			printLOCISErrorInfo(LM_LOCIS_KINSOL_SETUP_FAIL);
			return LM_KINSOL_INIT_FAIL;
		}

		NLSKinsolObj.printHeader(tolerance);
		try
		{
			retflag = NLSKinsolObj.solve(DAESimVars->Vars);
		}
		catch (...)
		{

		}

		if (retflag == 0)
		{
			int n = 0;
			std::stringstream AllNames;
			getAllVarsNames(&n, AllNames);
			cbSSRes(DAESimVars->NumVars, AllNames.str().c_str(), DAESimVars->Vars);
			NLSKinsolObj.printFinalStats();
			printLOCISErrorInfo(LM_LOCIS_KINSOL_SUCCESS);
		}
		else
		{
			printLOCISErrorInfo(LM_LOCIS_KINSOL_FAIL);
		}

		// Back to Interpreter Mode
		LOCISEquation->setEquationModeForInterpreter(false);
	}
	
	///////////////////////////////////////////////////////////////////
	// Performs the dynamic simulation
	if (simulationMode == EQ_DYNAMIC)
	{
		int NumEqInit = 0;
		int NumTotalVars = 0;
		stateVars* DAESimVars = NULL;

		// Get State vectors
		DAESimVars = LOCISEquation->MapToStateVector();
		assert(DAESimVars);

		printLOCISErrorInfo(LM_LOCIS_DAE_INIT);

		// Get first full equation set(Equation + InitEquation)
		DAESimVars->createInitializationData();
		LOCISEquation->setInitializationMode(true, DAESimVars);
		printLOCISErrorInfo(LM_LOCIS_GENERATE_EQUATIONS);
		LOCISEquation->setT0Value(tStart);
		assert(LOCISEquation->getNewEquationsFromState());
		LOCISEquation->setInitializationMode(false, DAESimVars);

		// Get total number of Equations for Initialization
		NumEqInit = Equations->size() + InitEquations->size();

		// Get total number of Variables
		NumTotalVars = DAESimVars->initIncr + 1;

		printLOCISDAEInitNumbers(NumTotalVars, InitEquations->size(), Equations->size());

		// Check if Initialization system is consistent
		if (NumEqInit != NumTotalVars)
		{
			printLOCISErrorInfo(LM_LOCIS_VARIABLES_EQUATION_MISMATCH);
			return LM_INCONSISTENT_VARIABLE_NUMBER;
		}

		// Add the equations together
		for (auto it = InitEquations->begin(); it != InitEquations->end(); ++it)
			AllEquations->push_back(*it);

		for (auto it = Equations->begin(); it != Equations->end(); ++it)
			AllEquations->push_back(*it);

		// Create Initialization functions
		PyFunctionServer PySSServer;
		PySSServer.setAllVariableData(DAESimVars);
		PySSServer.setEquationsList(AllEquations);
		PySSServer.inputDataInit(true);

		std::stringstream* ss;
		ss = PySSServer.generateSSResidualFunction();
		PySSServer.registerSSResidualFunction(ss);
		delete ss;

		ss = PySSServer.generateSSDenseJacobianFunction();
		PySSServer.registerSSDenseJacobianFunction(ss);
		delete ss;

		// KINSOL (Solve the Initialization problem)
		int retflag = 0;
		double* initGuess = new double[8];
		initGuess = DAESimVars->getInitializationGuess();
		assert(initGuess);

		NLSKinsol NLSKinsolObj(cbInfo, cbError);
		NLSKinsolObj.setup(NumTotalVars, tolerance, &PySSServer, LOCISerror);
		if (retflag < 0)
		{
			printLOCISErrorInfo(LM_LOCIS_KINSOL_SETUP_FAIL);
			return LM_KINSOL_INIT_FAIL;
		}
		NLSKinsolObj.printHeader(tolerance);

		if (NLSKinsolObj.solve(initGuess) == 0)
		{
			NLSKinsolObj.printFinalStats();
			printLOCISErrorInfo(LM_LOCIS_DAE_SOL);

			int numAlg = 0, numDiff = 0;
			LOCISEquation->getNewEquationsFromState();
			assert(DAESimVars->mapFromInitializationGuess(initGuess));
			delete initGuess;
			LOCISEquation->getOccuringVariablesNumbers(numAlg, numDiff);
			printLOCISDAEOccuringVarNums(numAlg, numDiff);

			// Create DAE functions
			PyFunctionServer* PyDAEServer = new PyFunctionServer;
			assert(PyDAEServer->setAllVariableData(DAESimVars));
			assert(PyDAEServer->setEquationsList(Equations));
			if (!PyDAEServer->inputDataInit(false))
			{
				printLOCISErrorInfo(LM_LOCIS_VARIABLES_EQUATION_MISMATCH);
				return LM_INCONSISTENT_VARIABLE_NUMBER;
			}

			std::stringstream* ss;
			ss = PyDAEServer->generateDAEResidualFunction();
			PyDAEServer->registerDAEResidualFunction(ss);
			delete ss;

			ss = PyDAEServer->generateDAEDenseJacobianFunction();
			PyDAEServer->registerDAEDenseJacobianFunction(ss);
			delete ss;

			// Create reltol vector
			double* ftol = new double[DAESimVars->NumVars];
			for (int i = 0; i < DAESimVars->NumVars; i++)
				ftol[i] = reltol;

			// IDA (Solve the DAE)
			DAESIda DAESIdaObj(cbInfo, cbError);
			DAESIdaObj.setup(DAESimVars->NumVars, tolerance, ftol, PyDAEServer,
				DAESimVars->Vars, DAESimVars->dVars, tStart, LOCISerror);

			double tstep = (tEnd - tStart) / numtSteps;
			double t = tStart + tstep;
			double* sol = new double[DAESimVars->NumVars];
			int temp;

			// Get All variable Names
			int n = 0;
			std::stringstream AllNames;
			getAllVarsNames(&n, AllNames);

			// 1st Call (for Variable Name Priniting Only)
			double firstTime = t - tstep;
			cbDYRes(1, -1, AllNames.str().c_str(), &firstTime, DAESimVars->Vars);

			//2nd Call (for Initialization Results)
			cbDYRes(1, DAESimVars->NumVars, AllNames.str().c_str(), &firstTime, DAESimVars->Vars);

			DAESIdaObj.printHeader();
			for (int i = 1; i <= numtSteps; i++)
			{
				if (DAESIdaObj.solve(t, DAESimVars) == 0)
				{
					// Display
					DAESIdaObj.printFinalStats(t);
					cbDYRes(i, DAESimVars->NumVars, AllNames.str().c_str(), &t, DAESimVars->Vars);
				}
				else
				{
					printLOCISErrorInfo(LM_LOCIS_DAE_SOL_FAIL);
					break;
				}
				t = t + tstep;
			}
			//DAESIdaObj.printFooter();
		}
		else
		{
			printLOCISErrorInfo(LM_LOCIS_DAE_INIT_FAIL);
			return LM_LOCIS_KINSOL_FAIL;
		}
		printLOCISErrorInfo(LM_LOCIS_DAE_SUCCESS);

		// Back to Interpreter Mode
		LOCISEquation->setEquationModeForInterpreter(false);
	}

	return LM_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Solve
bool LOCISCoreManager::solveSystem()
{
	solve(LOCISinterpreter->simuArg_iType,
		LOCISinterpreter->simuArg_startt,
		LOCISinterpreter->simuArg_endt,
		LOCISinterpreter->simuArg_numStepst,
		"",
		LOCISinterpreter->simuArg_asbtol,
		LOCISinterpreter->simuArg_reltol);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gets all the Variables for the System
bool LOCISCoreManager::getAllVarsNames(int *n, std::stringstream& Vars)
{
	std::vector<VariableData>* AllVarNames = NULL;
	AllVarNames = LOCISinterpreter->getAllVarNames();
	if (AllVarNames == NULL)
		return false;

	for(auto it = AllVarNames->begin(); it != AllVarNames->end(); ++it)
	{
		if(it != (AllVarNames->end()-1))
			Vars << it->fullname + "\n";
		else
			Vars << it->fullname + "\0";
	}

	*n = AllVarNames->size();
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prints a single line of output
void LOCISCoreManager::printLine(std::string line)
{
	std::cout << line << "\n";
}



#pragma optimize( "", on )

