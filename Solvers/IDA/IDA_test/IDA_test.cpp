/*

This Program is used to test the IDA DAE solver from LLNL.
Note: Code maybe be modified outside this environment
Created: 01/08/2017

*/

// General includes
#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>

//IDA helpers
#include "IDA_helpers.h"

void RunTest(IDA_HELPERS::f_Init funcinit, IDA_HELPERS::f_setdata funcsetdata, IDA_HELPERS::f_setoptions funcsetoptions, IDA_HELPERS::f_funcisolve funcsolve, IDA_HELPERS::f_free funcfree);

int main()
{
	//Load IDA library
	HINSTANCE dllHandle = LoadLibrary("ida_object.dll");

	//Load All functions
	IDA_HELPERS::f_Init funcinit = (IDA_HELPERS::f_Init)GetProcAddress(dllHandle, "INIT");
	IDA_HELPERS::f_setdata funcsetdata = (IDA_HELPERS::f_setdata)GetProcAddress(dllHandle, "SETDATA");
	IDA_HELPERS::f_setoptions funcsetoptions = (IDA_HELPERS::f_setoptions)GetProcAddress(dllHandle, "SETOPTIONS");
	IDA_HELPERS::f_funcisolve funcsolve = (IDA_HELPERS::f_funcisolve)GetProcAddress(dllHandle, "SOLVE");
	IDA_HELPERS::f_free funcfree = (IDA_HELPERS::f_free)GetProcAddress(dllHandle, "FREE");

	RunTest(funcinit, funcsetdata, funcsetoptions, funcsolve, funcfree);

	FreeLibrary(dllHandle);

	return 0;

}