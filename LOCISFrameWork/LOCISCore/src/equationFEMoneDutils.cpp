#include "equation.h"
#include "FEM1D.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get the FEM equations
bool equation::getFEMoneDEquations(ASTNode* optimizedTree, ASTNode* leftBoundary, ASTNode* rightBoundary, int leftBoundaryType,
	int rightBoundaryType, int domainLeft, int domainRight, double startLength, double endLength, int nElements, int basisType)
{
	FEM FEM1Dobj;
	FEMUserData userdata;
	elementUserData elemdata;
	std::vector<elementUserData> allelemdata;
	int numnodes, numElems;
	double length;

	// preprocessing needs to be discussed and finalized
	// find number of nodes
	numnodes = domainRight - domainLeft + 1;

	elemdata.basisFunctionType = basisType;

	// populate number of elements
	numElems = nElements;

	// check consistency
	// get number of elements
	if (basisType == BASIS_LINEAR)
	{
		elemdata.integrationRule = INTEGRATION_TRAPEZOID;
	}
	else if (basisType == BASIS_QUADRATIC)
	{
		elemdata.integrationRule = INTEGRATION_SIMPSON13;
	}
	else if (basisType == BASIS_CUBIC)
	{
		elemdata.integrationRule = INTEGRATION_SIMPSON38;
	}
	// get length of each element
	length = (endLength - startLength) / double(numElems);

	// populate elemdata
	elemdata.len = length;
	

	for (int i = 0; i < numElems; i++)
	{
		allelemdata.push_back(elemdata);
	}


	userdata.allElementData = allelemdata;
	userdata.numElements = numElems;
	userdata.startNode = domainLeft;
	userdata.totalNumNodes = numnodes;

	// Getting boundary conditions
	if (leftBoundaryType == KW_VALUE && rightBoundaryType == KW_VALUE)
	{
		// dirichlet-dirichlet
		userdata.boundaryTypes = DD;

	}
	else if (leftBoundaryType == KW_FLUX && rightBoundaryType == KW_VALUE)
	{
		// neumann-dirichlet
		userdata.boundaryTypes = ND;
	}
	else if (leftBoundaryType == KW_VALUE && rightBoundaryType == KW_FLUX)
	{
		// dirichlet-neumann
		userdata.boundaryTypes = DN;
	}
	else if (leftBoundaryType == KW_FLUX && rightBoundaryType == KW_FLUX)
	{
		// neumann-neumann
		userdata.boundaryTypes = NN;
	}

	
	// Getting information about the boundary conditions
	

	// set data
	FEM1Dobj.setData(&userdata, this, optimizedTree, leftBoundary, rightBoundary, Equations, Ip);
	
//	FEM1Dobj.preProcess();

	FEM1Dobj.build();
	

	FEM1Dobj.solve();


	return true;
}
