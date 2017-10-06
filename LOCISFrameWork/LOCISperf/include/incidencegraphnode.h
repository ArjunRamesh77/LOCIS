#pragma once

#include <vector>
#include "lociscommon.h"
#include <map>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// defines
#define MT_UNSET -1
#define MT_VARIABLE_NODE 0
#define MT_EQUATION_NODE 1

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a graph node
class incidenceGraphNode
{
    int type;
    unsigned int index;
    std::map<unsigned int,incidenceGraphNode*> variableNodes;

public:
    incidenceGraphNode();
    ~incidenceGraphNode();

    int getType() const;
    void setType(int value);
    const std::map<unsigned int, incidenceGraphNode *> &getAllVariableNodes();
    unsigned int getIndex() const;
    void setIndex(unsigned int value);
    bool setVariableNode(unsigned int index, incidenceGraphNode* VariableNode);
};
