#pragma once
#include <string>

namespace IDA_HELPERS
{
	struct errormsgstruct
	{
		int code;
		std::string module;
		std::string function;
		std::string msg;
	};

	struct finalstatstructlevel1
	{
		int NEQ, nst, nre, nje, nni, netf, ncfn, nreLS, nge;
		double *Init_cond_y, *Init_cond_yp;
	};

	struct finalstatstructlevel2
	{
		int	NEQ, nst, nre, nje, nni, netf, ncfn, nreLS, nge,
			lenrw, leniw, nlinsetups, netfails, klast, kcur;
		double	hcur, hlast, hinused, tcur, tolsfac;
		double fnorm, steplength;

	};

	struct IDAInfoFuncData
	{
		int NEQ;
		int LastOrder;
		long int Numsteps;
		double LastStep, t;
		double* yval;
		double* yp_val;
		bool rootReturn;
		int *rootsfound;
		int numroots;

	};

	typedef struct IDAInfoFuncData(infomsgs);
	typedef struct errormsgstruct(errormsgs);
	typedef struct finalstatstructlevel1(finalstatsl1);
	typedef struct finalstatstructlevel2(finalstatsl2);
}
