#include "interpreter.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Object circular reference error during first pass
void interpreter::semanticErr_ModelNoExist(ASTNode* node)
{
	ASTmodel_entity_decl_groupNode* errNode = static_cast<ASTmodel_entity_decl_groupNode*>(node);

	int pos, line;
	static_cast<ASTgeneric_tokenNode*>(errNode->astnModelEntityType)->tok.getLnAndPos(line, pos);

    errptr->setError(1001, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine("object of model " + errNode->sModelBaseName + " is not defined");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Multiple declarations of Model
void interpreter::semanticErr_MultipleModelDefinitions(ASTNode* node)
{
	ASTmodelNode* modelNode = static_cast<ASTmodelNode*>(node);

	int line, pos;
	modelNode->tName.getLnAndPos(line, pos);

    errptr->setError(1002, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine("model " + modelNode->sName + " is already defined");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Variable initialization failure
void interpreter::semanticErr_FailedToInitializeAll()
{
    errptr->setError(1004, "SEMATIC ERROR", -1, -1);
    errptr->addErrorLine(" Failed to initialize all variables.");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// first pass consistency check failure
void interpreter::semanticErr_FirstPassConsistencyCheck()
{
    errptr->setError(1005, "SEMATIC ERROR", -1, -1);
    errptr->addErrorLine(" First pass consistency check failure");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Model entities can only be declared at the top
void interpreter::semanticErr_EntityWrongPositionDecl(ASTNode* node)
{
	ASTmodel_entity_decl_groupNode* node1 = static_cast<ASTmodel_entity_decl_groupNode*>(node);

	int line, pos;
	node1->tEntityType.getLnAndPos(line, pos);

    errptr->setError(1013, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine("Model entities can only be declared at the start of the model");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The model object is trying to be declared without defining the model first
void interpreter::semanticErr_ModelUndefined(ASTNode* node)
{
	ASTmodel_entity_declNode* node1 = static_cast<ASTmodel_entity_declNode*>(node);

	int line, pos;
	node1->tName.getLnAndPos(line, pos);

    errptr->setError(1006, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Model named is undefined");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Trying to define a $variable
void interpreter::semanticErr_dtVariableDefined(ASTNode* node)
{
	ASTmodel_entity_declNode* node1 = static_cast<ASTmodel_entity_declNode*>(node);

	int line, pos;
	node1->tName.getLnAndPos(line, pos);

    errptr->setError(1006, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" A time dependent variable cannot be defined directly.");
    errptr->addErrorLine(" It is automatically defined with a definition of a variable.");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Trying to define a $variable
void interpreter::semanticErr_EntityTypeNoVector(ASTNode* node)
{
	ASTmodel_entity_declNode* node1 = static_cast<ASTmodel_entity_declNode*>(node);

	int line, pos;
	node1->tName.getLnAndPos(line, pos);

    errptr->setError(1006, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" An iteration variable has to be a scalar.");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Trying to have a default for interation varriable
void interpreter::semanticErr_EntityTypeNoDefault(const std::string EntityBase, ASTNode* node)
{
	ASTmodel_entity_declNode* node1 = static_cast<ASTmodel_entity_declNode*>(node);

	int line, pos;
	node1->tName.getLnAndPos(line, pos);

    errptr->setError(1006, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" An " + EntityBase + " variable cannot have a default.");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Trying to have a default for interation variable
void interpreter::semanticErr_EntityTypeNoOptions(const std::string EntityBaseType, ASTNode* node)
{
	ASTmodel_entity_declNode* node1 = static_cast<ASTmodel_entity_declNode*>(node);

	int line, pos;
	node1->tName.getLnAndPos(line, pos);

    errptr->setError(1006, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" An " + EntityBaseType + " variable cannot have options.");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Null evaluation
void interpreter::semanticErr_NULLevaluation(ASTNode* node)
{

    errptr->setError(1006, "SEMANTIC ERROR", -1, -1);
    errptr->addErrorLine(" A node returned a null value");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Model entity with the same name already exists
void interpreter::semanticErr_EntityRedeclaration(ASTNode* node)
{
	ASTmodel_entity_declNode* node1 = static_cast<ASTmodel_entity_declNode*>(node);

	int line, pos;
	node1->tName.getLnAndPos(line, pos);

    errptr->setError(1006, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Entity with name \"" + node1->tName.getValue() +"\" already declared");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Model entity with the same name already exists
void interpreter::semanticErr_SectionRedeclaration(ASTNode* node)
{
	ASTmodel_sectionNode* node1 = static_cast<ASTmodel_sectionNode*>(node);

	int line, pos;
	node1->tSectionName.getLnAndPos(line, pos);

    errptr->setError(1006, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Section \"" + node1->sName + "\" is already declared");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Model entity with the same name already exists
void interpreter::semanticErr_SectionBehindGuess(ASTNode* node)
{
	ASTmodel_sectionNode* node1 = static_cast<ASTmodel_sectionNode*>(node);

	int line, pos;
	node1->tSectionName.getLnAndPos(line, pos);

    errptr->setError(1006, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Section \"" + node1->sName + "\" must be defined before \"GUESS\"");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Section must be before INIT or EQUATION
void interpreter::semanticErr_SectionBehindEquationInit(ASTNode* node)
{
	ASTmodel_sectionNode* node1 = static_cast<ASTmodel_sectionNode*>(node);

	int line, pos;
	node1->tSectionName.getLnAndPos(line, pos);

    errptr->setError(1006, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Section \"" + node1->sName + "\" must be defined before \"INIT\" or \"EQUATION\"");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bounds option only for variables
void interpreter::semanticErr_BoundsOnlyForVariables(ASTNode* node)
{
	ASTbounds_optionNode* node1 = static_cast<ASTbounds_optionNode*>(node);

	int line, pos;
	node1->tInequalityOp.getLnAndPos(line, pos);

    errptr->setError(1006, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" The entity " + IS.ob_tok.getValue() + " cannot have a " + node1->sInequalityOp + " option");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Currently only "=" supported in equation
void interpreter::semanticErr_OnlyEqualityInEquation(ASTNode* node)
{
	ASTequationNode* node1 = static_cast<ASTequationNode*>(node);

	int line, pos;
	node1->tEquationTypeOp.getLnAndPos(line, pos);

    errptr->setError(1007, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Currently only equality equations are supported in the the equality section");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Only an ITER variable can be used for iteration
void interpreter::semanticErr_OnlyIterAllowedInFor(ASTNode* node)
{
	ASTfor_loopNode* node1 = static_cast<ASTfor_loopNode*>(node);

	int line, pos;
	node1->tName.getLnAndPos(line, pos);

    errptr->setError(1008, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Only an iteration variable may be used in a for loop");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Invalid for
void interpreter::semanticErr_EndlessForDetected(ASTNode* node)
{
	ASTfor_loopNode* node1 = static_cast<ASTfor_loopNode*>(node);

	int line, pos;
	node1->tName.getLnAndPos(line, pos);

    errptr->setError(1009, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Invalid for statement, results in infinite iterations");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Undeclared entity
void interpreter::semanticErr_UndeclaredEntity(token tok)
{
	int line, pos;
	tok.getLnAndPos(line, pos);

    errptr->setError(1009, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Entity " + tok.getValue() + " does not exist in the model");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Not a vector
void interpreter::semanticErr_NotAVector(token tok)
{
	int line, pos;
	tok.getLnAndPos(line, pos);

    errptr->setError(1009, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Entity " + tok.getValue() + " is not a vector");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dimension mismatch
void interpreter::semanticErr_DimensionMismatch(token tok)
{
	int line, pos;
	tok.getLnAndPos(line, pos);

    errptr->setError(1009, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Entity " + tok.getValue() + " has a dimension mismatch while referencing");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Index out of bounds
void interpreter::semanticErr_IndexOutOfBounds(token tok)
{
	int line, pos;
	tok.getLnAndPos(line, pos);

    errptr->setError(1009, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Entity " + tok.getValue() + " has a dimension mismatch while referencing");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Invalid Qualification
void interpreter::semanticErr_InvalidQualification(token tok)
{
	int line, pos;
	tok.getLnAndPos(line, pos);

    errptr->setError(1009, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Entity " + tok.getValue() + " cannot be qualified");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Uninitialized vector
void interpreter::semanticErr_UninitializedVector(token tok)
{
	int line, pos;
	tok.getLnAndPos(line, pos);

    errptr->setError(1009, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Entity " + tok.getValue() + " is not initialized");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Not a scalar
void interpreter::semanticErr_EntityIsNotAScalar(token tok)
{
	int line, pos;
	tok.getLnAndPos(line, pos);

    errptr->setError(1009, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Entity " + tok.getValue() + " is not a scalar, cannot be referenced as one");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// iter cannot be assigned
void interpreter::semanticErr_IterCannotBeAssigned(token tok)
{
	int line, pos;
	tok.getLnAndPos(line, pos);

    errptr->setError(1009, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Entity " + tok.getValue() + " is an iteration variable, cannot be directly assigned");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Only parameters can be set
void interpreter::semanticErr_OnlyParametersSet(token tok)
{
	int line, pos;
	tok.getLnAndPos(line, pos);

    errptr->setError(1009, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Entity " + tok.getValue() + " is not a parameter, only parameters can be set");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// No bounds allowed
void interpreter::semanticErr_NoBoundsAllowed(token tok)
{
	int line, pos;
	tok.getLnAndPos(line, pos);

    errptr->setError(1009, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Entity " + tok.getValue() + " cannot have bounds");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Only Variables can be fix
void interpreter::semanticErr_OnlyVariablesFix(token tok)
{
	int line, pos;
	tok.getLnAndPos(line, pos);

    errptr->setError(1009, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Entity " + tok.getValue() + " only Variables can be fixed");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Only Variables can be fix
void interpreter::semanticErr_OnlyVariablesGuess(token tok)
{
	int line, pos;
	tok.getLnAndPos(line, pos);

    errptr->setError(1009, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Entity " + tok.getValue() + " only Variables can be given guesses");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// No bounds can be set in guess
void interpreter::semanticErr_NoBoundsInGuess(token tok)
{
	int line, pos;
	tok.getLnAndPos(line, pos);

    errptr->setError(1009, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Entity " + tok.getValue() + " No bounds can be set in Guess section");
    errptr->addError();

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
	node3->tName.getLnAndPos(line, pos);

    errptr->setError(1010, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Expected a variable reference in a domain");
    errptr->addError();

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
	node3->tName.getLnAndPos(line, pos);

    errptr->setError(1011, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Missing domain information");
    errptr->addError();

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
	node3->tName.getLnAndPos(line, pos);

    errptr->setError(1012, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node3->sName + "is not a valid field variable");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Not a valid Field variable
void interpreter::semanticErr_NoArgsFunction(ASTNode* node)
{
	ASTfunctionCallNode* node1 = static_cast<ASTfunctionCallNode*>(node);

	int line, pos;
	node1->tFunctionName.getLnAndPos(line, pos);

    errptr->setError(1012, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node1->sFunctionName + " | has zero arguments");
    errptr->addError();

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
	node3->tName.getLnAndPos(line, pos);

    errptr->setError(1012, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node3->sName + " | Internal error generating FEM equations");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name not found
void interpreter::semanticErr_InvalidFunction(ASTNode* node)
{
	ASTfunctionCallNode* node1 = static_cast<ASTfunctionCallNode*>(node);

	int line, pos;
	node1->tFunctionName.getLnAndPos(line, pos);

    errptr->setError(1012, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node1->sFunctionName + " | Function not found");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Invalid basis
void interpreter::semanticErr_InvalidBasisType(ASTNode* node)
{
	ASTBasisTypeNode* node1 = static_cast<ASTBasisTypeNode*>(node);

	int line, pos;
	node1->tBasisType.getLnAndPos(line, pos);

    errptr->setError(1012, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node1->sBasisType + " | is in invalid basis type.");
    errptr->addErrorLine("Try linear, quadratic or cubic");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Segment ID redefinition
void interpreter::semanticErr_RedefinitionOfSegmentID(ASTNode* node)
{
	ASTSegmentIdTypeNode* node1 = static_cast<ASTSegmentIdTypeNode*>(node);

	int line, pos;
	node1->tID.getLnAndPos(line, pos);

    errptr->setError(1012, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node1->sID + " | is already defined for this segment");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Redefinition of number of elements <Add a support token)
void interpreter::semanticErr_RedefinitionNumberOfElements(ASTNode* node)
{
	ASTNumelNode* node1 = static_cast<ASTNumelNode*>(node);
	
	int line, pos;
	node1->t_Tag.getLnAndPos(line, pos);

    errptr->setError(1012, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node1->t_Tag.getValue() + " | this attribute is already defined");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Redefinition of Length of element <Add a support token>
void interpreter::semanticErr_RedefinitionLength(ASTNode* node)
{
	ASTLengthNode* node1 = static_cast<ASTLengthNode*>(node);

	int line, pos;
	node1->t_Tag.getLnAndPos(line, pos);

    errptr->setError(1012, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node1->t_Tag.getValue() + " | this attribute is already defined");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Redefinition of Length of element <Add a support token>
void interpreter::semanticErr_MissingSegmentAttributes(token tok)
{
	int line, pos;
	tok.getLnAndPos(line, pos);

    errptr->setError(1012, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(tok.getValue() + " | this object has some missing segment attributes.");
    errptr->addErrorLine("This could be \"id\", \"numel\" or \"length\"");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Redefinition of Length of element <Add a support token>
void interpreter::semanticErr_SegmentIDExistsAlready(ASTNode* node)
{
	ASTSegmentIdTypeNode* node1 = static_cast<ASTSegmentIdTypeNode*>(node);

	int line, pos;
	node1->tID.getLnAndPos(line, pos);

    errptr->setError(1012, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node1->sID + " | This segment does not have a unique ID, it already exists in the object");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PDE id redefinition
void interpreter::semanticErr_RedefinitionPDEid(ASTNode* node)
{
	ASTSegmentIdTypeNode* node1 = static_cast<ASTSegmentIdTypeNode*>(node);

	int line, pos;
	node1->tID.getLnAndPos(line, pos);

    errptr->setError(1012, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node1->sID + " | Redefinition of \"id\" attribute");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PDE id redefinition
void interpreter::semanticErr_RedefinitionDomains(ASTNode* node)
{
	ASTdomainOptionNode* node1 = static_cast<ASTdomainOptionNode*>(node);

	int line, pos;
	node1->tTag.getLnAndPos(line, pos);

    errptr->setError(1012, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node1->tTag.getValue() + " | Redefinition of attribute");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Domain does not exist
void interpreter::semanticErr_DomainDoesNotExist(ASTNode* node)
{
	ASTNamedReferenceNode* node1 = static_cast<ASTNamedReferenceNode*>(node);

	int line, pos;
	node1->tName.getLnAndPos(line, pos);

    errptr->setError(1012, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node1->sName + " | domain with this name has not been defined");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Duplicate domain 
void interpreter::semanticErr_RedefinitionDomainName(ASTNode* node)
{
	ASTNamedReferenceNode* node1 = static_cast<ASTNamedReferenceNode*>(node);

	int line, pos;
	node1->tName.getLnAndPos(line, pos);

    errptr->setError(1012, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node1->sName + " | domain with this name has already been applied");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Model entity with the same name already exists
void interpreter::semanticErr_UndefinedLine(ASTNode* node)
{
	ASTmodel_entity_declNode* node1 = static_cast<ASTmodel_entity_declNode*>(node);

	int line, pos;
	node1->tName.getLnAndPos(line, pos);

    errptr->setError(1006, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node1->sName + "| the line is undefined, Needs options to define it");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Missing reference to domain or Variable
void interpreter::semanticErr_MissingReferenceToDomOrVar(ASTNode* node)
{
	ASTNamedReferenceNode* node1 = static_cast<ASTNamedReferenceNode*>(node);

	int line, pos;
	node1->tName.getLnAndPos(line, pos);

    errptr->setError(1006, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node1->sName + "| the line is undefined, Needs options to define it");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// For an index in a domain, it can only be string
void interpreter::semanticErr_IndexInDomainOnlyString(ASTNode* node)
{
	ASTNamedReferenceNode* node1 = static_cast<ASTNamedReferenceNode*>(node);

	int line, pos;
	node1->tName.getLnAndPos(line, pos);

    errptr->setError(1006, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node1->sName + "| only string indices allowed for this type");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Invalid string index
void interpreter::semanticErr_InvalidStringIndex(ASTNode* node)
{
	ASTstringIndex* node1 = static_cast<ASTstringIndex*>(node);

	int line, pos;
	node1->tString.getLnAndPos(line, pos);

    errptr->setError(1006, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node1->sString + "| is an invalid index");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Direct reference to Variable without domain info
void interpreter::semanticErr_DirectVarWithoutDomain(ASTNode* node)
{
	ASTNamedReferenceNode* node1 = static_cast<ASTNamedReferenceNode*>(node);

	int line, pos;
	node1->tName.getLnAndPos(line, pos);

    errptr->setError(1006, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node1->sName + "| Cannot be directly referenced without a domain qualification");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Direct reference to Variable without domain info
void interpreter::semanticErr_OnlyGuessForFEMVars(ASTNode* node)
{
	ASTNamedReferenceNode* node1 = static_cast<ASTNamedReferenceNode*>(node);

	int line, pos;
	node1->tName.getLnAndPos(line, pos);

    errptr->setError(1006, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(node1->sName + "| only guesses can be applied to FEM type variables");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Direct reference to Variable without domain info
void interpreter::semanticErr_CalculatingBoundaryValues(token tok)
{
	int line, pos;
	tok.getLnAndPos(line, pos);

    errptr->setError(1009, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Entity " + tok.getValue() + " | invalid inputs causing error in calculating boundary values");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// option redefinition
void interpreter::semanticErr_optionRedefined(const std::string, modelEntity* me)
{
	int line, pos;
    me->getTok()->getLnAndPos(line, pos);

    errptr->setError(1009, "SEMANTIC ERROR", line, pos);
    errptr->addErrorLine(" Entity " + me->getTok()->getValue() + " | The option 'description' has been redefined for this option");
    errptr->addError();

	THROW_INTERPRETER_EX(443, "Interpreter Error")
}
