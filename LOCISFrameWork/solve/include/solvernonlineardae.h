#pragma once

#include "solver.h"
#include "genericresidual.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// base class for all non-linear dae solvers
class solverNonLinearDae : public solver
{
protected:
    double* yyGuess;
    double* ypGuess;
    double* yyOrig;
    double* ypOrig;
    unsigned int numRoots;
    genericResidual* rootResidual;

public:
    solverNonLinearDae(int type, std::string name);
    ~solverNonLinearDae();

    void setGuessYyandYp(double* yy, double* yp);
    void setYyOrigAndYpOrig(double* yyOrig, double* ypOrig);
    void setRootResidual(genericResidual* rootRes, unsigned int numRoots);
    int evalRootResidual(double* yy, double* yp, double* root);

    //interface
    virtual int solve(double time) = 0;
};
