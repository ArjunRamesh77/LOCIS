#ifndef AST_FEMONED
#define AST_FEMONED

#include "AST.h"

enum FEMOneDRules {
	FEMOneD_basis_type, FEMoneD_segment_def, FEMoneD_segment_attributes, FEMoneD_segment_attribute,
	FEMoneD_segment_id, FEMoneD_segment_length, FEMoneD_domain_option, FEMoneD_equation, FEMoneD_pdevar_def,
	FEMoneD_boundary_type
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FEMoneD BasisType
class ASTBasisTypeNode : public ASTNode
{
public:
	token tBasisType;
	std::string sBasisType;

public:
	ASTBasisTypeNode() : ASTNode()
	{
		sBasisType.assign("");
	}
	~ASTBasisTypeNode()
	{

	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(token* t_BasisType);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FEMoneD Segment definition
class ASTSegmentAttributesNode : public ASTNode
{
public:
	std::vector<ASTNode*> astvnSegmentAttributes;

public:
	ASTSegmentAttributesNode() : ASTNode()
	{

	}
	~ASTSegmentAttributesNode()
	{
		for (std::vector<ASTNode*>::iterator it = astvnSegmentAttributes.begin(); it != astvnSegmentAttributes.end(); ++it)
		{
			if (*it)
				delete *it;
		}
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(std::vector<ASTNode*> &astvnSegmentAttributes);

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FEMoneD Segment definition
class ASTPdevarAttributesNode : public ASTNode
{
public:
	std::vector<ASTNode*> astvnSegmentAttributes;

public:
	ASTPdevarAttributesNode() : ASTNode()
	{

	}
	~ASTPdevarAttributesNode()
	{
		for (std::vector<ASTNode*>::iterator it = astvnSegmentAttributes.begin(); it != astvnSegmentAttributes.end(); ++it)
		{
			if (*it)
				delete *it;
		}
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(std::vector<ASTNode*> &astvnSegmentAttributes);

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FEMoneD ID type
class ASTSegmentIdTypeNode : public ASTNode
{
public:
	token tID;
	std::string sID;

public:
	ASTSegmentIdTypeNode()
	{

	}
	~ASTSegmentIdTypeNode()
	{

	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(token* t_ID);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FEMoneD numel 
class ASTNumelNode : public ASTNode
{
public:
	ASTNode* astnExp;
	token t_Tag;

public:
	ASTNumelNode()
	{
		astnExp = NULL;
	}
	~ASTNumelNode()
	{

	}

	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(ASTNode* astnExp, token* t_Tag);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FEMoneD length
class ASTLengthNode : public ASTNode
{
public:
	ASTNode* astnExp;
	token t_Tag;

public:
	ASTLengthNode()
	{
		astnExp = NULL;
	}
	~ASTLengthNode()
	{

	}

	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(ASTNode* astnExp, token* t_Tag);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FEMoneD domain
class ASTdomainOptionNode : public ASTNode
{
public:
	std::vector<ASTNode*> astvnDomain;
	token tTag;

public:
	ASTdomainOptionNode()
	{
	}

	~ASTdomainOptionNode()
	{
		for (std::vector<ASTNode*>::iterator it = astvnDomain.begin(); it != astvnDomain.end(); ++it)
		{
			if (*it)
				delete *it;
		}
	}

	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(std::vector<ASTNode*> &astvnDomain, token* tTag);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FEMoneD equation
class ASTFEMoneDEquation : public ASTNode
{
public:
	ASTNode* astnDomain;
	std::vector<ASTNode*> astvnDomain;
	ASTNode* astnEqn;
	ASTNode* astnBoundaryLeft;
	ASTNode* astnBoundaryRight;
	int iLeftBoundaryType;
	int iRightBoundaryType;

public:
	ASTFEMoneDEquation()
	{
		iLeftBoundaryType = -1;
		iRightBoundaryType = -1;
		astnDomain = NULL;
		astnEqn = NULL;
		astnBoundaryLeft = NULL;
		astnBoundaryRight = NULL;
	}

	~ASTFEMoneDEquation()
	{
		for (std::vector<ASTNode*>::iterator it = astvnDomain.begin(); it != astvnDomain.end(); ++it)
		{
			if (*it)
				delete *it;
		}

		if (astnEqn)
			delete astnEqn;

		if (astnBoundaryLeft)
			delete astnBoundaryLeft;

		if (astnBoundaryRight)
			delete astnBoundaryRight;
	}

	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(ASTNode* astDomain, ASTNode* astEqn, ASTNode* astnBoundaryLeft, ASTNode* astnBoundaryRight, ASTNode* astnLeftbType, ASTNode* astnRightbType);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FEM string index
class ASTstringIndex : public ASTNode
{
public:
	token tString;
	std::string sString;

public:
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(token* tString);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Boundary Condition for FEM oneD equation
class ASTBoundaryConditions : public ASTNode
{
public:
	token tBoundaryType;
	std::string sBoundaryType;

public:
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(token* tBoundaryType);
};


#endif // !AST_FEMONED
