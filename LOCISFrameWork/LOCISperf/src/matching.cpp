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

bool matchingBase::doBfs(incidenceGraphNode *sourceNode)
{
    //initialize
    bfsDepth = 1;
    bfsNumPops = 0;
    bfsNumPopsTillNextDepthIncr = sourceNode->getAllNodes().size();
    bfsUnmatchedVariableDepth = 0;
    bfsGotUnmatched = false;
    unmatchedVariableNodes.clear();

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
            sourceNode = bfsQueue.front();
            bfsQueue.pop();
            bfsNumPops++;
        }
    }
    return false;
}

bool matchingBase::runBfsDriver(incidenceGraphNode *sourceNode)
{

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

        //visit all the variables and push matched variables to the queue and unmatched variables tp the vector
        std::map<unsigned int,incidenceGraphNode*>::const_iterator varEnd = sourceNode->getAllNodes().end();
        for(std::map<unsigned int, incidenceGraphNode*>::const_iterator it = sourceNode->getAllNodes().begin(); it != varEnd; ++it)
        {
            // Now check if the variables are matched to find an augmenting path
            if(it->second->isMatched())
            {
                //push to queue only if it has visitable edges
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
        bfsQueue.push(sourceNode->getMatching());
    }
    return true;
}

incidenceGraphNode *matchingBase::doDfs(incidenceGraphNode *sourceNode)
{
    return NULL;
}

void matchingBase::doFirstMatching()
{
    if(biGraph)
    {
        unmatchedEquationNodes.clear();
        biGraph->initializeMatchingOnGraph(unmatchedEquationNodes);
    }
}

