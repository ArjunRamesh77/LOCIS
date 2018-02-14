#include "solver.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// options and output
solverOptions::solverOptions() :
    absXTol(0.0),
    relXTol(0.0),
    absFTol(0.0),
    relFTol(0.0),
    maxIter(1),
    traceLevel(0)
{

}

solverOutput::solverOutput() :
    workSpaceSize(0),
    numFevals(0),
    numJacevals(0),
    numNonLinIter(0),
    funcNorm(0.0),
    xNorm(0.0)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solver
solver::solver(int solverType_arg, std::string solverName_arg) :
    solverType(solverType_arg),
    solverName(solverName_arg),
    numVar(0),
    numEqu(0),
    residual(NULL),
    jacobian(NULL),
    option(NULL)
{

}

solver::~solver()
{

}

void solver::setResiduaAndJacobian(genericResidual *residual_arg, genericJacobian *jacobian_arg)
{
    residual = residual_arg;
    jacobian = jacobian_arg;
}

int solver::getSolverType()
{
    return solverType;
}

std::string solver::getSolverName()
{
    return solverName;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set the data from the user
bool solver::setSolverParameters(solverOptions *ops)
{
    if(ops->absXTol < 0.0)
        return false;

    if(ops->relXTol < 0.0)
        return false;

    if(ops->absFTol < 0.0)
        return false;

    if(ops->relFTol < 0.0)
        return false;

    if(ops->maxIter <= 0)
        return false;

    if(ops->traceLevel < 0)
        return false;

    return true;
}
