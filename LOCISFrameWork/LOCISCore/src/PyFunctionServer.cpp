#include "PyFunctionServer.h"
#include<fstream> 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Python Specific Globals
bool PYTHON_INITIALIZED = false;
int PYTHON_REF_COUNT = 0;
PYTHON_INFO_GLOBAL PYTHON_INFO_GLOBAL_OBJ;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initializes Numpy
void initNumpy()
{
	import_array();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Singleton function to be called only once for global initialization of Python
int InitializePythonEnv()
{
	if (!PYTHON_INITIALIZED)
	{
		try
		{
			//Py_NoSiteFlag = 1;
			//Py_SetPythonHome(".");
			Py_Initialize();
			//PyRun_SimpleString("import sys");
			//PySys_SetPath(PY_PATHS);
			
			//Import all 
			PYTHON_INFO_GLOBAL_OBJ.Python_module_main = PyImport_AddModule("__main__"); 
			PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy = PyImport_ImportModule("autograd.numpy");
			PYTHON_INFO_GLOBAL_OBJ.Python_module_autograd = PyImport_ImportModule("autograd");  

			// Get Dictionaries
			PYTHON_INFO_GLOBAL_OBJ.Python_module_main_dict = PyModule_GetDict(PYTHON_INFO_GLOBAL_OBJ.Python_module_main);
			PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy_dict = PyModule_GetDict(PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy);
			PYTHON_INFO_GLOBAL_OBJ.Python_module_autograd_dict = PyModule_GetDict(PYTHON_INFO_GLOBAL_OBJ.Python_module_autograd);

			// Check if modules exist
			if (!(PYTHON_INFO_GLOBAL_OBJ.Python_module_main && PYTHON_INFO_GLOBAL_OBJ.Python_module_main_dict))
			{
				return PY_MAIN_FAIL;
			}

			if (!(PYTHON_INFO_GLOBAL_OBJ.Python_module_autograd && PYTHON_INFO_GLOBAL_OBJ.Python_module_autograd_dict))
			{
				return PY_AUTOGRAD_FAIL;
			}

			if (!(PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy && PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy_dict))
			{
				return PY_NUMPY_FAIL;
			}

			initNumpy();

			PYTHON_INITIALIZED = true;
		}
		catch (...)
		{
			return PY_FAIL;
		}
	}
	else
	{
		PYTHON_REF_COUNT++;
	}
	return PY_SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Singleton function to be called only once for global initialization of Python
int FinalizePythonEnv()
{
	try
	{
		Py_Finalize();
	}
	catch (...)
	{
		return PY_FAIL;
	}
	return PY_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set all Equation Data from Equation Server
bool PyFunctionServer::setEquationsList(std::vector<EquationUnit>* AllEquationsData_arg)
{
	if (AllEquationsData_arg)
	{
		AllEquationsData = AllEquationsData_arg;
		return PY_SUCCESS;
	}
	return PY_FAIL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set All portable variables
bool PyFunctionServer::setAllVariableData(stateVars* AllVarsData_arg)
{
	if (AllVarsData_arg)
	{
		AllVarsData = AllVarsData_arg;
		return PY_SUCCESS;
	}
	return PY_FAIL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sets all the critical numbers 
int PyFunctionServer::inputDataInit(bool init)
{
	if (AllEquationsData && AllVarsData)
	{
		if (!init)
			NumVariables = AllVarsData->NumVars;
		else
			NumVariables = AllVarsData->initIncr + 1;

		NumEquations = AllEquationsData->size();

		if (NumVariables == 0)
		{
			return PY_FAIL;
		}

		if (NumEquations == 0)
		{
			return PY_FAIL;
		}

		if (NumVariables != NumEquations)
		{
			return PY_FAIL;
		}

		pyDAEResidualFunctiondims[0] = NumVariables;
		pyDAEDenseJacobianFunctiondims[0] = NumVariables;

		return PY_SUCCESS;
	}
	return PY_FAIL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generates the DAE Residual Function
std::stringstream* PyFunctionServer::generateDAEResidualFunction()
{
	//Sanity Checks
	if (!AllEquationsData)
	{
		return NULL;
	}
	std::stringstream* code = new std::stringstream;

	//write header
	*code << buildSimpleCodeLine(0, "def DAEResidualFunction(_t, V, DV, r):");

	//Loop over Equations
	int num_resi = 0;
	for (auto eqdat = AllEquationsData->begin(); eqdat != AllEquationsData->end(); ++eqdat)
	{
		*code << buildResidualCodeLine(1, num_resi, eqdat->sEquation);
		num_resi++;
	}

	return code;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Creates the module and assigns the residual 
int PyFunctionServer::registerDAEResidualFunction(std::stringstream* code)
{
	//Sanity Checks
	if (!code)
	{
		return PY_FAIL;
	}

	// Set dimensions of Vectors
	pyDAEResidualFunctiondims[0] = NumVariables;

	// Compile the function
	pyDAEResidualCompiledFn = Py_CompileString(code->str().c_str(), "", Py_file_input);
	if (!pyDAEResidualCompiledFn)
	{
		return PY_FAIL;
	}

	// Create a module and add the function to it    
	pyDAEResidualFunctionModule = PyImport_ExecCodeModule("DAEResidualFunctionModule", pyDAEResidualCompiledFn);
	if (!pyDAEResidualFunctionModule)
	{
		return PY_FAIL;
	}

	// Add Environment
	AddEnvironmentToCompiledModules(pyDAEResidualFunctionModule);

	// Retrieve function to run space from module
	pyDAEResidualFunction = PyObject_GetAttrString(pyDAEResidualFunctionModule, "DAEResidualFunction");
	if (!pyDAEResidualFunction)
	{
		return PY_FAIL;
	}

	// Check if function is callable
	if (!PyCallable_Check(pyDAEResidualFunction))
	{
		return PY_FAIL;
	}

	// Create prototype of argument list which is DAEResidualFunction(_t, V, DV, r)
	pyDAEResidualFunctionPosArgs = PyTuple_New(4);

	return PY_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Final DAEResidual Function used for Access
void PyFunctionServer::fDAEResidualFunction(double t, double *y, double* yp, double* r)
{
	// Attach to Data
	int rc;
	pyDAEResidualFunction_t = PyFloat_FromDouble(t);
	pyDAEResidualFunction_array_V = PyArray_SimpleNewFromData(1, pyDAEResidualFunctiondims, NPY_DOUBLE, y);
	pyDAEResidualFunction_array_DV = PyArray_SimpleNewFromData(1, pyDAEResidualFunctiondims, NPY_DOUBLE, yp);
	pyDAEResidualFunction_array_r = PyArray_SimpleNewFromData(1, pyDAEResidualFunctiondims, NPY_DOUBLE, r);

	// Make Parameter list
	rc = PyTuple_SetItem(pyDAEResidualFunctionPosArgs, 0, pyDAEResidualFunction_t);
	rc = PyTuple_SetItem(pyDAEResidualFunctionPosArgs, 1, pyDAEResidualFunction_array_V);
	rc = PyTuple_SetItem(pyDAEResidualFunctionPosArgs, 2, pyDAEResidualFunction_array_DV);
	rc = PyTuple_SetItem(pyDAEResidualFunctionPosArgs, 3, pyDAEResidualFunction_array_r);

	// Call the function
	PyObject_CallObject(pyDAEResidualFunction, pyDAEResidualFunctionPosArgs);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generates the DAE Dense Jacobian Function
std::stringstream* PyFunctionServer::generateDAEDenseJacobianFunction()
{
	//Sanity Checks
	if (!AllEquationsData)
	{
		return NULL;
	}

	std::stringstream* code = new std::stringstream;

	// Now build full Jacobian function
	*code << buildSimpleCodeLine(0, "def DAEDenseJacobianFunction(_t, V, DV, J, alpha):");

	// Make dy 
	int resi_num = 0;
	std::string rns("");
	for (auto eqdat = AllEquationsData->begin(); eqdat != AllEquationsData->end(); ++eqdat)
	{
		rns = std::to_string(resi_num);
		*code << buildSimpleCodeLine(1, "gy" + rns + "= grad(lambda V, DV: " + eqdat->sEquation + ")");
		resi_num++;
	}

	// Make dyp
	resi_num = 0;
	rns.assign("");
	for (auto eqdat = AllEquationsData->begin(); eqdat != AllEquationsData->end(); ++eqdat)
	{
		rns = std::to_string(resi_num);
		*code << buildSimpleCodeLine(1, "gyp" + rns + "= grad(lambda DV, V: " + eqdat->sEquation + ")");
		resi_num++;
	}

	resi_num = 0;
	rns.assign("");
	for (auto eqdat = AllEquationsData->begin(); eqdat != AllEquationsData->end(); ++eqdat)
	{
		rns = std::to_string(resi_num);
		*code << buildSimpleCodeLine(1, "J[:, " + rns + "] = add(gy" + rns + "(V,DV), alpha*gyp" + rns + "(V,DV))");
		resi_num++;
	}

	std::ofstream myfile("C:\\ARJUN\\DAEResidual.txt");
	if (myfile.is_open())
	{
		myfile << code->str();
		myfile.close();
	}

	return code;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Creates the module and assigns the Dense Jacobian
int PyFunctionServer::registerDAEDenseJacobianFunction(std::stringstream* code)
{
	//Sanity Checks
	if (!code)
	{
		return PY_FAIL;
	}

	// Set dimensions of Vectors
	pyDAEDenseJacobianFunctiondims[0] = pyDAEDenseJacobianFunctiondims[1] = NumVariables;

	// Compile the function
	pyDAEDenseJacobianCompiledFn = Py_CompileString(code->str().c_str(), "", Py_file_input);
	if (!pyDAEDenseJacobianCompiledFn)
	{
		return PY_FAIL;
	}

	// Create a module and add the function to it    
	pyDAEDenseJacobianFunctionModule = PyImport_ExecCodeModule("DAEDenseJacobianFunctionModule", pyDAEDenseJacobianCompiledFn);
	if (!pyDAEDenseJacobianFunctionModule)
	{
		return PY_FAIL;
	}

	// Add Environment
	AddEnvironmentToCompiledModules(pyDAEDenseJacobianFunctionModule);

	// Retrieve function to run space from module
	pyDAEDenseJacobianFunction = PyObject_GetAttrString(pyDAEDenseJacobianFunctionModule, "DAEDenseJacobianFunction");
	if (!pyDAEDenseJacobianFunction)
	{
		return PY_FAIL;
	}

	// Check if function is callable
	if (!PyCallable_Check(pyDAEDenseJacobianFunction))
	{
		return PY_FAIL;
	}

	// Create prototype of argument list which is DAEResidualFunction(V, DV, r)
	pyDAEDenseJacobianFunctionPosArgs = PyTuple_New(5);

	return PY_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Final DAEDenseJacobian Function used for Access
void PyFunctionServer::fDAEDenseJacobianFunction(double t, double* y, double* yp, double* J, double alpha)
{
	// Attach to Data
	int rc;
	pyDAEDenseJacobianFunction_t = PyFloat_FromDouble(t);
	pyDAEDenseJacobianFunction_array_V = PyArray_SimpleNewFromData(1, pyDAEResidualFunctiondims, NPY_DOUBLE, y);
	pyDAEDenseJacobianFunction_array_DV = PyArray_SimpleNewFromData(1, pyDAEResidualFunctiondims, NPY_DOUBLE, yp);
	pyDAEDenseJacobianFunction_array_J = PyArray_SimpleNewFromData(2, pyDAEDenseJacobianFunctiondims, NPY_DOUBLE, J);
	pyDAEDenseJacobianFunction_alpha = PyFloat_FromDouble(alpha);

	// Make Parameter list
	rc = PyTuple_SetItem(pyDAEDenseJacobianFunctionPosArgs, 0, pyDAEDenseJacobianFunction_t);
	rc = PyTuple_SetItem(pyDAEDenseJacobianFunctionPosArgs, 1, pyDAEDenseJacobianFunction_array_V);
	rc = PyTuple_SetItem(pyDAEDenseJacobianFunctionPosArgs, 2, pyDAEDenseJacobianFunction_array_DV);
	rc = PyTuple_SetItem(pyDAEDenseJacobianFunctionPosArgs, 3, pyDAEDenseJacobianFunction_array_J);
	rc = PyTuple_SetItem(pyDAEDenseJacobianFunctionPosArgs, 4, pyDAEDenseJacobianFunction_alpha);

	// Call the function
	PyObject_CallObject(pyDAEDenseJacobianFunction, pyDAEDenseJacobianFunctionPosArgs);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generates the Steady State Residual
std::stringstream* PyFunctionServer::generateSSResidualFunction()
{
	//Sanity Checks
	if (!AllEquationsData)
	{
		return NULL;
	}
	std::stringstream* code = new std::stringstream;

	//write header
	*code << buildSimpleCodeLine(0, "def SSResidualFunction(V, r):");

	//Loop over Equations
	int num_resi = 0;
	for (auto eqdat = AllEquationsData->begin(); eqdat != AllEquationsData->end(); ++eqdat)
	{
		*code << buildResidualCodeLine(1, num_resi, eqdat->sEquation);
		num_resi++;
	}

	return code;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Registers the Steady State residual
int PyFunctionServer::registerSSResidualFunction(std::stringstream* code)
{
	//Sanity Checks
	if (!code)
	{
		return PY_FAIL;
	}

	// Set dimensions of Vectors
	pySSResidualFunctiondims[0] = NumVariables;

	// Compile the function
	pySSResidualCompiledFn = Py_CompileString(code->str().c_str(), "", Py_file_input);
	if (!pySSResidualCompiledFn)
	{
		return PY_FAIL;
	}

	// Create a module and add the function to it    
	pySSResidualFunctionModule = PyImport_ExecCodeModule("SSResidualFunctionModule", pySSResidualCompiledFn);
	if (!pySSResidualFunctionModule)
	{
		return PY_FAIL;
	}

	// Add Environment
	AddEnvironmentToCompiledModules(pySSResidualFunctionModule);

	// Retrieve function to run space from module
	pySSResidualFunction = PyObject_GetAttrString(pySSResidualFunctionModule, "SSResidualFunction");
	if (!pySSResidualFunction)
	{
		return PY_FAIL;
	}

	// Check if function is callable
	if (!PyCallable_Check(pySSResidualFunction))
	{
		return PY_FAIL;
	}

	// Create prototype of argument list which is SSResidualFunction(V, DV, r)
	pySSResidualFunctionPosArgs = PyTuple_New(2);

	return PY_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Final SSResidual Function used for Access
void PyFunctionServer::fSSResidualFunction(double *y, double* r)
{
	// Attach to Data
	int rc;
	pySSResidualFunction_array_V = PyArray_SimpleNewFromData(1, pySSResidualFunctiondims, NPY_DOUBLE, y);
	pySSResidualFunction_array_r = PyArray_SimpleNewFromData(1, pySSResidualFunctiondims, NPY_DOUBLE, r);

	// Make Parameter list
	rc = PyTuple_SetItem(pySSResidualFunctionPosArgs, 0, pySSResidualFunction_array_V);
	rc = PyTuple_SetItem(pySSResidualFunctionPosArgs, 1, pySSResidualFunction_array_r);

	// Call the function
	PyObject_CallObject(pySSResidualFunction, pySSResidualFunctionPosArgs);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generates the SS Dense Jacobian Function
std::stringstream* PyFunctionServer::generateSSDenseJacobianFunction()
{
	//Sanity Checks
	if (!AllEquationsData)
	{
		return NULL;
	}

	std::stringstream* code = new std::stringstream;

	// Create flag

	// Now build full Jacobian function
	*code << buildSimpleCodeLine(0, "def SSDenseJacobianFunction(V, J):");

	// Make dy 
	int resi_num = 0;
	std::string rns("");
	for (auto eqdat = AllEquationsData->begin(); eqdat != AllEquationsData->end(); ++eqdat)
	{
		rns = std::to_string(resi_num);
		*code << buildSimpleCodeLine(1, "gy" + rns + "= grad(lambda V: " + eqdat->sEquation + ")");
		resi_num++;
	}

	resi_num = 0;
	rns.assign("");
	for (auto eqdat = AllEquationsData->begin(); eqdat != AllEquationsData->end(); ++eqdat)
	{
		rns = std::to_string(resi_num);
		*code << buildSimpleCodeLine(1, "J[:," + rns + "] = gy" + rns + "(V)");
		resi_num++;
	}

	return code;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Creates the module and assigns the Dense Jacobian
int PyFunctionServer::registerSSDenseJacobianFunction(std::stringstream* code)
{
	//Sanity Checks
	if (!code)
	{
		return PY_FAIL;
	}

	// Set dimensions of Vectors
	pySSDenseJacobianFunctiondims[0] = pySSDenseJacobianFunctiondims[1] = NumVariables;

	// Compile the function
	pySSDenseJacobianCompiledFn = Py_CompileString(code->str().c_str(), "", Py_file_input);
	if (!pySSDenseJacobianCompiledFn)
	{
		return PY_FAIL;
	}

	// Create a module and add the function to it    
	pySSDenseJacobianFunctionModule = PyImport_ExecCodeModule("SSDenseJacobianFunctionModule", pySSDenseJacobianCompiledFn);
	if (!pySSDenseJacobianFunctionModule)
	{
		return PY_FAIL;
	}

	// Add Environment
	AddEnvironmentToCompiledModules(pySSDenseJacobianFunctionModule);

	// Retrieve function to run space from module
	pySSDenseJacobianFunction = PyObject_GetAttrString(pySSDenseJacobianFunctionModule, "SSDenseJacobianFunction");
	if (!pySSDenseJacobianFunction)
	{
		return PY_FAIL;
	}

	// Check if function is callable
	if (!PyCallable_Check(pySSDenseJacobianFunction))
	{
		return PY_FAIL;
	}

	// Create prototype of argument list which is SSResidualFunction(V, DV, r)
	pySSDenseJacobianFunctionPosArgs = PyTuple_New(2);

	return PY_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Final SSDenseJacobian Function used for Access
void PyFunctionServer::fSSDenseJacobianFunction(double* y, double* J)
{
	// Attach to Data
	int rc;
	pySSDenseJacobianFunction_array_V = PyArray_SimpleNewFromData(1, pySSResidualFunctiondims, NPY_DOUBLE, y);
	pySSDenseJacobianFunction_array_J = PyArray_SimpleNewFromData(2, pySSDenseJacobianFunctiondims, NPY_DOUBLE, J);

	// Make Parameter list
	rc = PyTuple_SetItem(pySSDenseJacobianFunctionPosArgs, 0, pySSDenseJacobianFunction_array_V);
	rc = PyTuple_SetItem(pySSDenseJacobianFunctionPosArgs, 1, pySSDenseJacobianFunction_array_J);

	// Call the function
	PyObject_CallObject(pySSDenseJacobianFunction, pySSDenseJacobianFunctionPosArgs);

}



