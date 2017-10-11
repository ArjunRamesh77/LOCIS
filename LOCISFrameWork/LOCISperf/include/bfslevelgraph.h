#pragma once

#include "incidencegraph.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// level graph, result of bfs
class bfsLevelGraph
{
    //Full level graph
    std::map<unsigned int, incidenceGraphNode*> equationNodes;
    std::map<unsigned int, incidenceGraphNode*> variableNodes;

    //unmatched
    std::map<unsigned int, incidenceGraphNode*> unmatchedVariableNodes;

public:
    bfsLevelGraph();
    ~bfsLevelGraph();

    void setAsUnmatchedVariableNode(incidenceGraphNode* node);
    bool addEdge(incidenceGraphNode* node1, inicidenceGraphNode* node2, bool match);
    std::map<unsigned int, incidenceGraphNode *> getEquationNodes() const;
    std::map<unsigned int, incidenceGraphNode *> getVariableNodes() const;
};
