// FEM_1D_trial.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include "stdafx.h"

#include "kinsol_helpers.h"
#include "adolc\adolc.h"

#define N 9
#define nElem 10
# define k1 1
# define k2 1
# define k3 1

using namespace KINSOL_HELPERS;

void ResFunc(double *x, double *r);

void JacFunc(double *x, double **r);
double integral_phidp(int i, int j, double h);
double integral_phipp(int i, int j, double h);

void UserInfoHandler(void *s, int type);

int UserErrorHandler(void *s, int type);

int _tmain(int argc, _TCHAR* argv[])
{
//	int nElem;
	int NVAR, NEQ;
	double *init_val, *x;
	KinsolOptions *options;
	void *errorvector;
	void *objptr;
	int returncode;

//	nElem = 3;
	NVAR = nElem - 1;
	NEQ = nElem - 1;

	init_val = new double[NVAR];
	x = new double[NVAR];
	for (int i = 1; i <= NVAR; i++)
	{
		init_val[i - 1] = 0.2;
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
	for (int i = 0; i < NVAR; i++)
	{
		cout << "\n";
		cout << x[i];
	}

    return 0;
}

void ResFunc(double *x, double *res)
{
	int i, j, nEq;
	double u0, u1, h, temp1, temp2;
	

	u0 = 0.0; u1 = 0.0;
	// solve u" = 1 u(0) = u(1) = 0
	// solve u" + k1u' + k2u + k3 = 0 u(0) = u(1) = 0

	nEq = (nElem - 1);
	h = 1.0 / nElem;

	for (i = 0; i < nEq; i++)
	{
		res[i] = 0;
	}

	// res[0] = -integral[0:1/nElem](u0 * phi_1'*phi_2')-integral[0:1/nElem](x[0] * phi_2'*phi_2') 
	//- integral[0:1/nElem] f
	for (i = 2; i <= nElem; i++) // this loop is for v and number of equations
	{
		for (j = 1; j <= nElem + 1; j++) // this is for u (all nodes) and for each equation
		{
			if (i == 2 && j == 1)// special case on left boundary
			{
				res[i - 2] = res[i - 2] + u0 / h; 
				temp1 = integral_phidp(j, i, h);
				temp2 = integral_phipp(j, i, h);
				res[i - 2] = res[i - 2] + k1 * temp1* u0 + k2 * temp2* u0;
			}
			else if (i == nElem && (j == nElem + 1)) // special case on right boundary
			{
				res[i - 2] = res[i - 2] + u1 / h;
				temp1 = integral_phidp(j, i, h);
				temp2 = integral_phipp(j, i, h);
				res[i - 2] = res[i - 2] + k1 * temp1* u1 + k2 * temp2* u1;
			}
			else if (i == j)
			{
				res[i - 2] = res[i - 2] - 1.0 / h * x[j - 2] * 2; // will appear twice
				temp1 = integral_phidp(j, i, h);
				temp2 = integral_phipp(j, i, h);
				res[i - 2] = res[i - 2] + k1 * temp1* x[j - 2] + k2 * temp2* x[j - 2];
			}
			else if (abs(i - j) > 1)
			{
				// do nothing
			}
			else
			{
				res[i - 2] = res[i - 2] + 1.0 / h * x[j - 2];
				temp1 = integral_phidp(j, i, h);
				temp2 = integral_phipp(j, i, h);
				res[i - 2] = res[i - 2] + k1 * temp1* x[j - 2] + k2 * temp2* x[j - 2];
			}
		}

		res[i - 2] = res[i - 2] +  h * 1 * k3;
	}
	return;
}

void UserInfoHandler(void *s, int type)
{

}

int UserErrorHandler(void *s, int type)
{
	KINSOL_HELPERS::error* erro = (KINSOL_HELPERS::error*)s;

	for (std::vector<std::string>::iterator it = erro->ErrLines.begin(); it != erro->ErrLines.end(); it++)
	{
		std::cout << *it << "\n";
	}
	return(0);
}

void JacFunc(double *x, double **J)
{
//	int nElem;
	int i, j, nEq;
	double u0, u1, h, temp1, temp2;
	//const int N = 2;

	u0 = 0; u1 = 0;
	// solve u" = 1 u(0) = u(1) = 0
	//nElem = 3; //one initial one intermediate and one final
			   // if both conditions are DBC
	nEq = (nElem - 1);
	h = 1.0 / nElem;

	adouble ax[N];
	adouble ay[N];
	double oy[N];
	trace_on(1);
	for (int i = 0; i < N; i++)
	{
		ax[i] <<= x[i];
	}
	//***********************************************************


	// res[0] = -integral[0:1/nElem](u0 * phi_1'*phi_2')-integral[0:1/nElem](x[0] * phi_2'*phi_2') 
	//- integral[0:1/nElem] f
	for (i = 2; i <= nElem; i++) // this loop is for v and number of equations
	{
		for (j = 1; j <= nElem + 1; j++) // this is for u and for each equation
		{
			if (i == 2 && j == 1)
			{
				ay[i - 2] = ay[i - 2] + u0 / h;
				temp1 = integral_phidp(j, i, h);
				temp2 = integral_phipp(j, i, h);
				ay[i - 2] = ay[i - 2] + k1 * temp1 * u0 + k2 * temp2 * u0;
			}
			else if (i == (nElem) && (j == nElem + 1)) 
			{
				ay[i - 2] = ay[i - 2] + u1 / h;
				temp1 = integral_phidp(j, i, h);
				temp2 = integral_phipp(j, i, h);
				ay[i - 2] = ay[i - 2] + k1 * temp1 * u1 + k2 * temp2 * u1;
			}
			else if (i == j)
			{
				ay[i - 2] = ay[i - 2] - 1.0 / h * ax[j - 2] * 2; // will appear twice
				temp1 = integral_phidp(j, i, h);
				temp2 = integral_phipp(j, i, h);
				ay[i - 2] = ay[i - 2] + k1 * temp1 * ax[j - 2] + k2 * temp2 * ax[j - 2];
			}
			else if (abs(i - j) > 1)
			{
				// do nothing
			}
			else
			{
				ay[i - 2] = ay[i - 2] + 1 / h * ax[j - 2];
				temp1 = integral_phidp(j, i, h);
				temp2 = integral_phipp(j, i, h);
				ay[i - 2] = ay[i - 2] + k1 * temp1 * ax[j - 2] + k2 * temp2 * ax[j - 2];
			}
		}
		ay[i - 2] = ay[i - 2] + h * 1 * k3;
	}
	//***********************************************************

	for (int i = 0; i < N; i++)
		ay[i] >>= oy[i];
	trace_off();
	jacobian(1, nEq, nEq, x, J);

}

double integral_phidp(int i, int j, double h)
{
	// here j is equation
	// this intgral is supposed to take care of entire domain
	double triangle_area;

	triangle_area = 0.5 * h * 1;

	if ((i - j) == 1)
	{
		return 1.0 / h * triangle_area;
	}
	else if ((i - j) == -1)
	{
		return -1.0 / h * triangle_area;
	}
	else if (i == j)
	{
		return 0;
	}
	else
	{
		return 0;
	}
}

double integral_phipp(int i, int j, double h)
{
	//midpoint rule to simplify our lives
	// takes care of entire domain

	if (abs(i - j) == 1)
	{
		return 0.5 * 0.5 * h;
	}
	else if (i == j)
	{
		return 0.5 * h;
	}
	else
	{
		return 0;
	}
}