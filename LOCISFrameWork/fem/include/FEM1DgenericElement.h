#pragma once
#include <vector>
#include<string>
#include "FEM1Dbasis.h"
#include "FEM1Dintegrate.h"
namespace FEM1D
{

struct elementInfo
{
    int globalNodeNo, elementType;
    double globalNodeLocation;
    basis1D *basis;
    integration1D  *integration;
    double length;
};



class genericElement1D
{
	elementInfo element_info;

    int globalNodeNumber; //global node number of starting node

	double globalNodeLocation;

	int nEq; // each element should know number of equations

	int elementType; // 1. DBC, 2. NBC or 0. intermediate 	

	basis1D *basis;

	integration1D  *integration;

	genericElement1D *left, *right; 

	double length;

	//******************ALL STRING MANIPULATIONS********************
	std::string weakForm;  // this is the main input (raw)

	std::vector<std::string> beforeIntegrationVec; // Size of this vector will be number of equations

	std::vector<std::string> afterIntegrationVec; // Size of this vector will be number of equations

public:
	
	
	void setElem(int globalNo, double nodeLocation, int elemType, double len, int nEq,
		std::string weakformstr, genericElement1D* rightElem, genericElement1D* leftElem,
		basis1D *basis, integration1D *integration);

    elementInfo getElementInfo()
    {
        return element_info;
    };

    int getGlobalNodeNo()
    {
        return globalNodeNumber;
    }

    int getElementType()
    {
        return elementType;
    }

    double getGlobalNodeLocation()
    {
        return globalNodeLocation;
    }

    basis1D* getBasis1D()
    {
        return basis;
    }

    integration1D* getIntegration()
    {
        return integration;
    }

    double getElementLength()
    {
        return length;
    }

	genericElement1D();
	
};
}
