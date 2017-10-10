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
    std::map<unsigned int,incidenceGraphNode*> nodes;
    incidenceGraphNode* matching;
    bool bIsMatched;
    bool bIsAlive;

public:
    incidenceGraphNode();
    ~incidenceGraphNode();

    int getType() const;
    void setType(int value);
    const std::map<unsigned int, incidenceGraphNode *> &getAllNodes();
    unsigned int getIndex() const;
    unsigned int getIndexOfMatching() const;
    incidenceGraphNode *getMatching() const;
    bool getBIsAlive() const;
    void setIndex(unsigned int value);
    bool setNode(unsigned int index, incidenceGraphNode* node);
    void setMatching(incidenceGraphNode *value);
    bool isMatched();
    void setMatchingAsUnmacthed();
    void setBIsAlive(bool value);
};
