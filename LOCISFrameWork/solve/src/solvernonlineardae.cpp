#include "solvernonlineardae.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solverNonLinearDae
solverNonLinearDae::solverNonLinearDae(int type, std::string name) :
    solver(type, name),
    yyGuess(NULL),
    ypGuess(NULL),
    yyOrig(NULL),
    ypOrig(NULL),
    numRoots(0),
    rootResidual(NULL)
{

}

solverNonLinearDae::~solverNonLinearDae()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set the generic root residual, will be used for event tracking
void solverNonLinearDae::setRootResidual(genericResidual *rootRes, unsigned int numRoots_arg)
{
    rootResidual = rootRes;
    numRoots = numRoots_arg;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// evaluate the root residual
int solverNonLinearDae::evalRootResidual(double *yy, double *yp, double *root)
{
    rootResidual->evalResidual2StackBased(yyOrig, yy, ypOrig, yp, root);
}

void solverNonLinearDae::setGuessYyandYp(double *yy, double *yp)
{
    yyGuess = yy;
    ypGuess = yp;
}


