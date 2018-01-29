#include "genericjacobian.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dense evaluation Steady State
int genericJacobian::evalDenseJacobian1StackBased(double *xOrig, double *x, double *j)
{
    unsigned int jIndex = 0;
    clearVirtualMachineStack();
    clearVisrtualMachineInter();
    double dValue = 0;

    std::vector<virtualOper>::const_iterator allInst_end = instJacobianStack1.getAllInst()->end();
    for(std::vector<virtualOper>::const_iterator vo = instJacobianStack1.getAllInst()->begin(); vo != allInst_end; ++vo)
    {
        //values will be pushed to stack
        switch(vo->operType)
        {
        case VR_VAR1_INDEX:
            instStack.push(x[vo->index]);
            break;

        case VR_LOAD_VAR1_INDEX:
            instInter.push_back(x[vo->index]);
            break;

        case VR_CONST_VAR1:
            instStack.push(xOrig[vo->index]);
            break;

        case VR_LOAD_CONST_VAR1:
            instInter.push_back(xOrig[vo->index]);
            break;

        case VR_DERIV_INDEX:
            dValue = instStack.top();
            instStack.pop();
            j[nVar*jIndex + vo->index] = dValue;
            break;

        default:
            evalStackBased(vo);
        }

        //signals the evaluation of gradient
        if(vo->signal == VR_SIGNAL_GRADIENT_COMPLETE)
        {
            jIndex++;
        }
    }
    return 0;
}
