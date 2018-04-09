#pragma once

#include "solversolutionmethod.h"
#include "solversystem.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// supervisor for directly solving a system of equations
class solverDirect : public solverSolutionMethod
{

public:
    solverDirect();
    ~solverDirect();

    bool initSystem();
    bool solve(double time = 0.0);
    solverOutput* getSolverOutput();
};
