#include "solverkernel.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solverKernel
void solverKernel::setNumInstrInit(unsigned int value)
{
    numInstrInit = value;
}

solverKernel::solverKernel() :
    solutionMethodPtr(NULL)
{

}

solverKernel::~solverKernel()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set the system
void solverKernel::setMainSystem(const char *systemDesc)
{
    //parse the system
    rjdoc.Parse(systemDesc);

    rapidjson::Value root = rjdoc["root"].GetObject();
    readSystem(root);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// allocates memory for the main system
void solverKernel::allocateGlobalSystemVars()
{
    mainSystem.allocateSystemDims();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set Flattened equation set
void solverKernel::setFlatEquationSet(std::vector<virtualOper> *flatEquSet_arg)
{
    equationSet = flatEquSet_arg;
    mainSystem.setEquationVector(equationSet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// initialize the system
bool solverKernel::initSystem()
{
    bool ret = true;
    int solveMode = 0;
    int systemType = 0;

    //initialize the system based on direct or block decomposistion
    solveMode = mainSystem.getSolveMode();
    systemType = mainSystem.getSystemType();
    if(solveMode == SOLVER_MODE_DIRECT)
    {
        if(solutionMethodPtr)
        {
            delete solutionMethodPtr;
            solutionMethodPtr = NULL;
        }
        solutionMethodPtr = new solverDirect;
    }
    else if(solveMode == SOLVER_MODE_BLOCK_DECOMPOSITION)
    {
        if(solutionMethodPtr)
        {
            delete solutionMethodPtr;
            solutionMethodPtr = NULL;
        }
        solutionMethodPtr = new solverBlockDecomposition;

        if(systemType == SOLVER_ALG_NONLINEAR)
        {
            static_cast<solverBlockDecomposition*>(solutionMethodPtr)->setIsInitializer(true);
        }
    }

    //initialize the system if its a dae
    if(systemType == SOLVER_DAE_NONLINEAR)
    {
        if(!solveDaeInitialization())
        {
            return false;
        }
    }

    //Pass the solver system to the specific mode solver and initialize
    solutionMethodPtr->setSolverSystem(&mainSystem);
    if(!solutionMethodPtr->initSystem())
    {
        ret = false;
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solve the system
bool solverKernel::solveSystem()
{
    bool ret = true;
    int systemType = mainSystem.getSystemType();

    if(systemType == SOLVER_ALG_NONLINEAR)
    {
        if(!solutionMethodPtr->solve())
        {
            ret = false;
        }
    }

    if(systemType == SOLVER_DAE_NONLINEAR)
    {
        //get all time parameters
        double tStart = mainSystem.getTStart();
        double tEnd = mainSystem.getTEnd();
        double numSteps = mainSystem.getNumSteps();

        //compute the time step
        double tCurrent = tStart;
        double tStep = (tEnd - tStart)/(numSteps + 1);
        tCurrent = tCurrent + tStep;

        //loop over all time and solve
        for(unsigned int i = 0; i < numSteps; ++i)
        {
            if(!solutionMethodPtr->solve(tCurrent))
            {
                ret = false;
                break;
            }
            tCurrent += tStep;

            //debug
            std::cout<<"DAE SOLUTION @ time = "<< tCurrent - tStep <<std::endl;
            double* yySol = mainSystem.getVarYY();
            for(unsigned int i = 0; i < mainSystem.getNumVar(); ++i)
            {
                std::cout<<yySol[i]<<std::endl;
            }

        }
    }

    return ret;
}

solverSystem *solverKernel::getSystem()
{
    return &mainSystem;
}
