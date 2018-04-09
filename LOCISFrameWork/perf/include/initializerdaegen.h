#pragma once

#include "genericresidual.h"
#include <map>

//helper struct
struct initializerDaeGenRecognize
{
    __int8_t type;
    unsigned int index;

    initializerDaeGenRecognize(__int8_t type, unsigned int index);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// generates the DAE initializer residual from a DAE Residual with mappings
class initializerDaeGen
{
    unsigned int iNumAlg;
    unsigned int iNumDif;

public:
    initializerDaeGen();
    ~initializerDaeGen();

    int convertToDaeInitializerResidual(std::vector<virtualOper> *daeResInstr, std::vector<initializerDaeGenRecognize>& varMap);

    unsigned int getNumAlgVars();
    unsigned int getNumDiffVars();
    int revertToDaeResidual(std::vector<virtualOper> &equVec, std::vector<initializerDaeGenRecognize> &varMap);
};
