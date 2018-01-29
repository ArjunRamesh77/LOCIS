#pragma once

#include "solver.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// base class for all non-linear dae solvers
class solverNonLinearDae : public solver
{


public:
    solverNonLinearDae(int type);
    ~solverNonLinearDae();

    virtual void setGuessYyandYp(double* y, double* yp);
    virtual int solve(double* ySol, double* ypSol, double time);
};
