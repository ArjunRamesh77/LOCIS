#pragma once

#include "Python.h"
#include "equation.h"
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include "arrayobject.h"
#include <iostream>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Python Global Functions
void initNumpy();
int InitializePythonEnv();
int AddEnvironmentToCompiledModules(PyObject* CompiledModule);
int FinalizePythonEnv();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Python defines
#define PY_PATHS "..\\..\\..\\Python27;..\\..\\..\\Python27\\DLLs;..\\..\\..\\Python27\\Lib;..\\..\\..\\Python27\\Lib\\site-packages"
#define PY_MAIN_FAIL -3
#define PY_NUMPY_FAIL -2
#define PY_AUTOGRAD_FAIL -1
#define PY_FAIL 0
#define PY_SUCCESS 1

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Python data
struct PYTHON_INFO_GLOBAL
{
	// All modules
	PyObject *Python_module_main;
	PyObject *Python_module_autograd;
	PyObject *Python_module_numpy;

	// Module dictionaries
	PyObject *Python_module_main_dict;
	PyObject *Python_module_autograd_dict;
	PyObject *Python_module_numpy_dict;

	PYTHON_INFO_GLOBAL()
	{
		Python_module_main = NULL;
		Python_module_autograd = NULL;
		Python_module_numpy = NULL;
		Python_module_main_dict = NULL;
		Python_module_autograd_dict = NULL;
		Python_module_numpy_dict = NULL;
	}

	~PYTHON_INFO_GLOBAL()
	{
		Py_DecRef(Python_module_main);
		Py_DecRef(Python_module_autograd);
		Py_DecRef(Python_module_numpy);
		Py_DecRef(Python_module_main_dict);
		Py_DecRef(Python_module_autograd_dict);
		Py_DecRef(Python_module_numpy_dict);
	}

};
extern PYTHON_INFO_GLOBAL PYTHON_INFO_GLOBAL_OBJ;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Equation codes
#define EQ_SS_RESIDUAL	0
#define EQ_SS_JACOBIAN	1
#define EQ_DAE_RESIDUAL 2
#define EQ_DAE_JACOBIAN 3

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main class that encapsulates Python Functionality
class PyFunctionServer
{
	int NumEquations;
	int NumVariables;
	int NumAlgebraic;
	int NumDifferential;
	std::vector<EquationUnit>* AllEquationsData;
	stateVars* AllVarsData;

	// DAEResidual Function data
	PyObject* pyDAEResidualFunctionPosArgs;
	npy_intp  pyDAEResidualFunctiondims[1];
	PyObject* pyDAEResidualFunction_t;
	PyObject* pyDAEResidualFunction_array_V;
	PyObject* pyDAEResidualFunction_array_DV;
	PyObject* pyDAEResidualFunction_array_r;
	PyObject* pyDAEResidualFunction;
	PyObject* pyDAEResidualFunctionModule;
	PyObject* pyDAEResidualCompiledFn;

	// SSResidual Function data
	PyObject* pySSResidualFunctionPosArgs;
	npy_intp  pySSResidualFunctiondims[1];
	PyObject* pySSResidualFunction_array_V;
	PyObject* pySSResidualFunction_array_r;
	PyObject* pySSResidualFunction;
	PyObject* pySSResidualFunctionModule;
	PyObject* pySSResidualCompiledFn;

	// DAEDenseJacobian Function data
	PyObject* pyDAEDenseJacobianFunctionPosArgs;
	npy_intp  pyDAEDenseJacobianFunctiondims[2];
	PyObject* pyDAEDenseJacobianFunction_t;
	PyObject* pyDAEDenseJacobianFunction_array_V;
	PyObject* pyDAEDenseJacobianFunction_array_DV;
	PyObject* pyDAEDenseJacobianFunction_array_J;
	PyObject* pyDAEDenseJacobianFunction_alpha; 
	PyObject* pyDAEDenseJacobianCompiledFn;
	PyObject* pyDAEDenseJacobianFunctionModule;
	PyObject* pyDAEDenseJacobianFunction;

	// SSDenseJacobian Function data
	PyObject* pySSDenseJacobianFunctionPosArgs;
	npy_intp  pySSDenseJacobianFunctiondims[2];
	PyObject* pySSDenseJacobianFunction_array_V;
	PyObject* pySSDenseJacobianFunction_array_J;
	PyObject* pySSDenseJacobianCompiledFn;
	PyObject* pySSDenseJacobianFunctionModule;
	PyObject* pySSDenseJacobianFunction;

public:
	PyFunctionServer()
	{
		NumEquations = 0;
		NumAlgebraic = 0;
		NumDifferential = 0;
		AllEquationsData = NULL;
		AllVarsData = NULL;

		// DAEResidual Function data
		pyDAEResidualFunctionPosArgs = NULL;
		pyDAEResidualFunction_t = NULL;
		pyDAEResidualFunction_array_V = NULL;
		pyDAEResidualFunction_array_DV = NULL;
		pyDAEResidualFunction_array_r = NULL;
		pyDAEResidualFunction		= NULL;
		pyDAEResidualFunctionModule	= NULL;
		pyDAEResidualCompiledFn		= NULL;

		// SSResidual Function data
		pySSResidualFunctionPosArgs = NULL;
		pySSResidualFunction_array_V = NULL;
		pySSResidualFunction_array_r = NULL;
		pySSResidualFunction = NULL;
		pySSResidualFunctionModule = NULL;
		pySSResidualCompiledFn = NULL;

		// DAEDenseJacobian Function data
		pyDAEDenseJacobianFunctionPosArgs = NULL;
		pyDAEDenseJacobianFunction_t = NULL;
		pyDAEDenseJacobianFunction_array_V = NULL;
		pyDAEDenseJacobianFunction_array_DV = NULL;
		pyDAEDenseJacobianFunction_array_J = NULL;
		pyDAEDenseJacobianFunction_alpha = NULL;
		pyDAEDenseJacobianCompiledFn = NULL;
		pyDAEDenseJacobianFunctionModule = NULL;
		pyDAEDenseJacobianFunction = NULL;

		// SSDenseJacobian Function data
		pySSDenseJacobianFunctionPosArgs = NULL;
		pySSDenseJacobianFunction_array_V = NULL;
		pySSDenseJacobianFunction_array_J = NULL;
		pySSDenseJacobianCompiledFn = NULL;
		pySSDenseJacobianFunctionModule = NULL;
		pySSDenseJacobianFunction = NULL;
	}

	~PyFunctionServer()
	{
		// DAEResidual Function data
		//if(pyDAEResidualFunctionPosArgs)
		//	Py_DecRef(pyDAEResidualFunctionPosArgs);
		//if(pyDAEResidualFunction_t)
		//	Py_DecRef(pyDAEResidualFunction_t);
		//if(pyDAEResidualFunction_array_V)
		//	Py_DecRef(pyDAEResidualFunction_array_V);
		//if(pyDAEResidualFunction_array_DV)
		//	Py_DecRef(pyDAEResidualFunction_array_DV);
		//if(pyDAEResidualFunction_array_r)
		//	Py_DecRef(pyDAEResidualFunction_array_r);
		//if(pyDAEResidualFunction)
		//	Py_DecRef(pyDAEResidualFunction);
		//if(pyDAEResidualFunctionModule)
		//	Py_DecRef(pyDAEResidualFunctionModule);
		//if(pyDAEResidualCompiledFn)
		//	Py_DecRef(pyDAEResidualCompiledFn);

		//// SSResidual Function data
		//if(pySSResidualFunctionPosArgs)
		//	Py_DecRef(pySSResidualFunctionPosArgs);
		//if(pySSResidualFunction_array_V)
		//	Py_DecRef(pySSResidualFunction_array_V);
		//if(pySSResidualFunction_array_r)
		//	Py_DecRef(pySSResidualFunction_array_r);
		//if(pySSResidualFunction)
		//	Py_DecRef(pySSResidualFunction);
		//if(pySSResidualFunctionModule)
		//	Py_DecRef(pySSResidualFunctionModule);
		//if(pySSResidualCompiledFn)
		//	Py_DecRef(pySSResidualCompiledFn);

		//// DAEDenseJacobian Function data
		//if(pyDAEDenseJacobianFunctionPosArgs)
		//	Py_DecRef(pyDAEDenseJacobianFunctionPosArgs);
		//if(pyDAEDenseJacobianFunction_t)
		//	Py_DecRef(pyDAEDenseJacobianFunction_t);
		//if(pyDAEDenseJacobianFunction_array_V)
		//	Py_DecRef(pyDAEDenseJacobianFunction_array_V);
		//if(pyDAEDenseJacobianFunction_array_DV)
		//	Py_DecRef(pyDAEDenseJacobianFunction_array_DV);
		//if(pyDAEDenseJacobianFunction_array_J)
		//	Py_DecRef(pyDAEDenseJacobianFunction_array_J);
		//if(pyDAEDenseJacobianFunction_alpha)
		//	Py_DecRef(pyDAEDenseJacobianFunction_alpha);
		//if(pyDAEDenseJacobianCompiledFn)
		//	Py_DecRef(pyDAEDenseJacobianCompiledFn);
		//if(pyDAEDenseJacobianFunctionModule)
		//	Py_DecRef(pyDAEDenseJacobianFunctionModule);
		//if(pyDAEDenseJacobianFunction)
		//	Py_DecRef(pyDAEDenseJacobianFunction);

		//// SSDenseJacobian Function data
		//if(pySSDenseJacobianFunctionPosArgs)
		//	Py_DecRef(pySSDenseJacobianFunctionPosArgs);
		//if(pySSDenseJacobianFunction_array_V)
		//	Py_DecRef(pySSDenseJacobianFunction_array_V);
		//if(pySSDenseJacobianFunction_array_J)
		//	Py_DecRef(pySSDenseJacobianFunction_array_J);
		//if(pySSDenseJacobianCompiledFn)
		//	Py_DecRef(pySSDenseJacobianCompiledFn);
		//if(pySSDenseJacobianFunctionModule)
		//	Py_DecRef(pySSDenseJacobianFunctionModule);
		//if(pySSDenseJacobianFunction)
		//	Py_DecRef(pySSDenseJacobianFunction);
	}

	// Input
	bool setEquationsList(std::vector<EquationUnit>* AllEquationsData);
	bool setAllVariableData(stateVars* AllVarsData);

	// Intermediate
	int inputDataInit(bool init);

	// Process(dynamic)
	std::stringstream* generateDAEResidualFunction();
	std::stringstream* generateDAEDenseJacobianFunction();

	// Process(Steady State)
	std::stringstream* generateSSResidualFunction();
	std::stringstream* generateSSDenseJacobianFunction();

	// Function registration (dynamic)
	int registerDAEResidualFunction(std::stringstream* code);
	int registerDAEDenseJacobianFunction(std::stringstream* code);

	// Function registration (Steady State)
	int registerSSResidualFunction(std::stringstream* code);
	int registerSSDenseJacobianFunction(std::stringstream* code);

	// Final Access Functions (dynamic)
	void fDAEResidualFunction(double t, double *y, double* yp, double* r);
	void fDAEDenseJacobianFunction(double t, double* y, double* yp, double* J, double alpha);

	// Final Access Functions (steady state)
	void fSSResidualFunction(double *y, double* r);
	void fSSDenseJacobianFunction(double* y, double* J);

	//Helper functions
	std::string buildSimpleCodeLine(const int &num_tabs, const std::string& line);
	std::string buildResidualCodeLine(const int &num_tabs, const int & res_num, const std::string& line);
};