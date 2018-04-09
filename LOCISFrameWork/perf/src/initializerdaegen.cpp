#include "initializerdaegen.h"

initializerDaeGenRecognize::initializerDaeGenRecognize(__int8_t type_arg, unsigned int index_arg) :
    type(type_arg),
    index(index_arg)
{

}

initializerDaeGen::initializerDaeGen() :
    iNumAlg(0),
    iNumDif(0)
{

}

initializerDaeGen::~initializerDaeGen()
{

}

int initializerDaeGen::convertToDaeInitializerResidual(std::vector<virtualOper> *daeRes, std::vector<initializerDaeGenRecognize> &varMap)
{
    //init
    varMap.clear();
    iNumAlg = 0;
    iNumDif = 0;
    unsigned int vCount = 0;

    //temporary arrays
    std::map<unsigned int, unsigned int> var1Map, var2Map;
    auto find = var1Map.find(0);

    //Loop over all instructions and replace any variable index(index 1 or 2) with counter index
    std::vector<virtualOper>::iterator daeRes_end = daeRes->end();
    for(std::vector<virtualOper>::iterator it = daeRes->begin(); it != daeRes_end; ++it)
    {
        if(it->operType == VR_VAR1_INDEX)
        {
            find = var1Map.find(it->index);
            if(find == var1Map.end())
            {
                varMap.push_back(initializerDaeGenRecognize(VR_VAR1_INDEX, it->index));
                var1Map[it->index] = vCount;
                it->index = vCount;
                ++iNumAlg;
                ++vCount;
            }
            else
            {
                it->index = var1Map[it->index];
            }
            continue;
        }

        if(it->operType == VR_VAR2_INDEX)
        {
            it->operType = VR_VAR1_INDEX;
            find = var2Map.find(it->index);
            if(find == var2Map.end())
            {
                varMap.push_back(initializerDaeGenRecognize(VR_VAR2_INDEX, it->index));
                var2Map[it->index] = vCount;
                it->index = vCount;
                ++iNumDif;
                ++vCount;
            }
            else
            {
                it->index = var2Map[it->index];
            }
            continue;
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// reverts a non-linear initialization type residual to the original dae residual
int initializerDaeGen::revertToDaeResidual(std::vector<virtualOper>& equVec, std::vector<initializerDaeGenRecognize> &varMap)
{
    //Loop over all instructions and replace any variable index(index 1 or 2) with counter index
    for(std::vector<virtualOper>::iterator it =equVec.begin(); it != equVec.end(); ++it)
    {
        if(it->operType == VR_VAR1_INDEX)
        {
            if(varMap[it->index].type == VR_VAR1_INDEX)
            {
                it->index = varMap[it->index].index;
            }
            else
            {
                it->index = varMap[it->index].index;
                it->operType = VR_VAR2_INDEX;
            }
        }
    }

    return 0;
}

unsigned int initializerDaeGen::getNumAlgVars()
{
    return iNumAlg;
}

unsigned int initializerDaeGen::getNumDiffVars()
{
    return iNumDif;
}


