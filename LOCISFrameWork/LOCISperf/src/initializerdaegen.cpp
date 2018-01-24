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

int initializerDaeGen::convertToDaeInitializerResidual(genericResidual *daeRes, std::vector<initializerDaeGenRecognize> &varMap)
{
    //init
    varMap.clear();
    iNumAlg = 0;
    iNumDif = 0;
    unsigned int vCount = 0;

    //Loop over all instructions and replace any variable index(index 1 or 2) with counter index
    std::vector<virtualOper>::iterator daeRes_end = daeRes->getAllInst()->end();
    for(std::vector<virtualOper>::iterator it = daeRes->getAllInst()->begin(); it != daeRes_end; ++it)
    {
        if(it->operType == VR_VAR1_INDEX)
        {
            varMap.push_back(initializerDaeGenRecognize(VR_VAR1_INDEX, it->index));
            it->index = vCount;
            ++iNumAlg;
            ++vCount;
            continue;
        }

        if(it->operType == VR_VAR2_INDEX)
        {
            varMap.push_back(initializerDaeGenRecognize(VR_VAR2_INDEX, it->index));
            it->index = vCount;
            ++iNumDif;
            ++vCount;
            continue;
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// reverts a non-linear initialization type residual to the original dae residual
int initializerDaeGen::revertToDaeResidual(genericResidual *daeInitRes, std::vector<initializerDaeGenRecognize> &varMap)
{
    //Loop over all instructions and replace any variable index(index 1 or 2) with counter index
    std::vector<virtualOper>::iterator daeInitRes_end = daeInitRes->getAllInst()->end();
    for(std::vector<virtualOper>::iterator it = daeInitRes->getAllInst()->begin(); it != daeInitRes_end; ++it)
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
        else
        {
            return -1;
        }
    }

    return 0;
}

unsigned int initializerDaeGen::getNumberAlgVars()
{
    return iNumAlg;
}

unsigned int initializerDaeGen::getNumDiffVars()
{
    return iNumDif;
}


