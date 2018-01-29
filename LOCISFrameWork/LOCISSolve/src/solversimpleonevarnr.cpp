#pragma once

#include "solversimpleonevarnr.h"

solverSimpleOneVarNR::solverSimpleOneVarNR()
{

}

solverSimpleOneVarNR::~solverSimpleOneVarNR()
{

}

void solverSimpleOneVarNR::setGuess(double *xGuess_arg)
{
    xGuess = *xGuess_arg;
}

void solverSimpleOneVarNR::setDampingFactor(double damp_arg)
{
    damp = damp_arg;
}

void solverSimpleOneVarNR::setBuseFNorm(bool val)
{
    bUseFnorm = val;
}

void solverSimpleOneVarNR::setBuseAbsTol(bool val)
{
    bUseAbsTol = val;
}

void solverSimpleOneVarNR::setMaxNumTrys(int maxNumTrys_arg)
{
    maxNumTrys = maxNumTrys_arg;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// one variable damped newton raphson solver
int solverSimpleOneVarNR::solve(double *xSol)
{
    double localSol = xGuess;
    double localSolprev = 0;
    double feval = 0;
    double fevalPrev = 0;
    double dfeval = 0;
    double localDamp = damp;
    double val1 = 0;
    double val2 = 0;
    double norm = 0;
    bool solFound = false;
    int numTrys = 0;

    //check sanity
    if(maxIter <= 0)
    {
        return -1;
    }

    if(maxNumTrys < 0)
    {
        return -1;
    }

    //main loop
start:
    for(int i = 0; i < maxIter; ++i)
    {
        fevalPrev = feval;
        residual->evalResidualOneVarStackBased(feval, localSol);
        jacobian->evalOneVarDerivativeStackBased(dfeval, localSol);

        if(dfeval == 0)
        {
            break;
        }

        //newton update
        localSolprev = localSol;
        localSol = localSol - localDamp*(feval/dfeval);

        //if fnorm or xnorm
        if(bUseFnorm)
        {
            val1 = fevalPrev;
            val2 = feval;
        }
        else
        {
            val1 = localSolprev;
            val2 = localSol;
        }

        //check convergence
        if(bUseAbsTol)
        {
            norm = fabs(val1 - val2);
            if(norm <= absTol)
            {
                solFound = true;
                break;
            }
        }
        else
        {
            norm = fabs((val1 - val2)/val1);
            if(norm <= relTol)
            {
                solFound = true;
            }
        }
    }

    //reduce the damping and try again
    if(numTrys <= maxNumTrys && !solFound)
    {
        localDamp = 0.9*localDamp;
        numTrys++;
        goto start;
    }

    if(!solFound)
    {
        return -2;
    }

    return 0;
}
