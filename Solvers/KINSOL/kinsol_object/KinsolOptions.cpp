#include "Kinsol_Options.h"

KINSOL_HELPERS::KinsolOptions::KinsolOptions()
{
	LinearSolver = "UNDEFINED";

	GlobalStrategy = -1;

	FuncNormTolerance = -1;

	ScaledStepTol = -1;

	MaxSetupCalls = -1;

	MaxNLIters = -1;

	MaxSubSetupCalls = -1;

	EtaLSForm = -1;

	EtaLSConstVal = -1;

	EtaParams[0] = -1;
	EtaParams[1] = -1;

	ResMonConstVal = -1;

	ResMonParams[0] = -1;
	ResMonParams[1] = -1;

	NoMinEps = false;

	MaxNewtonStep = -1;

	MaxBetaFails = -1;

	RelErrFunc = -1;

	if_jacobian = 1;

	mupper = -1;
	mlower = -1;

	NNZ = -1;

	finalstatsPrintLevel = -1;
}

KINSOL_HELPERS::KinsolOptions::~KinsolOptions()
{

}