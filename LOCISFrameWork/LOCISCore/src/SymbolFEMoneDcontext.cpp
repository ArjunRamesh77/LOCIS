#include "Symbol.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// modelEntityFEMoneDContext
modelEntityFEMoneDContext::modelEntityFEMoneDContext() :
    isFEMoneDDomain(false),
    isFEMOneDLine(false),
    isFEMOneDVariable(false),
    isFEMOneDdVariable(false),
    isFEMtrialFunction(false),
    isTval(false),
    isFEMxval(false),
    currentDomain(NULL)
{

}

void modelEntityFEMoneDContext::setIsFEMoneDDomain(bool value)
{
    isFEMoneDDomain = value;
}

void modelEntityFEMoneDContext::setIsFEMOneDLine(bool value)
{
    isFEMOneDLine = value;
}

void modelEntityFEMoneDContext::setIsFEMOneDVariable(bool value)
{
    isFEMOneDVariable = value;
}

void modelEntityFEMoneDContext::setIsFEMOneDdVariable(bool value)
{
    isFEMOneDdVariable = value;
}

void modelEntityFEMoneDContext::setIsFEMtrialFunction(bool value)
{
    isFEMtrialFunction = value;
}

void modelEntityFEMoneDContext::setIsTval(bool value)
{
    isTval = value;
}

void modelEntityFEMoneDContext::setIsFEMxval(bool value)
{
    isFEMxval = value;
}

void modelEntityFEMoneDContext::setCurrentDomain(modelEntity *currentDomain_arg)
{
    currentDomain = currentDomain_arg;
}

bool modelEntityFEMoneDContext::getIsFEMoneDDomain()
{
    return isFEMoneDDomain;
}

bool modelEntityFEMoneDContext::getIsFEMOneDLine()
{
    return isFEMOneDLine;
}

bool modelEntityFEMoneDContext::getIsFEMOneDVariable()
{
    return isFEMOneDVariable;
}

bool modelEntityFEMoneDContext::getIsFEMOneDdVariable()
{
    return isFEMOneDdVariable;
}

bool modelEntityFEMoneDContext::getIsFEMtrialFunction()
{
    return isFEMtrialFunction;
}

bool modelEntityFEMoneDContext::getIsTval()
{
    return isTval;
}

bool modelEntityFEMoneDContext::getIsFEMxval()
{
    return isFEMxval;
}

modelEntity* modelEntityFEMoneDContext::getCurrentDomain()
{
    return currentDomain;
}
