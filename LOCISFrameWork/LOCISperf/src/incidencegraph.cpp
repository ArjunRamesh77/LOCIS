#include "incidencegraph.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// E-V graph manager
incidenceGraph::incidenceGraph() :
    numEquationNodes(0),
    numVariableNodes(0),
    equationNodes(),
    variableNodes()
{

}

incidenceGraph::~incidenceGraph()
{
    DELETE_VECTOR_ENTRIES(equationNodes)
    DELETE_VECTOR_ENTRIES(variableNodes)
}

unsigned int incidenceGraph::getNumEquationNodes() const
{
    return numEquationNodes;
}

unsigned int incidenceGraph::getNumVariableNodes() const
{
    return numVariableNodes;
}

void incidenceGraph::addEquationNode()
{
    incidenceGraphNode* eNode = new incidenceGraphNode;
    numEquationNodes++;
    eNode->setIndex(numEquationNodes);
    equationNodes.push_back(eNode);
}

void incidenceGraph::addVariableNode()
{
    incidenceGraphNode* eNode = new incidenceGraphNode;
    numVariableNodes++;
    eNode->setIndex(numVariableNodes);
    variableNodes.push_back(eNode);
}

bool incidenceGraph::addEdge(unsigned int indexEquation, unsigned int indexVariable)
{
    if((indexEquation < numEquationNodes) && (indexVariable < numVariableNodes))
    {
        incidenceGraphNode* equationNode = equationNodes.at(indexEquation);
        incidenceGraphNode* variableNode = variableNodes.at(indexVariable);

        equationNode->addNode(variableNodes.at(indexVariable));
        equationNode->setMatchingAsUnmacthed();

        variableNode->addNode(equationNodes.at(indexEquation));
        variableNode->setMatchingAsUnmacthed();

        return true;
    }
    return false;
}

void incidenceGraph::addMatching(incidenceGraphNode* equationNode, incidenceGraphNode* variableNode)
{
    equationNode->setMatching(variableNode);
    variableNode->setMatching(equationNode);
}

void incidenceGraph::matrixCOOClear()
{
    matrixCOO.clear();
}

void incidenceGraph::matrixCOOAddCoordinate(unsigned int row, unsigned int col)
{
    std::pair<unsigned int, unsigned int> rowCol{row, col};
    matrixCOO.push_back(rowCol);
}

bool incidenceGraph::createBipartiteEVGraphFromMatrixCOO()
{
    // First create the unconnected Graph
    for(unsigned int i = 0; i < numEquationNodes; ++i)
        addEquationNode();

    for(unsigned int i = 0; i < numVariableNodes; ++i)
        addVariableNode();

    //Loop over entries in the matrixCOO to build the bipartite graph
    // Applies to a general matrix
    for(std::vector<std::pair<unsigned int, unsigned int>>::const_iterator it = matrixCOO.begin(); it != matrixCOO.end(); ++it)
    {
        // Row indicates the equation index, column indicates the incidence of a variable
        if(!addEdge(it->first, it->second))
        {
            return false;
        }
    }

    return true;
}

void incidenceGraph::initializeMatchingOnGraph(std::list<incidenceGraphNode*>& unmatched)
{
    //Loop over Equation Nodes and match with first Unmatched variable
    bool foundMatching(false);
    for(std::vector<incidenceGraphNode*>::const_iterator eqIt = equationNodes.begin(); eqIt != equationNodes.end(); ++eqIt)
    {
        foundMatching = false;
        std::list<incidenceGraphNode*>::const_iterator varEnd = (*eqIt)->getAllNodes().end();
        for(std::list<incidenceGraphNode*>::const_iterator vrIt = (*eqIt)->getAllNodes().begin();
            vrIt != varEnd; ++vrIt)
        {
            if(!(*vrIt)->getMatching())
            {
                addMatching(*eqIt, *vrIt);
                foundMatching = true;
                break;
            }
        }
        if(foundMatching)
            unmatched.push_back(*eqIt);
    }
}

void incidenceGraph::unMatchGraph()
{
    //unmatch all equations
    for(std::vector<incidenceGraphNode*>::const_iterator it = equationNodes.begin(); it != equationNodes.end(); ++it)
    {
        (*it)->setMatching(NULL);
    }

    //unmatch all variables
    for(std::vector<incidenceGraphNode*>::const_iterator it = variableNodes.begin(); it != variableNodes.end(); ++it)
    {
        (*it)->setMatching(NULL);
    }
}
