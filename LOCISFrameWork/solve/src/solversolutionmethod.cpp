#include "solversolutionmethod.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solversolutionmethod
solverSolutionMethod::solverSolutionMethod() :
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
