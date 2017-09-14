#pragma once
#include <string>

namespace KINSOL_HELPERS
{

	struct infomsgstruct
	{
		std::string module;
		std::string function;
		std::string msg;
	};

	struct errormsgstruct
	{
		int code;
		std::string module;
		std::string function;
		std::string msg;
	};

	struct finalstatstructlevel1
	{
		int nni, nfe, nje, nfeD;
	};

	struct finalstatstructlevel2
	{
		long int nni, nfe, nje, nfeD, lenrw, leniw, nbcfails,
			nbacktr, lenrwB, leniwB, lsflag;
		double fnorm, steplength;

	};

	typedef struct infomsgstruct(infomsgs);
	typedef struct errormsgstruct(errormsgs);
	typedef struct finalstatstructlevel1(finalstatsl1);
	typedef struct finalstatstructlevel2(finalstatsl2);

}

