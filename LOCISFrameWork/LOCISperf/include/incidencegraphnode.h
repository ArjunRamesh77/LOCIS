#pragma once

#include <vector>
#include "lociscommon.h"
#include <iostream>
#include <list>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// defines
#define MT_UNSET -1
#define MT_VARIABLE_NODE 0
#define MT_EQUATION_NODE 1

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a graph node
class incidenceGraphNode
{
    std::list<incidenceGraphNode*> nodes;
    incidenceGraphNode* matching;
    unsigned int aliveIndex;
    unsigned int index;

public:
    incidenceGraphNode();
    ~incidenceGraphNode();

    const std::list<incidenceGraphNode *> &getAllNodes();
    incidenceGraphNode *getMatching() const;
    void addNode(incidenceGraphNode* node);
    void setMatching(incidenceGraphNode *value);
    void setMatchingAsUnmacthed();
    unsigned int getAliveIndex() const;
    void setAliveIndex(unsigned int value);
    unsigned int getIndex() const;
    void setIndex(unsigned int value);
};
