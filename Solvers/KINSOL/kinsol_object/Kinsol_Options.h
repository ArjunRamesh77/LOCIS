#ifndef KINSOL_OPTIONS_H
#define KINSOL_OPTIONS_H

#include<string>

namespace KINSOL_HELPERS
{

	struct KinsolOptions
	{

		double
			FuncNormTolerance,
			EtaParams[2],
			EtaLSConstVal,
			ResMonConstVal,
			ResMonParams[2],
			MaxNewtonStep,
			ScaledStepTol;

		std::string
			LinearSolver;

		bool
			NoMinEps;


		int
			MaxSetupCalls,
			GlobalStrategy,
			MaxNLIters,
			MaxSubSetupCalls,
			EtaLSForm,
			MaxBetaFails,
			RelErrFunc,
			if_jacobian,
			mupper, // used only if linear solver is KINDENSE
			mlower,  // used only if linear solver is KINDENSE
			NNZ, // used only if linear solver is KINKLU
			finalstatsPrintLevel
			;

		KinsolOptions();

		~KinsolOptions();

	};
}
#endif