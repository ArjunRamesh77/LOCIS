#include "PyFunctionServer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Singleton function to be called only once for global initialization of Python
int AddEnvironmentToCompiledModules(PyObject* CompiledModule)
{
	// Get Dictionary of module
	PyObject* CompiledModuleDict = PyModule_GetDict(CompiledModule);

	// Add numpy and ad modules
	PyDict_SetItemString(CompiledModuleDict, "autograd.numpy", PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy);
	PyDict_SetItemString(CompiledModuleDict, "autograd", PYTHON_INFO_GLOBAL_OBJ.Python_module_autograd);

	// Get all symbols
	PyObject* autograd_grad = PyMapping_GetItemString(PYTHON_INFO_GLOBAL_OBJ.Python_module_autograd_dict, "grad");
	PyObject* numpy_add = PyMapping_GetItemString(PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy_dict, "add");
	PyObject* numpy_sin = PyMapping_GetItemString(PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy_dict, "sin");
	PyObject* numpy_cos = PyMapping_GetItemString(PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy_dict, "cos");
	PyObject* numpy_tan = PyMapping_GetItemString(PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy_dict, "tan");
	PyObject* numpy_arccos = PyMapping_GetItemString(PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy_dict, "arccos");
	PyObject* numpy_arctan = PyMapping_GetItemString(PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy_dict, "arctan");
	PyObject* numpy_sinh = PyMapping_GetItemString(PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy_dict, "sinh");
	PyObject* numpy_cosh = PyMapping_GetItemString(PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy_dict, "cosh");
	PyObject* numpy_tanh = PyMapping_GetItemString(PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy_dict, "tanh");
	PyObject* numpy_arcsinh = PyMapping_GetItemString(PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy_dict, "arcsinh");
	PyObject* numpy_arccosh = PyMapping_GetItemString(PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy_dict, "arccosh");
	PyObject* numpy_arctanh = PyMapping_GetItemString(PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy_dict, "arctanh");
	PyObject* numpy_exp = PyMapping_GetItemString(PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy_dict, "exp");
	PyObject* numpy_log = PyMapping_GetItemString(PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy_dict, "log");
	PyObject* numpy_log10 = PyMapping_GetItemString(PYTHON_INFO_GLOBAL_OBJ.Python_module_numpy_dict, "log10");


	// Add symbols to module
	PyDict_SetItemString(CompiledModuleDict, "grad", autograd_grad);
	PyDict_SetItemString(CompiledModuleDict, "add", numpy_add);
	PyDict_SetItemString(CompiledModuleDict, "sin", numpy_sin);
	PyDict_SetItemString(CompiledModuleDict, "cos", numpy_cos);
	PyDict_SetItemString(CompiledModuleDict, "tan", numpy_tan);
	PyDict_SetItemString(CompiledModuleDict, "arccos", numpy_arccos);
	PyDict_SetItemString(CompiledModuleDict, "arctan", numpy_arctan);
	PyDict_SetItemString(CompiledModuleDict, "sinh", numpy_sinh);
	PyDict_SetItemString(CompiledModuleDict, "cosh", numpy_cosh);
	PyDict_SetItemString(CompiledModuleDict, "tanh", numpy_tanh);
	PyDict_SetItemString(CompiledModuleDict, "arcsinh", numpy_arcsinh);
	PyDict_SetItemString(CompiledModuleDict, "arccosh", numpy_arccosh);
	PyDict_SetItemString(CompiledModuleDict, "arctanh", numpy_arctanh);
	PyDict_SetItemString(CompiledModuleDict, "exp", numpy_exp);
	PyDict_SetItemString(CompiledModuleDict, "log", numpy_log);
	PyDict_SetItemString(CompiledModuleDict, "log10", numpy_log10);

	// Dereference locals
	Py_DecRef(CompiledModuleDict);
	Py_DecRef(autograd_grad);
	Py_DecRef(numpy_add);
	Py_DecRef(numpy_sin);
	Py_DecRef(numpy_cos);
	Py_DecRef(numpy_tan);
	Py_DecRef(numpy_arccos);
	Py_DecRef(numpy_arctan);
	Py_DecRef(numpy_sinh);
	Py_DecRef(numpy_cosh);
	Py_DecRef(numpy_tanh);
	Py_DecRef(numpy_arcsinh);
	Py_DecRef(numpy_arccosh);
	Py_DecRef(numpy_arctanh);
	Py_DecRef(numpy_exp);
	Py_DecRef(numpy_log);
	Py_DecRef(numpy_log10);

	return PY_SUCCESS;
}