#include "virtualdependencytree.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dependency tree node

virtualDependencyTreeNode::virtualDependencyTreeNode() :
    virtualDependencyTreeNode(0,0)
{

}

virtualDependencyTreeNode::virtualDependencyTreeNode(__int8_t nodeType_arg, unsigned int wIndex_arg) :
    nodeType(nodeType_arg),
    wIndex(wIndex_arg),
    to(),
    wIndexDeriv(NULL)
{

}

virtualDependencyTreeNode::~virtualDependencyTreeNode()
{
    DELETE_VECTOR_ENTRIES(to)
}

unsigned int *virtualDependencyTreeNode::getWIndexDeriv() const
{
    return wIndexDeriv;
}

void virtualDependencyTreeNode::setWIndexDeriv(unsigned int *value)
{
    wIndexDeriv = new unsigned int(*value);
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

unsigned int virtualDependencyTreeNode::getWIndex() const
{
    return wIndex;
}

void virtualDependencyTreeNode::setNodeType(const __int8_t &value)
{
    nodeType = value;
}

void virtualDependencyTreeNode::setWIndex(const unsigned int &value)
{
    wIndex = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dependency tree
virtualDependencyTree::virtualDependencyTree()
{

}

virtualDependencyTree::~virtualDependencyTree()
{

}

std::vector<virtualOper> *virtualDependencyTree::getInstr() const
{
    return instr;
}

void virtualDependencyTree::setInstr(std::vector<virtualOper> *value)
{
    instr = value;
}

int virtualDependencyTree::getJacType() const
{
    return jacType;
}

void virtualDependencyTree::setJacType(int value)
{
    jacType = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// build the final reverse mode automatic differentiation function stack for f(xi, x2, x3...)
int virtualDependencyTree::getReverseModeAutoDiffInst()
{
    //local
    virtualDependencyTreeNode* item = NULL;
    virtualDependencyTreeNode* itemPrev = NULL;

    //find max index to get correct order
    std::map<unsigned int, virtualDependencyTreeNode*>::const_iterator indepVariables_end = indepVariables.end();
    std::map<unsigned int, virtualDependencyTreeNode*>::const_iterator itb= indepVariables.begin();
    unsigned int maxIndex = itb->first;
    unsigned int minIndex = itb->first;
    while(itb != indepVariables_end)
    {
        if(itb->first > maxIndex)
        {
            maxIndex = itb->first;
        }
        if(itb->first < minIndex)
        {
            minIndex = itb->first;
        }
        ++itb;
    }

    //loop in order
    std::map<unsigned int, virtualDependencyTreeNode*>::const_iterator it1;
    for(unsigned int i = minIndex; i <= maxIndex; ++i)
    //for(std::map<unsigned int, virtualDependencyTreeNode*>::const_iterator it1 = indepVariables.begin(); it1 != indepVariables_end; ++it1)
    {
        //generate derivatives in the correct order
        auto find = indepVariables.find(i);
        if(find == indepVariables.end())
        {
            continue;
        }
        it1 = find;

        //Check for fixed vars
        if(it1->second == NULL)
            continue;

        //loop over each independent variables' possibly multiple paths( this needs to be summed)
        std::vector<std::vector<unsigned int>*> piSigmaNumbers;
        std::vector<virtualDependencyTreeNode*>::const_iterator it1_second_getAllTo_end = it1->second->getAllTo().end();
        for(std::vector<virtualDependencyTreeNode*>::const_iterator it2 = it1->second->getAllTo().begin(); it2 != it1_second_getAllTo_end; ++it2)
        {
            // init
            bZeroPi = false;
            std::vector<unsigned int>* piNumbers = new std::vector<unsigned int>;

            // first find derivative w.r.t the root variable
            item = *it2;
            itemPrev = it1->second;
            addIntermediateDerivativeInstr(item, itemPrev, true);
            piNumbers->push_back(*(itemPrev->getWIndexDeriv()));
            itemPrev = item;

            while(item->getAllTo().size() != 0)
            {
                item = *(item->getAllTo().begin());
                addIntermediateDerivativeInstr(item, itemPrev);
                piNumbers->push_back(*(itemPrev->getWIndexDeriv()));
                itemPrev = item;
            }
            piSigmaNumbers.push_back(piNumbers);
        }

        //now derivative with respect to the outer for's independent variable is available
        if(piSigmaNumbers.size() > 0)
        {
            addFinalDerivative(piSigmaNumbers);
            instr->push_back(virtualOper(VR_DERIV_INDEX, it1->first));
        }
    }

    // the derivatives w.r.t to all independent variables have been added
    (instr->end() - 1)->signal = VR_SIGNAL_GRADIENT_COMPLETE;
    return 0;
}


