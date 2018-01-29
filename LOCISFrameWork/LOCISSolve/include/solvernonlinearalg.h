#pragma once

#include "solver.h"
#include "genericresidual.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// base class for all non-linear algebraic solver
class solverNonLinearAlg : public solver
{
protected:


public:
    solverNonLinearAlg(int type);
    ~solverNonLinearAlg();

    virtual void setGuess(double* xGuess_arg) = 0;
    virtual int solve(double* xSol) = 0;

};
