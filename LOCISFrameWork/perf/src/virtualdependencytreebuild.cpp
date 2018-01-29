#include "virtualdependencytree.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// builds a dependency tree from a residual function
virtualDependencyTreeNode *virtualDependencyTree::buildTree(std::vector<virtualOper>::const_iterator& iterFucntion)
{
    //init
    wIndex = 0; //this should be larger than number of variables
    std::map<unsigned int, virtualDependencyTreeNode*> otherIndepVars;
    indepVariables.clear();
    interBegin.clear();

    //iterate till final
    std::vector<virtualOper>::const_iterator& vo = iterFucntion;
    std::stack<virtualDependencyTreeNode*> localNodeStack;
    std::vector<unsigned int> nullNodes;
    auto find = indepVariables.find(0);

    //intermediates
    virtualDependencyTreeNode* node = NULL;
    virtualDependencyTreeNode* node1 = NULL;
    virtualDependencyTreeNode* node2 = NULL;
    virtualDependencyTreeNode* node3 = NULL;

    //save the size of instructions (optimization)
    numTreeInstr = instr->size();

    //iterate till f(x)
    while(1)
    {
        switch(vo->operType)
        {
        case VR_CONST_VAR1:
        case VR_VAR1_INDEX:
            node = NULL;
            if(jacType == VDT_INDEX2)
            {
                find = otherIndepVars.find(vo->index);
                if(find == otherIndepVars.end())
                {
                   node = new virtualDependencyTreeNode(VR_LOAD_VAR1_INDEX, wIndex);
                   instr->push_back(virtualOper(VR_LOAD_VAR1_INDEX, vo->index));
                   otherIndepVars[vo->index] = node;
                   wIndex++;
                }
                else
                {
                    node = otherIndepVars[vo->index];
                }
                localNodeStack.push(node);
                break;
            }

            find = indepVariables.find(vo->index);
            if(find == indepVariables.end())
            {
                // if the variable is encountered for the first time put in the map of dependents
                node = new virtualDependencyTreeNode(VR_LOAD_VAR1_INDEX, wIndex);
                instr->push_back(virtualOper(VR_LOAD_VAR1_INDEX, vo->index));
                indepVariables[vo->index] = node;
                wIndex++;
            }
            else
            {
                node = indepVariables[vo->index];
                if(vo->operType == VR_CONST_VAR1)
                {
                    nullNodes.push_back(vo->index); //variables that are fixed
                }
            }
            localNodeStack.push(node);
            break;

        case VR_CONST_VAR2:
        case VR_VAR2_INDEX:
            node = NULL;
            if(jacType == VDT_INDEX1)
            {
                find = otherIndepVars.find(vo->index);
                if(find == otherIndepVars.end())
                {
                   node = new virtualDependencyTreeNode(VR_LOAD_VAR2_INDEX, wIndex);
                   instr->push_back(virtualOper(VR_LOAD_VAR2_INDEX, vo->index));
                   otherIndepVars[vo->index] = node;
                   wIndex++;
                }
                else
                {
                    node = otherIndepVars[vo->index];
                }
                localNodeStack.push(node);
                break;
            }

            find = indepVariables.find(vo->index);
            if(find == indepVariables.end())
            {
                // if the variable is encountered for the first time put in the map of dependents
                node = new virtualDependencyTreeNode(VR_LOAD_VAR2_INDEX, wIndex);
                instr->push_back(virtualOper(VR_LOAD_VAR2_INDEX, vo->index));
                indepVariables[vo->index] = node;
                wIndex++;
            }
            else
            {
                node = indepVariables[vo->index];
                if(vo->operType == VR_CONST_VAR2)
                {
                    nullNodes.push_back(vo->index);
                }
            }
            localNodeStack.push(node);
            break;

        case VR_CONST:
            //a constant is always considered to a be a new intermediate
            node = new virtualDependencyTreeNode(vo->operType, wIndex);

            // create const = inter
            instr->push_back(virtualOper(vo->operType, vo->value, wIndex));
            instr->push_back(virtualOper(VR_ASSIGN, wIndex));

            localNodeStack.push(node);
            ++wIndex;
            break;

        case VR_BIN_ADD:
        case VR_BIN_SUB:
        case VR_BIN_MUL:
        case VR_BIN_DIV:
        case VR_BIN_RAISE:
            //add operands to the stack
            node1 = localNodeStack.top();
            localNodeStack.pop();
            node2 = localNodeStack.top();
            localNodeStack.pop();

            //add connections
            node3 = new virtualDependencyTreeNode(vo->operType, wIndex);
            node1->connectTo(node3);
            node2->connectTo(node3);
            localNodeStack.push(node3);

            //create a b binop = inter
            instr->push_back(virtualOper(VR_INTER_INDEX, node2->getWIndex()));
            interBegin[wIndex] = instr->size()-1;
            instr->push_back(virtualOper(VR_INTER_INDEX, node1->getWIndex()));

            instr->push_back(*vo);
            instr->push_back(virtualOper(VR_ASSIGN, wIndex));

            ++wIndex;
            break;

        case VR_UNA_ADD:
        case VR_UNA_SUB:
        case VR_FUNCTION_SISO:
            //add operands to the stack
            node1 = localNodeStack.top();
            localNodeStack.pop();

            //add connections
            node3 = new virtualDependencyTreeNode(vo->operType, wIndex);
            node1->connectTo(node3);
            localNodeStack.push(node3);

            //create a b binop = inter
            instr->push_back(virtualOper(VR_INTER_INDEX, node1->getWIndex()));
            interBegin[wIndex] = instr->size()-1;
            instr->push_back(*vo);
            instr->push_back(virtualOper(VR_ASSIGN, wIndex));

            ++wIndex;
            break;

        default:
            return NULL;
        }

        if(vo->signal == VR_SIGNAL_LAST)
        {
            ++vo;
            break;
        }
        ++vo;
    }

    //Nullify CONST_VARX_INDEX types in the indepVars
    std::vector<unsigned int>::const_iterator nullNodes_end = nullNodes.end();
    for(std::vector<unsigned int>::const_iterator it = nullNodes.begin(); it != nullNodes_end; ++it)
    {
        indepVariables[*it] = NULL;
    }

    if(localNodeStack.size() == 1)
        return localNodeStack.top();

    return NULL;
}
