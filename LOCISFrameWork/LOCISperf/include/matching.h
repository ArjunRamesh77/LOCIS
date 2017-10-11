#pragma once

#include "incidencegraph.h"
#include "bfssecondarygraph.h"
#include <queue>
#include <stack>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Base class for implementing a matching algorithm
class matchingBase
{
protected:
    incidenceGraph* biGraph;
    std::queue<incidenceGraphNode*> bfsQueue;
    std::stack<incidenceGraphNode*> dfsStack;
    unsigned int bfsDepth;
    unsigned int bfsNumPopsTillNextDepthIncr;
    unsigned int bfsNumPops;
    unsigned int bfsUnmatchedVariableDepth;
    bool bfsGotUnmatched;
    std::vector<incidenceGraphNode*> unmatchedEquationNodes;
    std::vector<incidenceGraphNode*> unmatchedVariableNodes;
    bool dfsGotUnMatched;
    unsigned int visitCount;
    std::stack<std::pair<incidenceGraphNode*,incidenceGraphNode*>> dfsMatchUnmatchPairs;
    bfsSecondaryGraph* bfsResult;

public:
    matchingBase(incidenceGraph* biGraph_arg);
    matchingBase();
    ~matchingBase();

    incidenceGraph *getBiGraph() const;
    void setBiGraph(incidenceGraph *value);
    bool runBfsDriver(incidenceGraphNode* sourceNode);
    bool doBfs(incidenceGraphNode* sourceNode);
    bool runDfsDriver(incidenceGraphNode* sourceNode);
    bool doDfs(incidenceGraphNode* sourceNode);
    void doFirstMatching();

    //virtual
    virtual incidenceGraph* doMatching() = 0;
};


