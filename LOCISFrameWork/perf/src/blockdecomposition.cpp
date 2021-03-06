#include "blockdecomposition.h"


blockDecomposition::blockDecomposition() :
    numVar(0),
    numEqu(0),
    systemType(0),
    mainInputResidual(NULL),
    isInitializer(false),
    igraph(),
    matchAlgo(),
    tarjansAlgo(),
    mapVar1(NULL),
    mapVar2(NULL),
    blockVarsMap(),
    residualIndexMap(),
    finalMatching(),
    scc(NULL),
    outputResiduals()
{

}

blockDecomposition::~blockDecomposition()
{
    if(mapVar1)
        delete mapVar1;
    if(mapVar2)
        delete mapVar2;

    for(std::vector<std::pair<std::vector<unsigned int>*, std::vector<unsigned int>*>>::iterator it = blockVarsMap.begin(); it != blockVarsMap.end(); ++it)
    {
        delete it->first;
        delete it->second;
    }

    for(std::vector<genericResidual*>::iterator it = outputResiduals.begin(); it != outputResiduals.end(); ++it)
    {
        delete (*it);
    }
}

void blockDecomposition::setMainInputResidual(genericResidual *mainInputResidual_arg)
{
    mainInputResidual = mainInputResidual_arg;
}

void blockDecomposition::setNumVarsEqu(unsigned int numVar_arg, unsigned int numEqu_arg)
{
    numVar = numVar_arg;
    numEqu = numEqu_arg;
}

void blockDecomposition::setInInitializer(bool val)
{
    isInitializer = val;
}

bool blockDecomposition::computeBlockSystem(genericResidual *mainInputResidual_arg, unsigned int numVar_arg, unsigned int numEqu_arg)
{
    int ret = 0;
    setNumVarsEqu(numVar_arg, numEqu_arg);

    //check input sanity
    if(mainInputResidual_arg == NULL)
        return false;

    if(numVar == 0 || numEqu == 0)
        return false;

    setMainInputResidual(mainInputResidual_arg);


    ret = getEquationSets();
    if(ret != 0)
        return false;

    ret = generateResidualsFromConnectedSystem();
    if(ret != 0)
        return false;

    return true;
}

unsigned int blockDecomposition::getNumEquationSets()
{
    return scc->size();
}

std::vector<unsigned int> *blockDecomposition::getVariable1Mapping(unsigned int systemNum)
{
    std::pair<std::vector<unsigned int>*, std::vector<unsigned int>*> block = blockVarsMap.at(systemNum);
    return block.first;
}

std::vector<unsigned int> *blockDecomposition::getVariable2Mapping(unsigned int systemNum)
{
    std::pair<std::vector<unsigned int>*, std::vector<unsigned int>*> block = blockVarsMap.at(systemNum);
    return block.second;
}

genericResidual *blockDecomposition::getBlockResidual(unsigned int systemNum)
{
    return outputResiduals.at(systemNum);
}
