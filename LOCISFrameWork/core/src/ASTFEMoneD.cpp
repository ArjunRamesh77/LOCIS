#include "ASTFEMoneD.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Basis type 
ASTNode* ASTBasisTypeNode::CreateNode(token* t_BasisType)
{
	tBasisType = *t_BasisType;
	sBasisType = t_BasisType->getValue();
	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Segment Attributes
ASTNode* ASTSegmentAttributesNode::CreateNode(std::vector<ASTNode*> &astvnSegmentAttributes)
{
	this->astvnSegmentAttributes = astvnSegmentAttributes;
	return this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PDEvar attributes
ASTNode* ASTPdevarAttributesNode::CreateNode(std::vector<ASTNode*> &astvnSegmentAttributes)
{
	this->astvnSegmentAttributes = astvnSegmentAttributes;
	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Segment ID name
ASTNode* ASTSegmentIdTypeNode::CreateNode(token* t_ID)
{
	tID = *t_ID;
	sID = t_ID->getValue();
	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Segment numel
ASTNode* ASTNumelNode::CreateNode(ASTNode* astnExp, token* t_Tag)
{
	this->astnExp = astnExp;
	this->t_Tag = *t_Tag;
	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Segment length
ASTNode* ASTLengthNode::CreateNode(ASTNode* astnExp, token* t_Tag)
{
	this->astnExp = astnExp;
	this->t_Tag = *t_Tag;
	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// domain description
ASTNode* ASTdomainOptionNode::CreateNode(std::vector<ASTNode*> &astvnDomain, token* tTag)
{
	this->astvnDomain = astvnDomain;
	this->tTag = *tTag;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FEM one D equation
ASTNode* ASTFEMoneDEquation::CreateNode(ASTNode* astnDomain, ASTNode* astnEqn, ASTNode* astnBoundaryLeft, ASTNode* astnBoundaryRight, ASTNode* astnLeftbType, ASTNode* astnRightbType)
{
	this->astnDomain = astnDomain;
	this->astnEqn = astnEqn;
	this->astnBoundaryLeft = astnBoundaryLeft;
	this->astnBoundaryRight = astnBoundaryRight;

	//Get boundary Conditions
	if (astnLeftbType)
	{
		ASTBoundaryConditions* bc = static_cast<ASTBoundaryConditions*>(astnLeftbType);
		iLeftBoundaryType = bc->tBoundaryType.getType();
	}

	if (astnRightbType)
	{
		ASTBoundaryConditions* bc = static_cast<ASTBoundaryConditions*>(astnRightbType);
		iRightBoundaryType = bc->tBoundaryType.getType();
	}

	astvnDomain = static_cast<ASTQualifiedNamedReferenceNode*>(astnDomain)->astvnQualifiedName;
	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FEM string index
ASTNode* ASTstringIndex::CreateNode(token* tString)
{
	this->tString = *tString;
	bIsStringIndex = true;
	sString = tString->getValue();
	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FEM oneD boundary conditions
ASTNode* ASTBoundaryConditions::CreateNode(token* tBoundaryType)
{
	this->tBoundaryType = *tBoundaryType;
	sBoundaryType = tBoundaryType->getValue();
	return this;
}
