#pragma once

#include "genericresidual.h"

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

    int convertToDaeInitializerResidual(genericResidual* daeRes, std::vector<initializerDaeGenRecognize>& varMap);
    int revertToDaeResidual(genericResidual* daeInitRes, std::vector<initializerDaeGenRecognize>& varMap);

    unsigned int getNumberAlgVars();
    unsigned int getNumDiffVars();
};
