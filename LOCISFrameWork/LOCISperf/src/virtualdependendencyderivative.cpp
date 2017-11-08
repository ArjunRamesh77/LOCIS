#include "virtualdependencytree.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Adds the derivative instructions to the main instruction set
int virtualDependencyTree::addIntermediateDerivativeInstr(virtualDependencyTreeNode *item, virtualDependencyTreeNode *itemPrev)
{
    //get index
    unsigned int wIndexFunction = item->getWIndex();
    unsigned int wIndexIndepVar = item->getWIndex();
    //unsigned int wIndexDeriv = 0;

    //get the iterator pointer for the Function in the main instruction list
    std::vector<virtualOper>::const_iterator fnIt = interBegin[wIndexFunction];

    //locals
    unsigned int wIndexL = 0;
    unsigned int wIndexR = 0;
    std::stack<virtualOper> localStack;

    //run the instruction till the assign operation to determine the derivative
    while(fnIt->operType != VR_ASSIGN)
    {
        switch(fnIt->operType)
        {
        case VR_INTER_INDEX:
            localStack.push(*fnIt);
            break;

        case VR_BIN_ADD:
        case VR_BIN_SUB:
            wIndexL = localStack.top().index;
            localStack.pop();
            wIndexR = localStack.top().index;
            localStack.pop();

            if(wIndexL == wIndexIndepVar)
            {
                instr->push_back(virtualOper(VR_CONST, 1.0, 0));
            }
            else
            {
                instr->push_back(virtualOper(VR_CONST, 0.0, 0));
            }

            if(wIndexR == wIndexIndepVar)
            {
                instr->push_back(virtualOper(VR_CONST, 1.0, 0));
            }
            else
            {
                instr->push_back(virtualOper(VR_CONST, 0.0, 0));
            }

            instr->push_back(virtualOper(fnIt->operType, 0.0, 0));
            instr->push_back(virtualOper(VR_ASSIGN, 0, wIndex));
            wIndex++;
            break;

        case VR_BIN_MUL:
            wIndexR = localStack.top().index;
            localStack.pop();
            wIndexL = localStack.top().index;
            localStack.pop();

            // d(x*x) = 2*x
            if((wIndexL == wIndexIndepVar) && (wIndexR == wIndexIndepVar))
            {
                instr->push_back(virtualOper(VR_CONST, 2.0, 0));
                instr->push_back(virtualOper(VR_INTER_INDEX, 0.0, wIndexIndepVar));
                instr->push_back(virtualOper(VR_BIN_ADD, 0.0, 0));
            }
            else
            {
                //d(x*y) = y
                if(wIndexL == wIndexIndepVar)
                {
                    instr->push_back(virtualOper(VR_INTER_INDEX, 0.0, wIndexR));
                }
                //d(y*x) = x
                else if(wIndexR == wIndexIndepVar)
                {
                    instr->push_back(virtualOper(VR_INTER_INDEX, 0.0, wIndexL));
                }
                else
                {
                    instr->push_back(virtualOper(VR_CONST, 0, 0));
                }
            }
            instr->push_back(virtualOper(VR_ASSIGN, 0, wIndex));
            wIndex++;
            break;

        case VR_BIN_DIV:
            wIndexR = localStack.top().index;
            localStack.pop();
            wIndexL = localStack.top().index;
            localStack.pop();

            // d(x/x) = 0
            if((wIndexL == wIndexIndepVar) && (wIndexR == wIndexIndepVar))
            {
                instr->push_back(virtualOper(VR_CONST, 0.0, 0));
            }
            else
            {
                // d(x/y) = 1/y
                if(wIndexL == wIndexIndepVar)
                {
                    instr->push_back(virtualOper(VR_CONST, 1.0, 0));
                    instr->push_back(virtualOper(VR_INTER_INDEX, 0.0, wIndexR));
                    instr->push_back(virtualOper(VR_BIN_DIV, 0.0, 0));
                }
                // d(y/x) = -y*(1/x*x)
                else if(wIndexR == wIndexIndepVar)
                {
                    instr->push_back(virtualOper(VR_INTER_INDEX, 0.0, wIndexL));
                    instr->push_back(virtualOper(VR_UNA_SUB, 0.0, 0));
                    instr->push_back(virtualOper(VR_INTER_INDEX, 0.0, wIndexR));
                    instr->push_back(virtualOper(VR_CONST, 2.0, 0));
                    instr->push_back(virtualOper(VR_INTER_INDEX, 0.0, wIndexL));
                    instr->push_back(virtualOper(VR_BIN_DIV, 0.0, 0));
                }
                else
                {
                    instr->push_back(virtualOper(VR_CONST, 0, 0));
                }
            }
            instr->push_back(virtualOper(VR_ASSIGN, 0, wIndex));
            wIndex++;
            break;
        }
        ++fnIt;
    }
    return 0;
}
