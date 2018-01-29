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

