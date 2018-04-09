#include "solverkernel.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solves the DAE initialization problem
bool solverKernel::solveDaeInitialization()
{
    //initialize the dae with consistent initial guesses
    bool ret = true;
    solverSystem* daeInitSystem = mainSystem.getDaeInitSystem();

    //get the dae init instructions
    std::vector<initializerDaeGenRecognize> varMap;
    initializerDaeGen daeGen;
    daeGen.convertToDaeInitializerResidual(mainSystem.getPEquationVec(), varMap);

    //count total number of variables
    unsigned int numAlgVars = daeGen.getNumAlgVars();
    unsigned int numDiffVars = daeGen.getNumDiffVars();
    unsigned int totalVars = numAlgVars + numDiffVars;

    //set the converted system of equations
    daeInitSystem->setSystemType(SOLVER_ALG_NONLINEAR);
    daeInitSystem->setEquationVector(mainSystem.getPEquationVec());

    //set guess values from original system
    unsigned int count = 0;
    double* xGuessVal = daeInitSystem->getVarX();
    double* yyGuessVal = mainSystem.getVarYY();
    double* ypGuessVal = mainSystem.getVarYP();
    for(auto it = varMap.begin(); it != varMap.end(); ++it)
    {
        if(it->type == VR_VAR1_INDEX)
        {
            xGuessVal[count] = yyGuessVal[it->index];
        }
        if(it->type == VR_VAR2_INDEX)
        {
            xGuessVal[count] = ypGuessVal[it->index];
        }
        ++count;
    }

    //debug
    std::cout<<" GUESS VAL"<<std::endl;
    for(unsigned int i = 0; i < count; ++i)
    {
        std::cout<<xGuessVal[i]<<std::endl;
    }

    //get and initialize the solver mode for dae initialization
    solverSolutionMethod* daeInitSolutionMethodPtr;
    int daeSolveMode = daeInitSystem->getSolveMode();
    if(daeSolveMode == SOLVER_MODE_DIRECT)
    {
        daeInitSolutionMethodPtr = new solverDirect;
    }

    /*
    if(daeSolveMode == SOLVER_MODE_BLOCK_DECOMPOSITION)
    {
        daeInitSolutionMethodPtr = new solverBlockDecomposition;
    }
    */

    daeInitSolutionMethodPtr->setSolverSystem(daeInitSystem);
    if(!daeInitSolutionMethodPtr->initSystem())
    {
        delete daeInitSolutionMethodPtr;
        ret = false;
    }

    //solve the system
    if(!daeInitSolutionMethodPtr->solve())
    {
        ret = false;
    }

    //map back solution
    count = 0;
    for(auto it = varMap.begin(); it != varMap.end(); ++it)
    {
        if(it->type == VR_VAR1_INDEX)
        {
            yyGuessVal[it->index] = xGuessVal[count];
        }
        if(it->type == VR_VAR2_INDEX)
        {
            ypGuessVal[it->index] = xGuessVal[count];
        }
        ++count;
    }

    //debug
    std::cout<<std::endl<<"SOLUTION"<<std::endl;
    for(unsigned int i = 0; i < count; ++i)
    {
        std::cout<<xGuessVal[i]<<std::endl;
    }

    if(!ret)
    {
        return ret;
    }

    //convert the residual back to original
    daeGen.revertToDaeResidual(*(mainSystem.getPEquationVec()), varMap);

    //Remove init instructions
    for(unsigned int i = 0; i < numInstrInit; ++i)
        mainSystem.getPEquationVec()->pop_back();

    return ret;
}
