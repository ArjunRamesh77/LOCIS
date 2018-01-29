#ifndef LOCIS_MANAGER_H
#define LOCIS_MANAGER_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include all Classes
#include "error.h"
#include "lexer.h"
#include "parser.h"
#include "AST.h"
#include "interpreter.h"
#include "equation.h"
#include "Symbol.h"
#include "PyFunctionServer.h"
#include "LOCIS_API_INCLUDE.h"
#include "stdafx.h"

#include <fstream>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// defines
#define LM_SUCCESS 0
#define LM_AST_NULL -1
#define LM_INTERPTER_FAIL -2 
#define LM_EQUATION_FAIL -3
#define LM_INCONSISTENT_VARIABLE_NUMBER -4
#define LM_KINSOL_INIT_FAIL -5

// Heading type
#define LM_LOCIS_HEADER_MSG 0
#define LM_LOCIS_HEADER_ERR 1

// Headings
#define LM_LOCIS_SIMPLE_HEADER 0
#define LM_LOCIS_HEADER 1
#define LM_LOCIS_SMALL_HEADER 2
#define LM_LOCIS_FOOTER 3
#define LM_LOCIS_INPUT_PROCESSING 4
#define LM_LOCIS_FAILED_INIT_LEXER 5
#define LM_LOCIS_FAILED_INIT_PARSER 6
#define LM_LOCIS_ERROR_IN_INPUT 7
#define LM_LOCIS_FAILED_INIT_PYTHON_ENV 8
#define LM_LOCIS_FAILED_INIT_INTERPRETER 9
#define LM_LOCIS_SEMANTIC_FAIL 10
#define LM_LOCIS_FAILED_INIT_EQUATION 11
#define LM_LOCIS_FAILED_INIT_EQUATION_DATA 12
#define LM_LOCIS_STEADY_STATE_START 13
#define LM_LOCIS_VARIABLES_EQUATION_MISMATCH 14
#define LM_LOCIS_GENERATE_EQUATIONS 15
#define LM_LOCIS_KINSOL_SETUP_FAIL 16
#define LM_LOCIS_KINSOL_SUCCESS 17
#define LM_LOCIS_DAE_INIT 18
#define LM_LOCIS_KINSOL_FAIL 19
#define LM_LOCIS_DAE_INIT_FAIL 20
#define LM_LOCIS_DAE_SOL 21
#define LM_LOCIS_DAE_SUCCESS 22
#define LM_LOCIS_DAE_SOL_FAIL 23

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Output Report Structure

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main Class for Handling a unit simulation transaction(For now)
class LOCISCoreManager
{
	cerrors* LOCISerror;			 // One container for all errors
	std::string LOCISSourceFile;    // Currently the source file is processed as one long file
	const char* cSource;
	const char* cTraceFileName;

	// Callbacks
	LOCISErrorCallBack cbError;
	LOCISInfoCallBack cbInfo;
	LOCISSteadyStateResults cbSSRes;
	LOCISSteadyDynamicResults cbDYRes;

	// LOCIS building blocks
	lexer* LOCISlexer;
	parser* LOCISparser;
	interpreter* LOCISinterpreter;
	equation* LOCISEquation;
	PyFunctionServer* LOCISFunctionServer;
	ASTNode* LOCISAST;

public:
	LOCISCoreManager()
	{
		// Initialize
		LOCISerror = NULL;
		cbError = NULL;
		cbInfo = NULL;
		cbSSRes = NULL;
		cbDYRes = NULL;
		LOCISerror = NULL;
		LOCISlexer = NULL;
		LOCISparser = NULL;;
		LOCISinterpreter = NULL;;
		LOCISEquation = NULL;;
		LOCISFunctionServer = NULL;
		LOCISAST = NULL;
	}

	~LOCISCoreManager()
	{
		if (LOCISerror)
			delete LOCISerror;

		// Dereference
		if (LOCISlexer)
			//delete LOCISlexer;

		if (LOCISparser)
			delete LOCISparser;

		if (LOCISinterpreter)
			delete LOCISinterpreter;

		if (LOCISEquation)
			delete LOCISEquation;

		if (LOCISFunctionServer)
			delete LOCISFunctionServer;

		if (LOCISAST)
			delete LOCISAST;

	}

	bool setSource(char* cSource);
	bool setTraceFile(const char* cFileName);
	bool setCallbacks(LOCISErrorCallBack cbError, LOCISInfoCallBack cbInfo);
	bool initManager(const char* LOCISSourceFile_arg, LOCISErrorCallBack cbErr, LOCISInfoCallBack cbInfo, LOCISSteadyStateResults cbSSRes, LOCISSteadyDynamicResults cbDYRes);
	int solve(int simulationMode, double tStart, double tEnd, int numtSteps, std::string solverName, double tolerance, double ftol);
	bool runFile(const char* fileName);
	
	bool solveSystem();
	bool getAllVarsNames(int *n, std::stringstream& ss);
	// Output functions
	void printLine(std::string line);

	//Messages
	void printLOCISErrorInfo(int type);
	void printLOCISSteadyStateNumbers(int numVars, int numEqu);
	void printLOCISDAEInitNumbers(int numVars, int numInitEqu, int numEquations);
	void printLOCISDAEOccuringVarNums(int algNum, int diffNum);
};

#endif