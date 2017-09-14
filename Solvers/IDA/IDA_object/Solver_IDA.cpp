#include "IDAclass.h"

solver_IDA::solver_IDA()
{

	ESCAPE = false;
};

solver_IDA::~solver_IDA()
{
	if (problem_data.initVal_y != NULL)
	{
		delete problem_data.initVal_y;
	}


	if (problem_data.initVal_yp != NULL)
	{
		delete problem_data.initVal_yp;
	}

	if (IDAmem != NULL)
		IDAFree(&IDAmem);

	if (InfoFuncdata.rootsfound)
		delete InfoFuncdata.rootsfound;

	if (problem_data.yp != NULL)
	{
		N_VDestroy_Serial(problem_data.yp);
	}

	if (problem_data.yy != NULL)
	{
		N_VDestroy_Serial(problem_data.yy);
	}

}