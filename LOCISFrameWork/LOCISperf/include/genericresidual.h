#pragma once

#include "virtualinstruction.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// implements a residual based on the virtualInstruction
// the residual maybe evaluated by stack
// or by JIT
class genericResiual
{
    virtualInstructionStack* instStackPtr;

public:
    genericResiual();
    ~genericResiual();

    int evalResidual1StackBased(double* r, double* x);
    int evalResidual2StackBased(double* r, double* yy, double* yp);
    void setInstStackPtr(virtualInstructionStack *value);
};
