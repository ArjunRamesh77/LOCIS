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
    bool bGotRoot;
    unsigned int numRoots;
    genericResidual* rootResidual;

public:
    solverNonLinearDae(int type, std::string name);
    ~solverNonLinearDae();

    void setGuessYyandYp(double* yy, double* yp);
    void setYyOrigAndYpOrig(double* yyOrig_arg, double* ypOrig_arg);
    void setRootResidual(genericResidual* rootRes, unsigned int numRoots);
    int evalRootResidual(double* yy, double* yp, double t, double* root);
    bool checkFoundRoot();
    bool buildResidualAndJacobian(std::vector<virtualOper>* pEquationVec);

    //interface
    virtual bool solve(double time) = 0;
};
