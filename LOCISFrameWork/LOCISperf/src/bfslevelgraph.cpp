#include "bfslevelgraph.h"

bfsLevelGraph::bfsLevelGraph() :
    equationNodes(),
    variableNodes()
{

}

bfsLevelGraph::~bfsLevelGraph()
{
    DELETE_MAP_ENTRIES(equationNodes)
            DELETE_MAP_ENTRIES(variableNodes)
}

void bfsLevelGraph::setAsUnmatchedVariableNode(incidenceGraphNode *node)
{
    unsigned int index = node->getIndex();
    auto find = variableNodes.find(index);

    if(find == variableNodes.end())
    {
        incidenceGraphNode* node = NULL;
        node = new incidenceGraphNode();
        node->setIndex(index);
        node->setType(MT_VARIABLE_NODE);
        variableNodes[index] = node;
    }

    find = unmatchedVariableNodes.find(index);
    if(find == unmatchedVariableNodes.end())
    {
        unmatchedVariableNodes[index] = variableNodes[index];
    }
}

bool bfsLevelGraph::addEdge(incidenceGraphNode *node1, inicidenceGraphNode *node2, bool match = false)
{
    unsigned int indexNode1 = node1->getIndex();
    unsigned int indexNode2 = node2->getIndex();

    if(node1->getType() == node2->getType())
    {
        return false;
    }

    // Add the nodes if they do not already exist
    //node1
    incidenceGraphNode* newNode1 = NULL;
    incidenceGraphNode* newNode2 = NULL;
    if(node1->getType() == MT_EQUATION_NODE)
    {
        auto find = equationNodes.find(indexNode1);
        if(find == equationNodes.end())
        {
            incidenceGraphNode* node = new incidenceGraphNode();
            node->setIndex(indexNode1);
            node->setType(MT_EQUATION_NODE);
            equationNodes[indexNode1] = node;
            newNode1 = node;
        }
        else
        {
            newNode1 = equationNodes[indexNode1];
        }
    }
    else
    {
        auto find = variableNodes.find(indexNode1);
        if(find == variableNodes.end())
        {
            incidenceGraphNode* node = new incidenceGraphNode();
            node->setIndex(indexNode1);
            node->setType(MT_VARIABLE_NODE);
            variableNodes[indexNode1] = node;
            newNode1 = node;
        }
        else
        {
            newNode1 = variableNodes[indexNode1];
        }
    }

    //node2
    if(node2->getType() == MT_EQUATION_NODE)
    {
        auto find = equationNodes.find(indexNode2);
        if(find == equationNodes.end())
        {
            incidenceGraphNode* node = new incidenceGraphNode();
            node->setIndex(indexNode2);
            node->setType(MT_EQUATION_NODE);
            equationNodes[indexNode2] = node;
            newNode2 = node;
        }
        else
        {
            newNode2 = equationNodes[indexNode2];
        }
    }
    else
    {
        auto find = variableNodes.find(indexNode2);
        if(find == variableNodes.end())
        {
            incidenceGraphNode* node = new incidenceGraphNode();
            node->setIndex(indexNode2);
            node->setType(MT_VARIABLE_NODE);
            variableNodes[indexNode2] = node;
            newNode2 = node;
        }
        else
        {
            newNode2 = variableNodes[indexNode2];
        }
    }

    //Now add the edge
    if(newNode2->setNode(indexNode1, newNode2))
    {
        if(match)
        {
            newNode2->setMatching(newNode1);
        }
        return true;
    }
    return false;
}

std::map<unsigned int, incidenceGraphNode *> bfsLevelGraph::getEquationNodes() const
{
    return equationNodes;
}

std::map<unsigned int, incidenceGraphNode *> bfsLevelGraph::getVariableNodes() const
{
    return variableNodes;
}
