#include "matching.h"

matchingHopkroftKarp::matchingHopkroftKarp(incidenceGraph *biGraph_arg) :
    biGraph(biGraph_arg),
    unmatchedEquationNodes(),
    unmatchedVariableNodes(),
    bfsAliveIndex(0),
    bPerfectMatching(false)
{

}

matchingHopkroftKarp::matchingHopkroftKarp() :
    matchingHopkroftKarp(NULL)
{

}

matchingHopkroftKarp::~matchingHopkroftKarp()
{

}

incidenceGraph *matchingHopkroftKarp::getBiGraph() const
{
    return biGraph;
}

void matchingHopkroftKarp::setBiGraph(incidenceGraph *value)
{
    biGraph = value;
}

bool matchingHopkroftKarp::getBPerfectMatching() const
{
    return bPerfectMatching;
}

bool matchingHopkroftKarp::doBfs()
{
    //initialize
    bool bfsGotUnmatched(false);
    incidenceGraphNode* sourceNode(NULL);
    std::queue<incidenceGraphNode*> bfsQueue;
    unsigned int bfsDepth(0);
    unsigned int bfsNumPops(0);
    unsigned int bfsUnmatchedVariableDepth(0);
    unmatchedVariableNodes.clear();
    incidenceGraphNode* match(NULL);
    unsigned int nodesBfsAliveIndex(0);
    incidenceGraphNode* itDeref(NULL);
    bPerfectMatching = true;
    bfsAliveIndex += 2;

    //push all unmatched equation nodes to queue
    std::list<incidenceGraphNode*>::const_iterator unmatchedEquationNodes_end = unmatchedEquationNodes.end();
    for(std::list<incidenceGraphNode*>::const_iterator it = unmatchedEquationNodes.begin(); it != unmatchedEquationNodes_end; ++it)
    {
        //push only unmatched equations
        if(!(*it)->getMatching())
        {
            bfsQueue.push(*it);
            bPerfectMatching = false;
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
        //this ensures shortest augmenting paths
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
            itDeref = *it;
            match = NULL;
            match = itDeref->getMatching();
            nodesBfsAliveIndex = itDeref->getAliveIndex();
            if(nodesBfsAliveIndex != bfsAliveIndex)
            {
                if(match)
                {
                    bfsQueue.push(match);
                }
                else
                {
                    //unmatched variables must be pushed only once
                    unmatchedVariableNodes.push_back(itDeref);
                    bfsUnmatchedVariableDepth = bfsDepth;
                    bfsGotUnmatched = true;
                }
                itDeref->setAliveIndex(bfsAliveIndex);
            }
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

bool matchingHopkroftKarp::doDfs(incidenceGraphNode* startNode)
{
    // initialize
    incidenceGraphNode* match(NULL);
    incidenceGraphNode* sourceNode(NULL);
    incidenceGraphNode* sourceNodeMatch(NULL);
    incidenceGraphNode* lastEquationNode(NULL);
    std::stack<incidenceGraphNode*> dfsStack;
    std::vector<incidenceGraphNode*> pairs;
    incidenceGraphNode* itDeref(NULL);
    bool foundUnMatched(false);

    //Initialize stack to the start node
    dfsStack.push(startNode);
    sourceNode = dfsStack.top();

    //non recursive depth first search
    while(dfsStack.size() != 0)
    {
        dfsStack.pop();
        sourceNodeMatch = sourceNode->getMatching();
        lastEquationNode = NULL;

        std::list<incidenceGraphNode*>::const_iterator equEnd = sourceNode->getAllNodes().end();
        for(std::list<incidenceGraphNode*>::const_iterator it = sourceNode->getAllNodes().begin(); it != equEnd; ++it)
        {
            itDeref = *it;
            // Now check if the variables are matched to find an augmenting path
            // only visited paths are allowed
            if(itDeref->getAliveIndex() == bfsAliveIndex)
            {
                // Now check if the variables are matched to find an unmatched variable
                match = NULL;
                match = itDeref->getMatching();
                if(match)
                {
                     if(sourceNodeMatch)
                     {
                         // iterate only through unmatched edges
                         if(sourceNodeMatch->getIndex() != itDeref->getIndex())
                         {
                             dfsStack.push(match);
                             lastEquationNode = itDeref;
                         }
                     }
                     else
                     {
                        dfsStack.push(match);
                        lastEquationNode = itDeref;
                     }
                }
                else
                {
                   //reached an unmateched equation
                    foundUnMatched = true;
                    lastEquationNode = itDeref;
                    break;
                }
            }
        }

        //if chosen path failed
        if(lastEquationNode)
        {
            //add pairs for matching
            pairs.push_back(sourceNode);
            pairs.push_back(lastEquationNode);

            //orphan visited nodes
            sourceNode->setAliveIndex(0);
            lastEquationNode->setAliveIndex(0);
        }
        else
        {
            break;
        }

        //if an unmatched equation was found
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

void matchingHopkroftKarp::doFirstMatching()
{
    if(biGraph)
    {
        unmatchedEquationNodes.clear();
        biGraph->initializeMatchingOnGraph(unmatchedEquationNodes);
    }
}

int matchingHopkroftKarp::doMatchingHopcroftKarp()
{
    if(!biGraph)
        return HC_GRAPH_NULL;

    //initialize graph with trivial matching
    incidenceGraphNode* itDeref(NULL);
    doFirstMatching();
    bool bDfsResultSuccess(false);

    //main
    if(unmatchedEquationNodes.size() > 0)
    {
        //do a breadth first search on the unmatched equation nodes
        //returns true if at least one augmenting path is found
        while(doBfs())
        {
            bDfsResultSuccess = false;

            std::list<incidenceGraphNode*>::const_iterator unmatchedVariableNodes_end = unmatchedVariableNodes.end();
            for(std::list<incidenceGraphNode*>::const_iterator it = unmatchedVariableNodes.begin(); it != unmatchedVariableNodes_end; ++it)
            {
                itDeref = *it;
                if(itDeref->getMatching() == NULL)
                {
                    if(doDfs(itDeref))
                    {
                        bDfsResultSuccess = true;
                    }
                }
            }

            //check if at least one successful matchin occurred
            if(!bDfsResultSuccess)
            {
                return HC_DFS_NORESULT;
            }
        }
    }

    if(!bPerfectMatching)
        return HC_NO_PERFECT_MATCHING;

    return HC_SUCCESS;
}

