#include "blockdecomposition.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// generates the functions and infrastructure needed for block decomposition solver
int blockDecomposition::getEquationSets()
{
    //first contruct the bipartite graph
    igraph.addEquationNodes(numEqu + 1);
    igraph.addVariableNodes(numVar + 1);

    //loop throgh all the instructions create the incidence graph
    virtualOper vo;
    unsigned int fIndex = 1;   //1 based indexing
    std::vector<virtualOper>::const_iterator resi_end = mainInputResidual->getAllInst()->end();
    unsigned int resiInstrCount = 0;
    unsigned int numAlg = 0;
    unsigned int numDif = 0;
    unsigned int vTotalIndex = 1; //1 based indexing
    residualIndexMap.clear();
    residualIndexMap.push_back(0); //since first residual starts at index 0;

    //declare and initialize maps
    //initializer for dae does not need mapvar2
    mapVar1 = new std::pair<unsigned int, unsigned int>[numVar];
    if(!isInitializer)
    {
        mapVar2 = new std::pair<unsigned int, unsigned int>[numVar];
        for(unsigned int i = 0; i < numVar; ++i)
        {
            mapVar1[i] = std::make_pair(0,0);
            mapVar2[i] = std::make_pair(0,0);
        }
    }
    else
    {
        for(unsigned int i = 0; i < numVar; ++i)
        {
            mapVar1[i] = std::make_pair(0,0);
        }
    }

    std::pair<unsigned int, unsigned int>* mapPair;
    for(std::vector<virtualOper>::const_iterator it = mainInputResidual->getAllInst()->begin(); it != resi_end; ++it)
    {
        //dereference
        vo = *it;

        if(vo.operType == VR_VAR1_INDEX)
        {
            mapPair = &mapVar1[vo.index];
            //if variable encountered for the first time
            if(mapPair->first == 0)
            {
                mapPair->first = vTotalIndex;
                mapPair->second = fIndex;
                igraph.addEdge(fIndex - 1, vTotalIndex - 1);
                ++vTotalIndex;
                ++numAlg;
            }
            else
            {
                //check if already encountered variable already added the function incidence
                if(mapPair->second != fIndex)
                {
                    mapPair->second = fIndex;
                    igraph.addEdge(fIndex - 1, mapPair->first - 1);
                }
            }
        }

        else if(vo.operType == VR_VAR2_INDEX)
        {
            mapPair = &mapVar2[vo.index];
            if(mapPair->first == 0)
            {
                mapPair->first = vTotalIndex;
                mapPair->second = fIndex;
                igraph.addEdge(fIndex - 1, vTotalIndex - 1);
                ++vTotalIndex;
                ++numDif;
            }
            else
            {
                if(mapPair->second != fIndex)
                {
                    mapPair->second = fIndex;
                    igraph.addEdge(fIndex - 1, mapPair->first - 1);
                }
            }
        }

        if(vo.signal == VR_SIGNAL_LAST)
        {
            ++fIndex;
            residualIndexMap.push_back(resiInstrCount + 1);
        }
        ++resiInstrCount;
    }

    //convert to zero based indexing
    if(!isInitializer)
    {
        for(unsigned int i = 0; i < numVar; ++i)
        {
            --mapVar1[i].first;
            --mapVar1[i].second;
            --mapVar2[i].first;
            --mapVar2[i].second;
        }
    }
    else
    {
        for(unsigned int i = 0; i < numVar; ++i)
        {
            --mapVar1[i].first;
            --mapVar1[i].second;
        }
    }

    //Add dummy source node
    for(unsigned int i = 0; i <= numVar; ++i)
    {
        igraph.addEdge(numVar, i);
    }

    //do the matching on the incidence graph
    matchAlgo.setBiGraph(&igraph);
    if(matchAlgo.doMatchingHopcroftKarp() != HC_SUCCESS)
    {
        return -1;
    }

    //extract matching from bigraph(variable to equation)
    finalMatching.clear();
    for(unsigned int i = 0; i < numVar; ++i)
    {
        finalMatching.push_back(igraph.getEquationAt(i)->getMatching()->getIndex());
    }

    //get the strongly connected systems
    tarjansAlgo.setBiGraph(&igraph);
    scc = tarjansAlgo.doStronglyConnectedTarjans(igraph.getVariableAt(numVar));

    return 0;
}
