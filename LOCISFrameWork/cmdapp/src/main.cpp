#include <iostream>
#include <sstream>
#include <fstream>
#include "LOCIS_API.h"
#include "LOCIS_API_INCLUDE.h"
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>


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
    //Load so
    void* handle = NULL;
    char* error;
    handle = dlopen ("/home/arjun/ARJUN/locis/Output/debug/liblocisapi.so", RTLD_LAZY);
    if (!handle)
    {
        std::cout<< dlerror();
        exit(1);
    }

    //Load all functions
    LOCISDLL_LOCISCreate LOCISCreate;
    LOCISDLL_LOCISInit LOCISInit;
    LOCISDLL_LOCISSolve LOCISSolve;
    LOCISDLL_LOCISExit LOCISExit;

    LOCISCreate = dlsym(handle, "LOCISCreate");
    if ((error = dlerror()) != NULL)  {
        exit(1);
    }

    LOCISInit = dlsym(handle, "LOCISInit");
    if ((error = dlerror()) != NULL)  {
        exit(1);
    }

    LOCISSolve = dlsym(handle, "LOCISSolve");
    if ((error = dlerror()) != NULL)  {
        exit(1);
    }

    LOCISExit = dlsym(handle, "LOCISExit");
    if ((error = dlerror()) != NULL)  {
        exit(1);
    }

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

    dlclose(handle);
    return 0;
}


//#include "matching.h"
//#include "stronglyconnected.h"
//#include "genericresidual.h"
//#include "genericjacobian.h"

int main1()
{
    /*
    incidenceGraph biPartite;

    //set graph size
    biPartite.addEquationNodes(6);
    biPartite.addVariableNodes(6);
    */

    //add edges
    /*
    biPartite.matrixCOOClear();
    biPartite.MatrixCOOAddCoordinate(0,0);
    biPartite.MatrixCOOAddCoordinate(0,1);
    biPartite.MatrixCOOAddCoordinate(1,2);
    biPartite.MatrixCOOAddCoordinate(1,3);
    biPartite.MatrixCOOAddCoordinate(2,0);
    biPartite.MatrixCOOAddCoordinate(2,4);
    biPartite.MatrixCOOAddCoordinate(2,5);
    biPartite.MatrixCOOAddCoordinate(3,1);
    biPartite.MatrixCOOAddCoordinate(3,4);
    biPartite.MatrixCOOAddCoordinate(3,6);
    biPartite.MatrixCOOAddCoordinate(4,2);
    biPartite.MatrixCOOAddCoordinate(4,4);
    biPartite.MatrixCOOAddCoordinate(5,3);
    biPartite.MatrixCOOAddCoordinate(5,7);
    biPartite.MatrixCOOAddCoordinate(6,5);
    biPartite.MatrixCOOAddCoordinate(7,7);
    */

    /*
       biPartite.MatrixCOOAddCoordinate(0, 1);
       biPartite.MatrixCOOAddCoordinate(0, 2);
       biPartite.MatrixCOOAddCoordinate(1, 0);
       biPartite.MatrixCOOAddCoordinate(2, 1);
       biPartite.MatrixCOOAddCoordinate(3, 1);
       biPartite.MatrixCOOAddCoordinate(3, 3);
       */

    /*
     biPartite.MatrixCOOAddCoordinate(0, 2);
     biPartite.MatrixCOOAddCoordinate(0, 3);
     biPartite.MatrixCOOAddCoordinate(1, 1);
     biPartite.MatrixCOOAddCoordinate(2, 1);
     biPartite.MatrixCOOAddCoordinate(2, 2);
     biPartite.MatrixCOOAddCoordinate(2, 4);
     biPartite.MatrixCOOAddCoordinate(3, 0);
     biPartite.MatrixCOOAddCoordinate(3, 1);
     biPartite.MatrixCOOAddCoordinate(4, 0);
     biPartite.MatrixCOOAddCoordinate(4, 2);
     biPartite.MatrixCOOAddCoordinate(4, 4);

     //dummy node
     biPartite.MatrixCOOAddCoordinate(5, 0);
     biPartite.MatrixCOOAddCoordinate(5, 1);
     biPartite.MatrixCOOAddCoordinate(5, 2);
     biPartite.MatrixCOOAddCoordinate(5, 3);
     biPartite.MatrixCOOAddCoordinate(5, 4);
     biPartite.MatrixCOOAddCoordinate(5, 5);



    //create graph
    biPartite.createBipartiteEVGraphFromMatrixCOO();

    //matching
   matchingHopkroftKarp hk(&biPartite);
   hk.doMatchingHopcroftKarp();

   //tarjans
   stronglyConnectedTarjans st(&biPartite);
   std::list<std::list<incidenceGraphNode*>*>* scc = st.doStronglyConnectedTarjans(biPartite.getVariableAt(5));

   for(std::list<std::list<incidenceGraphNode*>*>::const_iterator it1 = scc->begin(); it1 != scc->end(); it1++)
   {
        std::cout<<"SCC :";
        for(std::list<incidenceGraphNode*>::const_iterator it2 = (*it1)->begin(); it2 != (*it1)->end(); it2++)
        {
            std::cout<<" "<<(*it2)->getIndex();
        }
        std::cout<<std::endl;
   }
   */

    //residual test
    //virtualInstructionStack inst;
    //genericJacobian jac;
    //genericResiual gr;

    //long n = 10;
    //v[0]*(v[0] +  v[1])
    //long count = 0;
    //for(long i = 0; i < n; i++)
    //{
    //    inst.addVariable1Index(i);
    //   if(count > 0)
    //        inst.addMathInstr(VR_BIN_MUL);
    //    count++;
    //}
    //inst.makeLast();

    //gr.setInstStackPtr(&inst);

    //double* v = new double[n];
    //double* j = new double[n];
    //for(long i = 0; i < n; i++)
    //    j[i] = 0.0;

    //for(long i = 0; i < n; i++)
    //   v[i] = i + 1;

    //jac.setInstStackPtr(&inst);
    //jac.setNVar(n);
    //jac.generateFullJacobianInstr(0);
    //jac.evalDenseJacobian1StackBased(j, v);
    //for(long i = 0; i < n; i++)
    //    std::cout<<j[i]<<std::endl;

    //gr.evalResidual1StackBased(r, v);
    //delete []v;
    //delete []j;



   return 0;
}
