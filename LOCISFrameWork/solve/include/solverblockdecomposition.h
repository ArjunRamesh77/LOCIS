
#pragma once

#include "blockdecomposition.h"
#include "solversolutionmethod.h"
#include "solversystem.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// block decomposition output
struct solverOutputBlockDecomp : public solverOutput
{
    double* xAns;
    double* yyAns;
    double* ypAns;

    solverOutputBlockDecomp();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// supervisor for using block decomposition method for solving system of equations
class solverBlockDecomposition : public solverSolutionMethod
{
    bool isInitializer;
    blockDecomposition bd;
    unsigned int numEquBlocks;
    std::list<solverSystem*> blocks;

public:
    solverBlockDecomposition();
    ~solverBlockDecomposition();

    void setIsInitializer(bool val);

    virtual bool initSystem();
    virtual bool solve(double time = 0.0);
    virtual solverOutput* getSolverOutput();
};

