#include "incidencegraphnode.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//incidence graph node
incidenceGraphNode::incidenceGraphNode() :
    type(MT_UNSET),
    index(-1),
    variableNodes()
{

}

incidenceGraphNode::~incidenceGraphNode()
{
    DELETE_VECTOR_ENTRIES(variableNodes);
}

unsigned int incidenceGraphNode::getIndex() const
{
    return index;
}

int incidenceGraphNode::getType() const
{
    return type;
}

const std::map<unsigned int, incidenceGraphNode*> &incidenceGraphNode::getAllVariableNodes()
{
    return variableNodes;
}

void incidenceGraphNode::setType(int value)
{
    type = value;
}

bool incidenceGraphNode::setVariableNode(unsigned int index, incidenceGraphNode *variableNode)
{
    auto find = variableNodes.find(index);
    if(find == variableNodes.end())
    {
        variableNodes[index] = variableNode;
        return true;
    }
    return false;
}

void incidenceGraphNode::setIndex(unsigned int value)
{
    index = value;
}
