#include "stronglyconnected.h"
#include <cmath>

stronglyConnectedTarjans::stronglyConnectedTarjans(incidenceGraph* graph) :
    biGraph(graph)
{

}

stronglyConnectedTarjans::stronglyConnectedTarjans()
{

}

stronglyConnectedTarjans::~stronglyConnectedTarjans()
{

}

incidenceGraph *stronglyConnectedTarjans::getBiGraph() const
{
    return biGraph;
}

void stronglyConnectedTarjans::setBiGraph(incidenceGraph *value)
{
    biGraph = value;
}

std::list<std::list<incidenceGraphNode *> *> *stronglyConnectedTarjans::doStronglyConnectedTarjans(incidenceGraphNode *node)
{
    //sanity checks
    if(biGraph == NULL)
        return NULL;

    //initialize
    incidenceGraphNode* sourceNode = NULL;
    incidenceGraphNode* childNode = NULL;
    incidenceGraphNode* match = NULL;
    std::stack<incidenceGraphNode*> dfsStack;
    std::stack<incidenceGraphNode*> tarjansStack;
    unsigned int index(0);
    dfsStack.push(node);

    std::list<std::list<incidenceGraphNode*>*>* sccs = NULL;
    sccs = new std::list<std::list<incidenceGraphNode*>*>;

    //setup graph
    biGraph->resetAllIndicators();

    //iterartive depth first search
    while(dfsStack.size() != 0)
    {
        sourceNode = dfsStack.top();
        dfsStack.pop();

        if(sourceNode->getAliveIndex() == 0) //before recusion
        {
            if(sourceNode->getTIndex() == 0)
            {
                //strongly connected book keeping
                sourceNode->setTIndex(index);
                sourceNode->setLowlink(index);
                sourceNode->setAliveIndex(1);
                ++index;
                tarjansStack.push(sourceNode);
                dfsStack.push(sourceNode);

                //push all the matching equations variables on to the stack
                match = sourceNode->getMatching();
                incidenceGraphNode* itDeref;
                std::list<incidenceGraphNode*>::const_iterator varEnd = match->getAllNodes().end();
                for(std::list<incidenceGraphNode*>::const_iterator it = match->getAllNodes().begin();
                    it != varEnd; ++it)
                {
                    itDeref = *it;
                    if(itDeref->getAliveIndex() == 0)
                    {
                        dfsStack.push(itDeref);
                    }
                }
            }
        }
        else //after recursion
        {
            //push all the matching equations variables on to the stack
            match = sourceNode->getMatching();
            incidenceGraphNode* itDeref;
            std::list<incidenceGraphNode*>::const_iterator varEnd = match->getAllNodes().end();
            for(std::list<incidenceGraphNode*>::const_iterator it = match->getAllNodes().begin();
                it != varEnd; ++it)
            {
                itDeref = *it;

                if(itDeref->getAliveIndex() == 1)
                {
                    if(itDeref->getTIndex() <= sourceNode->getTIndex())
                       sourceNode->setLowlink(std::min(sourceNode->getLowlink(), itDeref->getTIndex()));
                    else
                       sourceNode->setLowlink(std::min(sourceNode->getLowlink(), itDeref->getLowlink()));
                }
            }

            //found a stronly connected system
            if(sourceNode->getLowlink() == sourceNode->getTIndex())
            {
                std::list<incidenceGraphNode*>* sccItem = new std::list<incidenceGraphNode*>;
                do
                {
                    childNode = tarjansStack.top();
                    tarjansStack.pop();
                    childNode->setAliveIndex(0);
                    sccItem->push_back(childNode);
                }
                while(sourceNode->getIndex() != childNode->getIndex());
                sccs->push_back(sccItem);
            }
        }
    }
    return sccs;
}
