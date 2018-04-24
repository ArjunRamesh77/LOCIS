#include "solversystem.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solver System
solverSystem::solverSystem() :
    varx(NULL),
    varyy(NULL),
    varyp(NULL)
{

}

solverSystem::~solverSystem()
{

}

void solverSystem::setSystemType(int val)
{
    systemType = val;
}

void solverSystem::setSolveMode(int val)
{
    solveMode = val;
}

void solverSystem::setSolverType(int val)
{
    solverType = val;
}

void solverSystem::setSolverName(int val)
{
    solverName = val;
}

void solverSystem::setSolverInputAndOutput(rapidjson::Value* in, rapidjson::Value* out)
{
    solveInput = in;
    solveOutput = out;
}

void solverSystem::setDaeInitSystem(solverSystem *daeInitSystem_arg)
{
    daeInitSystem = daeInitSystem_arg;
}

void solverSystem::setTimeRange(double tStart_arg, double tEnd_arg, unsigned int nSteps_arg)
{
    tStart = tStart_arg;
    tEnd = tEnd_arg;
    numSteps = nSteps_arg;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// allocate variables based on systemType
bool solverSystem::setSystemDims(unsigned int numVar_arg, unsigned int numEqu_arg)
{
    bool ret = true;
    numVar = numVar_arg;
    numEqu = numEqu_arg;

    return ret;
}

bool solverSystem::allocateSystemDims()
{
    if(numVar > 0 && numEqu > 0)
    {
        switch(systemType)
        {
        case SOLVER_ALG_NONLINEAR:
            if(!varx)
            {
                varx = new double[numVar];
            }           
            break;

        case SOLVER_DAE_NONLINEAR:
            if(!varyy)
            {
                varyy = new double[numVar];
            }

            if(!varyp)
            {
                varyp = new double[numVar];
            }

            break;
        }
        return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set the falttened system of equations
void solverSystem::setEquationVector(std::vector<virtualOper> *pEquationVec_arg)
{
    pEquationVec = pEquationVec_arg;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set the system solver
void solverSystem::setSolver(solver* solver_arg, solverOptions *sops_arg, solverOutput *sout_arg)
{
    solv = solver_arg;
    sops = sops_arg;
    sout = sout_arg;
}

void solverSystem::setRootSystem(solverSystem *val)
{
    root = val;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set the id for a block in block decomposition mode
void solverSystem::setBlockId(unsigned int blid)
{
    blockId = blid;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set the number of blocks to be solved in block decomp mode
void solverSystem::setNumBlocks(unsigned int blnum)
{
    numBlocks = blnum;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Add a block to the list
void solverSystem::addBlock(solverSystem *blsystem)
{
    solveBlocks.push_back(blsystem);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// this is used to map to the original variable set
void solverSystem::setBlockMapVarX(std::vector<unsigned int> *mapVarX_arg)
{
    mapVarX = mapVarX_arg;
}

void solverSystem::setBlockMapVarYY(std::vector<unsigned int> *mapVarYY_arg)
{
    mapVarYY = mapVarYY_arg;
}

void solverSystem::setBlockMapVarYP(std::vector<unsigned int> *mapVarYP_arg)
{
    mapVarYP = mapVarYP_arg;
}

void solverSystem::setBlockSolvers(int alg, int dae)
{
    blockAlgSolver = alg;
    blockDaeSolver = dae;
}

void solverSystem::setBlockAlgInputOutput(rapidjson::Value *in, rapidjson::Value *out)
{
    blockSolverInputAlg = in;
    blockSolverOutputAlg = out;
}

void solverSystem::setBlockDaeInputOutput(rapidjson::Value *in, rapidjson::Value *out)
{
    blockSolverInputDae = in;
    blockSolverOutputDae = out;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get the main root node
solverSystem *solverSystem::getRoot()
{
    return root;
}

solverSystem *solverSystem::getDaeInitSystem()
{
    return daeInitSystem;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return the system type
int solverSystem::getSystemType()
{
    return systemType;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return the solve mode
int solverSystem::getSolveMode()
{
    return solveMode;
}

int solverSystem::getSolverName()
{
    return solverName;
}

rapidjson::Value *solverSystem::getSolveInput()
{
    return solveInput;
}

rapidjson::Value *solverSystem::getSolveOutput()
{
    return solveOutput;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return system dimensions
unsigned int solverSystem::getNumVar()
{
    return numVar;
}

unsigned int solverSystem::getNumEqu()
{
    return numEqu;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return the flattened system of equations
std::vector<virtualOper> *solverSystem::getPEquationVec()
{
    return pEquationVec;
}

int solverSystem::getBlockSolverAlgName()
{
    return blockAlgSolver;
}

int solverSystem::getBlockSolverDaeName()
{
    return blockDaeSolver;
}

rapidjson::Value *solverSystem::getBlockSolveAlgInput()
{
    return blockSolverInputAlg;
}

rapidjson::Value *solverSystem::getBlockSolveAlgOutput()
{
    return blockSolverOutputAlg;
}

rapidjson::Value *solverSystem::getBlockSolveDaeInput()
{
    return blockSolverInputDae;
}

rapidjson::Value *solverSystem::getBlockSolveDaeOutput()
{
    return blockSolverOutputDae;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dae time
double solverSystem::getTStart()
{
    return tStart;
}

double solverSystem::getTEnd()
{
    return tEnd;
}

unsigned int solverSystem::getNumSteps()
{
    return numSteps;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return the system variables
double *solverSystem::getVarX()
{
    return varx;
}

double *solverSystem::getVarYY()
{
    return varyy;
}

double *solverSystem::getVarYP()
{
    return varyp;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return the solver to be used in the computation
solver *solverSystem::getSolver()
{
    return solv;
}

solverOptions *solverSystem::getSolverOptions()
{
    return sops;
}

solverOutput *solverSystem::getSolverOutput()
{
    return sout;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return event vars
int solverSystem::getNumRoots()
{
    return numRoots;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// block decomp vars
unsigned int solverSystem::getBlockId()
{
    return blockId;
}

unsigned int solverSystem::getNumBlocks()
{
    return numBlocks;
}

solverSystem *solverSystem::getBlockAt(unsigned int id)
{
    if(solveBlocks.size() > (id + 1))
    {
        return solveBlocks[id];
    }
    return NULL;
}

//solverOptions *solverSystem::getBlockSolverOptions()
//{
//    return blockSops;
//}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// aids the block solver to track Original variables
void solverSystem::copyBlockVarXToRoot()
{
    double* varRoot = root->getVarX();
    unsigned int count = 0;
    for(auto it = mapVarX->begin(); it != mapVarX->end(); ++it)
    {
        varRoot[*it] = varx[count];
        ++count;
    }
}

void solverSystem::copyBlockVarYYToRoot()
{
    double* varRoot = root->getVarYY();
    unsigned int count = 0;
    for(auto it = mapVarX->begin(); it != mapVarX->end(); ++it)
    {
        varRoot[*it] = varyy[count];
        ++count;
    }
}

void solverSystem::copyBlockVarYPToRoot()
{
    double* varRoot = root->getVarYP();
    unsigned int count = 0;
    for(auto it = mapVarX->begin(); it != mapVarX->end(); ++it)
    {
        varRoot[*it] = varyp[count];
        ++count;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// utility
int getSolverTypeFromString(std::string val)
{
    if(val == "SOLVER_ALG_NONLINEAR")
    {
        return SOLVER_ALG_NONLINEAR;
    }
    else if(val == "SOLVER_DAE_NONLINEAR")
    {
        return SOLVER_DAE_NONLINEAR;
    }
    return -1;
}

int getSolveModeFromString(std::string val)
{
    if(val == "SOLVER_MODE_DIRECT")
    {
        return SOLVER_MODE_DIRECT;
    }
    else if(val == "SOLVER_MODE_BLOCK_DECOMPOSITION")
    {
        return SOLVER_MODE_BLOCK_DECOMPOSITION;
    }
    return -1;
}

int getSolverNamefromString(std::string val)
{
    if(val == "SOLVER_KINSOL")
    {
        return SOLVER_KINSOL;
    }
    else if(val == "SOLVER_IDA")
    {
        return SOLVER_IDA;
    }
    return -1;
}

solver* getSolverfromSolverName(int val)
{
    if(val == SOLVER_KINSOL)
    {
        return new solverSundialsKinsol;
    }
    else if(val == SOLVER_IDA)
    {
        return new solverSundialsIda;
    }
    return NULL;
}

solverOptions* getSolverOptionsFromSolverName(int val)
{
    if(val == SOLVER_KINSOL)
    {
        return new solverOptionsKinsol;
    }
    else if(val == SOLVER_IDA)
    {
        return new solverOptionsIda;
    }
    return NULL;
}

solverOutput* getSolverOutputFromSolverName(int val)
{
    if(val == SOLVER_KINSOL)
    {
        return new solverOutputKinsol;
    }
    else if(val == SOLVER_IDA)
    {
        return new solverOutputIda;
    }
    return NULL;
}
