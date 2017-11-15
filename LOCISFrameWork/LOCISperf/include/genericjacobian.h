#pragma once

#include "virtualinstruction.h"
#include "virtualdependencytree.h"

class genericJacobian
{
    virtualInstructionStack* instResidualStackPtr;
    virtualInstructionStack instJacobianStack1;
    virtualInstructionStack instJacobianStack2;
    virtualDependencyTree vdt;
    unsigned int nVar;

public:
    genericJacobian();
    ~genericJacobian();

    //construction
    int generateFullJacobianInstr(int type);
    int generateDualPartJacobian();

    //evaluation
    int evalDenseJacobian1StackBased(double* j, double* x);
    int evalDenseJacobian2StackBased(double* j, double alpha, double* yy, double* yp);
    void setInstStackPtr(virtualInstructionStack *value);
    unsigned int getNVar() const;
    void setNVar(unsigned int value);
};
