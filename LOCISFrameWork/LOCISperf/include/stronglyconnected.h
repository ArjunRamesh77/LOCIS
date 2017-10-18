#pragma once

#include "matching.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// implementation of the Tarjan's algorithm
class stronglyConnectedTarjans
{
    incidenceGraph* biGraph;
    std::list<std::list<incidenceGraphNode*>*>* sccs;

public:
    stronglyConnectedTarjans(incidenceGraph *graph);
    stronglyConnectedTarjans();
    ~stronglyConnectedTarjans();

    incidenceGraph *getBiGraph() const;
    void setBiGraph(incidenceGraph *value);
    std::list<std::list<incidenceGraphNode*>*>* doStronglyConnectedTarjans(incidenceGraphNode* node);
};
