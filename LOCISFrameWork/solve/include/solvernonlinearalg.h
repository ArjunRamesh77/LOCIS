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

    void setXGuess(double* xGuess_arg);
    void setXOrig(double *xOrig_arg);
    bool buildResidualAndJacobian(std::vector<virtualOper>* pEquationVec);

    //interface
    virtual bool solve() = 0;
};
