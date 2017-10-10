#include "incidencegraphnode.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//incidence graph node
incidenceGraphNode::incidenceGraphNode() :
    type(MT_UNSET),
    index(-1),
    nodes(),
    bIsMatched(false)
{

}

incidenceGraphNode::~incidenceGraphNode()
{
   DELETE_MAP_ENTRIES(nodes);
}

unsigned int incidenceGraphNode::getIndex() const
{
    return index;
}

int incidenceGraphNode::getType() const
{
    return type;
}

const std::map<unsigned int, incidenceGraphNode*> &incidenceGraphNode::getAllNodes()
{
    return nodes;
}

incidenceGraphNode *incidenceGraphNode::getMatching() const
{
    return matching;
}

bool incidenceGraphNode::getBIsAlive() const
{
    return bIsAlive;
}

void incidenceGraphNode::setType(int value)
{
    type = value;
}

bool incidenceGraphNode::setNode(unsigned int index, incidenceGraphNode *node)
{
    auto find = nodes.find(index);
    if(find == nodes.end())
    {
        nodes[index] = node;
        return true;
    }
    return false;
}

void incidenceGraphNode::setIndex(unsigned int value)
{
    index = value;
}

void incidenceGraphNode::setMatching(incidenceGraphNode *value)
{
    bIsMatched = true;
    matching = value;
}

void incidenceGraphNode::setMatchingAsUnmacthed()
{
    bIsMatched = false;
    matching = NULL;
}

void incidenceGraphNode::setBIsAlive(bool value)
{
    bIsAlive = value;
}

bool incidenceGraphNode::isMatched()
{
    return bIsMatched;
}
