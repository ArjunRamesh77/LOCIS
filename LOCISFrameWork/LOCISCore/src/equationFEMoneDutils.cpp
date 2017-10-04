#include "equation.h"
#include "FEM1D.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get the FEM equations
bool equation::getFEMoneDEquations(ASTNode* optimizedTree, ASTNode* leftBoundary, ASTNode* rightBoundary, int leftBoundaryType,
	int rightBoundaryType, int domainLeft, int domainRight, double startLength, double endLength, int nElements, int basisType)
{
    using namespace FEM1D;
    FEM FEM1DObj;
    FEMUserData userData;
    elementUserData elemData;
	std::vector<elementUserData> allelemdata;
    int numNodes, numElems;
	double length;

	// preprocessing needs to be discussed and finalized
	// find number of nodes
    numNodes = domainRight - domainLeft + 1;

    elemData.basisFunctionType = basisType;

	// populate number of elements
	numElems = nElements;

	// check consistency
	// get number of elements
	if (basisType == BASIS_LINEAR)
	{
        elemData.integrationRule = INTEGRATION_TRAPEZOID;
	}
	else if (basisType == BASIS_QUADRATIC)
	{
        elemData.integrationRule = INTEGRATION_SIMPSON13;
	}
	else if (basisType == BASIS_CUBIC)
	{
        elemData.integrationRule = INTEGRATION_SIMPSON38;
	}
	// get length of each element
	length = (endLength - startLength) / double(numElems);

	// populate elemdata
    elemData.len = length;
	

	for (int i = 0; i < numElems; i++)
	{
        allelemdata.push_back(elemData);
	}


    userData.allElementData = allelemdata;
    userData.numElements = numElems;
    userData.startNode = domainLeft;
    userData.totalNumNodes = numNodes;

	// Getting boundary conditions
	if (leftBoundaryType == VALUE && rightBoundaryType == VALUE)
	{
		// dirichlet-dirichlet
        userData.boundaryTypes = DD;

	}
	else if (leftBoundaryType == FLUX && rightBoundaryType == VALUE)
	{
		// neumann-dirichlet
        userData.boundaryTypes = ND;
	}
	else if (leftBoundaryType == VALUE && rightBoundaryType == FLUX)
	{
		// dirichlet-neumann
        userData.boundaryTypes = DN;
	}
	else if (leftBoundaryType == FLUX && rightBoundaryType == FLUX)
	{
		// neumann-neumann
        userData.boundaryTypes = NN;
	}

	
	// Getting information about the boundary conditions
	

	// set data
    FEM1DObj.setData(&userData, this, optimizedTree, leftBoundary, rightBoundary, Equations, Ip);
	
//	FEM1Dobj.preProcess();

    FEM1DObj.build();
	

    FEM1DObj.solve();


	return true;
}
