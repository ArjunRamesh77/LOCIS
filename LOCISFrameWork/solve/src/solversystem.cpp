#include "solversystem.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solver System
solverSystem::solverSystem()
{

}

solverSystem::~solverSystem()
{

}

void solverSystem::setSystemType(int type)
{
    systemType = type;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// allocate variables based on systemType
void solverSystem::setAndAllocateSystemDims(unsigned int numVar_arg, unsigned int numEqu_arg)
{
    numVar = numVar_arg;
    numEqu = numEqu_arg;

    switch(systemType)
    {
    case 0:
        if(varx)
        {
            delete varx;
            varx = NULL;
        }
        varx = new double[numVar];

    case 1:
        if(varyy)
        {
            delete varyy;
            varyy = NULL;
        }
        varyp = new double[numVar];
        if(varyp)
        {
            delete varyp;
            varyp = NULL;
        }
        varyp = new double[numVar];
    }
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
void solverSystem::setSolver(solver* solver_arg, solverOptions *sops_args, solverOutput *sout_arg)
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get the main root node
solverSystem *solverSystem::getRoot()
{
    return root;
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

bool solverSystem::buildResidualFromEquationVector()
{
    if(pEquationVec)
    {
        genericResidual.createNewInstructionStack(pEquationVec);
        return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// build jacobian based of matrix type
bool solverSystem::buildJacobianFromResidual()
{
    if(jacobian)
    {
        switch(sops->matrixType)
        {
        case MATRIX_DENSE:
            break;

        case MATRIX_SPARSE_CSC:
            break;

        case MATRIX_SPARSE_CSR:
            break;

        }
    }
    return false;
}

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































