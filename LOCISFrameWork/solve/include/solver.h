#pragma once

#include "genericresidual.h"
#include "genericjacobian.h"
#include <string>
#include <math.h>

enum solverTypes
{
    SOLVER_ALG_NONLINEAR,
    SOLVER_ALG_LINEAR,
    SOLVER_DAE_NONLINEAR,
    SOLVER_ODE
};

enum solverNames
{
    SOLVER_KINSOL,
    SOLVER_IDA
};

enum matrixTypes
{
    MATRIX_DENSE,
    MATRIX_SPARSE_CSR,
    MATRIX_SPARSE_CSC
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// generic options strucnture
struct solverOptions
{
    unsigned int numVars;
    double absXTol;
    double relXTol;
    double absFTol;
    double relFTol;
    int maxIter;
    int traceLevel;

    solverOptions();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// generic output
struct solverOutput
{
    bool bGood;
    long int workSpaceSize;
    long int numFevals;
    long int numJacevals;
    long int numNonLinIter;
    double funcNorm;
    double xNorm;

    solverOutput();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// base class for all solver types
class solver
{
protected:
    int solverType;
    std::string solverName;
    unsigned int numVar;
    unsigned int numEqu;
    genericResidual* residual;
    genericJacobian* jacobian;
    solverOptions* option;

public:
    solver(int sovlerType_arg, std::string solverName_arg);
    ~solver();

    void setSolveDimensions(unsigned int numEqu_arg, unsigned int numVar_arg);
    void setResiduaAndJacobian(genericResidual* residual_arg, genericJacobian* jacobian_arg);
    int getSolverType();
    std::string getSolverName();

    //interface
    virtual bool setSolverParameters(solverOptions* ops);
    virtual bool init() = 0;
    virtual bool exit() = 0;
    virtual bool getSolverOutput(solverOutput* out) = 0;
};
