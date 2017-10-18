#include "stronglyconnected.h"
#include <cmath>

stronglyConnectedTarjans::stronglyConnectedTarjans() :
    stronglyConnectedTarjans(NULL)
{

}

stronglyConnectedTarjans::stronglyConnectedTarjans(incidenceGraphNode *graph) :
    biGraph(graph),
    sccs(NULL)
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

std::list<std::list<incidenceGraphNode*>>* stronglyConnectedTarjans::doStronglyConnectedTarjans(incidenceGraphNode *node)
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

    if(sccs)
        delete sccs;

    sccs = new std::list<std::list<incidenceGraphNode*>>;

    //setup graph
    biGraph->reIndexVariableNodes();

    //iterartive depth first search
    while(dfsStack.size() != 0)
    {
        sourceNode = dfsStack.top();
        dfsStack.pop();

        //strongly connected book keeping
        sourceNode->setIndex(index);
        sourceNode->setLowlink(index);
        ++index;
        tarjansStack.push(sourceNode);
        sourceNode->setAliveIndex(1);

        //push all the matching equations variables on to the stack
        match = sourceNode->getMatching();
        incidenceGraphNode* itDeref;
        std::list<incidenceGraphNode*>::const_iterator varEnd = match->getAllNodes().end();
        for(std::list<incidenceGraphNode*>::const_iterator it = match->getAllNodes().begin();
            it != varEnd; ++it)
        {
            itDeref = *it;

            //push to stack only if node not previosly visited
            if(itDeref->getIndex() == 0)
            {
                dfsStack.push(itDeref);
                tarjansStack.push(itDeref);
                sourceNode->setLowlink(std::min(sourceNode->getLowlink(), itDeref->getLowlink()));
            }
            else if(itDeref->getAliveIndex())
            {
                sourceNode->setLowlink(std::min(sourceNode->getLowlink(), itDeref->getIndex()));
            }
        }

        //found a stronly connected system
        if(sourceNode->getLowlink() == sourceNode->getIndex())
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

    return sccs;
}
