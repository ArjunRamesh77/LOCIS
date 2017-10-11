#include "matching.h"

matchingBase::matchingBase(incidenceGraph *biGraph_arg) :
    biGraph(biGraph_arg),
    bfsQueue(),
    unmatchedEquationNodes()
{

}

matchingBase::matchingBase() :
    biGraph(NULL)
{

}

incidenceGraph *matchingBase::getBiGraph() const
{
    return biGraph;
}

void matchingBase::setBiGraph(incidenceGraph *value)
{
    biGraph = value;
}

bool matchingBase::runBfsDriver(incidenceGraphNode *sourceNode)
{
    //initialize
    bfsDepth = 0;
    bfsNumPops = 0;
    bfsNumPopsTillNextDepthIncr = 0;
    bfsUnmatchedVariableDepth = 0;
    bfsGotUnmatched = false;
    unmatchedVariableNodes.clear();
    bfsResult = new bfsSecondaryGraph;

    //main driver loop
    while(doBfs(sourceNode))
    {
        if(bfsNumPops == bfsNumPopsTillNextDepthIncr)
        {
            bfsDepth++;
            bfsNumPops = 0;
            bfsNumPopsTillNextDepthIncr = bfsQueue.size();
        }
        else
        {
            sourceNode = NULL;
            sourceNode = bfsQueue.front();
            bfsQueue.pop();
            bfsNumPops++;
        }
    }
    return true;
}

bool matchingBase::doBfs(incidenceGraphNode *sourceNode)
{
    if(sourceNode == NULL)
        return false;

    //Only serach till first unmatched variable depth
    if(bfsGotUnmatched)
    {
        if(bfsDepth > bfsUnmatchedVariableDepth)
        {
            return false;
        }
    }
    else
    {
        //possibly cyclic
        if(bfsDepth > (biGraph->getNumEquationNodes() + biGraph->getNumVariableNodes()))
        {
            return false;
        }
    }

    //for variable node
    if(sourceNode->getType() == MT_EQUATION_NODE)
    {
        //make sure only unmatched nodes get through
        if(sourceNode->isMatched())
            return true;

        //Add to bfs secondary tree
        //add to secondary tree

        //visit all the variables and push matched variables to the queue and unmatched variables tp the vector
        std::map<unsigned int,incidenceGraphNode*>::const_iterator varEnd = sourceNode->getAllNodes().end();
        for(std::map<unsigned int, incidenceGraphNode*>::const_iterator it = sourceNode->getAllNodes().begin(); it != varEnd; ++it)
        {
            // Now check if the variables are matched to find an augmenting path
            if(it->second->isMatched())
            {
                 bfsQueue.push(it->second);
            }
            else
            {
                unmatchedVariableNodes.push_back(it->second);
                bfsUnmatchedVariableDepth = bfsDepth;
                bfsGotUnmatched = true;
            }
        }
    }
    //for equation node
    else
    {
        //push the matching equation of the matching variable
        if(sourceNode->getMatching())
            bfsQueue.push(sourceNode->getMatching());
        else
            throw 1; //This cannot happen right?
    }
    return true;
}

bool matchingBase::runDfsDriver(incidenceGraphNode *sourceNode)
{
    // initialize
    visitCount = 0;
    dfsGotUnMatched = false;

    //non recursive depth first search
    while(doDfs(sourceNode))
    {
        sourceNode = NULL;
        sourceNode = dfsStack.top();
        dfsStack.pop();
    }

    //flip
    if(dfsGotUnMatched)
    {

    }
    return true;
}

bool matchingBase::doDfs(incidenceGraphNode *sourceNode)
{
    //stop if even one augmenting path discovered
    if(bfsGotUnmatched)
    {
        return false;
    }
    else
    {
        //possibly cylic
        if(visitCount > (biGraph->getNumEquationNodes() + biGraph->getNumVariableNodes()))
        {
            return false;
        }
    }

    //get pairs

    if(sourceNode->getType() == MT_VARIABLE_NODE)
    {
        if(sourceNode->isMatched())
            return true;

        //visit all the variables and push matched variables to the queue and unmatched variables tp the vector
        std::map<unsigned int,incidenceGraphNode*>::const_iterator equEnd = sourceNode->getAllNodes().end();
        for(std::map<unsigned int, incidenceGraphNode*>::const_iterator it = sourceNode->getAllNodes().begin(); it != equEnd; ++it)
        {
            // Now check if the variables are matched to find an augmenting path
            if(it->second->isMatched())
            {
                dfsStack.push(it->second);
            }
            else
            {
                dfsGotUnMatched = true;
                return false;
            }
        }
    }
    else
    {
        //push the matching variable of the matching equation
        if(sourceNode->getMatching())
            dfsStack.push(sourceNode->getMatching());
    }

    return true;
}

void matchingBase::doFirstMatching()
{
    if(biGraph)
    {
        unmatchedEquationNodes.clear();
        biGraph->initializeMatchingOnGraph(unmatchedEquationNodes);
    }
}

