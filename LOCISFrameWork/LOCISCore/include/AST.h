#ifndef LOCIS_AST_H
#define LOCIS_AST_H

#include "lexer.h"
#include "Symbol.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Node Visitor dispatcher (pure abstract class)
#define VIRTUAL_DECL_DISPATCH(name) virtual ASTNode* dispatch(name*) = 0;

#define DECL_DISPATCH(name) ASTNode* dispatch(name*);

#define DISPATCH_FUCTION_DECL ASTNode* visit(NodeVisitor* ns) override { return ns->dispatch(this); }

#define UNUSED_AST_NODE_FUNCTION(MAIN_CLASS, NODE_TYPE) ASTNode* MAIN_CLASS::dispatch(NODE_TYPE* node) {return NULL;} 

#define AST_DOMAIN_NODE 0

#define DELETE_VECTOR(name) for (std::vector<ASTNode*>::iterator it = name.begin(); it != name.end(); ++it) \
                            { \
                                if(*it) \
                                    delete *it; \
                            } \
                            name.clear();

# define DELETE_SCALAR(name) if(name) delete name;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Node Visitor Class forward defines
class NodeVisitor;
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
    modelEntity* me;

	// Scalar temporary storage
	std::string equation_data;		
    double value;
	bool lvalue;

	ASTNode* node;

public:
    virtual ~ASTNode() {}
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
    virtual ASTNode* CreateNode(std::vector<ASTNode*> &astvnDomain, token tTag)
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
    ASTmodel_collectionNode() : ASTNode()
    {

	}
	~ASTmodel_collectionNode()
	{
        DELETE_VECTOR(astvnModelCollection)
	}
	DISPATCH_FUCTION_DECL
    ASTNode* CreateNode(std::vector<ASTNode*> astvnModelCollection_arg);
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
    ASTmodelNode() : ASTNode(),
        tName(),
        sName(""),
        astvnModelStatements(),
        astnSimulationArgs(NULL)
	{
	}

	~ASTmodelNode()
	{
        DELETE_VECTOR(astvnModelStatements)
        DELETE_SCALAR(astnSimulationArgs)
	}
	DISPATCH_FUCTION_DECL
    ASTNode* CreateNode(token* t_name_arg, std::vector<ASTNode*> &astvnModelStatements_arg, ASTNode* astnSimulationArgs_arg);
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
    ASTmodel_entity_decl_groupNode() : ASTNode(),
        astnModelEntityType(NULL),
        astnNumtype(NULL),
        astnModelEntities(),
        iDeclType(-1),
        iNumType(-1),
        sModelBaseName(""),
        astvnModelEntities(),
        tEntityType(),
        tEntityNumberType()
    {
	}
	~ASTmodel_entity_decl_groupNode()
	{
        DELETE_SCALAR(astnModelEntityType)
        DELETE_SCALAR(astnNumtype)
        DELETE_SCALAR(astnModelEntities)
	}
	DISPATCH_FUCTION_DECL
    ASTNode* CreateNode(ASTNode* astnModelEntityType_arg, ASTNode* astnNumtype_arg, ASTNode* astnModelEntities_arg);
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
    ASTmodel_entity_declNode() : ASTNode(),
        astnTypeName(NULL),
        astnOptions(NULL),
        astnDefault(NULL),
        sName(""),
        bIsdt(false),
        bIsArray(false),
        bHasOptions(false),
        bHasDefault(false),
        tName(),
        astvnOptions(),
        astvnArrayIndices()
    {
	}
	~ASTmodel_entity_declNode()
	{
        DELETE_SCALAR(astnTypeName)
        DELETE_SCALAR(astnOptions)
        DELETE_SCALAR(astnDefault)
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
    ASTmodel_entities_collectionNode() : ASTNode(),
        astvnModelEntities()
    {

	}
	~ASTmodel_entities_collectionNode()
	{
        DELETE_VECTOR(astvnModelEntities)
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
    ASTmodel_sectionNode() : ASTNode(),
        astnSectionType(NULL),
        astnOptions(NULL),
        astnSectionStatements(NULL),
        tSectionName(),
        sName(""),
        bHasOptions(false),
        astvnSectionStatements()
    {

	}
	~ASTmodel_sectionNode()
	{
        DELETE_SCALAR(astnSectionType)
        DELETE_SCALAR(astnOptions)
        DELETE_SCALAR(astnSectionStatements)
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
    ASTgeneric_tokenNode() : ASTNode(),
        tok(),
        iTokenType(-1),
        sTokenValue("")
    {

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
    ASToptions_groupNode() : ASTNode(),
        astvnOptions()
    {

	}
	~ASToptions_groupNode()
	{
        DELETE_VECTOR(astvnOptions)
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
    ASTunit_optionNode() : ASTNode(),
        astnUnitValue(NULL),
        tUnitVal(),
        sValue("")
    {

	}
	~ASTunit_optionNode()
	{
        DELETE_SCALAR(astnUnitValue)
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
    ASTdesc_optionNode() : ASTNode(),
        astnNodeDescValue(NULL),
        tDescValue(),
        sValue("")
    {

	}
	~ASTdesc_optionNode()
	{
        DELETE_SCALAR(astnNodeDescValue)
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
    ASTtype_optionNode() : ASTNode(),
        astnType(NULL),
        iNumType(-1),
        sNumType("")
    {

	}
	~ASTtype_optionNode()
	{
        DELETE_SCALAR(astnType)
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
    ASTbounds_optionNode() : ASTNode(),
        astnInequality(NULL),
        astnExpr(NULL),
        iInequalityOp(-1),
        tInequalityOp(),
        sInequalityOp("")
    {

	}
	~ASTbounds_optionNode()
	{
        DELETE_SCALAR(astnInequality)
        DELETE_SCALAR(astnExpr)
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
    ASTdefualtNode() : ASTNode(),
        tEqualityOp(),
        astnExpr(NULL),
        iEqualityOp(-1),
        sEqualityOp("")
    {

	}
	~ASTdefualtNode()
	{
        DELETE_SCALAR(astnExpr)
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
    ASTequationNode() : ASTNode(),
        astnLHS(NULL),
        astnEquationTypeOp(NULL),
        astnRHS(NULL),
        tEquationTypeOp(),
        iEquationTypeOp(-1),
        sEquationTypeOp("")
    {

	}
	~ASTequationNode()
	{
        DELETE_SCALAR(astnLHS)
        DELETE_SCALAR(astnEquationTypeOp)
        DELETE_SCALAR(astnRHS)
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
    ASTMathBinaryOpNode() : ASTNode(),
        astnLeft(NULL),
        tBinaryOp(),
        astnRight(NULL),
        iBinaryOp(-1),
        sBinaryOp("")
    {

	}
	~ASTMathBinaryOpNode()
	{
        DELETE_SCALAR(astnLeft)
        DELETE_SCALAR(astnRight)
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
    ASTMathUnaryNode() : ASTNode(),
        tUnaryOp(),
        astnRight(NULL),
        iUnaryOp(-1),
        sUnaryOp("")
    {

	}
	~ASTMathUnaryNode()
	{
        DELETE_SCALAR(astnRight)
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
    ASTNumberNode() : ASTNode(),
        tNumber(),
        iNumber(-1),
        sNumber("")
    {

	}
    ~ASTNumberNode()
    {

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
    ASTNamedReferenceNode() : ASTNode(),
        tdt(),
        tName(),
        astnArrayIndices(NULL),
        astnDerivative(NULL),
        sName(""),
        bIsArray(false),
        bIsdt(false),
        astvnArrayIndices()
    {

	}
	~ASTNamedReferenceNode()
	{
        DELETE_SCALAR(astnArrayIndices)
        DELETE_SCALAR(astnDerivative)
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
    ASTArrayIndicesNode() : ASTNode(),
        astvnArrayIndices()
    {

	}
	~ASTArrayIndicesNode()
	{
        DELETE_VECTOR(astvnArrayIndices)
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
    ASTarray_indexNode() : ASTNode(),
        astnExpr(NULL)
    {

	}
	~ASTarray_indexNode()
	{
        DELETE_SCALAR(astnExpr)
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
    ASTQualifiedNamedReferenceNode() : ASTNode(),
        astvnQualifiedName()
    {

	}
	~ASTQualifiedNamedReferenceNode()
	{
        DELETE_VECTOR(astvnQualifiedName)
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
    ASTfunctionCallNode() : ASTNode(),
        tFunctionName(),
        sFunctionName(""),
        iFuncType(-1),
        astvnFunctionArgs()
    {
		sFunctionName.assign("");
		iFuncType = -1;
	}
	~ASTfunctionCallNode()
	{
        DELETE_VECTOR(astvnFunctionArgs)
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
    ASTLogicalBinaryOpNode() : ASTNode(),
        astnLeft(NULL),
        tBinaryOp(),
        astnRight(NULL),
        iBinaryOp(-1),
        sBinaryOp("")
    {

	}
	~ASTLogicalBinaryOpNode()
	{
        DELETE_SCALAR(astnLeft)
        DELETE_SCALAR(astnRight)
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
    ASTLogicalUnaryNode() : ASTNode(),
        tUnaryOp(),
        astnRight(NULL),
        iUnaryOp(-1),
        sUnaryOp("")
    {

	}
	~ASTLogicalUnaryNode()
	{
        DELETE_SCALAR(astnRight)
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
    ASTfor_loopNode() : ASTNode(),
        astnStartExpr(NULL),
        astnTillExpr(NULL),
        astnIncrExpr(NULL),
        astnStatementBlock(NULL),
        sName(""),
        astvnfors()
    {

	}
	~ASTfor_loopNode()
	{
        DELETE_SCALAR(astnStartExpr)
        DELETE_SCALAR(astnTillExpr)
        DELETE_SCALAR(astnIncrExpr)
        DELETE_SCALAR(astnStatementBlock)
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
    ASTif_statmentNode() : ASTNode(),
        astnCondition(NULL),
        astnThen(NULL),
        astnElse(NULL),
        bHasElse(false),
        astvnifs(),
        astvnelses()
    {
	}
	~ASTif_statmentNode()
	{
        DELETE_SCALAR(astnCondition)
        DELETE_SCALAR(astnThen)
        DELETE_SCALAR(astnElse)
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
    ASTstatement_blockNode() : ASTNode(),
        astvnStatements()
    {

	}
	~ASTstatement_blockNode()
	{
        DELETE_VECTOR(astvnStatements)
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
    ASTassignmentNode() : ASTNode(),
        astnLHS(NULL),
        tEquationTypeOp(),
        astnRHS(NULL),
        iEquationTypeOp(-1),
        sEquationTypeOp("")
    {

	}
	~ASTassignmentNode()
	{
        DELETE_SCALAR(astnLHS)
        DELETE_SCALAR(astnRHS)
	}
	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(ASTNode* astnLHS, token *tEquationTypeOp, ASTNode* astnRHS);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// single variable derivative
class ASTDerivativeSingleVar : public ASTNode
{
public:
	int iNumQuotes;
	std::vector<token> astvtnSingleQuotes;

public:
    ASTDerivativeSingleVar() : ASTNode(),
        iNumQuotes(0),
        astvtnSingleQuotes()
	{

	}
    ~ASTDerivativeSingleVar()
    {

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
    ASTSimulationArgs() : ASTNode(),
        simType(),
        simStartt(NULL),
        simEndt(NULL),
        simNumSteps(),
        simAbsTol(NULL),
        simRelTol(NULL),
        dsimStartt(0),
        dsimEndt(0),
        dsimAbsTol(0),
        dsimRelTol(0)

	{

	}
    ~ASTSimulationArgs()
    {
        DELETE_SCALAR(simStartt)
        DELETE_SCALAR(simEndt)
        DELETE_SCALAR(simAbsTol)
        DELETE_SCALAR(simRelTol)
    }

	DISPATCH_FUCTION_DECL
	ASTNode* CreateNode(token* simType, ASTNode* simStartt, ASTNode* simEndt, token* simNumSteps, ASTNode* simAbsTol, ASTNode* simRelTol);
};

#endif // !LOCIS_AST_H
