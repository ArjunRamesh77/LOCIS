#include"IDAclass.h"

int solver_IDA::SetIntegrationTol()
{
	if (ESCAPE) return 0;
	N_Vector abstolNvector;
	double* abstolNvectorptr;
	int flag, returncode;


	if (Useroptionsptr->tolerancetype == 1)
	{
		flag = IDASStolerances(IDAmem, Useroptionsptr->reltol, Useroptionsptr->abstolscalar);
		returncode = CheckFlag(flag, "IDASVtolerances");
		if (returncode == 0) return 0;

	}
	else if (Useroptionsptr->tolerancetype == 2)
	{
		abstolNvector = N_VNew_Serial(problem_data.NEq);
		abstolNvectorptr = NULL;
		abstolNvectorptr = NV_DATA_S(abstolNvector);
		for (int i = 0; i <= problem_data.NEq - 1; i++)
		{
			abstolNvectorptr[i] = (*Useroptionsptr).abstol[i];
		}

		flag = IDASVtolerances(IDAmem, (*Useroptionsptr).reltol, abstolNvector);
		returncode = CheckFlag(flag, "IDASVtolerances");
		if (returncode == 0) return 0;

		N_VDestroy_Serial(abstolNvector);
	}
	else
	{

	}


	return 1;
}