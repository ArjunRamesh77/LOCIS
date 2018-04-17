#pragma once

#include "solverdirect.h"
#include "solverblockdecomposition.h"
#include "initializerdaegen.h"
#include "rapidjsonwrapper.h"
#include <memory>

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

    //Read functions
    bool readSystem(rapidjson::Value &rootObj, bool initDae = false);
    bool readSolutionMethod(rapidjson::Value& val, bool initDae = false);
    bool readSolver();

    //get information from JSON

    bool solveDaeInitialization();

    void setFlatEquationSet(std::vector<virtualOper>* flatEquSet_arg);
    void setMainSystem(const char* systemDesc);
    void allocateGlobalSystemVars();
    bool initSystem();
    bool solveSystem();
    bool exitSystem();

    solverSystem* getSystem();
    void setNumInstrInit(unsigned int value);
};
