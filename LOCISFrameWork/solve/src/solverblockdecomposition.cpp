
/*

#include "solverblockdecomposition.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solverOutputBlockDecomp (Can be considered a special type of solver)
solverOutputBlockDecomp::solverOutputBlockDecomp() :
    xAns(NULL),
    yyAns(NULL),
    ypAns(NULL)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solverBlockdecomposition
solverBlockDecomposition::solverBlockDecomposition() :
    solverSolutionMethod()
{

}

solverBlockDecomposition::~solverBlockDecomposition()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// initialize the system with block decomposition strategy
bool solverBlockDecomposition::initSystem()
{
    bool ret = false;
    if(!system)
        return false;

    //set the system dimensions
    unsigned int numVar = system->getNumVar();
    if(numVar <= 0)
        return false;

    unsigned int numEqu = system->getNumEqu();
    if(numEqu <= 0)
        return false;

    //allocate global system
    system->setAndAllocateSystemDims(numVar, numEqu);

    //build the main residual
    ret = system->buildResidualFromEquationVector();
    if(!ret)
        return ret;

    //block decomposition
    ret = bd.computeBlockSystem(system->getResidual(), numVar, numEqu);
    if(!ret)
        return false;

    //set the number of equation blocks
    unsigned int numEquBlocks = bd.getNumEquationSets();
    if(numEquBlocks <= 0)
        return false;

    system->setNumBlocks(numEquBlocks);

    //get each block decomposed residual
    for(unsigned int i = 0; i < numEquSets; ++i)
    {
        //create a new solver system for each block
        solverSystem* ss = new solverSystem();

        //add block to chain
        blocks.push_back(ss);

        //set the blocks solver options
        ss->setBlockSolverOptions(system->getBlockSolverOptions());

        //block system size
        unsigned int blNumVar = bd.getVariable1Mapping(i)->size(); //this is the best way to get it
        unsigned int blNumEqu = blRes->getNumEquations();
        if(blNumEqu == 0)
        {
            blRes->countNumEquations();
            if(blRes->getNumEquations() <= 0)
            {
                return false;
            }
        }

        //set block system dimensions
        ss->setAndAllocateSystemDims(blNumVar, blNumEqu);

        //add mapings of variables depending on type of system
        switch(system->getSystemType())
        {
        case SOLVER_ALG_NONLINEAR:
            ss->setBlockMapVarX(bd.getVariable1Mapping(i));
            break;

        case SOLVER_DAE_NONLINEAR:
            ss->setBlockMapVarYY(bd.getVariable1Mapping(i));
            ss->setBlockMapVarYP(bd.getVariable2Mapping(i));
            break;
        }

        //setup the solver
        solver* solv = NULL;
        solverOptions* ssops = NULL;
        solverOutput* ssout = NULL;

        //create and set the solver, its options and output for each block
        solv = system->createBlockSolver(blRes->getFunctionType(), sssops, ssout);
        if(solv == NULL)
            return false;
        ss->setSolver(solv, sssops, ssout);

        //set the problem size, and residual from block decomposition for each block
        bool ret = false;
        solv->setSolveDimensions(blNumVar, blNumEqu);
        genericResidual* blRes = bd.getBlockResidual(i);
        ss->setResidual(blRes);

        //set the solver options in the solver
        ret = solv->setSolverParameters(ssops);
        if(!ret)
            break;

        //build the jacobian from the residual
        ret = ss->buildJacobianFromResidual();
        if(!ret)
            break;

        //set the residual and jacobian for the solver
        genericJacobian* blJac = ss->getJacobian();
        solv->setResiduaAndJacobian(blRes, blJac);

        //initialize the solver
        ret = solv->init();
        if(!ret)
            return false;
    }

    bInitDone = true;

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solve the system
int solverBlockDecomposition::solve(double time)
{
    if(!bInitDone)
        return -1;

    //iterate over each block and solve
    solverSystem* ss = NULL;
    solver* sol = NULL;
    int ret = 0;
    for(auto it = blocks.begin(); it != blocks.end(); ++it)
    {
        //deref
        ss = *it;

        //get the solver
        sol = ss->getSolver();

        //solve the block
        switch(ss->getSystemType())
        {
        case SOLVER_ALG_NONLINEAR:
            solverNonLinearAlg* solNonLinAlg = static_cast<solverNonLinearAlg*>(sol);
            ret = solNonLinAlg->solve();
            if(ret)
            {
                //post solution to Orig
                ss->copyBlockVarXToRoot();

                //intermediate results
            }
            else
            {
                ret = -1;
                break;
            }
            break;

        case SOLVER_DAE_NONLINEAR:
            solverNonLinearDae* solNonLinDae = static_cast<solverNonLinearDae*>(sol);
            ret = solNonLinDae->solve(time);
            if(ret)
            {
                //post solution to Orig
                ss->copyBlockVarYYToRoot();
                ss->copyBlockVarYPToRoot();

                //intermediate results
            }
            else
            {
                ret = -1;
                break;
            }
            break;
        }

        if(ret == -1)
        {
            break;
        }
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get the output
solverOutput *solverBlockDecomposition::getSolverOutput(solverOutputBlockDecomp *out)
{

}

*/

