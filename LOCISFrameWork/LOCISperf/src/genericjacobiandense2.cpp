#include "genericjacobian.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dense evaluation DAE
int genericJacobian::evalDenseJacobian2StackBased(double alpha, double *yyOrig, double *yy, double *ypOrig, double *yp, double *j)
{
    for(long i = 1; i <= 2; ++i)
    {
        unsigned int jIndex = 0;
        clearVirtualMachineStack();
        clearVisrtualMachineInter();
        double dValue = 0;
        virtualInstructionStack* vis = NULL;

        if(i == 1)
        {
            vis = &instJacobianStack1;
        }
        else
        {
            vis = &instJacobianStack2;
        }

        std::vector<virtualOper>::const_iterator allInst_end = vis->getAllInst()->end();
        for(std::vector<virtualOper>::const_iterator vo = vis->getAllInst()->begin(); vo != allInst_end; ++vo)
        {
            //values will be pushed to stack
            switch(vo->operType)
            {

            case VR_VAR1_INDEX:
                instStack.push(yy[vo->index]);
                break;

            case VR_VAR2_INDEX:
                instStack.push(yp[vo->index]);
                break;

            case VR_LOAD_VAR1_INDEX:
                instInter.push_back(yy[vo->index]);
                break;

            case VR_LOAD_VAR2_INDEX:
                instInter.push_back(yp[vo->index]);
                break;

            case VR_CONST_VAR1:
                instStack.push(yyOrig[vo->index]);
                break;

            case VR_CONST_VAR2:
                instStack.push(ypOrig[vo->index]);
                break;

            case VR_LOAD_CONST_VAR1:
                instInter.push_back(yyOrig[vo->index]);
                break;

            case VR_LOAD_CONST_VAR2:
                instInter.push_back(ypOrig[vo->index]);
                break;

            case VR_DERIV_INDEX:
                dValue = instStack.top();
                instStack.pop();
                if(i == 1)
                    j[nVar*jIndex + vo->index] = dValue;
                else
                    j[nVar*jIndex + vo->index] = j[nVar*jIndex + vo->index] + alpha*dValue;
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
    }
    return 0;
}
