#include "interpreter.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Object circular reference error during first pass
void interpreter::semanticErr_ModelNoExist(ASTNode* node)
{
	ASTmodel_entity_decl_groupNode* errNode = static_cast<ASTmodel_entity_decl_groupNode*>(node);

	int pos, line;
	static_cast<ASTgeneric_tokenNode*>(errNode->astnModelEntityType)->tok.GetLnAndPos(line, pos);

	errptr->SetError(1001, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine("object of model " + errNode->sModelBaseName + " is not defined");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Multiple declarations of Model
void interpreter::semanticErr_MultipleModelDefinitions(ASTNode* node)
{
	ASTmodelNode* modelNode = static_cast<ASTmodelNode*>(node);

	int line, pos;
	modelNode->tName.GetLnAndPos(line, pos);

	errptr->SetError(1002, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine("model " + modelNode->sName + " is already defined");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Variable initialization failure
void interpreter::semanticErr_FailedToInitializeAll()
{
	errptr->SetError(1004, "SEMATIC ERROR", -1, -1);
	errptr->AddErrorLine(" Failed to initialize all variables.");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// first pass consistency check failure
void interpreter::semanticErr_FirstPassConsistencyCheck()
{
	errptr->SetError(1005, "SEMATIC ERROR", -1, -1);
	errptr->AddErrorLine(" First pass consistency check failure");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Model entities can only be declared at the top
void interpreter::semanticErr_EntityWrongPositionDecl(ASTNode* node)
{
	ASTmodel_entity_decl_groupNode* node1 = static_cast<ASTmodel_entity_decl_groupNode*>(node);

	int line, pos;
	node1->tEntityType.GetLnAndPos(line, pos);

	errptr->SetError(1013, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine("Model entities can only be declared at the start of the model");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The model object is trying to be declared without defining the model first
void interpreter::semanticErr_ModelUndefined(ASTNode* node)
{
	ASTmodel_entity_declNode* node1 = static_cast<ASTmodel_entity_declNode*>(node);

	int line, pos;
	node1->tName.GetLnAndPos(line, pos);

	errptr->SetError(1006, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Model named is undefined");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Trying to define a $variable
void interpreter::semanticErr_dtVariableDefined(ASTNode* node)
{
	ASTmodel_entity_declNode* node1 = static_cast<ASTmodel_entity_declNode*>(node);

	int line, pos;
	node1->tName.GetLnAndPos(line, pos);

	errptr->SetError(1006, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" A time dependent variable cannot be defined directly.");
	errptr->AddErrorLine(" It is automatically defined with a definition of a variable.");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Trying to define a $variable
void interpreter::semanticErr_EntityTypeNoVector(ASTNode* node)
{
	ASTmodel_entity_declNode* node1 = static_cast<ASTmodel_entity_declNode*>(node);

	int line, pos;
	node1->tName.GetLnAndPos(line, pos);

	errptr->SetError(1006, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" An iteration variable has to be a scalar.");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Trying to have a default for interation varriable
void interpreter::semanticErr_EntityTypeNoDefault(const std::string EntityBase, ASTNode* node)
{
	ASTmodel_entity_declNode* node1 = static_cast<ASTmodel_entity_declNode*>(node);

	int line, pos;
	node1->tName.GetLnAndPos(line, pos);

	errptr->SetError(1006, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" An " + EntityBase + " variable cannot have a default.");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Trying to have a default for interation variable
void interpreter::semanticErr_EntityTypeNoOptions(const std::string EntityBaseType, ASTNode* node)
{
	ASTmodel_entity_declNode* node1 = static_cast<ASTmodel_entity_declNode*>(node);

	int line, pos;
	node1->tName.GetLnAndPos(line, pos);

	errptr->SetError(1006, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" An " + EntityBaseType + " variable cannot have options.");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Null evaluation
void interpreter::semanticErr_NULLevaluation(ASTNode* node)
{

	errptr->SetError(1006, "SEMANTIC ERROR", -1, -1);
	errptr->AddErrorLine(" A node returned a null value");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Model entity with the same name already exists
void interpreter::semanticErr_EntityRedeclaration(ASTNode* node)
{
	ASTmodel_entity_declNode* node1 = static_cast<ASTmodel_entity_declNode*>(node);

	int line, pos;
	node1->tName.GetLnAndPos(line, pos);

	errptr->SetError(1006, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Entity with name \"" + node1->tName.GetValue() +"\" already declared");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Model entity with the same name already exists
void interpreter::semanticErr_SectionRedeclaration(ASTNode* node)
{
	ASTmodel_sectionNode* node1 = static_cast<ASTmodel_sectionNode*>(node);

	int line, pos;
	node1->tSectionName.GetLnAndPos(line, pos);

	errptr->SetError(1006, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Section \"" + node1->sName + "\" is already declared");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Model entity with the same name already exists
void interpreter::semanticErr_SectionBehindGuess(ASTNode* node)
{
	ASTmodel_sectionNode* node1 = static_cast<ASTmodel_sectionNode*>(node);

	int line, pos;
	node1->tSectionName.GetLnAndPos(line, pos);

	errptr->SetError(1006, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Section \"" + node1->sName + "\" must be defined before \"GUESS\"");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Section must be before INIT or EQUATION
void interpreter::semanticErr_SectionBehindEquationInit(ASTNode* node)
{
	ASTmodel_sectionNode* node1 = static_cast<ASTmodel_sectionNode*>(node);

	int line, pos;
	node1->tSectionName.GetLnAndPos(line, pos);

	errptr->SetError(1006, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Section \"" + node1->sName + "\" must be defined before \"INIT\" or \"EQUATION\"");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bounds option only for variables
void interpreter::semanticErr_BoundsOnlyForVariables(ASTNode* node)
{
	ASTbounds_optionNode* node1 = static_cast<ASTbounds_optionNode*>(node);

	int line, pos;
	node1->tInequalityOp.GetLnAndPos(line, pos);

	errptr->SetError(1006, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" The entity " + IS.ob_tok.GetValue() + " cannot have a " + node1->sInequalityOp + " option");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Currently only "=" supported in equation
void interpreter::semanticErr_OnlyEqualityInEquation(ASTNode* node)
{
	ASTequationNode* node1 = static_cast<ASTequationNode*>(node);

	int line, pos;
	node1->tEquationTypeOp.GetLnAndPos(line, pos);

	errptr->SetError(1007, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Currently only equality equations are supported in the the equality section");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Only an ITER variable can be used for iteration
void interpreter::semanticErr_OnlyIterAllowedInFor(ASTNode* node)
{
	ASTfor_loopNode* node1 = static_cast<ASTfor_loopNode*>(node);

	int line, pos;
	node1->tName.GetLnAndPos(line, pos);

	errptr->SetError(1008, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Only an iteration variable may be used in a for loop");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Invalid for
void interpreter::semanticErr_EndlessForDetected(ASTNode* node)
{
	ASTfor_loopNode* node1 = static_cast<ASTfor_loopNode*>(node);

	int line, pos;
	node1->tName.GetLnAndPos(line, pos);

	errptr->SetError(1009, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Invalid for statement, results in infinite iterations");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Undeclared entity
void interpreter::semanticErr_UndeclaredEntity(token tok)
{
	int line, pos;
	tok.GetLnAndPos(line, pos);

	errptr->SetError(1009, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Entity " + tok.GetValue() + " does not exist in the model");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Not a vector
void interpreter::semanticErr_NotAVector(token tok)
{
	int line, pos;
	tok.GetLnAndPos(line, pos);

	errptr->SetError(1009, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Entity " + tok.GetValue() + " is not a vector");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dimension mismatch
void interpreter::semanticErr_DimensionMismatch(token tok)
{
	int line, pos;
	tok.GetLnAndPos(line, pos);

	errptr->SetError(1009, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Entity " + tok.GetValue() + " has a dimension mismatch while referencing");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Index out of bounds
void interpreter::semanticErr_IndexOutOfBounds(token tok)
{
	int line, pos;
	tok.GetLnAndPos(line, pos);

	errptr->SetError(1009, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Entity " + tok.GetValue() + " has a dimension mismatch while referencing");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Invalid Qualification
void interpreter::semanticErr_InvalidQualification(token tok)
{
	int line, pos;
	tok.GetLnAndPos(line, pos);

	errptr->SetError(1009, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Entity " + tok.GetValue() + " cannot be qualified");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Uninitialized vector
void interpreter::semanticErr_UninitializedVector(token tok)
{
	int line, pos;
	tok.GetLnAndPos(line, pos);

	errptr->SetError(1009, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Entity " + tok.GetValue() + " is not initialized");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Not a scalar
void interpreter::semanticErr_EntityIsNotAScalar(token tok)
{
	int line, pos;
	tok.GetLnAndPos(line, pos);

	errptr->SetError(1009, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Entity " + tok.GetValue() + " is not a scalar, cannot be referenced as one");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// iter cannot be assigned
void interpreter::semanticErr_IterCannotBeAssigned(token tok)
{
	int line, pos;
	tok.GetLnAndPos(line, pos);

	errptr->SetError(1009, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Entity " + tok.GetValue() + " is an iteration variable, cannot be directly assigned");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Only parameters can be set
void interpreter::semanticErr_OnlyParametersSet(token tok)
{
	int line, pos;
	tok.GetLnAndPos(line, pos);

	errptr->SetError(1009, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Entity " + tok.GetValue() + " is not a parameter, only parameters can be set");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// No bounds allowed
void interpreter::semanticErr_NoBoundsAllowed(token tok)
{
	int line, pos;
	tok.GetLnAndPos(line, pos);

	errptr->SetError(1009, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Entity " + tok.GetValue() + " cannot have bounds");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Only Variables can be fix
void interpreter::semanticErr_OnlyVariablesFix(token tok)
{
	int line, pos;
	tok.GetLnAndPos(line, pos);

	errptr->SetError(1009, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Entity " + tok.GetValue() + " only Variables can be fixed");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Only Variables can be fix
void interpreter::semanticErr_OnlyVariablesGuess(token tok)
{
	int line, pos;
	tok.GetLnAndPos(line, pos);

	errptr->SetError(1009, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Entity " + tok.GetValue() + " only Variables can be given guesses");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// No bounds can be set in guess
void interpreter::semanticErr_NoBoundsInGuess(token tok)
{
	int line, pos;
	tok.GetLnAndPos(line, pos);

	errptr->SetError(1009, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Entity " + tok.GetValue() + " No bounds can be set in Guess section");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// domain evaluation
void interpreter::semanticErr_ExpectedAVariableRefInDomain(ASTNode* node)
{
	ASTFEMoneDEquation* node1 = static_cast<ASTFEMoneDEquation*>(node);
	ASTQualifiedNamedReferenceNode* node2 = static_cast<ASTQualifiedNamedReferenceNode*>(node1->astnDomain);
	ASTNamedReferenceNode* node3 = static_cast<ASTNamedReferenceNode*>(node2->astvnQualifiedName.at(0));

	int line, pos;
	node3->tName.GetLnAndPos(line, pos);

	errptr->SetError(1010, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Expected a variable reference in a domain");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Missing domain information
void interpreter::semanticErr_NoDomainAssociatedWithVar(ASTNode* node)
{
	ASTFEMoneDEquation* node1 = static_cast<ASTFEMoneDEquation*>(node);
	ASTQualifiedNamedReferenceNode* node2 = static_cast<ASTQualifiedNamedReferenceNode*>(node1->astnDomain);
	ASTNamedReferenceNode* node3 = static_cast<ASTNamedReferenceNode*>(node2->astvnQualifiedName.at(0));

	int line, pos;
	node3->tName.GetLnAndPos(line, pos);

	errptr->SetError(1011, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Missing domain information");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Not a valid Field variable
void interpreter::semanticErr_NotAFieldVariable(ASTNode* node)
{
	ASTFEMoneDEquation* node1 = static_cast<ASTFEMoneDEquation*>(node);
	ASTQualifiedNamedReferenceNode* node2 = static_cast<ASTQualifiedNamedReferenceNode*>(node1->astnDomain);
	ASTNamedReferenceNode* node3 = static_cast<ASTNamedReferenceNode*>(*(node2->astvnQualifiedName.end() - 1));

	int line, pos;
	node3->tName.GetLnAndPos(line, pos);

	errptr->SetError(1012, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node3->sName + "is not a valid field variable");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Not a valid Field variable
void interpreter::semanticErr_NoArgsFunction(ASTNode* node)
{
	ASTfunctionCallNode* node1 = static_cast<ASTfunctionCallNode*>(node);

	int line, pos;
	node1->tFunctionName.GetLnAndPos(line, pos);

	errptr->SetError(1012, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node1->sFunctionName + " | has zero arguments");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FEM equations failed to generate
void interpreter::semanticErr_FEMEquationGenFail(ASTNode* node)
{
	ASTFEMoneDEquation* node1 = static_cast<ASTFEMoneDEquation*>(node);
	ASTQualifiedNamedReferenceNode* node2 = static_cast<ASTQualifiedNamedReferenceNode*>(node1->astnDomain);
	ASTNamedReferenceNode* node3 = static_cast<ASTNamedReferenceNode*>(node2->astvnQualifiedName.at(0));

	int line, pos;
	node3->tName.GetLnAndPos(line, pos);

	errptr->SetError(1012, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node3->sName + " | Internal error generating FEM equations");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name not found
void interpreter::semanticErr_InvalidFunction(ASTNode* node)
{
	ASTfunctionCallNode* node1 = static_cast<ASTfunctionCallNode*>(node);

	int line, pos;
	node1->tFunctionName.GetLnAndPos(line, pos);

	errptr->SetError(1012, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node1->sFunctionName + " | Function not found");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Invalid basis
void interpreter::semanticErr_InvalidBasisType(ASTNode* node)
{
	ASTBasisTypeNode* node1 = static_cast<ASTBasisTypeNode*>(node);

	int line, pos;
	node1->tBasisType.GetLnAndPos(line, pos);

	errptr->SetError(1012, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node1->sBasisType + " | is in invalid basis type.");
	errptr->AddErrorLine("Try linear, quadratic or cubic");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Segment ID redefinition
void interpreter::semanticErr_RedefinitionOfSegmentID(ASTNode* node)
{
	ASTSegmentIdTypeNode* node1 = static_cast<ASTSegmentIdTypeNode*>(node);

	int line, pos;
	node1->tID.GetLnAndPos(line, pos);

	errptr->SetError(1012, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node1->sID + " | is already defined for this segment");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Redefinition of number of elements <Add a support token)
void interpreter::semanticErr_RedefinitionNumberOfElements(ASTNode* node)
{
	ASTNumelNode* node1 = static_cast<ASTNumelNode*>(node);
	
	int line, pos;
	node1->t_Tag.GetLnAndPos(line, pos);

	errptr->SetError(1012, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node1->t_Tag.GetValue() + " | this attribute is already defined");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Redefinition of Length of element <Add a support token>
void interpreter::semanticErr_RedefinitionLength(ASTNode* node)
{
	ASTLengthNode* node1 = static_cast<ASTLengthNode*>(node);

	int line, pos;
	node1->t_Tag.GetLnAndPos(line, pos);

	errptr->SetError(1012, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node1->t_Tag.GetValue() + " | this attribute is already defined");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Redefinition of Length of element <Add a support token>
void interpreter::semanticErr_MissingSegmentAttributes(token tok)
{
	int line, pos;
	tok.GetLnAndPos(line, pos);

	errptr->SetError(1012, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(tok.GetValue() + " | this object has some missing segment attributes.");
	errptr->AddErrorLine("This could be \"id\", \"numel\" or \"length\"");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Redefinition of Length of element <Add a support token>
void interpreter::semanticErr_SegmentIDExistsAlready(ASTNode* node)
{
	ASTSegmentIdTypeNode* node1 = static_cast<ASTSegmentIdTypeNode*>(node);

	int line, pos;
	node1->tID.GetLnAndPos(line, pos);

	errptr->SetError(1012, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node1->sID + " | This segment does not have a unique ID, it already exists in the object");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PDE id redefinition
void interpreter::semanticErr_RedefinitionPDEid(ASTNode* node)
{
	ASTSegmentIdTypeNode* node1 = static_cast<ASTSegmentIdTypeNode*>(node);

	int line, pos;
	node1->tID.GetLnAndPos(line, pos);

	errptr->SetError(1012, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node1->sID + " | Redefinition of \"id\" attribute");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PDE id redefinition
void interpreter::semanticErr_RedefinitionDomains(ASTNode* node)
{
	ASTdomainOptionNode* node1 = static_cast<ASTdomainOptionNode*>(node);

	int line, pos;
	node1->tTag.GetLnAndPos(line, pos);

	errptr->SetError(1012, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node1->tTag.GetValue() + " | Redefinition of attribute");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Domain does not exist
void interpreter::semanticErr_DomainDoesNotExist(ASTNode* node)
{
	ASTNamedReferenceNode* node1 = static_cast<ASTNamedReferenceNode*>(node);

	int line, pos;
	node1->tName.GetLnAndPos(line, pos);

	errptr->SetError(1012, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node1->sName + " | domain with this name has not been defined");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Duplicate domain 
void interpreter::semanticErr_RedefinitionDomainName(ASTNode* node)
{
	ASTNamedReferenceNode* node1 = static_cast<ASTNamedReferenceNode*>(node);

	int line, pos;
	node1->tName.GetLnAndPos(line, pos);

	errptr->SetError(1012, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node1->sName + " | domain with this name has already been applied");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Model entity with the same name already exists
void interpreter::semanticErr_UndefinedLine(ASTNode* node)
{
	ASTmodel_entity_declNode* node1 = static_cast<ASTmodel_entity_declNode*>(node);

	int line, pos;
	node1->tName.GetLnAndPos(line, pos);

	errptr->SetError(1006, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node1->sName + "| the line is undefined, Needs options to define it");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Missing reference to domain or Variable
void interpreter::semanticErr_MissingReferenceToDomOrVar(ASTNode* node)
{
	ASTNamedReferenceNode* node1 = static_cast<ASTNamedReferenceNode*>(node);

	int line, pos;
	node1->tName.GetLnAndPos(line, pos);

	errptr->SetError(1006, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node1->sName + "| the line is undefined, Needs options to define it");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// For an index in a domain, it can only be string
void interpreter::semanticErr_IndexInDomainOnlyString(ASTNode* node)
{
	ASTNamedReferenceNode* node1 = static_cast<ASTNamedReferenceNode*>(node);

	int line, pos;
	node1->tName.GetLnAndPos(line, pos);

	errptr->SetError(1006, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node1->sName + "| only string indices allowed for this type");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Invalid string index
void interpreter::semanticErr_InvalidStringIndex(ASTNode* node)
{
	ASTstringIndex* node1 = static_cast<ASTstringIndex*>(node);

	int line, pos;
	node1->tString.GetLnAndPos(line, pos);

	errptr->SetError(1006, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node1->sString + "| is an invalid index");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Direct reference to Variable without domain info
void interpreter::semanticErr_DirectVarWithoutDomain(ASTNode* node)
{
	ASTNamedReferenceNode* node1 = static_cast<ASTNamedReferenceNode*>(node);

	int line, pos;
	node1->tName.GetLnAndPos(line, pos);

	errptr->SetError(1006, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node1->sName + "| Cannot be directly referenced without a domain qualification");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Direct reference to Variable without domain info
void interpreter::semanticErr_OnlyGuessForFEMVars(ASTNode* node)
{
	ASTNamedReferenceNode* node1 = static_cast<ASTNamedReferenceNode*>(node);

	int line, pos;
	node1->tName.GetLnAndPos(line, pos);

	errptr->SetError(1006, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(node1->sName + "| only guesses can be applied to FEM type variables");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Direct reference to Variable without domain info
void interpreter::semanticErr_CalculatingBoundaryValues(token tok)
{
	int line, pos;
	tok.GetLnAndPos(line, pos);

	errptr->SetError(1009, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Entity " + tok.GetValue() + " | invalid inputs causing error in calculating boundary values");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// option redefinition
void interpreter::semanticErr_optionRedefined(const std::string, ModelEntity* me)
{
	int line, pos;
	me->tok->GetLnAndPos(line, pos);

	errptr->SetError(1009, "SEMANTIC ERROR", line, pos);
	errptr->AddErrorLine(" Entity " + me->tok->GetValue() + " | The option 'description' has been redefined for this option");
	errptr->AddError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}