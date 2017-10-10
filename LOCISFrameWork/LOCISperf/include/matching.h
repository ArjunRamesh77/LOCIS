#pragma once

#include "incidencegraph.h"
#include <queue>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Base class for implementing a matching algorithm
class matchingBase
{
protected:
    incidenceGraph* biGraph;
    std::queue<incidenceGraphNode*> bfsQueue;
    unsigned int bfsDepth;
    unsigned int bfsNumPopsTillNextDepthIncr;
    unsigned int bfsNumPops;
    unsigned int bfsUnmatchedVariableDepth;
    bool bfsGotUnmatched;
    std::vector<incidenceGraphNode*> unmatchedEquationNodes;
    std::vector<incidenceGraphNode*> unmatchedVariableNodes;

public:
    matchingBase(incidenceGraph* biGraph_arg);
    matchingBase();
    ~matchingBase();

    incidenceGraph *getBiGraph() const;
    void setBiGraph(incidenceGraph *value);
    bool doBfs(incidenceGraphNode* sourceNode);
    bool runBfsDriver(incidenceGraphNode* sourceNode);
    incidenceGraphNode* doDfs(incidenceGraphNode* sourceNode);
    void doFirstMatching();

    //virtual
    virtual incidenceGraph* doMatching() = 0;
};


