#include "solverkernel.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// higher level system Info
bool solverKernel::readSystem(rapidjson::Value& rootObj, bool initDae)
{
    //check if dae init system
    solverSystem* system = NULL;
    if(initDae)
    {
        system = new solverSystem;
        mainSystem.setDaeInitSystem(system);
    }
    else
    {
        system = &mainSystem;
    }

    //get basic system info
    std::string  systemTypeStr(rootObj["system-type"].GetString());
    unsigned int numVars = rootObj["num-vars"].GetInt();
    unsigned int numEqus = rootObj["num-equs"].GetInt();
    system->setSystemType(getSolverTypeFromString(systemTypeStr));
    system->setSystemDims(numVars, numEqus);

    //get t range if dae
    if(system->getSystemType() == SOLVER_DAE_NONLINEAR)
    {
        double timeStart = rootObj["time-start"].GetDouble();
        double timeEnd = rootObj["time-end"].GetDouble();
        unsigned int timeStep = rootObj["time-step"].GetInt();
        system->setTimeRange(timeStart, timeEnd, timeStep);
    }

    //get solution method infos
    rapidjson::Value solutionMethodObj = rootObj["solution-method"].GetObject();
    if(readSolutionMethod(solutionMethodObj, initDae))
    {
        //check if dae system (requires initialization)
        if(system->getSystemType() == SOLVER_DAE_NONLINEAR && !initDae)
        {
            rapidjson::Value initDaeObj = rootObj["init-dae"].GetObject();
            if(readSystem(initDaeObj, true))
            {
                return true;
            }
        }
        return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solution Method reader
bool solverKernel::readSolutionMethod(rapidjson::Value &val, bool initDae)
{
    solverSystem* system = NULL;
    if(initDae)
        system = mainSystem.getDaeInitSystem();
    else
        system = &mainSystem;

    std::string solutionMethodStr = val["type"].GetString();
    int solutionMethod = getSolveModeFromString(solutionMethodStr);
    system->setSolveMode(solutionMethod);

    if(solutionMethod == SOLVER_MODE_DIRECT)
    {
        rapidjson::Value solverObj = val["solver"].GetObject();
        std::string solverTypeStr = solverObj["type"].GetString();
        int solverType = getSolverTypeFromString(solverTypeStr);

        //check if it matched main system
        if(system->getSystemType() != solverType)
            return false;

        system->setSolverType(solverType);

        //solver name
        std::string solverNameStr = solverObj["name"].GetString();
        int solverName = getSolverNamefromString(solverNameStr);
        system->setSolverName(solverName);

        //solver input/output
        rapidjson::Value* rjIn = new rapidjson::Value;
        rapidjson::Value* rjOut = new rapidjson::Value;
        *rjIn = solverObj["input"].GetObject();
        *rjOut = solverObj["output"].GetObject();
        system->setSolverInputAndOutput(rjIn, rjOut);

    }
    else if(solutionMethod == SOLVER_MODE_BLOCK_DECOMPOSITION)
    {
        //get dae solver
        rapidjson::Value solverObjDae = val["solver-dae"].GetObject();

        //dae solver name
        std::string solverNameStr = solverObjDae["name"].GetString();
        int solverNameDae = getSolverNamefromString(solverNameStr);

        //dae input
        rapidjson::Value* rjDaeIn = new rapidjson::Value;
        rapidjson::Value* rjDaeOut = new rapidjson::Value;
        *rjDaeIn = solverObjDae["input"].GetObject();
        *rjDaeOut = solverObjDae["output"].GetObject();
        system->setBlockDaeInputOutput(rjDaeIn, rjDaeOut);

        //get alg solver
        rapidjson::Value solverObjAlg = val["solver-alg"].GetObject();

        //alg solver name
        solverNameStr = solverObjAlg["name"].GetString();
        int solverNameAlg = getSolverNamefromString(solverNameStr);

        //dae input
        rapidjson::Value* rjAlgIn = new rapidjson::Value;
        rapidjson::Value* rjAlgOut = new rapidjson::Value;
        *rjAlgIn = solverObjAlg["input"].GetObject();
        *rjAlgOut = solverObjAlg["output"].GetObject();
        system->setBlockAlgInputOutput(rjAlgIn, rjAlgOut);

        //set up solver names
        system->setBlockSolvers(solverNameAlg, solverNameDae);
    }
    else
    {
        return false;
    }
    return true;
}
