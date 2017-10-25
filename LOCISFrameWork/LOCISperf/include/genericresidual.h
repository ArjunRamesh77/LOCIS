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

    int evalResidualStackBased(double* r, double* x);
    void setInstStackPtr(virtualInstructionStack *value);
};
