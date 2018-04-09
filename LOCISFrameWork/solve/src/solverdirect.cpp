#include "solverdirect.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solverDirect
solverDirect::solverDirect() :
    solverSolutionMethod()
{

}

solverDirect::~solverDirect()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// initialize based on solver chosen
bool solverDirect::initSystem()
{
    if(!system)
        return false;

    //get the solver
    solver* solv = system->getSolver();

    //set the instructions
    solv->setPEquationVec(system->getPEquationVec());

    //set the system dimensions
    solv->setSolveDimensions(system->getNumEqu(), system->getNumVar());

    //setup variable based on type of system
    solverNonLinearAlg* solvNonLinAlg = NULL;
    solverNonLinearDae* solvNonLinDae = NULL;
    switch (system->getSystemType())
    {
    case SOLVER_ALG_NONLINEAR:
        solvNonLinAlg = static_cast<solverNonLinearAlg*>(solv);
        solvNonLinAlg->setXGuess(system->getVarX());
        solvNonLinAlg->setXOrig(system->getVarX());
        break;

    case SOLVER_DAE_NONLINEAR:
        solvNonLinDae = static_cast<solverNonLinearDae*>(solv);
        solvNonLinDae->setGuessYyandYp(system->getVarYY(), system->getVarYP());
        solvNonLinDae->setYyOrigAndYpOrig(system->getVarYY(), system->getVarYP());
        break;
    }

    //set the solver parameters
    if(!solv->init(system->getSolverOptions()))
        return false;

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solve the system!! (Returns solver specific return code)
bool solverDirect::solve(double time)
{
     solverNonLinearAlg* sol1 = NULL;
     solverNonLinearDae* sol2 = NULL;
     switch(system->getSystemType())
     {
     case SOLVER_ALG_NONLINEAR:
         sol1 = static_cast<solverNonLinearAlg*>(system->getSolver());
         return sol1->solve();
         break;

     case SOLVER_DAE_NONLINEAR:
         sol2 = static_cast<solverNonLinearDae*>(system->getSolver());
         return sol2->solve(time);
         break;
     }

     return -1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get final output from solver
solverOutput *solverDirect::getSolverOutput()
{
    solverOutput* sout = system->getSolverOutput();
    if(!sout)
        return NULL;

    system->getSolver()->getSolverOutput(system->getSolverOutput());
    return system->getSolverOutput();
}
