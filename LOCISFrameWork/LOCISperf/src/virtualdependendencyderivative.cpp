#include "virtualdependencytree.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Adds the derivative instructions to the main instruction set
int virtualDependencyTree::addIntermediateDerivativeInstr(virtualDependencyTreeNode *item, virtualDependencyTreeNode *itemPrev, bool root/* = false*/)
{
    //get index
    unsigned int wIndexFunction = item->getWIndex();
    unsigned int wIndexIndepVar = itemPrev->getWIndex();

    //add instructions only if not previously added
    //only exception is root because root derivative will always be overriden by possibly multiple paths
    if(!root)
    {
        if(itemPrev->getWIndexDeriv() != NULL)
        {
            return 0;
        }
    }

    //get the iterator pointer for the Function in the main instruction list
    //std::vector<virtualOper>::const_iterator fnIt = interBegin[wIndexFunction];
    unsigned int interIndex = interBegin[wIndexFunction];
    virtualOper fnIt = instr->at(interIndex);

    //locals
    unsigned int wIndexL = 0;
    unsigned int wIndexR = 0;
    std::stack<virtualOper> localStack;

    //run the instruction till the assign operation to determine the derivative
    while(fnIt.operType != VR_ASSIGN)
    {
        switch(fnIt.operType)
        {
        case VR_INTER_INDEX:
            localStack.push(fnIt);
            break;

        case VR_BIN_ADD:
        case VR_BIN_SUB:
            wIndexR = localStack.top().index;
            localStack.pop();
            wIndexL = localStack.top().index;
            localStack.pop();

            //d(x + y) = 1 + 0
            if(wIndexL == wIndexIndepVar)
            {
                instr->push_back(virtualOper(VR_CONST, 1.0));
            }
            else
            {
                instr->push_back(virtualOper(VR_CONST, 0.0));
            }

            //d(y + x) = 0 + 1
            if(wIndexR == wIndexIndepVar)
            {
                instr->push_back(virtualOper(VR_CONST, 1.0));
            }
            else
            {
                instr->push_back(virtualOper(VR_CONST, 0.0));
            }

            instr->push_back(virtualOper(fnIt.operType));
            instr->push_back(virtualOper(VR_ASSIGN, wIndex));
            itemPrev->setWIndexDeriv(&wIndex);
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
                instr->push_back(virtualOper(VR_CONST, 2.0));
                instr->push_back(virtualOper(VR_INTER_INDEX, wIndexIndepVar));
                instr->push_back(virtualOper(VR_BIN_ADD));
            }
            else
            {
                //d(x*y) = y
                if(wIndexL == wIndexIndepVar)
                {
                    instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
                }
                //d(y*x) = x
                else if(wIndexR == wIndexIndepVar)
                {
                    instr->push_back(virtualOper(VR_INTER_INDEX, wIndexL));
                }
                else
                {
                    instr->push_back(virtualOper(VR_CONST, 0.0));
                }
            }
            instr->push_back(virtualOper(VR_ASSIGN, wIndex));
            itemPrev->setWIndexDeriv(&wIndex);
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
                instr->push_back(virtualOper(VR_CONST, 0.0));
            }
            else
            {
                // d(x/y) = 1/y
                if(wIndexL == wIndexIndepVar)
                {
                    instr->push_back(virtualOper(VR_CONST, 1.0));
                    instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
                    instr->push_back(virtualOper(VR_BIN_DIV));
                }
                // d(y/x) = -y*(1/x*x)
                else if(wIndexR == wIndexIndepVar)
                {
                    instr->push_back(virtualOper(VR_INTER_INDEX, wIndexL));
                    instr->push_back(virtualOper(VR_UNA_SUB));
                    instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
                    instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
                    instr->push_back(virtualOper(VR_BIN_MUL));
                    instr->push_back(virtualOper(VR_BIN_DIV));
                }
                else
                {
                    instr->push_back(virtualOper(VR_CONST, 0.0));
                }
            }
            instr->push_back(virtualOper(VR_ASSIGN, wIndex));
            itemPrev->setWIndexDeriv(&wIndex);
            wIndex++;
            break;

        case VR_BIN_RAISE:
            wIndexR = localStack.top().index;
            localStack.pop();
            wIndexL = localStack.top().index;
            localStack.pop();

            // d(x^x) = x^x(ln(x) + 1)
            if((wIndexL == wIndexIndepVar) && (wIndexR == wIndexIndepVar))
            {
                instr->push_back(virtualOper(VR_INTER_INDEX, wIndexL));
                instr->push_back(virtualOper(VR_INTER_INDEX, wIndexL));
                instr->push_back(virtualOper(VR_BIN_RAISE));
                instr->push_back(virtualOper(VR_INTER_INDEX, wIndexL));
                instr->push_back(virtualOper(VR_FUNC_LOG));
                instr->push_back(virtualOper(VR_CONST, 1.0));
                instr->push_back(virtualOper(VR_BIN_ADD));
                instr->push_back(virtualOper(VR_BIN_MUL));
            }
            else
            {
                // d(x^y) = y*x^(y-1)
                if(wIndexL == wIndexIndepVar)
                {
                    instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
                    instr->push_back(virtualOper(VR_INTER_INDEX, wIndexL));
                    instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
                    instr->push_back(virtualOper(VR_CONST, 1.0));
                    instr->push_back(virtualOper(VR_BIN_SUB));
                    instr->push_back(virtualOper(VR_BIN_RAISE));
                    instr->push_back(virtualOper(VR_BIN_MUL));
                }
                // d(y^x) = ln(y).y^x
                else if(wIndexR == wIndexIndepVar)
                {
                    instr->push_back(virtualOper(VR_INTER_INDEX, wIndexL));
                    instr->push_back(virtualOper(VR_FUNC_LOG));
                    instr->push_back(virtualOper(VR_INTER_INDEX, wIndexL));
                    instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
                    instr->push_back(virtualOper(VR_BIN_RAISE));
                    instr->push_back(virtualOper(VR_BIN_MUL));
                }
                else
                {
                    instr->push_back(virtualOper(VR_CONST, 0.0));
                }
            }
            instr->push_back(virtualOper(VR_ASSIGN, wIndex));
            itemPrev->setWIndexDeriv(&wIndex);
            wIndex++;
            break;

        case VR_UNA_ADD:
        case VR_UNA_SUB:
            wIndexR = localStack.top().index;
            localStack.pop();

            if(wIndexR == wIndexIndepVar)
            {
                instr->push_back(virtualOper(VR_CONST, 1.0));
            }
            else
            {
                instr->push_back(virtualOper(VR_CONST, 0.0));
            }
            instr->push_back(virtualOper(fnIt.operType));
            instr->push_back(virtualOper(VR_ASSIGN, wIndex));
            itemPrev->setWIndexDeriv(&wIndex);
            wIndex++;
            break;

        case VR_FUNCTION_SISO:
            wIndexR = localStack.top().index;
            localStack.pop();

            if(wIndexR == wIndexIndepVar)
            {
                getSISOFunctionDerivative(&fnIt, wIndexR);
            }
            else
            {
                instr->push_back(virtualOper(VR_CONST, 0.0));
            }
            instr->push_back(virtualOper(VR_ASSIGN, wIndex));
            itemPrev->setWIndexDeriv(&wIndex);
            wIndex++;
            break;

        }
        fnIt = (instr->at(++interIndex));
    }
    return 0;
}

int virtualDependencyTree::addFinalDerivative(std::vector<std::vector<unsigned int>*>& psnum)
{
    //loop over sigma
    std::vector<unsigned int>::const_iterator piNum_end;
    std::vector<std::vector<unsigned int>*>::const_iterator psnum_end = psnum.end();
    unsigned int sigmaNumn = 0;
    unsigned int piNumn = 0;
    for(std::vector<std::vector<unsigned int>*>::const_iterator sigmaNum = psnum.begin(); sigmaNum != psnum_end; ++sigmaNum)
    {
        piNumn = 0;
        piNum_end = (*sigmaNum)->end();
        for(std::vector<unsigned int>::const_iterator piNum = (*sigmaNum)->begin(); piNum != piNum_end; ++piNum)
        {
            instr->push_back(virtualOper(VR_INTER_INDEX, *piNum));
            if(piNumn > 0)
            {
                instr->push_back(virtualOper(VR_BIN_MUL));
            }
            ++piNumn;
        }
        //delete (*sigmaNum);
        if(sigmaNumn > 0)
        {
            instr->push_back(virtualOper(VR_BIN_ADD));
        }
        ++sigmaNumn;
    }
    return 0;
}
