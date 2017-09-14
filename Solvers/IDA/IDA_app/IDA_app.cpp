// IDA_app.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include "stdafx.h"

#include "IDA_helpers.h"
#include "adolc\adolc.h"

using namespace IDA_HELPERS;

void JacFunc(double t, double *y, double *yp, double **J, double cj);

void ResFunc(double t, double *y, double *yp, double *r);

void RootFunc(double *yy, double *yp, double *gout);

int UserErrorHandler(void *s, int type);

int UserInfoHandler(void *s, int type);

int _tmain(int argc, _TCHAR* argv[])
{

	double *y_init, *yp_init;
	IDAOptions *options;
	error_IDA *errorvector;
	int NEQ;
	int *setidptr;

	NEQ = 3;

	options = new IDAOptions;
	errorvector = new error_IDA;
	setidptr = new int[NEQ];
	setidptr[0] = 1;
	setidptr[1] = 1;
	setidptr[2] = 0;

	y_init = NULL;
	yp_init = NULL;
	y_init = new double[3];
	yp_init = new double[3];

	y_init[0] = 1;
	y_init[1] = 0;
	y_init[2] = 0;

	yp_init[0] = -0.04;
	yp_init[1] = 0.04;
	yp_init[2] = 0;

	//	abstol = new double[3];
	(*options).abstol = NULL;

	(*options).abstol = new double[3];
	(*options).abstol[0] = 1.0e-8;
	(*options).abstol[1] = 1.0e-14;
	(*options).abstol[2] = 1.0e-6;

	(*options).reltol = 1e-4;

	(*options).nout = 6;
	(*options).t0 = 10;
	(*options).tfinal = 40;
	options->finalstatsprintlevel = 1;

	options->LinearSolver = "IDADENSE";

	options->callrootfn = true;
	options->NrootFn = 2;
	options->solvetype = 3;

	options->callCalcIC = true;
	options->if_jacobian = true;
	options->setidptr = setidptr;
	options->setidlength = NEQ;
	options->callSetID = true;
	options->tolerancetype = 2;
	options->tolerancesize = NEQ;

	HINSTANCE dllHandle = NULL;

	dllHandle = LoadLibrary("IDA_object.dll");

	f_boss funcboss = (f_boss)GetProcAddress(dllHandle, "IDABOSS");
	funcboss(0, JacFunc, ResFunc, UserErrorHandler, UserInfoHandler, RootFunc,
		errorvector, NEQ, y_init, yp_init, options);

	//f_Init funcinit = (f_Init)GetProcAddress(dllHandle, "INIT");
	//objnum = funcinit();

	//f_setdata funcsetdata = (f_setdata)GetProcAddress(dllHandle, "SETDATA");
	//funcsetdata(JacFunc, ResFunc, UserErrorHandler, UserInfoHandler, RootFunc, objnum,
	//	errorvector, NEQ, y_init, yp_init);

	//f_setoptions funcsetoptions = (f_setoptions)GetProcAddress(dllHandle, "SETOPTIONS");
	//returncode = funcsetoptions(objnum, options);
	//if (returncode == 0) return 0;

	//f_funcisolve funcsolve = (f_funcisolve)GetProcAddress(dllHandle, "SOLVE");
	//funcsolve(objnum);

	//f_free funcfree = (f_free)GetProcAddress(dllHandle, "FREE");
	//funcfree(objnum);

	return(1);

}

void JacFunc(double t, double *y, double *yp, double **J, double cj)
{

	adouble x1, x2, x3;
	adouble	xp1, xp2, xp3;

	double **J1, **J2;

	adouble y1, y2, y3;

	double y1_d, y2_d, y3_d;

	double var1, var2, var3;
	double varp1, varp2, varp3;

	J1 = new double *[3];
	for (int i = 0; i < 3; i++)
	{
		J1[i] = new double[3];
	}

	J2 = new double *[3];
	for (int i = 0; i < 3; i++)
	{
		J2[i] = new double[3];
	}

	trace_on(1);
	x1 <<= 1.0;
	x2 <<= 1.0;
	x3 <<= 1.0;

	y1 = -0.04*x1 + 1.0e4 * x2 * x3 - yp[0];
	y2 = -(-0.04*x1 + 1.0e4*x2 * x3) - 3.0e7*x2 * x2 - yp[1];
	y3 = x1 + x2 + x3 - 1;

	y1 >>= y1_d;
	y2 >>= y2_d;
	y3 >>= y3_d;

	trace_off();

	var1 = y[0];
	var2 = y[1];
	var3 = y[2];

	double x0[3] = { var1, var2, var3};

	jacobian(1, 3, 3, x0, J1);

	trace_on(1);
	xp1 <<= 1.0;
	xp2 <<= 1.0;
	xp3 <<= 1.0;

	y1 = -0.04*y[0] + 1.0e4 * y[1] * y[2] - xp1;
	y2 = -(-0.04*y[0] + 1.0e4*y[1] * y[2]) - 3.0e7*y[1] * y[1] - xp2;
	y3 = y[0] + y[1] + y[2] - 1;

	y1 >>= y1_d;
	y2 >>= y2_d;
	y3 >>= y3_d;

	trace_off();

	varp1 = yp[0];
	varp2 = yp[1];
	varp3 = yp[2];

	double xp0[3] = { varp1, varp2, varp3 };

	jacobian(1, 3, 3, xp0, J2);

	for (int i = 0; i <= 2; i++)
	{
		for (int j = 0; j <= 2; j++)
		{
			J[i][j] = J1[i][j] + cj * J2[i][j];
		}
	}
	delete[]J1;
	delete[]J2;


}

void ResFunc(double t, double *yy, double *yp, double *r)
{

	r[0] = -0.04*yy[0] + 1.0e4*yy[1] * yy[2];

	r[1] = -r[0] - 3.0e7*yy[1] * yy[1] - yp[1];
	r[0] -= yp[0];
	r[2] = yy[0] + yy[1] + yy[2] - 1;

}

void RootFunc(double *yy, double *yp, double *gout)
{
	double y1, y3;

	y1 = yy[0]; y3 = yy[2];
	gout[0] = y1 - 0.0001;
	gout[1] = y3 - 0.01;
}

int UserErrorHandler(void *s, int type)
{
	return(0);
}

int UserInfoHandler(void *s, int type)
{
	
	return 1;
}
