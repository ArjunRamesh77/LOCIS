#include"IDAOptions.h"

IDA_HELPERS::IDAOptions::IDAOptions()
{

	NrootFn = -1;
	abstol = NULL;
	reltol = -1;
	nout = -1;//**
	t0 = NaN; // **
	tfinal = NaN;// **
	LinearSolver = "UNDEFINED";
	mupper = -1;
	mlower = -1;
	NNZ = -1;
	maxord = -1;
	maxnumsteps = -1;
	initstep = -1;
	maxstep = -1;
	stoptime = NaN;
	maxerrtestfails = -1;
	nonlineariters = -1;
	maxconvfails = -1;
	nonlinearconvcoeff = -1;
	suppressalg = false;
	suppressid = NULL;
	finalstatsprintlevel = 1;
	timepointvector = NULL;
	timepoint = NaN;
	callrootfn = false;
	solvetype = -1;
	timevectorlength = -1;
	callCalcIC = false;
	setidptr = NULL;
	if_jacobian = false;
	setidlength = -1;
	callSetID = false;
	tolerancetype = -1;
	tolerancesize = -1;
	abstolscalar = -1;
}

IDA_HELPERS::IDAOptions::~IDAOptions()
{

	delete abstol;
}