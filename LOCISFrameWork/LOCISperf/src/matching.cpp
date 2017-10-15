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

matchingBase::~matchingBase()
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
    bfsAliveIndex += 2;

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

        //local
        incidenceGraphNode* match(NULL);

        //visit all the variables and push matched variables to the queue and unmatched variables tp the vector
        std::list<incidenceGraphNode*>::const_iterator varEnd = sourceNode->getAllNodes().end();
        for(std::list<incidenceGraphNode*>::const_iterator it = sourceNode->getAllNodes().begin(); it != varEnd; ++it)
        {
            // Now check if the variables are matched to find an unmatched variable
            match = NULL;
            match = (*it)->getMatching();
            if(match)
            {
                 if((*it)->getAliveIndex() != bfsAliveIndex)
                    bfsQueue.push(match);
            }
            else
            {
                //unmatched variables must be pushed only once
                if((*it)->getAliveIndex() != bfsAliveIndex)
                {
                    unmatchedVariableNodes.push_back((*it));
                    bfsUnmatchedVariableDepth = bfsDepth;
                    bfsGotUnmatched = true;
                }
            }
            (*it)->setAliveIndex(bfsAliveIndex);
        }

        sourceNode = NULL;
        bfsQueue.pop();
        sourceNode = bfsQueue.front();
        bfsNumPops++;

        // Get graph depth
        if(bfsNumPops == bfsNumPopsTillNextDepthIncr)
        {
            bfsDepth++;
            bfsNumPops = 0;
            bfsNumPopsTillNextDepthIncr = bfsQueue.size();
        }

    }

    return bfsGotUnmatched;
}

bool matchingBase::doDfs(incidenceGraphNode* startNode)
{
    // initialize
    incidenceGraphNode* match(NULL);
    incidenceGraphNode* sourceNode(NULL);
    incidenceGraphNode* sourceNodeMatch(NULL);
    incidenceGraphNode* lastEquationNode(NULL);
    std::stack<incidenceGraphNode*> dfsStack;
    std::vector<incidenceGraphNode*> pairs;
    bool foundUnMatched(false);

    //Initialize stack to the start node
    dfsStack.push(startNode);
    sourceNode = dfsStack.top();

    //non recursive depth first search
    while(dfsStack.size() != 0)
    {
        dfsStack.pop();
        sourceNodeMatch = sourceNode->getMatching();

        std::list<incidenceGraphNode*>::const_iterator equEnd = sourceNode->getAllNodes().end();
        for(std::list<incidenceGraphNode*>::const_iterator it = sourceNode->getAllNodes().begin(); it != equEnd; ++it)
        {
            // Now check if the variables are matched to find an augmenting path
            // only visited paths are allowed
            if((*it)->getAliveIndex() == bfsAliveIndex)
            {
                // Now check if the variables are matched to find an unmatched variable
                match = NULL;
                match = (*it)->getMatching();
                if(match)
                {
                     if(sourceNodeMatch)
                     {
                         // iterate only through unmatched edges
                         if(sourceNodeMatch->getIndex() != (*it)->getIndex())
                         {
                             dfsStack.push(match);
                             lastEquationNode = *it;
                         }
                     }
                     else
                     {
                        dfsStack.push(match);
                        lastEquationNode = *it;
                     }
                }
                else
                {
                   //reached an unmateched equation
                    foundUnMatched = true;
                    lastEquationNode = *it;
                }
            }
        }

        //add pairs for matching
        pairs.push_back(sourceNode);
        pairs.push_back(lastEquationNode);

        //orphan visited nodes
        sourceNode->setAliveIndex(bfsAliveIndex+1);
        lastEquationNode->setAliveIndex(bfsAliveIndex+1);

        //DEBUG

        if(foundUnMatched)
        {
            for(std::vector<incidenceGraphNode*>::const_iterator it = pairs.begin(); it != pairs.end(); it = it + 2)
            {
                (*it)->setMatching(*(it + 1));
                (*(it + 1))->setMatching(*it);
                std::cout<<" Matched E" << (*(it + 1))->getIndex() <<" with V"<<(*it)->getIndex() <<"\n";
            }
            break;
        }

        //next
        sourceNode = NULL;
        sourceNode = dfsStack.top();
    }

    return foundUnMatched;
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
                if((*it)->getMatching() == NULL)
                    doDfs(*it);
            }
        }
    }

    return HC_SUCCESS;
}

