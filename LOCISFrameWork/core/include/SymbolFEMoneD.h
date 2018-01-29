#ifndef SYMBOL_FEMONED
#define SYMBOL_FEMONED

#include "Symbol.h"
#include <map>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Defines
#define BASIS_LINEAR	0
#define BASIS_QUADRATIC 1
#define BASIS_CUBIC     2

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Variable in domain information
struct variableAndDomain
{
	std::string vName;
	std::vector<std::string> dNames;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// domain is a special type of ModelEntity so that it can be saved with the variables
class FEMOneDModelDomainData : public modelEntity
{
public:
	bool mark;
	int iBasis;
	int order;
	double startLength;
	double endLength;
	ASTNode* astnLength;
	ASTNode* astnNumel;
	double dLength;
	int iNumel;
	std::map<std::string, int> domainSize;  // To find the extremities of a variable in a domain
	std::map<std::string, int> domainNuemannSize;  // To find the extremities of a variable in a domain

public:
    FEMOneDModelDomainData() : modelEntity(-1)
	{
		iBasis = 0;
		mark = false;
		startLength = 0.0;
		endLength = 0.0;
		astnLength = NULL;
		astnNumel = NULL;
		order = 0;
		dLength = 0.0;
		iNumel = 0;

		// Regular 
		domainSize["left"] = -1;
		domainSize["right"] = -1;

		// Special use for Nuemann BC
		domainNuemannSize["left"] = -1;
		domainNuemannSize["right"] = -1;
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// defines the Model that defines a line
class FEMOneDLineObject : public object
{
public:

	bool init; //flag to signal whether line is initialized

	//temp
	int temp_num_domains;

	// Variables and domain information to add during line initialization
	std::map<std::string, variableAndDomain> PDEvars;

	// Attributes
	int ibasisType;
	std::map<std::string, FEMOneDModelDomainData*>  domains;  // <domain_name, domain_data>
	std::vector<FEMOneDModelDomainData*> idomains; // To index by order  

public:
	FEMOneDLineObject()
	{
		init = false;
		temp_num_domains = -1;
		ibasisType = BASIS_LINEAR;
	}
	~FEMOneDLineObject()
	{

	}

	// Main functions
	int setBasisType(std::string basisType);
	int addSubDomain(std::string name, ASTNode* astnLength, ASTNode* astnNumel);
	int createVariable(std::string name, std::vector<std::string> domain_names);

	// Helpers
	int getNumNodesFromElementType(int numel);
	bool calculateBoundaryNodeNumbers(int domain_order, int &start_index, int &end_index);
};

#endif // !SYMBOL_FEMONED

