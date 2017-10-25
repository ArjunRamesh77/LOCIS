
#include "genericresidual.h"

void genericResiual::setInstStackPtr(virtualInstructionStack *value)
{
    instStackPtr = value;
}

genericResiual::genericResiual()
{

}

genericResiual::~genericResiual()
{

}

int genericResiual::evalResidualStackBased(double *r, double *x)
{
    unsigned int rIndex = 0;
    std::stack<double> localStack;

    std::vector<virtualOper>::const_iterator allInst_end = instStackPtr->getAllInst()->end();
    for(std::vector<virtualOper>::const_iterator vo = instStackPtr->getAllInst()->begin(); vo != allInst_end; ++vo)
    {
        //values will be pushed to stack
        switch(vo->operType)
        {
        case VR_VAR1_INDEX:
            localStack.push(x[vo->index]);
            continue;

        case VR_CONST:
            localStack.push(vo->value);
            continue;

        default:
            instStackPtr->evalStackBased(vo, localStack);
        }

        if(vo->last == 1)
        {
            //update residual
            r[rIndex] = localStack.top();
            rIndex++;
            localStack.pop();
        }
    }
    return 0;
}

