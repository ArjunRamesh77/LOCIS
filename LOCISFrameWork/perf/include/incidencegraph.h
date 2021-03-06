#pragma once

#include "incidencegraphnode.h"
#include <stddef.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Full bipartite graph
class incidenceGraph
{
    unsigned int numEquationNodes;
    unsigned int numVariableNodes;
    std::vector<incidenceGraphNode*> equationNodes;
    std::vector<incidenceGraphNode*> variableNodes;

public:
    incidenceGraph();
    incidenceGraph(unsigned int numU, unsigned int numV);
    ~incidenceGraph();

    unsigned int getNumEquationNodes() const;
    unsigned int getNumVariableNodes() const;
    void addEquationNodes(unsigned int num);
    void addVariableNodes(unsigned int num);
    bool addEdge(unsigned int indexEquation, unsigned int indexVariable);
    void addMatching(incidenceGraphNode *equationNode, incidenceGraphNode *variableNode);

    //Matrix functions
    void initializeMatchingOnGraph(std::list<incidenceGraphNode *> &unmatched);
    void unMatchGraph();
    bool checkIfPerfectMatching();
    void cleanGraph();
    void resetAllIndicators();
    incidenceGraphNode* getEquationAt(unsigned int index);
    incidenceGraphNode* getVariableAt(unsigned int index);

};
