#include "LOCISCoreManager.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prints the LOCIS header/footer
void LOCISCoreManager::printLOCISErrorInfo(int type)
{
	std::stringstream LOCISMsg;
	switch (type)
	{
	case LM_LOCIS_SIMPLE_HEADER:
		LOCISMsg << "-> Initializing LOCISEngine";
		cbInfo(LM_LOCIS_HEADER_MSG, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_INPUT_PROCESSING:
		LOCISMsg << "-> Completed processing input";
		cbInfo(LM_LOCIS_HEADER_MSG, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_FAILED_INIT_LEXER:
		LOCISMsg << "    ERROR: Failed to Initialize Lexer";
		cbError(LM_LOCIS_HEADER_ERR, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_FAILED_INIT_PARSER:
		LOCISMsg << "    ERROR: Failed to Initialize Parser";
		cbError(LM_LOCIS_HEADER_ERR, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_ERROR_IN_INPUT:
		LOCISMsg << "    ERROR: Input failed to validate correctly possibly due to a syntax error.\n Failed in Parser.";
		cbError(LM_LOCIS_HEADER_ERR, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_FAILED_INIT_INTERPRETER:
		LOCISMsg << "    ERROR: Failed to Initialize Interpreter";
		cbError(LM_LOCIS_HEADER_ERR, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_FAILED_INIT_PYTHON_ENV:
		LOCISMsg << "    ERROR: Failed to initialize Python Environment";
		cbError(LM_LOCIS_HEADER_ERR, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_SEMANTIC_FAIL:
		LOCISMsg << "    ERROR: Input failed to validate correctly possibly due to a semantic error.\n Failed in Interpreter.";
		cbError(LM_LOCIS_HEADER_ERR, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_FAILED_INIT_EQUATION:
		LOCISMsg << "    ERROR: Failed to Initialize Equation Processor";
		cbError(LM_LOCIS_HEADER_ERR, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_FAILED_INIT_EQUATION_DATA:
		LOCISMsg << "    ERROR: Failed to Initialize Equation Data";
		cbError(LM_LOCIS_HEADER_ERR, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_STEADY_STATE_START:
		LOCISMsg << "-> Starting Steady State solution process";
		cbInfo(LM_LOCIS_HEADER_MSG, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_VARIABLES_EQUATION_MISMATCH:
		LOCISMsg << "    ERROR: Number of Variables not equal to number of Equations";
		cbError(LM_LOCIS_HEADER_ERR, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_GENERATE_EQUATIONS:
		LOCISMsg << "-> Generating All Equations";
		cbInfo(LM_LOCIS_HEADER_MSG, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_KINSOL_SETUP_FAIL:
		LOCISMsg << "    ERROR: Failed in setting up KINSOL non-linear solver";
		cbError(LM_LOCIS_HEADER_ERR, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_KINSOL_SUCCESS:
		LOCISMsg << "-> Completed non-linear solution procesdure";
		cbInfo(LM_LOCIS_HEADER_MSG, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_KINSOL_FAIL:
		LOCISMsg << "	ERROR: KINSOL non-linear solver failed to converge";
		cbError(LM_LOCIS_HEADER_MSG, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_DAE_INIT:
		LOCISMsg << "-> Starting DAE initialization";
		cbInfo(LM_LOCIS_HEADER_MSG, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_DAE_INIT_FAIL:
		LOCISMsg << "	ERROR: KINSOL failed to obtain DAE initial solution";
		cbError(LM_LOCIS_HEADER_MSG, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_DAE_SOL:
		LOCISMsg << "-> Starting DAE solution";
		cbInfo(LM_LOCIS_HEADER_MSG, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_DAE_SUCCESS:
		LOCISMsg << "-> Completed non-linear DAE solution process";
		cbInfo(LM_LOCIS_HEADER_MSG, LOCISMsg.str().c_str());
		break;

	case LM_LOCIS_DAE_SOL_FAIL:
		LOCISMsg << "	ERROR: IDA failed to obtain DAE solution";
		cbError(LM_LOCIS_HEADER_MSG, LOCISMsg.str().c_str());
		break;

	default:
		LOCISMsg << "<INTERNAL ERROR: Undefined Header>";
		cbInfo(LM_LOCIS_HEADER_ERR, LOCISMsg.str().c_str());
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Display Number of equations and variables (STEADY STATE)
void LOCISCoreManager::printLOCISSteadyStateNumbers(int numVars, int numEqu)
{
	std::string sMsg("    -> Number of Equations = " + std::to_string(numEqu) + "\n    -> Number of Variables = " + std::to_string(numVars));
	const char* LOCISMsg = sMsg.c_str();
	cbInfo(LM_LOCIS_SIMPLE_HEADER, LOCISMsg);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Display Number of equations and variables (DAE INITIALIZATION)
void LOCISCoreManager::printLOCISDAEInitNumbers(int numVars, int numInitEqu, int numEquations)
{
	std::string sMsg("    -> Number of Init Equations = " + std::to_string(numInitEqu) + "\n    -> Number of Equations = " + std::to_string(numEquations) + "\n    -> Number of Variables = " + std::to_string(numVars));
	const char* LOCISMsg = sMsg.c_str();
	cbInfo(LM_LOCIS_SIMPLE_HEADER, LOCISMsg);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Occurring Variable Numbers
void LOCISCoreManager::printLOCISDAEOccuringVarNums(int algNum, int diffNum)
{
	std::string sMsg("    -> Number of algebraic variables = " + std::to_string(algNum) + "\n    -> Number of differntial variables = " + std::to_string(diffNum));
	const char* LOCISMsg = sMsg.c_str();
	cbInfo(LM_LOCIS_SIMPLE_HEADER, LOCISMsg);
}