#include "solvernonlinearalg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solverNonLinearAlg
solverNonLinearAlg::solverNonLinearAlg(int type, std::string name) :
    solver(type, name),
    xGuess(NULL),
    xOrig(NULL)
{

}

solverNonLinearAlg::~solverNonLinearAlg()
{

}

void solverNonLinearAlg::setGuess(double *xGuess_arg)
{
    xGuess = xGuess_arg;
}

void solverNonLinearAlg::setXOrig(double xOrig_arg)
{
    xOrig = xOrig_arg;
}


