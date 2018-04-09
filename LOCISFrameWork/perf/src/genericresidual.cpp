
#include "genericresidual.h"

genericResidual::genericResidual()
{

}

genericResidual::~genericResidual()
{

}

void genericResidual::setNumEquations(unsigned int numEquations_arg)
{
    numEquations = numEquations_arg;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// count the number of equations in the residual
void genericResidual::countNumEquations()
{
    if(allInst)
    {
        unsigned int count = 0;
        for(auto it = allInst->begin(); it != allInst->end(); ++it)
        {
            if(it->signal == VR_SIGNAL_LAST)
            {
                ++count;
            }
        }
        numEquations = count;
    }
}

unsigned int genericResidual::getNumEquations()
{
    return numEquations;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// evaluate steady state residual
int genericResidual::evalResidual1StackBased(double *xOrig, double *x, double *r)
{
    unsigned int rIndex = 0;
    clearVirtualMachineStack();
    clearVisrtualMachineInter();

    //debug
    std::cout<<"RESIDUAL"<<std::endl;

    std::vector<virtualOper>::const_iterator allInst_end = allInst->end();
    for(std::vector<virtualOper>::const_iterator vo = allInst->begin(); vo != allInst_end; ++vo)
    {
        //values will be pushed to stack
        switch(vo->operType)
        {
        case VR_VAR1_INDEX:
            instStack.push(x[vo->index]);
            break;

        case VR_CONST_VAR1:
            instStack.push(xOrig[vo->index]);
            break;

        default:
            evalStackBased(vo);
        }

        if(vo->signal == VR_SIGNAL_LAST)
        {
            //update residual
            r[rIndex] = instStack.top();
            //debug
            std::cout<< r[rIndex] << std::endl;
            rIndex++;
            instStack.pop();
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// evaluate DAE residual
int genericResidual::evalResidual2StackBased(double *yyOrig, double *yy, double *ypOrig, double *yp, double t, double *r)
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
            instStack.push(yy[vo->index]);
            break;

        case VR_VAR2_INDEX:
            instStack.push(yp[vo->index]);
            break;

        case VR_CONST_VAR1:
            instStack.push(yyOrig[vo->index]);
            break;

        case VR_CONST_VAR2:
            instStack.push(ypOrig[vo->index]);
            break;

        case VR_TIME:
            instStack.push(t);
            break;

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

