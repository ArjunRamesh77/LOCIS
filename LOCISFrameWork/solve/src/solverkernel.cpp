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

int solverKernel::getSystemTypeFromString(std::__cxx11::string systemType)
{
    if(systemType == "SOLVER_ALG_NONLINEAR")
        return SOLVER_ALG_NONLINEAR;

    if(systemType == "SOLVER_DAE_NONLINEAR")
        return SOLVER_DAE_NONLINEAR;

    return -1;
}

solver *solverKernel::getSolverFromString(std::__cxx11::string solverName)
{
    if(solverName == "SOLVER_KINSOL")
        return new solverSundialsKinsol;

    if(solverName == "SOLVER_IDA")
        return new solverSundialsIda;

    return NULL;
}

solverOptions *solverKernel::getOptionsForSolverFromString(std::__cxx11::string solverName)
{
    if(solverName == "SOLVER_KINSOL")
        return new solverOptionsKinsol;

    if(solverName == "SOLVER_IDA")
        return new solverOptionsIda;

    return NULL;
}

solverOutput *solverKernel::getOutputForSolverFromString(std::__cxx11::string solverName)
{
    if(solverName == "SOLVER_KINSOL")
        return new solverOutputKinsol;

    if(solverName == "SOLVER_IDA")
        return new solverOutputIda;

    return NULL;
}

int solverKernel::getSolutionMethodFromString(std::__cxx11::string solutionMethod)
{
    if(solutionMethod == "DIRECT")
        return SOLVER_MODE_DIRECT;

    if(solutionMethod == "BLOCK-DECOMP")
        return SOLVER_MODE_BLOCK_DECOMPOSITION;

    return -1;
}

void solverKernel::setFlatEquationSet(std::vector<virtualOper> *flatEquSet_arg)
{
    equationSet = flatEquSet_arg;
    mainSystem.setEquationVector(equationSet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set the system
void solverKernel::setMainSystem(const char *systemDesc)
{
    //parse the system
    rjdoc.Parse(systemDesc);

    //Read all key-value and store in main system
    std::string systemType(rjdoc["system-type"].GetString());
    unsigned int numVars = rjdoc["num-vars"].GetInt();
    unsigned int numEqus = rjdoc["num-equs"].GetInt();
    std::string solverName(rjdoc["solver-name"].GetString());
    std::string solutionMethod(rjdoc["solution-method"].GetString());

    //id dae get time range
    if(getSystemTypeFromString(systemType) == SOLVER_DAE_NONLINEAR)
    {
        double timeStart = rjdoc["time-start"].GetDouble();
        double timeEnd = rjdoc["time-end"].GetDouble();
        unsigned int timeStep =rjdoc["time-step"].GetInt();
        mainSystem.setTimeRange(timeStart, timeEnd, timeStep);
    }

    //setup the system type and variables
    mainSystem.setSystemType(getSystemTypeFromString(systemType));

    //set the solution method
    mainSystem.setSolveMode(getSolutionMethodFromString(solutionMethod));

    //set problem dimensions
    mainSystem.setAndAllocateSystemDims(numVars, numEqus);

    //setup the solver
    solver* solv = getSolverFromString(solverName);

    //setup the solver options
    solverOptions* ops = getOptionsForSolverFromString(solverName);
    solverOutput* out = getOutputForSolverFromString(solverName);

    //extract options for the solver
    rapidjson::Value rjSolverOptionsObj;
    std::string solverOptionsString = "options-" + solverName;
    rjSolverOptionsObj = rjdoc[solverOptionsString.c_str()].GetObject();

    //Get the input options
    rapidjson::Value* rjSolverOptionsInputObj = new rapidjson::Value;
    *rjSolverOptionsInputObj = rjSolverOptionsObj["input"].GetObject();
    ops->rjw.setJSONobjRoot(rjSolverOptionsInputObj); //delete the object rjw
    ops->rjw.setJSONdoc(&rjdoc);

    //Set the output options
    rapidjson::Value* rjSolverOptionsOutputObj = new rapidjson::Value;
    *rjSolverOptionsOutputObj = rjSolverOptionsObj["output"].GetObject();
    out->rjw.setJSONobjRoot(rjSolverOptionsOutputObj); //delete the object rjw
    out->rjw.setJSONdoc(&rjdoc);

    //Set any init options
    if(rjdoc.HasMember("init-dae") && mainSystem.getSystemType() == SOLVER_DAE_NONLINEAR)
    {
        rapidjson::Value initRjInitObj;
        initRjInitObj = rjdoc["init-dae"].GetObject();

        solverSystem* initSolverSystem = new solverSystem;
        mainSystem.setDaeInitSystem(initSolverSystem);

        //get basic info
        std::string initSystemType(initRjInitObj["system-type"].GetString());
        unsigned int initNumVars = initRjInitObj["num-vars"].GetInt();
        unsigned int initNumEqus = initRjInitObj["num-equs"].GetInt();
        std::string initSolverName(initRjInitObj["solver-name"].GetString());
        std::string initSolutionMethod(initRjInitObj["solution-method"].GetString());

        //setup the system type and variables
        initSolverSystem->setSystemType(getSystemTypeFromString(initSystemType));

        //set the solution method
        initSolverSystem->setSolveMode(getSolutionMethodFromString(initSolutionMethod));

        //set problem dimensions
        initSolverSystem->setAndAllocateSystemDims(initNumVars, initNumEqus);

        //setup the solver
        solver* initSolv = getSolverFromString(initSolverName);

        //setup the solver options
        solverOptions* initOps = getOptionsForSolverFromString(initSolverName);
        solverOutput* initOut = getOutputForSolverFromString(initSolverName);

        //extract options for the solver
        rapidjson::Value initRjSolverOptionsObj;
        std::string initSolverOptionsString = "options-" + initSolverName;
        initRjSolverOptionsObj = initRjInitObj[initSolverOptionsString.c_str()].GetObject();

        //Get the input options
        rapidjson::Value* initRjSolverOptionsInputObj = new rapidjson::Value;
        *initRjSolverOptionsInputObj = initRjSolverOptionsObj["input"].GetObject();
        initOps->rjw.setJSONobjRoot(initRjSolverOptionsInputObj); //delete the object rjw
        ops->rjw.setJSONdoc(&rjdoc);

        //Set the output options
        rapidjson::Value* initRjSolverOptionsOutputObj = new rapidjson::Value;
        *initRjSolverOptionsOutputObj = initRjSolverOptionsObj["output"].GetObject();
        out->rjw.setJSONobjRoot(initRjSolverOptionsOutputObj); //delete the object rjw
        out->rjw.setJSONdoc(&rjdoc);

        // setup the dae init solver
        initSolverSystem->setSolver(initSolv, initOps, initOut);
    }

    // setup the solver
    mainSystem.setSolver(solv, ops, out);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// initialize the system
bool solverKernel::initSystem()
{
    bool ret = true;
    int solveMode = 0;

    //initialize the system based on direct or block decomposistion
    solveMode = mainSystem.getSolveMode();
    if(solveMode == SOLVER_MODE_DIRECT)
    {
        if(solutionMethodPtr)
        {
            delete solutionMethodPtr;
            solutionMethodPtr = NULL;
        }
        solutionMethodPtr = new solverDirect;
    }

    /*
    if(systemType == SOLVER_MODE_BLOCK_DECOMPOSITION)
    {
        if(solutionMethodPtr)
        {
            delete solutionMethodPtr;
            solutionMethodPtr = NULL;
            solutionMethodPtr = new solverBlockDecomposition;
        }
    }
    */

    //initialize the system if its a dae
    if(mainSystem.getSystemType() == SOLVER_DAE_NONLINEAR)
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
