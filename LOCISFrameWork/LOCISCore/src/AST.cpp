#include "AST.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// modelCollections
modelCollection::modelCollection()
{

}

modelCollection::~modelCollection()
{
	for (auto it = models.begin(); it != models.end(); ++it)
	{
		if (it->second)
			delete it->second;
	}
}

bool modelCollection::insertModel(std::string &symbolName, ASTNode* node)
{
	if (models[symbolName] == NULL)
	{
		//models.insert(std::make_pair(27, 10));
		models[symbolName] = node;
		return true;
	}
	return false; //model with same name already exists
}

ASTNode* modelCollection::getModel(std::string &symbolName)
{
	return models[symbolName];
}

std::unordered_map<std::string, ASTNode*>* modelCollection::getAllmodel()
{
	return &models;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// model collection
ASTNode* ASTmodel_collectionNode::CreateNode(std::vector<ASTNode*> astvnModelCollection)
{
	this->astvnModelCollection = astvnModelCollection;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// model
ASTNode* ASTmodelNode::CreateNode(token* t_name, std::vector<ASTNode*> &astvnModelStatements, ASTNode* astnSimulationArgs)
{
	this->tName = *t_name;
	sName = this->tName.GetValue();
	this->astvnModelStatements = astvnModelStatements;
	this->astnSimulationArgs = astnSimulationArgs;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// model entity group
ASTNode* ASTmodel_entity_decl_groupNode::CreateNode(ASTNode* astnModelEntityType, ASTNode* astnNumtype,
	ASTNode* astnModelEntities)
{
	this->astnModelEntityType = astnModelEntityType;
	this->astnNumtype = astnNumtype;
	this->astnModelEntities = astnModelEntities;

	ASTgeneric_tokenNode* g_astnModelEntityType = (ASTgeneric_tokenNode*)astnModelEntityType;
	iDeclType = g_astnModelEntityType->get_iTokenType();

	if (iDeclType == IDENT)
	{
		sModelBaseName = g_astnModelEntityType->get_sTokenValue();
		tEntityType = g_astnModelEntityType->tok;
	}
	else if (iDeclType == VARIABLE || iDeclType == PARAMETER)
	{
		// Default is Real
		if (astnNumtype)
		{
			g_astnModelEntityType = (ASTgeneric_tokenNode*)astnNumtype;
			iNumType = g_astnModelEntityType->get_iTokenType();
			tEntityNumberType = g_astnModelEntityType->tok;
		}
		else
		{
			iNumType = REAL;
		}
	}

	astvnModelEntities = dynamic_cast<ASTmodel_entities_collectionNode*>(astnModelEntities)->astvnModelEntities;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// model entity
ASTNode* ASTmodel_entity_declNode::CreateNode(ASTNode* astnTypeName, ASTNode* astnOptions, ASTNode* astnDefault)
{
	this->astnTypeName = astnTypeName;
	this->astnOptions = astnOptions;
	this->astnDefault = astnDefault;

	if (this->astnOptions)
		bHasOptions = true;

	if (this->astnDefault)
		bHasDefault = true;

	ASTNamedReferenceNode* g_astnTypeName = (ASTNamedReferenceNode*)astnTypeName;
	tName = g_astnTypeName->tName;
	sName = g_astnTypeName->get_sName();
	bIsArray = g_astnTypeName->get_IsArray();
	bIsdt = g_astnTypeName->bIsdt;

	if(bHasOptions)
		astvnOptions = dynamic_cast<ASToptions_groupNode*>(astnOptions)->astvnOptions;


	astvnArrayIndices = dynamic_cast<ASTNamedReferenceNode*>(astnTypeName)->astvnArrayIndices;

	return this;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// model entities

ASTNode* ASTmodel_entities_collectionNode::CreateNode(std::vector<ASTNode*> astvnModelEntities)
{
	this->astvnModelEntities = astvnModelEntities;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// model section
ASTNode* ASTmodel_sectionNode::CreateNode(ASTNode* astnSectionType, ASTNode* astnOptions, ASTNode* astnSectionStatements)
{
	this->astnSectionType = astnSectionType;
	this->astnOptions = astnOptions;
	this->astnSectionStatements = astnSectionStatements;

	if (this->astnOptions == NULL)
		bHasOptions = NULL;

	ASTgeneric_tokenNode* g_astnSectionType = (ASTgeneric_tokenNode*)astnSectionType;
	sName = g_astnSectionType->get_sTokenValue();
	tSectionName = g_astnSectionType->tok;

	astvnSectionStatements = dynamic_cast<ASTstatement_blockNode*>(astnSectionStatements)->astvnStatements;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// generic token only node
ASTNode* ASTgeneric_tokenNode::CreateNode(token *tok)
{
	this->tok = *tok;
	iTokenType = tok->GetType();
	sTokenValue = tok->GetValue();

	return this;
}

int ASTgeneric_tokenNode::get_iTokenType()
{
	return iTokenType;
}

std::string ASTgeneric_tokenNode::get_sTokenValue()
{
	return sTokenValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// options group
ASTNode* ASToptions_groupNode::CreateNode(std::vector<ASTNode*> &astnOptions)
{
	this->astvnOptions = astnOptions;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// option unit
ASTNode* ASTunit_optionNode::CreateNode(token* tUnitValue)
{
	this->tUnitVal = *tUnitValue;
	sValue = tUnitValue->GetValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// option desc
ASTNode* ASTdesc_optionNode::CreateNode(token* tDescValue)
{
	this->tDescValue = *tDescValue;

	sValue = tDescValue->GetValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// option type
ASTNode* ASTtype_optionNode::CreateNode(ASTNode* astnType)
{
	this->astnType = astnType;
	ASTgeneric_tokenNode* g_astnType = (ASTgeneric_tokenNode*)astnType;

	iNumType = g_astnType->get_iTokenType();
	sNumType = g_astnType->get_sTokenValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// option bounds
ASTNode* ASTbounds_optionNode::CreateNode(ASTNode* astnInequality, ASTNode* astnExpr)
{
	this->astnInequality = astnInequality;
	this->astnExpr = astnExpr;

	ASTgeneric_tokenNode* g_astnInequality = (ASTgeneric_tokenNode*)astnInequality;
	iInequalityOp = g_astnInequality->get_iTokenType();
	sInequalityOp = g_astnInequality->get_sTokenValue();
	tInequalityOp = g_astnInequality->tok;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// default value
ASTNode* ASTdefualtNode::CreateNode(token *tEqualityOp, ASTNode* astnExpr)
{
	this->tEqualityOp = *tEqualityOp;
	this->astnExpr = astnExpr;

	iEqualityOp = tEqualityOp->GetType();
	sEqualityOp = tEqualityOp->GetValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// default value
ASTNode* ASTequationNode::CreateNode(ASTNode* astnLHS, ASTNode* astnEquationTypeOp, ASTNode* astnRHS)
{
	this->astnLHS = astnLHS;
	this->astnEquationTypeOp = astnEquationTypeOp;
	this->astnRHS = astnRHS;

	ASTgeneric_tokenNode* g_astnEquationTypeOp = (ASTgeneric_tokenNode*)astnEquationTypeOp;
	iEquationTypeOp = g_astnEquationTypeOp->get_iTokenType();
	sEquationTypeOp = g_astnEquationTypeOp->get_sTokenValue();
	tEquationTypeOp = g_astnEquationTypeOp->tok;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Math Binary op
ASTNode* ASTMathBinaryOpNode::CreateNode(ASTNode* astnLeft, token *tBinaryOp, ASTNode* astnRight)
{
	this->tBinaryOp = *tBinaryOp;
	this->astnLeft = astnLeft;
	this->astnRight = astnRight;

	iBinaryOp = tBinaryOp->GetType();
	sBinaryOp = tBinaryOp->GetValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Math Unary op
ASTNode* ASTMathUnaryNode::CreateNode(token *tUnaryOp, ASTNode* astnRight)
{
	this->tUnaryOp = *tUnaryOp;
	this->astnRight = astnRight;

	iUnaryOp = tUnaryOp->GetType();
	sUnaryOp = tUnaryOp->GetValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Number
ASTNode* ASTNumberNode::CreateNode(token *tNumber)
{
	this->tNumber = *tNumber;
	iNumber = tNumber->GetType();
	sNumber = tNumber->GetValue();
	value = std::stod(sNumber);
	equation_data = sNumber;
	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Named Reference
ASTNode* ASTNamedReferenceNode::CreateNode(token *tdt, token *tName, ASTNode* astnArrayIndices, ASTNode* astnDerivative)
{
	this->tdt = *tdt;
	this->tName = *tName;
	this->astnArrayIndices = astnArrayIndices;
	this->astnDerivative = astnDerivative;

	sName = tName->GetValue();

	bIsArray = false;
	if (astnArrayIndices != NULL)
		bIsArray = true;
	
	if (tdt->GetType() == DOLLAR)
	{
		bIsdt = true;
		this->sName = "$" + this->sName; // Add dollar for unique identification of dt variable type
	}

	if(bIsArray)
		astvnArrayIndices = dynamic_cast<ASTArrayIndicesNode*>(astnArrayIndices)->astvnArrayIndices;

	// Derivative info
	if(astnDerivative)
		iNumDerivs = dynamic_cast<ASTDerivativeSingleVar*>(astnDerivative)->iNumQuotes;

	return this;
}

bool ASTNamedReferenceNode::get_IsArray()
{
	return bIsArray;
}

std::string ASTNamedReferenceNode::get_sName()
{
	return sName;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Call
ASTNode* ASTfunctionCallNode::CreateNode(token *tFunctionName, std::vector<ASTNode*> &astvnFunctionArgs)
{
	this->tFunctionName = *tFunctionName;
	sFunctionName = tFunctionName->GetValue();
	iFuncType = tFunctionName->GetType();
	this->astvnFunctionArgs = astvnFunctionArgs;
	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Array indices
ASTNode* ASTArrayIndicesNode::CreateNode(std::vector<ASTNode*> &astvnArrayIndices)
{
	this->astvnArrayIndices = astvnArrayIndices;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Array Index
ASTNode* ASTarray_indexNode::CreateNode(ASTNode* astnExpr)
{
	this->astnExpr = astnExpr;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Qualified Named Refernce
ASTNode* ASTQualifiedNamedReferenceNode::CreateNode(std::vector<ASTNode*> astvnQualifiedName)
{
	this->astvnQualifiedName = astvnQualifiedName;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Logical Binary op
ASTNode* ASTLogicalBinaryOpNode::CreateNode(ASTNode* astnLeft, token *tBinaryOp, ASTNode* astnRight)
{
	this->astnLeft = astnLeft;
	this->tBinaryOp = *tBinaryOp;
	this->astnRight = astnRight;

	iBinaryOp = tBinaryOp->GetType();
	sBinaryOp = tBinaryOp->GetValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Logical Unary op
ASTNode* ASTLogicalUnaryNode::CreateNode(token *tUnaryOp, ASTNode* astnRight)
{
	this->tUnaryOp = *tUnaryOp;
	this->astnRight = astnRight;

	iUnaryOp = tUnaryOp->GetType();
	sUnaryOp = tUnaryOp->GetValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// For loop
ASTNode* ASTfor_loopNode::CreateNode(token *tName, ASTNode* astnStartExpr, ASTNode* astnTillExpr, ASTNode* astnIncrExpr, ASTNode* astnStatementBlock)
{
	this->tName = *tName;
	this->astnStartExpr = astnStartExpr;
	this->astnTillExpr = astnTillExpr;
	this->astnIncrExpr = astnIncrExpr;
	this->astnStatementBlock = astnStatementBlock;

	sName = tName->GetValue();

	astvnfors = dynamic_cast<ASTstatement_blockNode*>(astnStatementBlock)->astvnStatements;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// if statement
ASTNode* ASTif_statmentNode::CreateNode(ASTNode* astnCondition, ASTNode* astnThen, ASTNode* astnElse)
{
	this->astnCondition = astnCondition;
	this->astnThen = astnThen;
	this->astnElse = astnElse;

	if (astnElse != NULL)
		bHasElse = true;

	astvnifs = dynamic_cast<ASTstatement_blockNode*>(astnThen)->astvnStatements;
	if(astnElse)
		astvnelses = dynamic_cast<ASTstatement_blockNode*>(astnElse)->astvnStatements;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// statement block
ASTNode* ASTstatement_blockNode::CreateNode(std::vector<ASTNode*> &astvnStatements)
{
	this->astvnStatements = astvnStatements;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// assignment
ASTNode* ASTassignmentNode::CreateNode(ASTNode* astnLHS, token *tEquationTypeOp, ASTNode* astnRHS)
{
	this->astnLHS = astnLHS;
	this->tEquationTypeOp = *tEquationTypeOp;
	this->astnRHS = astnRHS;

	iEquationTypeOp = tEquationTypeOp->GetType();
	sEquationTypeOp = tEquationTypeOp->GetValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// create the derivative for a single variable
ASTNode* ASTDerivativeSingleVar::CreateNode(std::vector<token>& astvtnSingleQuotes)
{
	this->astvtnSingleQuotes = astvtnSingleQuotes;
	iNumQuotes = astvtnSingleQuotes.size();
	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Container for Simulation Arguments
ASTNode* ASTSimulationArgs::CreateNode(token* simType, ASTNode* simStartt, ASTNode* simEndt, token* simNumSteps, ASTNode* simAbsTol, ASTNode* simRelTol)
{
	this->simType = *simType;
	this->simStartt = simStartt;
	this->simEndt = simEndt;
	this->simNumSteps = *simNumSteps;
	this->simAbsTol = simAbsTol;
	this->simRelTol = simRelTol;

	// Get the numbers
	dsimStartt = std::stod(static_cast<ASTNumberNode*>(simStartt)->tNumber.GetValue());
	dsimEndt = std::stod(static_cast<ASTNumberNode*>(simEndt)->tNumber.GetValue());
	dsimAbsTol = std::stod(static_cast<ASTNumberNode*>(simAbsTol)->tNumber.GetValue());
	dsimRelTol = std::stod(static_cast<ASTNumberNode*>(simRelTol)->tNumber.GetValue());

	return this;
}
