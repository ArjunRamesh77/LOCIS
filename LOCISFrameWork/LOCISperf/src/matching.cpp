#include "matching.h"

matchingBase::matchingBase(incidenceGraph *biGraph_arg) :
    biGraph(biGraph_arg),
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

bool matchingBase::doBfs()
{
    //initialize
    bool bfsGotUnmatched(false);
    incidenceGraphNode* sourceNode(NULL);
    std::queue<incidenceGraphNode*> bfsQueue;
    unsigned int bfsDepth(0);
    unsigned int bfsNumPops(0);
    unsigned int bfsUnmatchedVariableDepth(0);
    unmatchedVariableNodes.clear();
    bfsAliveIndex++;

    //push all unmatched equation nodes to queue
    std::list<incidenceGraphNode*>::const_iterator unmatchedEquationNodes_end = unmatchedEquationNodes.end();
    for(std::list<incidenceGraphNode*>::const_iterator it = unmatchedEquationNodes.begin(); it != unmatchedEquationNodes_end; ++it)
    {
        //push only unmatched equations
        if(!(*it)->getMatching())
        {
            bfsQueue.push(*it);
        }
    }

    //check if bfs even required
    if(bfsQueue.size() == 0)
    {
        return false;
    }

    //Initialize queue to first unmatched equation
    unsigned int bfsNumPopsTillNextDepthIncr(bfsQueue.size());
    sourceNode = bfsQueue.front();

    //main driver loop
    //break if queue is empty or the correct variable depth has been reached
    while(bfsQueue.size() != 0)
    {
        //Only serach till first unmatched variable depth
        if(bfsGotUnmatched)
        {
            if(bfsDepth > bfsUnmatchedVariableDepth)
            {
                break;
            }
        }

        // id to know if the node was visited (for dfs traversal)
        sourceNode->setAliveIndex(bfsAliveIndex);

        //visit all the variables and push matched variables to the queue and unmatched variables tp the vector
        std::list<incidenceGraphNode*>::const_iterator varEnd = sourceNode->getAllNodes().end();
        for(std::list<incidenceGraphNode*>::const_iterator it = sourceNode->getAllNodes().begin(); it != varEnd; ++it)
        {
            // Now check if the variables are matched to find an unmatched variable
            (*it)->setAliveIndex(bfsAliveIndex);
            if((*it)->getMatching())
            {
                 bfsQueue.push((*it)->getMatching());
            }
            else
            {
                unmatchedVariableNodes.push_back((*it));
                bfsUnmatchedVariableDepth = bfsDepth;
                bfsGotUnmatched = true;
            }
        }

        // Get graph depth
        if(bfsNumPops == bfsNumPopsTillNextDepthIncr)
        {
            bfsDepth++;
            bfsNumPops = 0;
            bfsNumPopsTillNextDepthIncr = bfsQueue.size();
        }
        else
        {
            sourceNode = NULL;
            bfsQueue.pop();
            sourceNode = bfsQueue.front();
            bfsNumPops++;
        }
    }

    return bfsGotUnmatched;
}

void matchingBase::doDfs(incidenceGraphNode* startNode)
{
    // initialize
    incidenceGraphNode* sourceNode;
    std::stack<incidenceGraphNode*> dfsStack;

    //Initialize stack to the start node
    dfsStack.push(startNode);
    sourceNode = dfsStack.top();

    //non recursive depth first search
    while(dfsStack.size() != 0)
    {
        std::list<incidenceGraphNode*>::const_iterator equEnd = sourceNode->getAllNodes().end();
        for(std::list<incidenceGraphNode*>::const_iterator it = sourceNode->getAllNodes().begin(); it != equEnd; ++it)
        {
            // Now check if the variables are matched to find an augmenting path
            // only visited paths are allowed
            if((*it)->getAliveIndex() == bfsAliveIndex)
            {
                dfsStack.push(sourceNode->getMatching());
            }
        }

        //match adjacent nodes and make them unvisitable
        sourceNode->setMatching(*equEnd);
        (*equEnd)->setMatching(sourceNode);
        sourceNode->setAliveIndex(bfsAliveIndex + 1);
        (*equEnd)->setAliveIndex(bfsAliveIndex + 1);

        //next
        sourceNode = NULL;
        dfsStack.pop();
        sourceNode = dfsStack.top();
    }
}

void matchingBase::doFirstMatching()
{
    if(biGraph)
    {
        unmatchedEquationNodes.clear();
        biGraph->initializeMatchingOnGraph(unmatchedEquationNodes);
    }
}

int matchingBase::matchingHopcroftKarp()
{
    if(!biGraph)
        return HC_GRAPH_NULL;

    //initialize graph with trivial matching
    doFirstMatching();

    //main
    if(unmatchedEquationNodes.size() > 0)
    {
        //do a breadth first search on the unmatched equation nodes
        //returns true if at least one augmenting path is found
        while(doBfs())
        {
            std::list<incidenceGraphNode*>::const_iterator unmatchedVariableNodes_end = unmatchedVariableNodes.end();
            for(std::list<incidenceGraphNode*>::const_iterator it = unmatchedVariableNodes.begin(); it != unmatchedVariableNodes_end; ++it)
            {
                doDfs(*it);
            }
        }
    }

    return HC_SUCCESS;
}

