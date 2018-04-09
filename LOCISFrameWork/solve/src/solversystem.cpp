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

void solverSystem::setSystemType(int type)
{
    systemType = type;
}

void solverSystem::setSolveMode(int mode)
{
    solveMode = mode;
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
bool solverSystem::setAndAllocateSystemDims(unsigned int numVar_arg, unsigned int numEqu_arg)
{
    bool ret = true;
    numVar = numVar_arg;
    numEqu = numEqu_arg;

    switch(systemType)
    {
    case SOLVER_ALG_NONLINEAR:
        if(varx)
        {
            delete varx;
            varx = NULL;
        }
        varx = new double[numVar];
        break;

    case SOLVER_DAE_NONLINEAR:
        if(varyy)
        {
            delete varyy;
            varyy = NULL;
        }
        varyy = new double[numVar];
        if(varyp)
        {
            delete varyp;
            varyp = NULL;
        }
        varyp = new double[numVar];
        break;
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set the falttened system of equations
void solverSystem::setEquationVector(std::vector<virtualOper> *pEquationVec_arg)
{
    pEquationVec = pEquationVec_arg;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set the residual directly
void solverSystem::setResidual(genericResidual *res)
{
    residual = res;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set the root residual
void solverSystem::setRootResidual(unsigned int numRoots_arg, genericResidual *rootResidual_arg)
{
    numRoots = numRoots_arg;
    rootResidual = rootResidual_arg;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set the system solver
void solverSystem::setSolver(solver* solver_arg, solverOptions *sops_arg, solverOutput *sout_arg)
{
    solv = solver_arg;
    sops = sops_arg;
    sout = sout_arg;
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

//void solverSystem::setBlockSolverOptionsNonLinAlg(solverOptions *bsops)
//{
//    blockSopsNonLinAlg = bsops;
//}

//void solverSystem::setBlockSolverOptionsNonLinDae(solverOptions *bsops)
//{
//    blockSolverNonLinDae = bsops;
//}

//void solverSystem::setBlockSolverNonLinAlgName(int type)
//{
//    blockSolverNonLinAlgName = type;
//}

//void solverSystem::setBlockSolverNonLinDaeName(int type)
//{
//    blockSolverNonLinDaeName = type;
//}

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return the system functions
genericResidual *solverSystem::getResidual()
{
    return residual;
}

genericJacobian *solverSystem::getJacobian()
{
    return jacobian;
}

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

genericResidual *solverSystem::getRootResidual()
{
    return rootResidual;
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
bool solverSystem::copyBlockVarXToRoot()
{
    double* varRoot = root->getVarX();
    if(varRoot)
    {
        if(mapVarX)
        {
            unsigned int count = 0;
            for(auto it = mapVarX->begin(); it != mapVarX->end(); ++it)
            {
                varRoot[*it] = varx[count];
                ++count;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool solverSystem::copyBlockVarYYToRoot()
{
    double* varRoot = root->getVarYY();
    if(varRoot)
    {
        if(mapVarX)
        {
            unsigned int count = 0;
            for(auto it = mapVarX->begin(); it != mapVarX->end(); ++it)
            {
                varRoot[*it] = varyy[count];
                ++count;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool solverSystem::copyBlockVarYPToRoot()
{
    double* varRoot = root->getVarYP();
    if(varRoot)
    {
        if(mapVarX)
        {
            unsigned int count = 0;
            for(auto it = mapVarX->begin(); it != mapVarX->end(); ++it)
            {
                varRoot[*it] = varyp[count];
                ++count;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

void solverSystem::setVariables()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// returns
solver *solverSystem::createBlockSolver(int systemType_arg, solverOptions* sops_arg, solverOutput* sout_arg)
{
    solver* sol = NULL;

    switch(systemType_arg)
    {
    case FUNCTION_TYPE_ALG:
        if(blockSolverNonLinAlg == SOLVER_KINSOL)
        {
            sol = new solverSundialsKinsol;
            sops_arg = blockSopsNonLinAlg;
            sout_arg = new solverOutputKinsol;
        }
        break;
    case FUNCTION_TYPE_DAE:
        if(blockSolverNonLinDae == SOLVER_IDA)
        {
            sol = new solverSundialsIda;
            sops_arg = blockSopsNonLinDae;
            sout_arg = new solverOutputIda;
        }
    }

    return sol;
}































