#pragma once

#include "virtualinstruction.h"
#include "virtualdependencytree.h"

class genericJacobian : public virtualInstructionStack
{
    virtualInstructionStack instJacobianStack1;
    virtualInstructionStack instJacobianStack2;
    virtualDependencyTree vdt;
    unsigned int nVar;

public:
    genericJacobian();
    ~genericJacobian();

    unsigned int getNVar() const;
    void setNVar(unsigned int value);

    //construction
    int generateFullJacobianInstr(int type);
    int generateDualPartJacobian();
    int evalDenseJacobian1StackBased(double* j, double* x);
    int evalDenseJacobian2StackBased(double* j, double alpha, double* yy, double* yp);
    int evalSparseJacobian3StackBased(double *rp, double *cp, double *value);

};
