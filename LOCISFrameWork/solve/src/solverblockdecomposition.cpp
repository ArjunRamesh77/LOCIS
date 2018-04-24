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
    solverSolutionMethod(),
    isInitializer(false)
{

}

solverBlockDecomposition::~solverBlockDecomposition()
{

}

void solverBlockDecomposition::setIsInitializer(bool val)
{
    isInitializer = val;
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
    bd.setInInitializer(isInitializer);
    ret = bd.computeBlockSystem(&tempMainRes, system->getNumVar(), system->getNumEqu());
    if(!ret)
        return false;

    //set the number of equation blocks
    unsigned int numEquBlocks = bd.getNumEquationSets() - 1;
    if(numEquBlocks <= 0)
        return false;
    system->setNumBlocks(numEquBlocks);

    //get each block decomposed residual
    for(unsigned int i = 0; i < numEquBlocks; ++i)
    {
        //create a new solver system for each block
        solverSystem* ss = new solverSystem();

        //add block to chain(lol)
        blocks.push_back(ss);

        //give the root system to the solver system
        ss->setRootSystem(system);

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

        //set each system type since its not known prior to block decopmposition
        if(blRes->getFunctionType() == FUNCTION_TYPE_ALG)
        {
            //for now assume all alg are of type non-linear
            ss->setSystemType(SOLVER_ALG_NONLINEAR);
        }
        else if (blRes->getFunctionType() == FUNCTION_TYPE_DAE)
        {
            ss->setSystemType(SOLVER_DAE_NONLINEAR);
        }

        //set block system dimensions
        ss->setSystemDims(blNumVar, blNumEqu);
        ss->allocateSystemDims();

        //add mapings of variables depending on type of system and solvers
        solver* ssolv = NULL;
        solverOptions* ssops = NULL;
        solverOutput* ssout = NULL;
        solverNonLinearAlg* solvNonLinAlg = NULL;
        solverNonLinearDae* solvNonLinDae = NULL;
        switch(system->getSystemType())
        {
        case SOLVER_ALG_NONLINEAR:
            ssolv = getSolverfromSolverName(system->getBlockSolverAlgName());
            ssops = getSolverOptionsFromSolverName(system->getBlockSolverAlgName());
            ssout = getSolverOutputFromSolverName(system->getBlockSolverAlgName());
            ssops->rjw.setJSONobjRoot(system->getBlockSolveAlgInput());
            ssout->rjw.setJSONobjRoot(system->getBlockSolveAlgOutput());

            //variable / mappings
            ss->setBlockMapVarX(bd.getVariable1Mapping(i));
            solvNonLinAlg = static_cast<solverNonLinearAlg*>(ssolv);
            solvNonLinAlg->setXGuess(ss->getVarX());     //local
            solvNonLinAlg->setXOrig(system->getVarX());  //global
            break;

        case SOLVER_DAE_NONLINEAR:
            ssolv = getSolverfromSolverName(system->getBlockSolverDaeName());
            ssops = getSolverOptionsFromSolverName(system->getBlockSolverDaeName());
            ssout = getSolverOutputFromSolverName(system->getBlockSolverDaeName());
            ssops->rjw.setJSONobjRoot(system->getBlockSolveDaeInput());
            ssout->rjw.setJSONobjRoot(system->getBlockSolveDaeOutput());

            //varible / mappings
            ss->setBlockMapVarYY(bd.getVariable1Mapping(i));
            ss->setBlockMapVarYP(bd.getVariable2Mapping(i));
            solvNonLinDae = static_cast<solverNonLinearDae*>(ssolv);
            solvNonLinDae->setGuessYyandYp(ss->getVarYY(), ss->getVarYP());             //local
            solvNonLinDae->setYyOrigAndYpOrig(system->getVarYY(), system->getVarYP());  //global
            break;
        }

        //setup the solver for each block
        ss->setSolver(ssolv, ssops, ssout);

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
solverOutput *solverBlockDecomposition::getSolverOutput()
{
    return NULL;
}

