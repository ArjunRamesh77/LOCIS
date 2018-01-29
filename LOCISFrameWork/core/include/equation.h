#ifndef LOCIS_EQUATION_H
#define LOCIS_EQUATION_H

#include "interpreter.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structure to store an equation unit
struct EquationUnit
{
	std::string sEquation;
	std::string sEqOp;
	int iEqOp;

	EquationUnit()
	{
		sEquation.assign("");
		sEqOp.assign("<>");
		iEqOp = -1;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structure to store discontinuity states
struct discon
{
	//ifs
	std::vector<int> if_states;

	void clear()
	{
		if_states.clear();
	}

	void Reset()
	{
		for (auto it = if_states.begin(); it != if_states.end(); ++it)
		{
			*it = -1;
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Simulation Modes
#define EQ_STEADY_STATE 0
#define EQ_DYNAMIC 1

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Equation Interpreter
class equation : public NodeVisitor
{
	int iSimulationType;
	bool bInitialization;
	intState IS;
	std::vector<EquationUnit>* Equations;
	std::vector<EquationUnit>* InitEquations;
	interpreter *Ip;
	model* MasterModel;

	//Variable Occurence counter
	std::map<int, int> occurV, occurdV;
	int numAlgVars;
	int numDiffVars;
	
	//Initialization
	double t0_init_val;
	stateVars* initHelpers;

public:
	equation(interpreter* Ip_arg) 
	{
		iSimulationType = EQ_DYNAMIC;
		bInitialization = false;
		Equations = NULL;
		Ip = Ip_arg;
		MasterModel = Ip->getMasterModel();
		initHelpers = NULL;
		numAlgVars = 0;
		numDiffVars = 0;
	}

	~equation()
	{

	}

	// Similar to Interpreter
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

	// Helpers
	ASTNode* VISIT_NODE(ASTNode* node);
	void setEquationModeForInterpreter(bool state);
	void setEquations(std::vector<EquationUnit>* Equations, std::vector<EquationUnit>* InitEquations);
	int getEquationsRecursive(model* mod);
	bool evaluate(ASTQualifiedNamedReferenceNode* qRefName);
	bool evaluateFEMoneDObject(FEMOneDLineObject* FEMobj, ASTQualifiedNamedReferenceNode* qRefName, std::vector<ASTNode*>::const_iterator it);
	std::string getSubstitutedEntityNameScalar(modelEntity* me);
	std::string getSubstitutedEntityNameVector(modelEntity* me, int &index);
	stateVars* MapToStateVector();
	bool MapFromStateVector(stateVars* pV);
	bool getNewEquationsFromState(stateVars* Pv = NULL);
	void getOccuringVariablesNumbers(int &Alg, int &Diff);
	void setSimulationType(int type);
	void setInitializationMode(bool val, stateVars* initHelpers);
	void setT0Value(double val);
	bool isInitialzation();

	//FEMoneD (Atharv to implement)
	bool getFEMoneDEquations(ASTNode* optimizedTree, ASTNode* leftBoundary, ASTNode* rightBoundary, int leftBoundaryType, int rightBoundaryType, int domainLeft, int domainRight, double startLength, double endLength, int nElements, int basisType);
};

#endif // !LOCIS_EQUATION_H



