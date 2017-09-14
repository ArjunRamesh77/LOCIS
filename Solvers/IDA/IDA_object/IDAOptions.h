#pragma once

#include<iostream>

#define NaN -1.0e35

namespace IDA_HELPERS
{
	struct IDAOptions
	{

		double *abstol, *timepointvector;
		double* suppressid;

		double
			reltol,
			tfinal,
			t0,
			initstep,
			maxstep,
			stoptime,
			timepoint,
			abstolscalar
			;

		int
			nout,
			mupper, // used only if linear solver is IDADENSE
			mlower,  // used only if linear solver is IDADENSE
			NNZ, // used only if linear solver is IDAKLU
			maxord,
			maxnumsteps,
			maxerrtestfails,
			nonlineariters,
			maxconvfails,
			nonlinearconvcoeff,
			finalstatsprintlevel,
			NrootFn,
			solvetype,
			timevectorlength,
			setidlength,
			tolerancetype, // 1 is scalar and 2 is vector 
			tolerancesize // only when tolerancetype is 2
			;

		int *setidptr;

		std::string
			LinearSolver;

		bool
			suppressalg,
			callrootfn,
			callCalcIC,
			if_jacobian,
			callSetID
			;

		IDAOptions();

		~IDAOptions();
	};
}
