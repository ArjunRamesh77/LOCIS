#ifndef LOCIS_AST_H
#define LOCIS_AST_H

#include "lexer.h"
#include "Symbol.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Contains the implementation of LOCIS AST
class NodeVisitor;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// All rules for parser
enum rule {
	model, section, parameter_section, variable_section, options, options_space, option, unit, desc, type,
	equation_section, numtype, parameter, type_name, array_indices, array_index, full_type_name, function_name,
	function_args, function_arg, term, default1, expression, factor, primary, real_number, bounds, inequality, variable,
	equationn, equation_type_operator, logical_expression, lterm, lfactor, lprimary, for_loop, for_body, statement,
	set_section, fix_section, initialize_section, object_section, object, if_statement, then, pelse, assignment,
	model_entity_decl, model_entity_type, model_entities, model_entity, model_section, section_type, model_statment,
	noneq_statment, eq_statment, model_statement, comparison_type_operator, single_or_compound_statement, math_bin_op,
	math_un_op, number, logical_bin_op, logical_un_op, generic_token, model_collection, functionCall, simulation_args
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Node Visitor dispatcher (pure abstract class)
#define VIRTUAL_DECL_DISPATCH(name) virtual ASTNode* dispatch(name*) = 0;
#define DECL_DISPATCH(name) ASTNode* dispatch(name*);
#define DISPATCH_FUCTION_DECL ASTNode* visit(NodeVisitor* ns) override { return ns->dispatch(this); }

#define UNUSED_AST_NODE_FUNCTION(MAIN_CLASS, NODE_TYPE) ASTNode* MAIN_CLASS::dispatch(NODE_TYPE* node) {return NULL;} 

#define AST_DOMAIN_NODE 0

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Node Visitor Class forward defines
class ASTmodel_collectionNode;
class ASTmodelNode;
class ASTmodel_entity_decl_groupNode;
class ASTmodel_entity_declNode;
class ASTmodel_entities_collectionNode;
class ASTmodel_sectionNode;
class ASTgeneric_tokenNode;
class ASToptions_groupNode;
class ASTunit_optionNode;
class ASTdesc_optionNode;
class ASTtype_optionNode;
class ASTbounds_optionNode;
class ASTdefualtNode;
class ASTequationNode;
class ASTMathBinaryOpNode;
class ASTMathUnaryNode;
class ASTNumberNode;
class ASTNamedReferenceNode;
class ASTArrayIndicesNode;
class ASTarray_indexNode;
class ASTQualifiedNamedReferenceNode;
class ASTLogicalBinaryOpNode;
class ASTLogicalUnaryNode;
class ASTfor_loopNode;
class ASTif_statmentNode;
class ASTstatement_blockNode;
class ASTassignmentNode;
class ASTfunctionCallNode;
class ASTSimulationArgs;

//FEMoneD
class ASTBasisTypeNode;
class ASTSegmentAttributesNode;
class ASTPdevarAttributesNode;
class ASTSegmentIdTypeNode;
class ASTNumelNode;
class ASTLengthNode;
class ASTdomainOptionNode;
class ASTFEMoneDEquation;
class ASTstringIndex;
class ASTDerivativeSingleVar;
class ASTBoundaryConditions;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Node Visitor Class
class NodeVisitor
{
public:
	VIRTUAL_DECL_DISPATCH(ASTmodel_collectionNode)
	VIRTUAL_DECL_DISPATCH(ASTmodelNode)
	VIRTUAL_DECL_DISPATCH(ASTmodel_entity_decl_groupNode)
	VIRTUAL_DECL_DISPATCH(ASTmodel_entity_declNode)
	VIRTUAL_DECL_DISPATCH(ASTmodel_entities_collectionNode)
	VIRTUAL_DECL_DISPATCH(ASTmodel_sectionNode)
	VIRTUAL_DECL_DISPATCH(ASTgeneric_tokenNode)
	VIRTUAL_DECL_DISPATCH(ASToptions_groupNode)
	VIRTUAL_DECL_DISPATCH(ASTunit_optionNode)
	VIRTUAL_DECL_DISPATCH(ASTdesc_optionNode)
	VIRTUAL_DECL_DISPATCH(ASTtype_optionNode)
	VIRTUAL_DECL_DISPATCH(ASTbounds_optionNode)
	VIRTUAL_DECL_DISPATCH(ASTdefualtNode)
	VIRTUAL_DECL_DISPATCH(ASTequationNode)
	VIRTUAL_DECL_DISPATCH(ASTMathBinaryOpNode)
	VIRTUAL_DECL_DISPATCH(ASTMathUnaryNode)
	VIRTUAL_DECL_DISPATCH(ASTNumberNode)
	VIRTUAL_DECL_DISPATCH(ASTNamedReferenceNode)
	VIRTUAL_DECL_DISPATCH(ASTfunctionCallNode)
	VIRTUAL_DECL_DISPATCH(ASTArrayIndicesNode)
	VIRTUAL_DECL_DISPATCH(ASTarray_indexNode)
	VIRTUAL_DECL_DISPATCH(ASTQualifiedNamedReferenceNode)
	VIRTUAL_DECL_DISPATCH(ASTLogicalBinaryOpNode)
	VIRTUAL_DECL_DISPATCH(ASTLogicalUnaryNode)
	VIRTUAL_DECL_DISPATCH(ASTfor_loopNode)
	VIRTUAL_DECL_DISPATCH(ASTif_statmentNode)
	VIRTUAL_DECL_DISPATCH(ASTstatement_blockNode)
	VIRTUAL_DECL_DISPATCH(ASTassignmentNode)
	VIRTUAL_DECL_DISPATCH(ASTSimulationArgs)
	
	//FEMoneD
	VIRTUAL_DECL_DISPATCH(ASTBasisTypeNode)
	VIRTUAL_DECL_DISPATCH(ASTSegmentAttributesNode)
	VIRTUAL_DECL_DISPATCH(ASTPdevarAttributesNode)
	VIRTUAL_DECL_DISPATCH(ASTSegmentIdTypeNode)
	VIRTUAL_DECL_DISPATCH(ASTNumelNode)
	VIRTUAL_DECL_DISPATCH(ASTLengthNode)
	VIRTUAL_DECL_DISPATCH(ASTdomainOptionNode)
	VIRTUAL_DECL_DISPATCH(ASTFEMoneDEquation)
	VIRTUAL_DECL_DISPATCH(ASTstringIndex)
	VIRTUAL_DECL_DISPATCH(ASTDerivativeSingleVar)
	VIRTUAL_DECL_DISPATCH(ASTBoundaryConditions)
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Base Node
class ASTNode
{
	
public:
	int id;

	//types
	bool bIsFEMtrialFunction;
	bool bIsStringIndex;
	int iNumDerivs;

	//type storage
	int meType;
	ModelEntity* me;

	// Scalar temporary storage
	std::string equation_data;		
	double value;
	bool lvalue;

	ASTNode* node;

public:
	virtual ~ASTNode() {};
	ASTNode()
	{
		iNumDerivs = 0;
		meType = -1;
		me = NULL;
		equation_data.assign("");
		node = NULL;
		value = 0.0;
		id = -1;
		lvalue = false;
		bIsFEMtrialFunction = false;
		bIsStringIndex = false;
	}
	virtual ASTNode* CreateNode(std::vector<ASTNode*> &astvnDomain, token* tTag)
	{
		return node;
	}
	virtual ASTNode* visit(NodeVisitor*) = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// model collection
class ASTmodel_collectionNode : public ASTNode
{
	
public:
	std::vector<ASTNode*> astvnModelCollection;

public:
	ASTmodel_collectionNode() : ASTNode()	{

	}
	~ASTmodel_collectionNode()
	{
		for (std::vector<ASTNode*>::iterator it = astvnModelCollection.begin(); it != astvnModelCollection.end(); ++it)
		{
			if(*it)
				delete *it;
		}
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(std::vector<ASTNode*> astvnModelCollection);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// model
class ASTmodelNode : public ASTNode
{
	
public:
	token tName;
	std::string sName;
	std::vector<ASTNode*> astvnModelStatements;
	ASTNode* astnSimulationArgs;

public:
	ASTmodelNode() : ASTNode()	
	{
		astnSimulationArgs = NULL;
		sName.assign("");
	}
	~ASTmodelNode()
	{
		for (std::vector<ASTNode*>::iterator it = astvnModelStatements.begin(); it != astvnModelStatements.end(); ++it)
		{
			if (*it)
				delete *it;
		}

		if (astnSimulationArgs)
			delete astnSimulationArgs;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(token* t_name, std::vector<ASTNode*> &astvnModelStatements, ASTNode* astnSimulationArgs);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// model entity group (collects a group of similar entities together)
class ASTmodel_entity_decl_groupNode : public ASTNode
{
	
public:
	ASTNode* astnModelEntityType;
	ASTNode* astnNumtype;
	ASTNode* astnModelEntities;

	int iDeclType;
	int iNumType;
	std::string sModelBaseName;
	std::vector<ASTNode*> astvnModelEntities;

	token tEntityType;
	token tEntityNumberType;

public:
	ASTmodel_entity_decl_groupNode() : ASTNode()	{
		astnModelEntityType = NULL;
		astnNumtype = NULL;
		astnModelEntities = NULL;
		iDeclType = -1;
		iNumType = -1;
		sModelBaseName.assign("");
	}
	~ASTmodel_entity_decl_groupNode()
	{
		if (astnModelEntityType)
			delete astnModelEntityType;
		if (astnNumtype)
			delete astnNumtype;
		if (astnModelEntities)
			delete astnModelEntities;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(ASTNode* astnModelEntityType, ASTNode* astnNumtype, ASTNode* astnModelEntities);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// model entity
class ASTmodel_entity_declNode : public ASTNode
{
	
public:
	ASTNode* astnTypeName;
	ASTNode* astnOptions;
	ASTNode* astnDefault;

	std::string sName;
	bool bIsdt;
	bool bIsArray;
	bool bHasOptions;
	bool bHasDefault;
	token tName;
	std::vector<ASTNode*> astvnOptions;
	std::vector<ASTNode*> astvnArrayIndices;
public:
	ASTmodel_entity_declNode() : ASTNode()	{
		astnTypeName = NULL;
		astnOptions = NULL;
		astnDefault = NULL;
		bIsdt = false;
		bIsArray = false;
		bHasOptions = false;
		bHasDefault = false;
	}
	~ASTmodel_entity_declNode()
	{
		if (astnTypeName)
			delete astnTypeName;
		if (astnOptions)
			delete astnOptions;
		if (astnDefault)
			delete astnDefault;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(ASTNode* astnTypeName, ASTNode* astnOptions, ASTNode* astnDefault);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// model entities collection
class ASTmodel_entities_collectionNode : public ASTNode
{
	
public:
	std::vector<ASTNode*> astvnModelEntities;

public:
	ASTmodel_entities_collectionNode() : ASTNode()	{

	}
	~ASTmodel_entities_collectionNode()
	{
		for (std::vector<ASTNode*>::iterator it = astvnModelEntities.begin(); it != astvnModelEntities.end(); ++it)
		{
			if (*it)
				delete *it;
		}
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(std::vector<ASTNode*> astvnModelEntities);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// model section
class ASTmodel_sectionNode : public ASTNode
{
	
public:
	ASTNode* astnSectionType;
	ASTNode* astnOptions;
	ASTNode* astnSectionStatements;

	token tSectionName;
	std::string sName;
	bool bHasOptions;
	std::vector<ASTNode*> astvnSectionStatements;
public:
	ASTmodel_sectionNode() : ASTNode()	{
		astnSectionType = NULL;
		astnOptions = NULL;
		astnSectionStatements = NULL;
		sName.assign("");
		bHasOptions = false;
	}
	~ASTmodel_sectionNode()
	{
		if (astnSectionType)
			delete astnSectionType;
		if (astnOptions)
			delete astnOptions;
		if (astnSectionStatements)
			delete astnSectionStatements;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(ASTNode* astnSectionType, ASTNode* astnOptions, ASTNode* astnSectionStatements);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// generic token only node
class ASTgeneric_tokenNode : public ASTNode
{
	
public:
	token tok;
	int iTokenType;
	std::string sTokenValue;

public:
	ASTgeneric_tokenNode() : ASTNode()	{
		iTokenType = -1;
		sTokenValue.assign("");
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(token *tok);
	int get_iTokenType();
	std::string get_sTokenValue();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// options group
class ASToptions_groupNode : public ASTNode
{

public:
	std::vector<ASTNode*> astvnOptions;

public:
	ASToptions_groupNode() : ASTNode()	{

	}
	~ASToptions_groupNode()
	{
		for (std::vector<ASTNode*>::iterator it = astvnOptions.begin(); it != astvnOptions.end(); ++it)
		{
			if (*it)
				delete *it;
		}
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(std::vector<ASTNode*> &astnOptions);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// option unit
class ASTunit_optionNode : public ASTNode
{
	
public:
	ASTNode* astnUnitValue;

	token tUnitVal;
	std::string sValue;
public:
	ASTunit_optionNode() : ASTNode()	{
		astnUnitValue = NULL;
		sValue.assign("");
	}
	~ASTunit_optionNode()
	{
		if (astnUnitValue)
			delete astnUnitValue;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(token* tUnitVal);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// option desc
class ASTdesc_optionNode : public ASTNode
{
	
public:
	ASTNode* astnNodeDescValue;

	token tDescValue;
	std::string sValue;
public:
	ASTdesc_optionNode() : ASTNode()	{
		astnNodeDescValue = NULL;
		sValue.assign("");
	}
	~ASTdesc_optionNode()
	{
		if (astnNodeDescValue)
			delete astnNodeDescValue;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(token* tNodeDescValue);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// option type
class ASTtype_optionNode : public ASTNode
{
	
public:
	ASTNode* astnType;

	int iNumType;
	std::string sNumType;
public:
	ASTtype_optionNode() : ASTNode()	{
		astnType = NULL;
		iNumType = -1;
		sNumType.assign("");
	}
	~ASTtype_optionNode()
	{
		if (astnType)
			delete astnType;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(ASTNode* astnType);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// option bounds
class ASTbounds_optionNode : public ASTNode
{
	
public:
	ASTNode* astnInequality;
	ASTNode* astnExpr;

	int iInequalityOp;
	token tInequalityOp;
	std::string sInequalityOp;
public:
	ASTbounds_optionNode() : ASTNode()	{
		astnInequality = NULL;
		astnExpr = NULL;
		iInequalityOp = -1;
		sInequalityOp.assign("");
	}
	~ASTbounds_optionNode()
	{
		if (astnInequality)
			delete astnInequality;
		if (astnExpr)
			delete astnInequality;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(ASTNode* astnInequality, ASTNode* astnExpr);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// default value
class ASTdefualtNode : public ASTNode
{
	
public:
	token tEqualityOp;
	ASTNode* astnExpr;

	int iEqualityOp;
	std::string sEqualityOp;
public:
	ASTdefualtNode() : ASTNode()	{
		astnExpr = NULL;
		iEqualityOp = -1;
		sEqualityOp.assign("");
	}
	~ASTdefualtNode()
	{
		if (astnExpr)
			delete astnExpr;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(token *tEqualityOp, ASTNode* astnExpr);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// default value
class ASTequationNode : public ASTNode
{
	
public:
	ASTNode* astnLHS;
	ASTNode* astnEquationTypeOp;
	ASTNode* astnRHS;

	token tEquationTypeOp;
	int iEquationTypeOp;
	std::string sEquationTypeOp;

public:
	ASTequationNode() : ASTNode()	{
		astnLHS = NULL;
		astnEquationTypeOp = NULL;
		astnRHS = NULL;
		iEquationTypeOp = -1;
		sEquationTypeOp.assign("");
	}
	~ASTequationNode()
	{
		if (astnLHS)
			delete astnLHS;
		if (astnEquationTypeOp)
			delete astnEquationTypeOp;
		if (astnRHS)
			delete astnRHS;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(ASTNode* astnLHS, ASTNode* astnEquationTypeOp, ASTNode* astnRHS);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Math Binary op
class ASTMathBinaryOpNode : public ASTNode
{
	
public:
	ASTNode* astnLeft;
	token tBinaryOp;
	ASTNode* astnRight;

	int iBinaryOp;
	std::string sBinaryOp;
public:
	ASTMathBinaryOpNode() : ASTNode()	{
		astnLeft = NULL;
		astnRight = NULL;
		iBinaryOp = -1;
		sBinaryOp.assign("");
	}
	~ASTMathBinaryOpNode()
	{
		if (astnLeft)
			delete astnLeft;
		if (astnRight)
			delete astnRight;
	}
	//DISPATCH_FUCTION_DECL
	ASTNode* visit(NodeVisitor* ns) override { return ns->dispatch(this); }
	ASTNode* CreateNode(ASTNode* astnLeft, token *tBinaryOp, ASTNode* astnRight);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Math Unary op
class ASTMathUnaryNode : public ASTNode
{
	
public:
	token tUnaryOp;
	ASTNode* astnRight;

	int iUnaryOp;
	std::string sUnaryOp;
public:
	ASTMathUnaryNode() : ASTNode()	{
		astnRight = NULL;
		iUnaryOp = -1;
		sUnaryOp.assign("");
	}
	~ASTMathUnaryNode()
	{
		if (astnRight)
			delete astnRight;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(token *tUnaryOp, ASTNode* astnRight);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Number
class ASTNumberNode : public ASTNode
{
	
public:
	token tNumber;

	int iNumber;
	std::string  sNumber;
public:
	ASTNumberNode() : ASTNode()	{
		value = 0.0;
		iNumber = -1;
		sNumber.assign("");
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(token *tNumber);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Named Reference
class ASTNamedReferenceNode : public ASTNode
{

public:
	token tdt;
	token tName;
	ASTNode* astnArrayIndices;
	ASTNode* astnDerivative;

	std::string sName;
	bool bIsArray;
	bool bIsdt;
	std::vector<ASTNode*> astvnArrayIndices;
public:
	ASTNamedReferenceNode() : ASTNode()	{
		bIsdt = false;
		astnArrayIndices = NULL;
		sName.assign("");
		bIsArray = false;
	}
	~ASTNamedReferenceNode()
	{
		if (astnArrayIndices)
			delete astnArrayIndices;
		if (astnDerivative)
			delete astnDerivative;
	}
	bool get_IsArray();
	std::string get_sName();
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(token *tdt,token *tName, ASTNode* astnArrayIndices, ASTNode* astnDerivative);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Array indices
class ASTArrayIndicesNode : public ASTNode
{

public:
	std::vector<ASTNode*> astvnArrayIndices;

public:
	ASTArrayIndicesNode() : ASTNode()	{

	}
	~ASTArrayIndicesNode()
	{
		for (std::vector<ASTNode*>::iterator it = astvnArrayIndices.begin(); it != astvnArrayIndices.end(); ++it)
		{
			if (*it)
				delete *it;
		}
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(std::vector<ASTNode*> &astvnArrayIndices);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Array Index
class ASTarray_indexNode : public ASTNode
{
	
public:
	ASTNode* astnExpr;

public:
	ASTarray_indexNode() : ASTNode()	{
		astnExpr = NULL;
	}
	~ASTarray_indexNode()
	{
		if (astnExpr)
			delete astnExpr;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(ASTNode* astnExpr);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Qualified Named Reference
class ASTQualifiedNamedReferenceNode : public ASTNode
{
	
public:
	std::vector<ASTNode*> astvnQualifiedName;

public:
	ASTQualifiedNamedReferenceNode() : ASTNode()	{

	}
	~ASTQualifiedNamedReferenceNode()
	{
		for (std::vector<ASTNode*>::iterator it = astvnQualifiedName.begin(); it != astvnQualifiedName.end(); ++it)
		{
			if (*it)
				delete *it;
		}
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(std::vector<ASTNode*> astvnQualifiedName);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Simple function 
class ASTfunctionCallNode : public ASTNode
{
	
public:
	token tFunctionName;
	std::string sFunctionName;
	int iFuncType;
	std::vector<ASTNode*> astvnFunctionArgs;
public:
	ASTfunctionCallNode() : ASTNode()	{
		sFunctionName.assign("");
		iFuncType = -1;
	}
	~ASTfunctionCallNode()
	{
		for (std::vector<ASTNode*>::iterator it = astvnFunctionArgs.begin(); it != astvnFunctionArgs.end(); ++it)
		{
			if (*it)
				delete *it;
		}
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(token *tFunctionName, std::vector<ASTNode*> &astvnFunctionArgs);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Logical Binary op
class ASTLogicalBinaryOpNode : public ASTNode
{
	
public:
	ASTNode* astnLeft;
	token tBinaryOp;
	ASTNode* astnRight;

	int iBinaryOp;
	std::string sBinaryOp;
public:
	ASTLogicalBinaryOpNode() : ASTNode()	{
		astnLeft = NULL;
		astnRight = NULL;
		iBinaryOp = -1;
		sBinaryOp.assign("");
	}
	~ASTLogicalBinaryOpNode()
	{
		if (astnLeft)
			delete astnLeft;
		if (astnRight)
			delete astnRight;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(ASTNode* astnLeft, token *tBinaryOp, ASTNode* astnRight);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Logical Unary op
class ASTLogicalUnaryNode : public ASTNode
{
	
public:
	token tUnaryOp;
	ASTNode* astnRight;

	int iUnaryOp;
	std::string sUnaryOp;
public:
	ASTLogicalUnaryNode() : ASTNode()	{
		astnRight = NULL;
		iUnaryOp = -1;
		sUnaryOp.assign("");
	}
	~ASTLogicalUnaryNode()
	{
		if (astnRight)
			delete astnRight;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(token *tUnaryOp, ASTNode* astnRight);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// For loop
class ASTfor_loopNode : public ASTNode
{
	
public:
	token tName;
	ASTNode* astnStartExpr;
	ASTNode* astnTillExpr;
	ASTNode* astnIncrExpr;
	ASTNode* astnStatementBlock;

	std::string sName;
	std::vector<ASTNode*> astvnfors;

public:
	ASTfor_loopNode() : ASTNode()	{
		astnStartExpr = NULL;
		astnTillExpr = NULL;
		astnIncrExpr = NULL;
		astnStatementBlock = NULL;
		sName.assign("");
	}
	~ASTfor_loopNode()
	{
		if (astnStartExpr)
			delete astnStartExpr;
		if (astnTillExpr)
			delete astnTillExpr;
		if (astnIncrExpr)
			delete astnIncrExpr;
		if (astnStatementBlock)
			delete astnStatementBlock;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(token *tName, ASTNode* astnStartExpr, ASTNode* astnTillExpr, ASTNode* astnIncrExpr, ASTNode* astnStatementBlock);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// if statement
class ASTif_statmentNode : public ASTNode
{
	
public:
	ASTNode* astnCondition;
	ASTNode* astnThen;
	ASTNode* astnElse;

	bool bHasElse;
	std::vector<ASTNode*> astvnifs;
	std::vector<ASTNode*> astvnelses;

public:
	ASTif_statmentNode() : ASTNode()	{
		astnCondition = NULL;
		astnThen = NULL;
		astnElse = NULL;
		bHasElse = false;
	}
	~ASTif_statmentNode()
	{
		if (astnCondition)
			delete astnCondition;
		if (astnThen)
			delete astnThen;
		if (astnElse)
			delete astnElse;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(ASTNode* astnCondition, ASTNode* astnThen, ASTNode* astnElse);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// statement block
class ASTstatement_blockNode : public ASTNode
{
	
public:
	std::vector<ASTNode*> astvnStatements;

public:
	ASTstatement_blockNode() : ASTNode()	{

	}
	~ASTstatement_blockNode()
	{
		for (std::vector<ASTNode*>::iterator it = astvnStatements.begin(); it != astvnStatements.end(); ++it)
		{
			if (*it)
				delete *it;
		}
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(std::vector<ASTNode*> &astvnStatements);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// assignment
class ASTassignmentNode : public ASTNode
{
	
public:
	ASTNode* astnLHS;
	token tEquationTypeOp;
	ASTNode* astnRHS;

	int iEquationTypeOp;
	std::string sEquationTypeOp;
public:
	ASTassignmentNode() : ASTNode()	{
		astnLHS = NULL;
		astnRHS = NULL;
		iEquationTypeOp = -1;
		sEquationTypeOp.assign("");
	}
	~ASTassignmentNode()
	{
		if (astnLHS)
			delete astnLHS;
		if (astnRHS)
			delete astnRHS;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(ASTNode* astnLHS, token *tEquationTypeOp, ASTNode* astnRHS);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Stores all the models
class modelCollection
{

public:
	std::unordered_map<std::string, ASTNode*> models;

public:
	modelCollection();
	~modelCollection();

	// Insert a new model
	bool insertModel(std::string &symbolName, ASTNode* node);

	// Get model
	ASTNode* getModel(std::string &symbolName);

	// Get all models
	std::unordered_map<std::string, ASTNode*>* getAllmodel();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// single variable derivative
class ASTDerivativeSingleVar : public ASTNode
{
public:
	int iNumQuotes;
	std::vector<token> astvtnSingleQuotes;

public:
	ASTDerivativeSingleVar()
	{
		iNumQuotes = 0;
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(std::vector<token>& astvtnSingleQuotes);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Container for Simulation Arguments
class ASTSimulationArgs : public ASTNode
{
public:
	token simType;
	ASTNode* simStartt;
	ASTNode* simEndt;
	token simNumSteps;
	ASTNode* simAbsTol;
	ASTNode* simRelTol;

	double dsimStartt;
	double dsimEndt;
	double dsimAbsTol;
	double dsimRelTol;

public:
	ASTSimulationArgs()
	{
		simStartt = NULL;
		simEndt = NULL;
		simAbsTol = NULL;
		simRelTol = NULL;

		dsimStartt = 0.0;
		dsimEndt = 0.0;
		dsimAbsTol = 0.0;
		dsimRelTol = 0.0;
	}

	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(token* simType, ASTNode* simStartt, ASTNode* simEndt, token* simNumSteps, ASTNode* simAbsTol, ASTNode* simRelTol);
};

#endif // !LOCIS_AST_H