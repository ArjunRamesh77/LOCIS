#pragma once

#include "virtualinstruction.h"
#include "virtualdependencytree.h"

class genericJacobian : public virtualInstructionStack
{
    virtualInstructionStack instJacobianStack1;
    virtualInstructionStack instJacobianStack2;
    virtualDependencyTree vdt;
    unsigned int nVar;
    unsigned int NNZ;
    int jacobianStyle;

public:
    genericJacobian();
    ~genericJacobian();

    unsigned int getNVar() const;
    void setNVar(unsigned int value);
    void setJacobianStyle(int value);

    //construction
    int generateFullJacobianInstr(int type);
    int generateDualPartJacobian();
    unsigned int getNNZ();
    int evalDenseJacobian1StackBased(double* xOrig, double *x, double* j);
    int evalDenseJacobian2StackBased(double alpha, double* yyOrig, double* yy, double* ypOrig, double* yp, double* j);
    int evalSparseJacobian3StackBased(double *xOrig, double *x, double *rp, double *cp, double *value);
    int evalOneVarDerivativeStackBased(double *xOrig, double x, double &j);

};
