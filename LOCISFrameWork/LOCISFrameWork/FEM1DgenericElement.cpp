#include "FEM1DgenericElement.h"


void genericElement1D::setElem(int globalNo, double nodeLocation, int elemType, double len, int numEq,
	std::string weakformstr, genericElement1D* rightElem, genericElement1D* leftElem,
	basis1D *basisptr, integration1D *integrationptr)
{

	globalNodeNumber = globalNo;
	globalNodeLocation = nodeLocation;
	nEq = numEq;

	elementType = elemType;
	length = len;
	weakForm = weakformstr;
	left = leftElem;
	right = rightElem;
	basis = basisptr;
	integration = integrationptr;

	element_info.basis = basisptr;
	element_info.elementType = elemType;
	element_info.globalNodeLocation = nodeLocation;
	element_info.globalNodeNo = globalNo;
	element_info.integration = integrationptr;
	element_info.length = len;
	return;
}


// constructor

genericElement1D::genericElement1D()
{
	weakForm = "";
	left = NULL;
	right = NULL;
	
}
