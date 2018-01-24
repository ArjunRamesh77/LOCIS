
#include "genericresidual.h"

genericResidual::genericResidual()
{

}

genericResidual::~genericResidual()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// evaluate steady state residual
int genericResidual::evalResidual1StackBased(double *r, double *x)
{
    unsigned int rIndex = 0;
    clearVirtualMachineStack();
    clearVisrtualMachineInter();

    std::vector<virtualOper>::const_iterator allInst_end = allInst->end();
    for(std::vector<virtualOper>::const_iterator vo = allInst->begin(); vo != allInst_end; ++vo)
    {
        //values will be pushed to stack
        switch(vo->operType)
        {
        case VR_VAR1_INDEX:
        case VR_CONST_VAR1:
            instStack.push(x[vo->index]);
            continue;

        default:
            evalStackBased(vo);
        }

        if(vo->signal == VR_SIGNAL_LAST)
        {
            //update residual
            r[rIndex] = instStack.top();
            rIndex++;
            instStack.pop();
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// evaluate DAE residual
int genericResidual::evalResidual2StackBased(double *r, double *yy, double *yp)
{
    unsigned int rIndex = 0;
    clearVirtualMachineStack();
    clearVisrtualMachineInter();

    std::vector<virtualOper>::const_iterator allInst_end = allInst->end();
    for(std::vector<virtualOper>::const_iterator vo = allInst->begin(); vo != allInst_end; ++vo)
    {
        //values will be pushed to stack
        switch(vo->operType)
        {
        case VR_VAR1_INDEX:
        case VR_CONST_VAR1:
            instStack.push(yy[vo->index]);
            continue;

        case VR_VAR2_INDEX:
        case VR_CONST_VAR2:
            instStack.push(yp[vo->index]);
            continue;

        default:
            evalStackBased(vo);
        }

        if(vo->signal == VR_SIGNAL_LAST)
        {
            //update residual
            r[rIndex] = instStack.top();
            rIndex++;
            instStack.pop();
        }
    }
    return 0;
}
