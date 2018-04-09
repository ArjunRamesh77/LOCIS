/*

#pragma once

#include "solvernonlinearalg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// one variable newton raphson solver
class solverSimpleOneVarNR : public solverNonLinearAlg
{
    double xGuess;
    double damp;
    int maxNumTrys;
    bool bUseFnorm;
    bool bUseAbsTol;

public:
    solverSimpleOneVarNR();
    ~solverSimpleOneVarNR();

    void setXGuess(double* xGuess_arg);
    void setDampingFactor(double damp);
    void setBuseFNorm(bool val);
    void setBuseAbsTol(bool val);
    void setMaxNumTrys(int numTrys_arg);
    int solve(double* xSol);
};

*/
