#ifndef LOCIS_INTERPRETER_H
#define LOCIS_INTERPRETER_H

#include "AST.h"
#include "ASTFEMoneD.h"
#include "Symbol.h"
#include "SymbolFEMoneD.h"
#include <iostream>
#undef NDEBUG
#include <assert.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Interpreter helpers
//#define VISIT_NODE(node) [this, node] { if(*node) (*node)->visit(this); else return NULL; }
							

#define VISIT_ALL_NODES(node, name) for(std::vector<ASTNode*>::const_iterator it = node->name.begin(); it != node->name.end(); ++it)	\
								    { VISIT_NODE(*it); }

#define VISIT(name) node->name->visit(this);

#define LOOP_OVER_NODES(node, name) for(std::vector<ASTNode*>::const_iterator it_##name = node->name.begin(); it_##name != node->name.end(); ++it_##name) 

#define SET_MODEL_SCOPE(name) IS.scp = name;

#define INSERT_NEW_ENTITY_IN_MODEL(name, ptr) IS.scp->insertModelEntity(name, ptr)

#define SEARCH_IN_MODEL(mod, name) IS.scp->getModelEntity(mod, name)

#define DECL_INTERSTATE bool interstate = true;

#define AND_GOOD_STATE && interstate

#define IF_GOOD_INTERSTATE(exp) if((exp) && interstate)

#define SET_BAD_INTERSTATE interstate = false;

//#define SAME_TYPE(NAME1, NAME2) typeid(NAME1) == typeid(NAME2) 
#define SAME_TYPE(NAME1, NAME2) dynamic_cast<NAME2>(NAME1) 

#define SAVE_INTERPRETER_STATE intState IS_copy = IS;

#define RESET_INTERPRETER_STATE IS = IS_copy;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Interpreter dependents
#define SCOPE_MAIN 0
#define SCOPE_SET 1
#define SCOPE_FIX 2
#define SCOPE_GUESS 3
#define SCOPE_EQUATION 4
#define SCOPE_INIT 5

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Interpreter exception 
struct interpreterEx
{
	int ExCode;
	std::string ExHeader;
	std::vector<std::string> Exlines;
};

#define THROW_INTERPRETER_EX(code, header) iEx.ExCode = code; iEx.ExHeader = header; throw iEx;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Data structure for Parameter data
struct ParameterData
{
	int index;
	std::string fullname;
	double* val;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Data structure for variable data
struct VariableData : public ParameterData
{
	char UpType, LoType;
	double Up, Lo;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InterpreterState
struct intState
{
	// Markers
	bool bInFor;
	bool bInFEMmode;
	bool bFromEquation;

	// Interpreter passes
	int pass;
	bool pass_err;

	// Representative model scope
	Model* scp;

	//for declaration only
	std::string modelName;
	int dectype;
	int numtype;
	ASTNode* node;

	//for setting options
	ModelEntity* ob;

	//Section signaling
	int section_type;

	intState()
	{
		bInFEMmode = false;
		bInFor = false;
		bFromEquation = false;
		pass = 1;
		pass_err = false;
		scp = NULL;
		modelName.assign("");
		dectype = -1;
		numtype = -1;
		ob = NULL;
		section_type = SCOPE_MAIN;
	}
	token ob_tok;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Portable structure setting state variables( used to pass to the python Equation server)
struct stateVars
{
	// State vars
	int NumVars;
	double* Vars;
	double* dVars;

	// Initialization helpers
	int initIncr;
	int *initVindex;
	int *initdVindex;

	// Regular
	stateVars()
	{
		NumVars = 0;
		Vars = NULL;
		dVars = NULL;

		initIncr = -1;
		initVindex = NULL;
		initdVindex = NULL;
	}

	// Specified
	stateVars(int NumVars_arg)
	{
		NumVars = NumVars_arg;
		Vars = new double[NumVars_arg];
		dVars = new double[NumVars_arg];

		initIncr = -1;
		initVindex = NULL;
		initdVindex = NULL;
	}

	// Function to aid in initialization for DAE
	void createInitializationData()
	{
		initIncr = -1;
		if (initVindex)
		{
			delete initVindex;
		}
		initVindex = new int[NumVars]; //To store absolute index of V

		if (initdVindex)
		{
			delete initdVindex;
		}
		initdVindex = new int[NumVars]; //To store absolute index of dV 

		// Set to -1 to signal not used
		for (int i = 0; i < NumVars; i++)
		{
			initVindex[i] = -1;
			initdVindex[i] = -1;
		}
	}

	// Maps the guess from main -> initguess
	double* getInitializationGuess()
	{
		if (initIncr > 0 && initVindex && initdVindex)
		{
			double* guess = new double[initIncr + 1];
			for (int i = 0; i < NumVars; i++)
			{
				if (initVindex[i] != -1)
				{
					guess[initVindex[i]] = Vars[i];
				}
				if (initdVindex[i] != -1)
				{
					guess[initdVindex[i]] = dVars[i];
				}
			}
			return guess;
		}
		return NULL;
	}

	// Maps the guess from initguess -> main
	bool mapFromInitializationGuess(double *guess)
	{
		if (guess && initVindex && initdVindex)
		{
			for (int i = 0; i < NumVars; i++)
			{
				if (initVindex[i] != -1)
				{
					 Vars[i] = guess[initVindex[i]];
				}
				if (initdVindex[i] != -1)
				{
					dVars[i] = guess[initdVindex[i]];
				}
			}
			return true;
		}
		return false;
	}

	~stateVars()
	{
		if (Vars)
		{
			delete Vars;
		}

		if (dVars)
		{
			delete dVars;
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Interpreter
class interpreter : public NodeVisitor
{
public:

	// Exception
	interpreterEx iEx;

	//simulation arguments
	int simuArg_iType;
	std::string simuArg_type;
	double simuArg_startt;
	double simuArg_endt;
	int simuArg_numStepst;
	double simuArg_asbtol;
	double simuArg_reltol;

	//Section information
	bool bSetSection;
	bool bFixSection;
	bool bGuessSection;
	bool bInitSection;
	bool bEquationSection;

	cerrors* errptr;
	int numVars, numdVars, numPars, numfVars, numfdVars;
	std::vector<VariableData> AllVars, AlldVars, AllfVars, AllfdVars;
	std::vector<ParameterData> AllPars;
	modelCollection* SymTab;
	ASTNode* AST;
	Object* MasterModel;
	intState IS;
	bool bInEquation;
	NodeVisitor* Eq;

	//Global Data
public:
	interpreter(ASTNode* AST_arg, cerrors* err)
	{
		simuArg_iType = -1;
		simuArg_type  = "";
		simuArg_startt = 0.0;
		simuArg_endt = 0.0;
		simuArg_numStepst = 1;
		simuArg_asbtol = 1.0e-8;
		simuArg_reltol = 1.0e-8;

		bSetSection = false;
		bFixSection = false;
		bGuessSection = false;
		bInitSection = false;
		bEquationSection = false;
		bInEquation = false;

		errptr = err;
		numVars = -1;
		numdVars = -1;
		numPars = -1;
		numfVars = -1;
		AST = AST_arg;
		SymTab = new modelCollection;
		MasterModel = NULL;
		Eq = NULL;
	}
	DECL_DISPATCH(ASTmodel_collectionNode)
	DECL_DISPATCH(ASTmodelNode)
	DECL_DISPATCH(ASTmodel_entity_decl_groupNode)
	DECL_DISPATCH(ASTmodel_entity_declNode)
	DECL_DISPATCH(ASTmodel_entities_collectionNode)
	DECL_DISPATCH(ASTmodel_sectionNode)
	DECL_DISPATCH(ASTgeneric_tokenNode)
	DECL_DISPATCH(ASToptions_groupNode)
	DECL_DISPATCH(ASTunit_optionNode)
	DECL_DISPATCH(ASTdesc_optionNode)
	DECL_DISPATCH(ASTtype_optionNode)
	DECL_DISPATCH(ASTbounds_optionNode)
	DECL_DISPATCH(ASTdefualtNode)
	DECL_DISPATCH(ASTequationNode)
	DECL_DISPATCH(ASTMathBinaryOpNode)
	DECL_DISPATCH(ASTMathUnaryNode)
	DECL_DISPATCH(ASTNumberNode)
	DECL_DISPATCH(ASTNamedReferenceNode)
	DECL_DISPATCH(ASTfunctionCallNode)
	DECL_DISPATCH(ASTArrayIndicesNode)
	DECL_DISPATCH(ASTarray_indexNode)
	DECL_DISPATCH(ASTQualifiedNamedReferenceNode)
	DECL_DISPATCH(ASTLogicalBinaryOpNode)
	DECL_DISPATCH(ASTLogicalUnaryNode)
	DECL_DISPATCH(ASTfor_loopNode)
	DECL_DISPATCH(ASTif_statmentNode)
	DECL_DISPATCH(ASTstatement_blockNode)
	DECL_DISPATCH(ASTassignmentNode)
	DECL_DISPATCH(ASTSimulationArgs)

	// FEMoneD 
	DECL_DISPATCH(ASTBasisTypeNode)
	DECL_DISPATCH(ASTSegmentAttributesNode)
	DECL_DISPATCH(ASTPdevarAttributesNode)
	DECL_DISPATCH(ASTSegmentIdTypeNode)
	DECL_DISPATCH(ASTNumelNode)
	DECL_DISPATCH(ASTLengthNode)
	DECL_DISPATCH(ASTdomainOptionNode)
	DECL_DISPATCH(ASTFEMoneDEquation)
	DECL_DISPATCH(ASTstringIndex)
	DECL_DISPATCH(ASTDerivativeSingleVar)
	DECL_DISPATCH(ASTBoundaryConditions)

	void Start();

	//Helper functions
	bool evaluate(ASTQualifiedNamedReferenceNode* qRefName);
	bool assign(ASTQualifiedNamedReferenceNode* qRefName, double &val, int &op);
	bool initVector(ModelEntity* en);
	bool initAllVector(ModelEntity* en);
	char optochar(int & op);
	void getAllVars(Model* MainObj, std::string Name_arg);
	bool InitAll(Model* MainObj);
	Model* getMasterModel();
	std::vector<VariableData>* getAllVarNames();
	bool addExtraSymbols(Model* md);
	bool setSimulationArgs(ASTNode* node, std::string type, double tStart, double tEnd, int numSteps, double absTol, double relTol);
	ASTNode* VISIT_NODE(ASTNode* node);
	void checkSectionConsistency(ASTNode* node);

	//FEMoneD functions
	bool initFEMLine(ASTNode* buildNode);
	bool evaluateFEMoneDObject(FEMOneDLineObject* FEMobj, ASTQualifiedNamedReferenceNode* qRefName, std::vector<ASTNode*>::const_iterator it);
	bool assignFEMoneDObject(FEMOneDLineObject* FEMobj, ASTQualifiedNamedReferenceNode* qRefName, std::vector<ASTNode*>::const_iterator it_in,
		double &val, int &op);
	bool createFEMoneDline(Object* qRefName);
	ASTNode* generateOptimizedFEMoneDTree();

	// Semantic Errors
	void semanticErr_ModelNoExist(ASTNode*);
	void semanticErr_MultipleModelDefinitions(ASTNode*);
	void semanticErr_FailedToInitializeAll();
	void semanticErr_FirstPassConsistencyCheck();
	void semanticErr_EntityWrongPositionDecl(ASTNode* node);
	void semanticErr_ModelUndefined(ASTNode* node);
	void semanticErr_dtVariableDefined(ASTNode* node);
	void semanticErr_EntityTypeNoVector(ASTNode* node);
	void semanticErr_EntityTypeNoDefault(const std::string EntityBase, ASTNode* node);
	void semanticErr_EntityTypeNoOptions(const std::string EntityBaseType, ASTNode* node);
	void semanticErr_NULLevaluation(ASTNode* node);
	void semanticErr_EntityRedeclaration(ASTNode* node);
	void semanticErr_SectionRedeclaration(ASTNode* node);
	void semanticErr_SectionBehindGuess(ASTNode* node);
	void semanticErr_SectionBehindEquationInit(ASTNode* node);
	void semanticErr_BoundsOnlyForVariables(ASTNode* node);
	void semanticErr_OnlyEqualityInEquation(ASTNode* node);
	void semanticErr_OnlyIterAllowedInFor(ASTNode* node);
	void semanticErr_EndlessForDetected(ASTNode* node);
	void semanticErr_UndeclaredEntity(token tok);
	void semanticErr_NotAVector(token tok);
	void semanticErr_DimensionMismatch(token tok);
	void semanticErr_IndexOutOfBounds(token tok);
	void semanticErr_InvalidQualification(token tok);
	void semanticErr_UninitializedVector(token tok);
	void semanticErr_EntityIsNotAScalar(token tok);
	void semanticErr_IterCannotBeAssigned(token tok);
	void semanticErr_OnlyParametersSet(token tok);
	void semanticErr_NoBoundsAllowed(token tok);
	void semanticErr_OnlyVariablesFix(token tok);
	void semanticErr_OnlyVariablesGuess(token tok);
	void semanticErr_NoBoundsInGuess(token tok);
	void semanticErr_ExpectedAVariableRefInDomain(ASTNode* node);
	void semanticErr_NoDomainAssociatedWithVar(ASTNode* node);
	void semanticErr_NotAFieldVariable(ASTNode* node);
	void semanticErr_NoArgsFunction(ASTNode* node);
	void semanticErr_FEMEquationGenFail(ASTNode* node);
	void semanticErr_InvalidFunction(ASTNode* node);
	void semanticErr_InvalidBasisType(ASTNode* node);
	void semanticErr_RedefinitionOfSegmentID(ASTNode* node);
	void semanticErr_RedefinitionNumberOfElements(ASTNode* node);
	void semanticErr_RedefinitionLength(ASTNode* node);
	void semanticErr_MissingSegmentAttributes(token tok);
	void semanticErr_SegmentIDExistsAlready(ASTNode* node);
	void semanticErr_RedefinitionPDEid(ASTNode* node);
	void semanticErr_RedefinitionDomains(ASTNode* node);
	void semanticErr_DomainDoesNotExist(ASTNode* node);
	void semanticErr_RedefinitionDomainName(ASTNode* node);
	void semanticErr_UndefinedLine(ASTNode* node);
	void semanticErr_MissingReferenceToDomOrVar(ASTNode* node);
	void semanticErr_IndexInDomainOnlyString(ASTNode* node);
	void semanticErr_InvalidStringIndex(ASTNode* node);
	void semanticErr_DirectVarWithoutDomain(ASTNode* node);
	void semanticErr_OnlyGuessForFEMVars(ASTNode* node);
	void semanticErr_CalculatingBoundaryValues(token tok);
	void semanticErr_optionRedefined(const std::string, ModelEntity* me);
};

#endif // LOCIS_INTERPRETER_H
