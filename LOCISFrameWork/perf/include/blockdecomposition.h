#pragma once

#include "genericresidual.h"
#include "genericjacobian.h"
#include "matching.h"
#include "stronglyconnected.h"
#include <assert.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// generates the functions and infrastructure needed for block decomposition solver
class blockDecomposition
{
    //input
    unsigned int numVar;
    unsigned int numEqu;
    int systemType;
    genericResidual* mainInputResidual;

    //algorithms
    incidenceGraph igraph;
    matchingHopkroftKarp matchAlgo;
    stronglyConnectedTarjans tarjansAlgo;

    //output
    std::pair<unsigned int, unsigned int>* mapVar1;
    std::pair<unsigned int, unsigned int>* mapVar2;
    std::vector<std::pair<std::vector<unsigned int>*, std::vector<unsigned int>*>> blockVarsMap;
    std::vector<unsigned int> residualIndexMap;
    std::vector<unsigned int> finalMatching;
    std::list<std::list<incidenceGraphNode*>*>* scc;
    std::vector<genericResidual*> outputResiduals;

public:
    blockDecomposition();
    ~blockDecomposition();

    void setMainInputResidual(genericResidual* mainInputResidual_arg);
    void setNumVarsEqu(unsigned int numVar, unsigned int numEqu);
    int getEquationSets();
    int generateResidualsFromConnectedSystem();
    bool computeBlockSystem(genericResidual* mainInputResidual_arg, unsigned int numVar_arg, unsigned int numEqu_arg);

    unsigned int getNumEquationSets();
    std::vector<unsigned int> *getVariable1Mapping(unsigned int systemNum);
    std::vector<unsigned int> *getVariable2Mapping(unsigned int systemNum);
    genericResidual* getBlockResidual(unsigned int systemNum);
};
