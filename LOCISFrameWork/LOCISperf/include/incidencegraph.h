#pragma once

#include "incidencegraphnode.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Full bipartite graph
class incidenceGraph
{
    unsigned int numEquationNodes;
    unsigned int numVariableNodes;
    std::vector<incidenceGraphNode*> equationNodes;
    std::vector<incidenceGraphNode*> variableNodes;

    //Matrix styles
    std::vector<std::pair<unsigned int,unsigned int>> matrixCOO;

public:
    incidenceGraph();
    ~incidenceGraph();

    unsigned int getNumEquationNodes() const;
    unsigned int getNumVariableNodes() const;
    void addEquationNode();
    void addVariableNode();
    bool addEdge(unsigned int indexEquation, unsigned int indexVariable);
    void addMatching(incidenceGraphNode *equationNode, incidenceGraphNode *variableNode);

    //Matrix functions
    void matrixCOOClear();
    void matrixCOOAddCoordinate(unsigned int row, unsigned int col);

    bool createBipartiteEVGraphFromMatrixCOO();
    void initializeMatchingOnGraph(std::vector<incidenceGraphNode *> &unmatched);
    void unMatchGraph();
};
