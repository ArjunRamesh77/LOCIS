#include "genericjacobian.h"

unsigned int genericJacobian::getNVar() const
{
    return nVar;
}

void genericJacobian::setNVar(unsigned int value)
{
    nVar = value;
}

genericJacobian::genericJacobian() :
    instJacobianStack1(),
    instJacobianStack2(),
    vdt(),
    nVar(0)
{

}

genericJacobian::~genericJacobian()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// generates full jacobian instruction form the residual
int genericJacobian::generateFullJacobianInstr(int type)
{
    //build the full jacobian from the residual function (reverse mode)
    std::vector<virtualOper>::const_iterator residualInstr = allInst->begin();
    std::vector<virtualOper>::const_iterator residualInstr_end = allInst->end();
    vdt.setJacType(type);
    switch(type)
    {
    case VDT_NORMAL:
    case VDT_INDEX1:
        instJacobianStack1.createNewInstructionStack(NULL);
        vdt.setInstr(instJacobianStack1.getAllInst());
        break;
    case VDT_INDEX2:
        instJacobianStack2.createNewInstructionStack(NULL);
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// generate the two part jacobian for DAE
int genericJacobian::generateDualPartJacobian()
{
    generateFullJacobianInstr(VDT_INDEX1);
    generateFullJacobianInstr(VDT_INDEX2);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dense evaluation Steady State
int genericJacobian::evalDenseJacobian1StackBased(double *j, double *x)
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
        case VR_CONST_VAR1:
            instStack.push(x[vo->index]);
            break;

        case VR_LOAD_VAR1_INDEX:
            instInter.push_back(x[vo->index]);
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dense evaluation DAE
int genericJacobian::evalDenseJacobian2StackBased(double *j, double alpha, double *yy, double *yp)
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
            case VR_CONST_VAR1:
                instStack.push(yy[vo->index]);
                break;

            case VR_VAR2_INDEX:
            case VR_CONST_VAR2:
                instStack.push(yp[vo->index]);
                break;

            case VR_LOAD_VAR1_INDEX:
                instInter.push_back(yy[vo->index]);
                break;

            case VR_LOAD_VAR2_INDEX:
                instInter.push_back(yp[vo->index]);
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sparse evaluation Steady state (COO)
int genericJacobian::evalSparseJacobian3StackBased(double *rp, double *cp, double *value)
{
    unsigned int jIndex = 0;
    unsigned int incr = 0;
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
        case VR_CONST_VAR1:
            instStack.push(rp[vo->index]);
            break;

        case VR_LOAD_VAR1_INDEX:
            instInter.push_back(rp[vo->index]);
            break;

        case VR_DERIV_INDEX:
            dValue = instStack.top();
            instStack.pop();

            cp[incr] = vo->index;
            rp[incr] = jIndex;
            value[incr] = dValue;
            ++incr;
            continue;

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

