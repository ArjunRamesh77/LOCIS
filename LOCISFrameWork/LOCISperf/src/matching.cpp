#include "matching.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//incidence graph node
incidenceGraphNode::incidenceGraphNode() :
    type(MT_UNSET),
    index(-1),
    variableNodes()
{

}

incidenceGraphNode::~incidenceGraphNode()
{
    DELETE_VECTOR_ENTRIES(variableNodes);
}

unsigned int incidenceGraphNode::getIndex() const
{
    return index;
}

int incidenceGraphNode::getType() const
{
    return type;
}

const std::map<unsigned int, incidenceGraphNode*> &incidenceGraphNode::getAllVariableNodes()
{
    return variableNodes;
}

void incidenceGraphNode::setType(int value)
{
    type = value;
}

bool incidenceGraphNode::setVariableNode(unsigned int index, incidenceGraphNode *variableNode)
{
    auto find = variableNodes.find(index);
    if(find == variableNodes.end())
    {
        variableNodes[index] = variableNode;
        return true;
    }
    return false;
}

void incidenceGraphNode::setIndex(unsigned int value)
{
    index = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// E-V graph manager
incidenceGraph::incidenceGraph() :
    numEquations(0),
    numVariables(0),
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

void incidenceGraph::setNumEquations(unsigned int value)
{
    numEquations = value;
}

void incidenceGraph::setNumVariables(unsigned int value)
{
    numVariables = value;
}

void incidenceGraph::addEquationNode()
{
    incidenceGraphNode* eNode = new incidenceGraphNode;
    numEquationNodes++;
    eNode->setIndex(numEquationNodes);
    eNode->setType(MT_EQUATION_NODE);
    equationNodes.push_back(eNode);
}

void incidenceGraph::addVariableNode()
{
    incidenceGraphNode* eNode = new incidenceGraphNode;
    numVariableNodes++;
    eNode->setIndex(numVariableNodes);
    eNode->setType(MT_VARIABLE_NODE);
    variableNodes.push_back(eNode);
}

bool incidenceGraph::addVariableNodeToEquationNode(unsigned int indexEquation, unsigned int indexVariable)
{
    if((indexEquation < numEquationNodes) && (indexVariable < numVariableNodes))
    {
        (*equationNodes.at(indexEquation))->setVariableNode(indexVariable, (*variableNodes.at(indexVariable)));
        return true;
    }
    return false;
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
    for(unsigned int i = 0; i < numEquations; ++i)
        addEquationNode();

    for(unsigned int i = 0; i < numVariables; ++i)
        addVariableNode();

    //Loop over entries in the matrixCOO to build the bipartite graph
    // Applies to a general matrix
    for(std::vector<std::pair<unsigned int, unsigned int>>::const_iterator it = matrixCOO.begin(); it != matrixCOO.end(); ++it)
    {
        // Row indicates the equation index, column indicates the incidence of a variable
        if(!addVariableNodeToEquationNode(it->first, it->second))
            return false;
    }

    return true;
}
