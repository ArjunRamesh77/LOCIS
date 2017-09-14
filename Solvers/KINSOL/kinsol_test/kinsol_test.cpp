/*

This Program is used to test the Kinsol non-linear solver from LLNL.
Note: Code maybe be modified outside this environment
Created: 12/21/2016

*/

// General includes
#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>

//Kinsol helpers
#include "kinsol_helpers.h"

void RunTest(KINSOL_HELPERS::f_Init funcinit, KINSOL_HELPERS::f_setdata funcsetdata, KINSOL_HELPERS::f_setoptions funcsetoptions, KINSOL_HELPERS::f_funcisolve funcsolve, KINSOL_HELPERS::f_free funcfree);

// Main code
int main()
{
	// Load Kinsol library
	HINSTANCE dllHandle = LoadLibrary("kinsol_object.dll");
	
	// Load All functions
	KINSOL_HELPERS::f_Init funcinit = (KINSOL_HELPERS::f_Init)GetProcAddress(dllHandle, "INIT");
	KINSOL_HELPERS::f_setdata funcsetdata = (KINSOL_HELPERS::f_setdata)GetProcAddress(dllHandle, "SETDATA");
	KINSOL_HELPERS::f_setoptions funcsetoptions = (KINSOL_HELPERS::f_setoptions)GetProcAddress(dllHandle, "SETOPTIONS");
	KINSOL_HELPERS::f_funcisolve funcsolve = (KINSOL_HELPERS::f_funcisolve)GetProcAddress(dllHandle, "SOLVE");
	KINSOL_HELPERS::f_free funcfree = (KINSOL_HELPERS::f_free)GetProcAddress(dllHandle, "FREE");

	RunTest(funcinit, funcsetdata, funcsetoptions, funcsolve, funcfree);

	FreeLibrary(dllHandle);
	
	return 0;
}


