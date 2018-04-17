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

    //allocate system dims
    system->allocateSystemDims();

    //get block decomposition equation sets
    genericResidual tempMainRes;
    tempMainRes.createNewInstructionStack(system->getPEquationVec());
    ret = bd.computeBlockSystem(&tempMainRes, system->getNumVar(), system->getNumEqu());
    if(!ret)
        return false;

    //set the number of equation blocks
    unsigned int numEquBlocks = bd.getNumEquationSets();
    if(numEquBlocks <= 0)
        return false;
    system->setNumBlocks(numEquBlocks);

    //get each block decomposed residual
    for(unsigned int i = 0; i < numEquBlocks; ++i)
    {
        //create a new solver system for each block
        solverSystem* ss = new solverSystem();

        //set root system

        //add block to chain(lol)
        blocks.push_back(ss);

        //block system size
        unsigned int blNumVar = bd.getVariable1Mapping(i)->size(); //this is the best way to get it
        genericResidual* blRes = bd.getBlockResidual(i);
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
        ss->setSystemDims(blNumVar, blNumEqu);
        ss->allocateSystemDims();

        //add mapings of variables depending on type of system and solvers
        solver* ssolv = NULL;
        solverOptions* ssops = NULL;
        solverOutput* ssout = NULL;
        switch(system->getSystemType())
        {
        case SOLVER_ALG_NONLINEAR:
            ssolv = getSolverfromSolverName(system->getBlockSolverAlgName());
            ssops = getSolverOptionsFromSolverName(system->getBlockSolverAlgName());
            ssout = getSolverOutputFromSolverName(system->getBlockSolverAlgName());
            ssops->rjw.setJSONobjRoot(system->getBlockSolveAlgInput());
            ssout->rjw.setJSONobjRoot(system->getBlockSolveAlgOutput());

            ss->setBlockMapVarX(bd.getVariable1Mapping(i));
            break;

        case SOLVER_DAE_NONLINEAR:
            ssolv = getSolverfromSolverName(system->getBlockSolverDaeName());
            ssops = getSolverOptionsFromSolverName(system->getBlockSolverDaeName());
            ssout = getSolverOutputFromSolverName(system->getBlockSolverDaeName());
            ssops->rjw.setJSONobjRoot(system->getBlockSolveDaeInput());
            ssout->rjw.setJSONobjRoot(system->getBlockSolveDaeOutput());

            ss->setBlockMapVarYY(bd.getVariable1Mapping(i));
            ss->setBlockMapVarYP(bd.getVariable2Mapping(i));
            break;
        }

        //setup the solver for each block
        ss->setSolver(ssolv, ssops, ssout);

        //setup input options/output options for each block


        //set the problem size, and residual from block decomposition for each block
        ssolv->setSolveDimensions(blNumVar, blNumEqu);
        ssolv->setPEquationVec(blRes->getAllInst());

        //initialize the solver
        ret = ssolv->init(ssops);
        if(!ret)
            return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solve the system
bool solverBlockDecomposition::solve(double time)
{
    //iterate over each block and solve
    solverSystem* ss = NULL;
    solver* sol = NULL;
    bool ret = true;
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
            {
                solverNonLinearAlg* solNonLinAlg = static_cast<solverNonLinearAlg*>(sol);
                ret = solNonLinAlg->solve();
                if(ret)
                    ss->copyBlockVarXToRoot();
            }
        break;

        case SOLVER_DAE_NONLINEAR:
            {
                solverNonLinearDae* solNonLinDae = static_cast<solverNonLinearDae*>(sol);
                ret = solNonLinDae->solve(time);
                if(ret)
                {
                    ss->copyBlockVarYYToRoot();
                    ss->copyBlockVarYPToRoot();
                }
            }
        break;
        }

        if(ret == false)
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
    return NULL;
}

