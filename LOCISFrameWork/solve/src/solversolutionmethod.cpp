#include "solversolutionmethod.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solversolutionmethod
solverSolutionMethod::solverSolutionMethod() :
    bInitDone(false),
    system(NULL)
{

}

solverSolutionMethod::~solverSolutionMethod()
{

}

void solverSolutionMethod::setSolverSystem(solverSystem *sys)
{
    system = sys;
}
