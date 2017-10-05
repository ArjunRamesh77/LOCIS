#include <iostream>
#include <sstream>
#include <fstream>
#include "LOCIS_API.h"
#include "LOCIS_API_INCLUDE.h"

std::stringstream dynResOut;
long dynFlag;

/////////////////////////////////////////////////////////////////////////////////
// Callbacks
long errorCallback(long lErrorType, const char* cMessage)
{
    std::cout << cMessage << std::endl;
}

long infoCallback(long lInfoType, const char* cMessage)
{
    std::cout<< cMessage;
}

long steadyStateResults(long lpNum, const char* cNames, double* dpValues)
{
    std::cout<<"STEADY STATE RESULTS ::::"<<std::endl;
    std::string Name;
    std::stringstream ss(cNames);
    for(long i = 0; i < lpNum; ++i)
    {
        std::getline(ss, Name);
        std::cout<< cNames[i] << "   " << dpValues[i] << std::endl;
    }
}

long dynamicResults(long lColIndex, long lpNum, const char* cNames, double* dpTime, double* dpValues)
{
    dynFlag = 1;
    std::string Name;
    if(lpNum == -1)
    {
        std::stringstream ss(cNames);
        dynResOut<<"t  ";
        while(std::getline(ss, Name))
        {
            dynResOut<<Name<<"  ";
        }
        dynResOut<<std::endl;
    }
    else
    {      
        dynResOut<<*dpTime<<"   ";
        for(long i = 0; i < lpNum; ++i)
        {
            dynResOut<<dpValues[i]<<"   ";
        }
        dynResOut<<std::endl;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// Entry point
int main(int argc, const char** argv)
{   
    int b;
    long instanceId = -1;
    long result = -1;
    dynFlag = 0;

    std::stringstream ss;
    std::string line;
    std::ifstream myfile (argv[1]);
    if (myfile.is_open())
    {
        while (std::getline (myfile,line) )
        {
            ss << line << '\n';
        }
        myfile.close();

        instanceId = LOCISCreate();
        result = LOCISInit(instanceId, ss.str().c_str(), errorCallback, infoCallback, steadyStateResults, dynamicResults);
        result = LOCISSolve(instanceId);

        if(dynFlag == 1)
        {
            std::cout<<"DYNAMIC RESULTS ::::"<<std::endl;
            std::string line;
            while(std::getline(dynResOut, line))
            {
                std::cout<<line<<std::endl;
            }
        }

        result = LOCISExit(instanceId, true);
    }

    return 0;
}
