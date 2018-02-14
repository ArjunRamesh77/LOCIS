#pragma once

#include "solver.h"
#include "genericresidual.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// base class for all non-linear algebraic solver
class solverNonLinearAlg : public solver
{
protected:
    double* xGuess;
    double* xOrig;

public:
    solverNonLinearAlg(int type, std::string name);
    ~solverNonLinearAlg();

    void setGuess(double* xGuess_arg);
    void setXOrig(double xOrig_arg);

    //interface
    virtual int solve() = 0;
};
