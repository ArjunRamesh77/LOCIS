#pragma once

#include "genericresidual.h"
#include "genericjacobian.h"
#include <math.h>

enum solverTypes
{
    SOLVER_ALG_NONLINEAR,
    SOLVER_ALG_LINEAR,
    SOLVER_DAE_NONLINEAR,
    SOLVER_ODE
};

enum jacobianType
{
    JACOBIAN_DENSE,
    JACOBIAN_SPARSE_CRS,
    JACOBIAN_SPARSE_CSC
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// base class for all solver types
class solver
{
protected:
    int solverType;
    unsigned int numVar;
    unsigned int numEqu;
    genericResidual* residual;
    genericJacobian* jacobian;
    int jacobianType;
    double absTol;
    double relTol;
    int maxIter;
    int traceLevel;

public:
    solver(int sovlerType_arg);
    ~solver();

    void setSolveDimensions(unsigned int numEqu_arg, unsigned int numVar_arg);
    void setResiduaAndJacobian(genericResidual* residual_arg, genericJacobian* jacobian_arg);
    void setJacobianType(int jacobianType_arg);
    void setTraceLevel(unsigned int traceLevel_arg);
    void setAbsoluteTolerance(double absTol_arg);
    void setRelativeTolerance(double relTol_arg);
    void setMaxIterations(int maxIter_arg);
};
