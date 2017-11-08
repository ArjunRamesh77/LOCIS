#include "virtualdependencytree.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dependency tree node
virtualDependencyTreeNode::virtualDependencyTreeNode(std::vector<virtualOper>::const_iterator oper_arg) :
    virtualDependencyTreeNode(0,0, oper_arg)
{

}

virtualDependencyTreeNode::virtualDependencyTreeNode(__int8_t nodeType_arg, __int8_t wIndex_arg, std::vector<virtualOper>::const_iterator oper_arg) :
    nodeType(nodeType_arg),
    wIndex(wIndex_arg),
    oper(oper_arg),
    to()
{

}

virtualDependencyTreeNode::~virtualDependencyTreeNode()
{
    DELETE_VECTOR_ENTRIES(to)
}

void virtualDependencyTreeNode::connectTo(virtualDependencyTreeNode *node)
{
    to.push_back(node);
}

std::vector<virtualDependencyTreeNode *> &virtualDependencyTreeNode::getAllTo()
{
    return to;
}

__int8_t virtualDependencyTreeNode::getNodeType() const
{
    return nodeType;
}

__int8_t virtualDependencyTreeNode::getWIndex() const
{
    return wIndex;
}

std::vector<virtualOper>::const_iterator virtualDependencyTreeNode::getOper() const
{
    return oper;
}

void virtualDependencyTreeNode::setNodeType(const __int8_t &value)
{
    nodeType = value;
}

void virtualDependencyTreeNode::setWIndex(const __int8_t &value)
{
    wIndex = value;
}

void virtualDependencyTreeNode::setOper(const std::vector<virtualOper>::const_iterator &value)
{
    oper = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dependency tree
virtualDependencyTree::virtualDependencyTree()
{

}

virtualDependencyTree::~virtualDependencyTree()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// builds a dependency tree from a residual function
virtualDependencyTreeNode *virtualDependencyTree::buildTree(std::vector<virtualOper>::const_iterator iterFucntion)
{
    //init
    wIndex = 0;
    indepVariables.clear();
    interBegin.clear();

    //iterate till final
    std::vector<virtualOper>::const_iterator vo = iterFucntion;
    std::stack<virtualDependencyTreeNode*> localNodeStack;
    auto find = indepVariables.find(0);

    //intermediates
    virtualDependencyTreeNode* node = NULL;
    virtualDependencyTreeNode* node1 = NULL;
    virtualDependencyTreeNode* node2 = NULL;
    virtualDependencyTreeNode* node3 = NULL;

    //iterate till f(x)
    do
    {
        switch(vo->operType)
        {
        case VR_VAR1_INDEX:
            node = NULL;
            find = indepVariables.find(vo->index);
            if(find != indepVariables.end())
            {
                // if the variable is encountered for the first time put in the map of dependents
                node = new virtualDependencyTreeNode(vo->operType, wIndex, vo);
                indepVariables[vo->index] = node;

                // all new variables need their own trivial intermediate expression
                // create a = inter
                instr->push_back(virtualOper(vo->operType, 0, vo->index));
                instr->push_back(virtualOper(VR_ASSIGN, 0, wIndex));

                // increment only if the variable is encountered for the first time
                ++wIndex;
            }
            else
            {
                node = indepVariables[vo->index];
            }
            localNodeStack.push(node);
            break;

        case VR_CONST:
            //a constant is always considered to a be a new intermediate
            node = new virtualDependencyTreeNode(vo->operType, wIndex, vo);

            // create const = inter
            instr->push_back(virtualOper(vo->operType, vo->value, wIndex));
            instr->push_back(virtualOper(VR_ASSIGN, 0, wIndex));

            localNodeStack.push(node);
            ++wIndex;
            break;;

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
            node3 = new virtualDependencyTreeNode(vo->operType, wIndex, vo);
            node1->connectTo(node3);
            node2->connectTo(node3);
            localNodeStack.push(node3);

            //create a b binop = inter
            instr->push_back(virtualOper(VR_INTER_INDEX, 0, node1->getWIndex())); interBegin.push_back(instr->end() - 1);
            instr->push_back(virtualOper(VR_INTER_INDEX, 0, node1->getWIndex()));
            instr->push_back(*vo);
            instr->push_back(virtualOper(VR_ASSIGN, 0, wIndex));

            ++wIndex;

        case VR_UNA_ADD:
        case VR_UNA_SUB:
            //add operands to the stack
            node1 = localNodeStack.top();
            localNodeStack.pop();

            //add connections
            node3 = new virtualDependencyTreeNode(vo->operType, wIndex, vo);
            node1->connectTo(node3);
            localNodeStack.push(node3);

            //create a b binop = inter
            instr->push_back(virtualOper(VR_INTER_INDEX, 0, node1->getWIndex())); interBegin.push_back(instr->end() - 1);
            instr->push_back(*vo);
            instr->push_back(virtualOper(VR_ASSIGN, 0, wIndex));

            ++wIndex;

        default:
            break;
        }

        ++vo;
    }while(vo->last != 1);

    if(localNodeStack.size() == 1)
        return localNodeStack.top();
    else
        return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// build the final reverse mode automatic differentiation function stack for f(xi, x2, x3...)
int virtualDependencyTree::getReverseModeAutoDiffInst()
{
    //local
    virtualDependencyTreeNode* item = NULL;
    virtualDependencyTreeNode* itemPrev = NULL;

    //loop over the each independendent variable
    std::map<unsigned int, virtualDependencyTreeNode*>::const_iterator indepVariables_end = indepVariables.end();
    for(std::map<unsigned int, virtualDependencyTreeNode*>::const_iterator it1 = indepVariables.begin(); it1 != indepVariables_end; ++it1)
    {
        //loop over each independent variables' possibly multiple paths( this needs to be summed)
        std::vector<virtualDependencyTreeNode*>::const_iterator it1_second_getAllTo_end = it1->second->getAllTo().end();
        for(std::vector<virtualDependencyTreeNode*>::const_iterator it2 = it1->second->getAllTo().begin(); it2 != it1_second_getAllTo_end; ++it2)
        {
            unsigned int c = 0;
            virtualDependencyTreeNode* aNodeInVar = *it2;
            while(aNodeInVar->getAllTo().size() != 0)
            {
                item = *(aNodeInVar->getAllTo().begin());
                aNodeInVar = item;
                c++;
                if(c > 1)
                {
                    addIntermediateDerivativeInstr(item, itemPrev);
                }
                itemPrev = item;
            }
        }
    }
    return 0;
}


