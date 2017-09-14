// kinsol_object.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include "stdafx.h"

#include "kinsol_helpers.h"
#include "adolc\adolc.h"

using namespace KINSOL_HELPERS;

typedef int (*f_Init)();

void ResFunc(double *x, double *r);

void JacFunc(double *x, double **r);

void UserInfoHandler(void *s, int type);

int UserErrorHandler(void *s, int type);

//This will contain problem definition and a call to kinsol.
int _tmain(int argc, _TCHAR* argv[])
{
	double *init_val, *x;
	int NVAR, NEQ;
	void *errorvector;//, *errOptions, *errSolve;
	KinsolOptions *options;
	void *objptr;
	int returncode;

	// Define problem size
	NVAR = 8;
	NEQ = NVAR;

	init_val = new double[NVAR];
	x = new double[NVAR];

	for (int i = 1; i <= NVAR; i++)
	{
		init_val[i - 1] = 0.707107;
	}

	options = new KinsolOptions;
	
	(*options).GlobalStrategy = 0;
	(*options).LinearSolver = "KINDENSE";

	(*options).FuncNormTolerance = 1.e-5;
	(*options).ScaledStepTol = 1.e-5;
	(*options).MaxSetupCalls = 1;
	(*options).if_jacobian = 1;
	(*options).finalstatsPrintLevel = 0;

	errorvector = new error_kinsol;
//	errOptions = new error_kinsol;
//	errSolve = new error_kinsol;

	objptr = NULL;

	HINSTANCE dllHandle = NULL;


	dllHandle = LoadLibrary("kinsol_object.dll");

	f_boss funcboss = (f_boss)GetProcAddress(dllHandle, "KINSOLBOSS");
	returncode = funcboss(0, NVAR, NEQ, init_val, ResFunc, JacFunc, 
		UserInfoHandler, UserErrorHandler, options, x, errorvector);
	cout << "\n";
	cout << x[0];
	cout << "\n";

	//f_Init funcinit = (f_Init)GetProcAddress(dllHandle, "INIT");
	//objnum = funcinit();

	//f_setdata funcsetdata = (f_setdata)GetProcAddress(dllHandle, "SETDATA");
	//funcsetdata(objnum, errorvector, NVAR, NEQ, init_val, ResFunc, JacFunc, UserInfoHandler);

	//f_setoptions funcsetoptions = (f_setoptions)GetProcAddress(dllHandle, "SETOPTIONS");
	//funcsetoptions(objnum, errorvector, options);

	//f_funcisolve funcsolve = (f_funcisolve)GetProcAddress(dllHandle, "SOLVE");
	//funcsolve(objnum, errorvector, options, x);

	//f_free funcfree = (f_free)GetProcAddress(dllHandle, "FREE");
	//funcfree(objnum);

	delete errorvector;
	delete options;

	return 0;
}

void ResFunc(double *x, double *r)
{

	double x1, x2, x3, x4, x5, x6, x7, x8;

	double eq1, eq2, eq3, eq4, eq5, eq6, eq7, eq8;

	x1 = x[0];
	x2 = x[1];
	x3 = x[2];
	x4 = x[3];
	x5 = x[4];
	x6 = x[5];
	x7 = x[6];
	x8 = x[7];

	/* Nonlinear equations */

	eq1 = -0.1238*x1 + x7 - 0.001637*x2
		- 0.9338*x4 + 0.004731*x1*x3 - 0.3578*x2*x3 - 0.3571;
	eq2 = 0.2638*x1 - x7 - 0.07745*x2
		- 0.6734*x4 + 0.2238*x1*x3 + 0.7623*x2*x3 - 0.6022;
	eq3 = 0.3578*x1 + 0.004731*x2 + x6*x8;
	eq4 = -0.7623*x1 + 0.2238*x2 + 0.3461;
	eq5 = x1*x1 + x2*x2 - 1;
	eq6 = x3*x3 + x4*x4 - 1;
	eq7 = x5*x5 + x6*x6 - 1;
	eq8 = x7*x7 + x8*x8 - 1;

	r[0] = eq1;
	r[1] = eq2;
	r[2] = eq3;
	r[3] = eq4;
	r[4] = eq5;
	r[5] = eq6;
	r[6] = eq7;
	r[7] = eq8;
}


void JacFunc(double *x, double **J)
{

	adouble x1, x2,x3, x4, x5, x6, x7, x8;
	adouble y1, y2, y3, y4, y5, y6, y7, y8;
	double y1_d, y2_d, y3_d, y4_d, y5_d, y6_d, y7_d, y8_d;
	double var1, var2, var3, var4, var5, var6, var7, var8;

	trace_on(1);
	x1 <<= 1.0;
	x2 <<= 1.0;
	x3 <<= 1.0;
	x4 <<= 1.0;
	x5 <<= 1.0;
	x6 <<= 1.0;
	x7 <<= 1.0;
	x8 <<= 1.0;

	y1 = -0.1238*x1 + x7 - 0.001637*x2
		- 0.9338*x4 + 0.004731*x1*x3 - 0.3578*x2*x3 - 0.3571;
	y2 = 0.2638*x1 - x7 - 0.07745*x2
		- 0.6734*x4 + 0.2238*x1*x3 + 0.7623*x2*x3 - 0.6022;
	y3 = 0.3578*x1 + 0.004731*x2 + x6*x8;
	y4 = -0.7623*x1 + 0.2238*x2 + 0.3461;
	y5 = x1*x1 + x2*x2 - 1;
	y6 = x3*x3 + x4*x4 - 1;
	y7 = x5*x5 + x6*x6 - 1;
	y8 = x7*x7 + x8*x8 - 1;

	y1 >>= y1_d;
	y2 >>= y2_d;
	y3 >>= y3_d;
	y4 >>= y4_d;
	y5 >>= y5_d;
	y6 >>= y6_d;
	y7 >>= y7_d;
	y8 >>= y8_d;

	trace_off();

	var1 = x[0];
	var2 = x[1];
	var3 = x[2];
	var4 = x[3];
	var5 = x[4];
	var6 = x[5];
	var7 = x[6];
	var8 = x[7];

	double x0[8] = {var1, var2, var3, var4, var5,
	var6, var7, var8};

	jacobian(1, 8, 8, x0, J);
}

void UserInfoHandler(void *s, int type)
{
	
}

int UserErrorHandler(void *s, int type)
{

	return(0);
}