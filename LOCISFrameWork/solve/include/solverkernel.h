#pragma once

#include "solverdirect.h"
#include "solverblockdecomposition.h"
#include "initializerdaegen.h"
#include "rapidjsonwrapper.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// driver for the solution of a system (supervisory kernel)
//this is the main entry point of the LOCISEngine after getting the flattened set of equations
class solverKernel
{
    rapidjson::Document rjdoc;
    solverSystem mainSystem;
    solverSolutionMethod* solutionMethodPtr;
    std::vector<virtualOper>* equationSet;

    //temp
    unsigned int numInstrInit;

public:
    solverKernel();
    ~solverKernel();

    //utils
    int getSystemTypeFromString(std::string systemType);
    solver *getSolverFromString(std::string solverName);
    solverOptions *getOptionsForSolverFromString(std::string solverName);
    solverOutput *getOutputForSolverFromString(std::string solverName);
    int getSolutionMethodFromString(std::string solutionMethod);

    bool solveDaeInitialization();

    void setFlatEquationSet(std::vector<virtualOper>* flatEquSet_arg);
    void setMainSystem(const char* systemDesc);
    bool initSystem();
    bool solveSystem();
    bool exitSystem();

    solverSystem* getSystem();
    void setNumInstrInit(unsigned int value);
};
