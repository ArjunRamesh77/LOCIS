#include "matchinghopcroftkarp.h"

matchingHopcroftKarp::matchingHopcroftKarp(incidenceGraph *biGraph) :
    matchingBase(biGraph)
{

}

matchingHopcroftKarp::matchingHopcroftKarp() :
    matchingBase(NULL)
{

}

incidenceGraph *matchingHopcroftKarp::doMatching()
{
    //do first iteration
    doFirstMatching();

    if(unmatchedEquationNodes.size() > 0)
    {

    }
    else
    {
        //First matching already maximum
        return biGraph;
    }
    return NULL;
}
