#include "incidencegraphnode.h"
#include <stddef.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//incidence graph node
incidenceGraphNode::incidenceGraphNode() :
    nodes(),
    matching(NULL),
    aliveIndex(0),
    index(0),
    lowlink(0)
{

}

incidenceGraphNode::~incidenceGraphNode()
{
    //DELETE_VECTOR_ENTRIES(nodes)
}

unsigned int incidenceGraphNode::getAliveIndex() const
{
    return aliveIndex;
}

void incidenceGraphNode::setAliveIndex(unsigned int value)
{
    aliveIndex = value;
}

unsigned int incidenceGraphNode::getIndex() const
{
    return index;
}

void incidenceGraphNode::setIndex(unsigned int value)
{
    index = value;
}

const std::list<incidenceGraphNode*> &incidenceGraphNode::getAllNodes()
{
    return nodes;
}

incidenceGraphNode *incidenceGraphNode::getMatching() const
{
    return matching;
}

void incidenceGraphNode::addNode(incidenceGraphNode *node)
{
    nodes.push_back(node);
}

void incidenceGraphNode::setMatching(incidenceGraphNode *value)
{
    matching = value;
}

void incidenceGraphNode::setMatchingAsUnmacthed()
{
    matching = NULL;
}

unsigned int incidenceGraphNode::getLowlink() const
{
    return lowlink;
}

void incidenceGraphNode::setLowlink(unsigned int value)
{
    lowlink = value;
}

