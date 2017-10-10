#pragma once

#include "matching.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hopcroft-Karp
class matchingHopcroftKarp : public matchingBase
{

public:
    matchingHopcroftKarp(incidenceGraph*biGraph);
    matchingHopcroftKarp();
    ~matchingHopcroftKarp();

    virtual incidenceGraph *doMatching();
};
