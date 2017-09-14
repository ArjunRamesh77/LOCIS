#include "SymbolFEMoneD.h"
#include "lexer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sets the basis for the line
int FEMOneDLineObject::setBasisType(std::string basisType)
{
	//default
	ibasisType = BASIS_LINEAR;

	if (basisType == "linear")
	{
		ibasisType = BASIS_LINEAR;
	}
	else if (basisType == "quadratic")
	{
		ibasisType = BASIS_QUADRATIC;
	}
	else if(basisType == "cubic")
	{
		ibasisType = BASIS_CUBIC;
	}
	else
	{
		return SY_FAIL;
	}
	return SY_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Adds a domain to the line
int FEMOneDLineObject::addSubDomain(std::string name, ASTNode* astnLength, ASTNode* astnNumel)
{
	// Check whether the domain exists in the line model
	auto f = domains.find(name);
	if (f == domains.end())
	{
		FEMOneDModelDomainData* DD = new FEMOneDModelDomainData;

		// Store the build
		DD->astnLength = astnLength;
		DD->astnNumel = astnNumel;

		temp_num_domains++;
		DD->order = temp_num_domains;
		domains[name] = DD;
		idomains.push_back(DD);
	
		return SY_SUCCESS;
	}
	return SY_FAIL;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Returns the number of nodes based on basis type
int FEMOneDLineObject::getNumNodesFromElementType(int numel)
{
	switch (ibasisType)
	{
	case BASIS_LINEAR:
		return numel + 1;

	case BASIS_QUADRATIC:
		return numel + 1 + numel;

	case BASIS_CUBIC:
		return numel + 1 + numel * 2;

	default:
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calculate the starting and ending indices for a variable in a domain
bool FEMOneDLineObject::calculateBoundaryNodeNumbers(int domain_order, int &start_index, int &end_index)
{
	if (domains.size() >= (domain_order + 1))
	{
		int offset = 0;
		int numNodes = 0;
		for (int i = 0; i <= domain_order; i++)
		{
			//Get domain
			auto domain = idomains.at(i);
			numNodes = getNumNodesFromElementType(domain->iNumel);
			if (numNodes > 0)
			{
				start_index = offset;
				end_index = offset + numNodes - 1;
				offset = offset + numNodes;

				//Neumann boundary condition
				offset += 2;
			}
			else
				return false;
		}
		return true;
	}
	return false;
}

