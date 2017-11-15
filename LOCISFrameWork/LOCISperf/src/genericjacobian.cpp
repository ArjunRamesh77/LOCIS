#include "genericjacobian.h"

unsigned int genericJacobian::getNVar() const
{
    return nVar;
}

void genericJacobian::setNVar(unsigned int value)
{
    nVar = value;
}

genericJacobian::genericJacobian()
{

}

genericJacobian::~genericJacobian()
{

}

void genericJacobian::setInstStackPtr(virtualInstructionStack *value)
{
    instResidualStackPtr = value;
}

int genericJacobian::generateFullJacobianInstr(int type)
{
    //build the full jacobian from the residual function (reverse mode)
    std::vector<virtualOper>::const_iterator residualInstr = instResidualStackPtr->getAllInst()->begin();
    std::vector<virtualOper>::const_iterator residualInstr_end = instResidualStackPtr->getAllInst()->end();
    vdt.setJacType(type);
    switch(type)
    {
    case VDT_NORMAL:
    case VDT_INDEX1:
        vdt.setInstr(instJacobianStack1.getAllInst());
        break;
    case VDT_INDEX2:
        vdt.setInstr(instJacobianStack2.getAllInst());
        break;
    }
    while(residualInstr != residualInstr_end)
    {
        if(vdt.buildTree(residualInstr)) //residualInstr will be incremented inside buildTree
        {
            vdt.getReverseModeAutoDiffInst();
        }
    }
    return 0;
}

int genericJacobian::generateDualPartJacobian()
{
    //generate the two part jacobian for DAE
    generateFullJacobianInstr(VDT_INDEX1);
    generateFullJacobianInstr(VDT_INDEX2);

    return 0;
}

int genericJacobian::evalDenseJacobian1StackBased(double *j, double *x)
{
    unsigned int jIndex = 0;
    std::stack<double> localStack;
    std::vector<double> inter;
    double dValue;

    std::vector<virtualOper>::const_iterator allInst_end = instJacobianStack1.getAllInst()->end();
    for(std::vector<virtualOper>::const_iterator vo = instJacobianStack1.getAllInst()->begin(); vo != allInst_end; ++vo)
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

        case VR_INTER_INDEX:
            localStack.push(inter[vo->index]);
            continue;

        case VR_DERIV_INDEX:
            dValue = localStack.top();
            localStack.pop();
            j[nVar*jIndex + vo->index] = dValue;
            continue;

        default:
            instJacobianStack1.evalStackBased(vo, localStack, inter);
        }

        //signals the evaluation of gradient
        if(vo->last == 4)
        {
            jIndex++;
        }
    }
    return 0;
}

int genericJacobian::evalDenseJacobian2StackBased(double *j, double alpha, double *yy, double *yp)
{
    for(long i = 1; i <= 2; ++i)
    {
        unsigned int jIndex = 0;
        std::stack<double> localStack;
        std::vector<double> inter;
        double dValue;
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
                localStack.push(yy[vo->index]);
                continue;

            case VR_VAR2_INDEX:
                localStack.push(yp[vo->index]);
                continue;

            case VR_CONST:
                if(vo->last == VR_VAR1_INDEX)
                {
                    localStack.push(yy[vo->index]);
                }
                else if(vo->last == VR_VAR2_INDEX)
                {
                    localStack.push(yp[vo->index]);
                }
                else
                {
                    localStack.push(vo->value);
                }
                continue;

            case VR_INTER_INDEX:
                localStack.push(inter[vo->index]);
                continue;

            case VR_DERIV_INDEX:
                dValue = localStack.top();
                localStack.pop();
                if(i == 1)
                    j[nVar*jIndex + vo->index] = dValue;
                else
                    j[nVar*jIndex + vo->index] = j[nVar*jIndex + vo->index] + alpha*dValue;
                continue;

            default:
                instJacobianStack1.evalStackBased(vo, localStack, inter);
            }

            //signals the evaluation of gradient
            if(vo->last == 1)
            {
                jIndex++;
            }
        }
    }
    return 0;
}

