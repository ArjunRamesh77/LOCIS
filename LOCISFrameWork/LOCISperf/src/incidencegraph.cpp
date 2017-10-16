#include "incidencegraph.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// E-V graph manager
incidenceGraph::incidenceGraph() :
    numEquationNodes(0),
    numVariableNodes(0),
    equationNodes(),
    variableNodes(),
    matrixCOO()
{

}

incidenceGraph::incidenceGraph(unsigned int numU, unsigned int numV) :
    incidenceGraph()
{
    if(numU > 0 && numV > 0)
    {
        addEquationNodes(numU);
        addVariableNodes(numV);
    }
}

incidenceGraph::~incidenceGraph()
{
    DELETE_VECTOR_ENTRIES(equationNodes)
    DELETE_VECTOR_ENTRIES(variableNodes)
}

unsigned int incidenceGraph::getNumEquationNodes() const
{
    return equationNodes.size();
}

unsigned int incidenceGraph::getNumVariableNodes() const
{
    return variableNodes.size();
}

void incidenceGraph::addEquationNodes(unsigned int num)
{
    numEquationNodes = 0;
    equationNodes.clear();
    for(unsigned int i = 0; i < num; ++i)
    {
        incidenceGraphNode* eNode = new incidenceGraphNode;
        numEquationNodes++;
        eNode->setIndex(i);
        equationNodes.push_back(eNode);
    }
}

void incidenceGraph::addVariableNodes(unsigned int num)
{
    numVariableNodes = 0;
    variableNodes.clear();
    for(unsigned int i = 0; i < num; ++i)
    {
        incidenceGraphNode* eNode = new incidenceGraphNode;
        numVariableNodes++;
        eNode->setIndex(i);
        variableNodes.push_back(eNode);
    }
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

void incidenceGraph::MatrixCOOAddCoordinate(unsigned int row, unsigned int col)
{
    matrixCOO.push_back({row, col});
}

bool incidenceGraph::createBipartiteEVGraphFromMatrixCOO()
{
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
    std::cout<<" INITIAL MATCHING::\n";
    bool foundMatching(false);
    std::vector<incidenceGraphNode*>::const_iterator equationNodes_end = equationNodes.end();
    for(std::vector<incidenceGraphNode*>::const_iterator eqIt = equationNodes.begin(); eqIt != equationNodes_end; eqIt++)
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
                std::cout<<" Matched E" << (*eqIt)->getIndex() <<" with V"<<(*vrIt)->getIndex() <<"\n";
                break;
            }
        }
        if(foundMatching == false)
            unmatched.push_back(*eqIt);
    }
    std::cout<<" INITIAL MATCHING ENDS::\n";
}

void incidenceGraph::unMatchGraph()
{
    //unmatch all equations
    std::vector<incidenceGraphNode*>::const_iterator equationNodes_end = equationNodes.end();
    for(std::vector<incidenceGraphNode*>::const_iterator it = equationNodes.begin(); it != equationNodes_end; ++it)
    {
        (*it)->setMatching(NULL);
    }

    //unmatch all variables
    std::vector<incidenceGraphNode*>::const_iterator variableNodes_end = variableNodes.end();
    for(std::vector<incidenceGraphNode*>::const_iterator it = variableNodes.begin(); it != variableNodes_end; ++it)
    {
        (*it)->setMatching(NULL);
    }
}
