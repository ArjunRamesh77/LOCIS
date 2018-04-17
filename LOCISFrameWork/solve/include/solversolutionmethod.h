#pragma once

#include "solversystem.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// abstract interface for the method used to solve a system of equations
class solverSolutionMethod
{
protected:
    solverSystem* system;

public:
    solverSolutionMethod();
    virtual ~solverSolutionMethod();

    void setSolverSystem(solverSystem* sys);
    virtual bool initSystem() = 0;
    virtual bool solve(double time = 0.0) = 0;
    virtual solverOutput* getSolverOutput() = 0;
};
