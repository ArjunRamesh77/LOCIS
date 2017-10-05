#include "AST.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// model collection
ASTNode* ASTmodel_collectionNode::CreateNode(std::vector<ASTNode*> astvnModelCollection_arg)
{
    astvnModelCollection = astvnModelCollection_arg;
	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// model
ASTNode* ASTmodelNode::CreateNode(token* t_name_arg, std::vector<ASTNode*> &astvnModelStatements_arg, ASTNode* astnSimulationArgs_arg)
{
    tName = *t_name_arg;
    sName = tName.getValue();
    astvnModelStatements = astvnModelStatements_arg;
    astnSimulationArgs = astnSimulationArgs_arg;
	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// model entity group
ASTNode* ASTmodel_entity_decl_groupNode::CreateNode(ASTNode* astnModelEntityType_arg, ASTNode* astnNumtype_arg,
                                                    ASTNode* astnModelEntities_arg)
{
    astnModelEntityType = astnModelEntityType_arg;
    astnNumtype = astnNumtype_arg;
    astnModelEntities = astnModelEntities_arg;

    ASTgeneric_tokenNode* g_astnModelEntityType = (ASTgeneric_tokenNode*)astnModelEntityType;
    iDeclType = g_astnModelEntityType->get_iTokenType();

    if (iDeclType == DT_IDENT)
	{
        sModelBaseName = g_astnModelEntityType->get_sTokenValue();
		tEntityType = g_astnModelEntityType->tok;
	}
    else if (iDeclType == KW_VARIABLE || iDeclType == KW_PARAMETER)
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
            iNumType = KW_REAL;
		}
	}

    astvnModelEntities = dynamic_cast<ASTmodel_entities_collectionNode*>(astnModelEntities)->astvnModelEntities;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// model entity
ASTNode* ASTmodel_entity_declNode::CreateNode(ASTNode* astnTypeName_arg, ASTNode* astnOptions_arg, ASTNode* astnDefault_arg)
{
    astnTypeName = astnTypeName_arg;
    astnOptions = astnOptions_arg;
    astnDefault = astnDefault_arg;

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
ASTNode* ASTmodel_entities_collectionNode::CreateNode(std::vector<ASTNode*> astvnModelEntities_arg)
{
    astvnModelEntities = astvnModelEntities_arg;
	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// model section
ASTNode* ASTmodel_sectionNode::CreateNode(ASTNode* astnSectionType_arg, ASTNode* astnOptions_arg, ASTNode* astnSectionStatements_arg)
{
    astnSectionType = astnSectionType_arg;
    astnOptions = astnOptions_arg;
    astnSectionStatements = astnSectionStatements_arg;

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
ASTNode* ASTgeneric_tokenNode::CreateNode(token *tok_arg)
{
    tok = *tok_arg;
    iTokenType = tok.getType();
    sTokenValue = tok.getValue();

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
ASTNode* ASToptions_groupNode::CreateNode(std::vector<ASTNode*> &astnOptions_arg)
{
    astvnOptions = astnOptions_arg;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// option unit
ASTNode* ASTunit_optionNode::CreateNode(token* tUnitValue_arg)
{
    tUnitVal = *tUnitValue_arg;
    sValue = tUnitVal.getValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// option desc
ASTNode* ASTdesc_optionNode::CreateNode(token* tDescValue_arg)
{
    tDescValue = *tDescValue_arg;
    sValue = tDescValue.getValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// option type
ASTNode* ASTtype_optionNode::CreateNode(ASTNode* astnType_arg)
{
    astnType = astnType_arg;
    ASTgeneric_tokenNode* g_astnType = (ASTgeneric_tokenNode*)astnType;

	iNumType = g_astnType->get_iTokenType();
	sNumType = g_astnType->get_sTokenValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// option bounds
ASTNode* ASTbounds_optionNode::CreateNode(ASTNode* astnInequality_arg, ASTNode* astnExpr_arg)
{
    astnInequality = astnInequality_arg;
    astnExpr = astnExpr_arg;

    ASTgeneric_tokenNode* g_astnInequality = (ASTgeneric_tokenNode*)astnInequality;
	iInequalityOp = g_astnInequality->get_iTokenType();
	sInequalityOp = g_astnInequality->get_sTokenValue();
	tInequalityOp = g_astnInequality->tok;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// default value
ASTNode* ASTdefualtNode::CreateNode(token *tEqualityOp_arg, ASTNode* astnExpr_arg)
{
    tEqualityOp = *tEqualityOp_arg;
    astnExpr = astnExpr_arg;

    iEqualityOp = tEqualityOp.getType();
    sEqualityOp = tEqualityOp.getValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// default value
ASTNode* ASTequationNode::CreateNode(ASTNode* astnLHS_arg, ASTNode* astnEquationTypeOp_arg, ASTNode* astnRHS_arg)
{
    astnLHS = astnLHS_arg;
    astnEquationTypeOp = astnEquationTypeOp_arg;
    astnRHS = astnRHS_arg;

    ASTgeneric_tokenNode* g_astnEquationTypeOp = (ASTgeneric_tokenNode*)astnEquationTypeOp;
	iEquationTypeOp = g_astnEquationTypeOp->get_iTokenType();
	sEquationTypeOp = g_astnEquationTypeOp->get_sTokenValue();
	tEquationTypeOp = g_astnEquationTypeOp->tok;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Math Binary op
ASTNode* ASTMathBinaryOpNode::CreateNode(ASTNode* astnLeft_arg, token *tBinaryOp_arg, ASTNode* astnRight_arg)
{
    tBinaryOp = *tBinaryOp_arg;
    astnLeft = astnLeft_arg;
    astnRight = astnRight_arg;

    iBinaryOp = tBinaryOp.getType();
    sBinaryOp = tBinaryOp.getValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Math Unary op
ASTNode* ASTMathUnaryNode::CreateNode(token *tUnaryOp_arg, ASTNode* astnRight_arg)
{
    tUnaryOp = *tUnaryOp_arg;
    astnRight = astnRight_arg;

    iUnaryOp = tUnaryOp.getType();
    sUnaryOp = tUnaryOp.getValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Number
ASTNode* ASTNumberNode::CreateNode(token *tNumber_arg)
{
    tNumber = *tNumber_arg;
    iNumber = tNumber.getType();
    sNumber = tNumber.getValue();
	value = std::stod(sNumber);
	equation_data = sNumber;
	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Named Reference
ASTNode* ASTNamedReferenceNode::CreateNode(token *tdt_arg, token *tName_arg, ASTNode* astnArrayIndices_arg, ASTNode* astnDerivative_arg)
{
    tdt = *tdt_arg;
    tName = *tName_arg;
    astnArrayIndices = astnArrayIndices_arg;
    astnDerivative = astnDerivative_arg;

    sName = tName.getValue();

	bIsArray = false;
    if (astnArrayIndices != NULL)
		bIsArray = true;
	
    if (tdt.getType() == OP_DOLLAR)
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
ASTNode* ASTfunctionCallNode::CreateNode(token *tFunctionName_arg, std::vector<ASTNode*> &astvnFunctionArgs_arg)
{
    tFunctionName = *tFunctionName_arg;
    sFunctionName = tFunctionName.getValue();
    iFuncType = tFunctionName.getType();
    astvnFunctionArgs = astvnFunctionArgs_arg;
	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Array indices
ASTNode* ASTArrayIndicesNode::CreateNode(std::vector<ASTNode*> &astvnArrayIndices_arg)
{
    astvnArrayIndices = astvnArrayIndices_arg;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Array Index
ASTNode* ASTarray_indexNode::CreateNode(ASTNode* astnExpr_arg)
{
    astnExpr = astnExpr_arg;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Qualified Named Refernce
ASTNode* ASTQualifiedNamedReferenceNode::CreateNode(std::vector<ASTNode*> astvnQualifiedName_arg)
{
    astvnQualifiedName = astvnQualifiedName_arg;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Logical Binary op
ASTNode* ASTLogicalBinaryOpNode::CreateNode(ASTNode* astnLeft_arg, token *tBinaryOp_arg, ASTNode* astnRight_arg)
{
    astnLeft = astnLeft_arg;
    tBinaryOp = *tBinaryOp_arg;
    astnRight = astnRight_arg;

    iBinaryOp = tBinaryOp.getType();
    sBinaryOp = tBinaryOp.getValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Logical Unary op
ASTNode* ASTLogicalUnaryNode::CreateNode(token *tUnaryOp_arg, ASTNode* astnRight_arg)
{
    tUnaryOp = *tUnaryOp_arg;
    astnRight = astnRight_arg;

    iUnaryOp = tUnaryOp.getType();
    sUnaryOp = tUnaryOp.getValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// For loop
ASTNode* ASTfor_loopNode::CreateNode(token *tName_arg, ASTNode* astnStartExpr_arg, ASTNode* astnTillExpr_arg, ASTNode* astnIncrExpr_arg,
                                     ASTNode* astnStatementBlock_arg)
{
    tName = *tName_arg;
    astnStartExpr = astnStartExpr_arg;
    astnTillExpr = astnTillExpr_arg;
    astnIncrExpr = astnIncrExpr_arg;
    astnStatementBlock = astnStatementBlock_arg;

    sName = tName.getValue();

    astvnfors = dynamic_cast<ASTstatement_blockNode*>(astnStatementBlock)->astvnStatements;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// if statement
ASTNode* ASTif_statmentNode::CreateNode(ASTNode* astnCondition_arg, ASTNode* astnThen_arg, ASTNode* astnElse_arg)
{
    astnCondition = astnCondition_arg;
    astnThen = astnThen_arg;
    astnElse = astnElse_arg;

    if (astnElse != NULL)
		bHasElse = true;

    astvnifs = dynamic_cast<ASTstatement_blockNode*>(astnThen)->astvnStatements;
    if(astnElse)
        astvnelses = dynamic_cast<ASTstatement_blockNode*>(astnElse)->astvnStatements;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// statement block
ASTNode* ASTstatement_blockNode::CreateNode(std::vector<ASTNode*> &astvnStatements_arg)
{
    astvnStatements = astvnStatements_arg;

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// assignment
ASTNode* ASTassignmentNode::CreateNode(ASTNode* astnLHS_arg, token *tEquationTypeOp_arg, ASTNode* astnRHS_arg)
{
    astnLHS = astnLHS_arg;
    tEquationTypeOp = *tEquationTypeOp_arg;
    astnRHS = astnRHS_arg;

    iEquationTypeOp = tEquationTypeOp.getType();
    sEquationTypeOp = tEquationTypeOp.getValue();

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// create the derivative for a single variable
ASTNode* ASTDerivativeSingleVar::CreateNode(std::vector<token>& astvtnSingleQuotes_arg)
{
    astvtnSingleQuotes = astvtnSingleQuotes_arg;
    iNumQuotes = astvtnSingleQuotes.size();
	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Container for Simulation Arguments
ASTNode* ASTSimulationArgs::CreateNode(token* simType_arg, ASTNode* simStartt_arg, ASTNode* simEndt_arg,
                                       token* simNumSteps_arg, ASTNode* simAbsTol_arg, ASTNode* simRelTol_arg)
{
    simType = *simType_arg;
    simStartt = simStartt_arg;
    simEndt = simEndt_arg;
    simNumSteps = *simNumSteps_arg;
    simAbsTol = simAbsTol_arg;
    simRelTol = simRelTol_arg;

	// Get the numbers
    dsimStartt = std::stod(static_cast<ASTNumberNode*>(simStartt)->tNumber.getValue());
    dsimEndt = std::stod(static_cast<ASTNumberNode*>(simEndt)->tNumber.getValue());
    dsimAbsTol = std::stod(static_cast<ASTNumberNode*>(simAbsTol)->tNumber.getValue());
    dsimRelTol = std::stod(static_cast<ASTNumberNode*>(simRelTol)->tNumber.getValue());

	return this;
}
