#pragma once

#include "incidencegraph.h"
#include <queue>
#include <stack>
#include <map>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return codes
#define HC_GRAPH_NULL -1
#define HC_DFS_NORESULT -2

#define HC_SUCCESS 0

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Base class for implementing a matching algorithm
class matchingBase
{
protected:
    incidenceGraph* biGraph;

    std::list<incidenceGraphNode*> unmatchedEquationNodes;
    std::list<incidenceGraphNode*> unmatchedVariableNodes;
    unsigned int bfsAliveIndex;

public:
    matchingBase(incidenceGraph* biGraph_arg);
    matchingBase();
    ~matchingBase();

    incidenceGraph *getBiGraph() const;
    void setBiGraph(incidenceGraph *value);
    bool doBfs();
    bool doDfs(incidenceGraphNode *startNode);
    void doFirstMatching();
    int matchingHopcroftKarp();

};


