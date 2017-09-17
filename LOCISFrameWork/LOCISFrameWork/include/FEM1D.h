#pragma once

//#include "AST.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "interpreter.h"
#include "equation.h"
#include "FEM1DgenericElement.h"
#include"FEM1Dbasis.h"
#include "FEM1Dintegrate.h"

#define DD 1
#define ND 2
#define DN 3
#define NN 4

#define INTEGRATION_TRAPEZOID 5
#define INTEGRATION_SIMPSON13 6
#define INTEGRATION_SIMPSON38 7
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct problemData
{
	std::string weakForm;
	double* boundaryvals;
	int* boundaryTypes;
};

// User just needs to know following two structs elementUserData and FEMUserData
struct elementUserData // vector of this will be passed by the user for customization
{
	double len;
	int basisFunctionType;
	int integrationRule;
	int elemType;
};

struct solveOptions
{
	std::vector<elementUserData> allElementData;
	int numElements;
};

struct FEMUserData // User will pass me this struct
{
	// for populating problem data
	std::string weakForm;
	double* boundaryvals;
	int boundaryTypes;

	// For populating solve options
	std::vector<elementUserData> allElementData;
	int numElements;
	int startNode;
	int totalNumNodes;
};

struct globalState
{
	int elemNo, eqNo, integNodeNo;// , elemNodeNo;
	elementInfo element_info;

};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FEM : public NodeVisitor
{
	std::vector<EquationUnit>* equationVecPtr;

	int numEquations;

	int boundaryTypes;

	problemData problem_data;

	solveOptions solve_options;

	std::vector<genericElement1D*> line;

	std::vector<std::string> allEquations;

	ASTNode* tree;
	equation* equationPtr;
	ASTNode *leftBoundary, *rightBoundary;

	globalState global_state;

	int startNode;
	int totalNumNodes;

	interpreter* interpreterPtr;

	int boundaryLoc; // flag for boundary expression processing
	//////////////////////////////////////////////

	std::vector<std::string> forIntegration;


public:
	DECL_DISPATCH(ASTfunctionCallNode)
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

	//FEMoneD
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

	//Helper functions
	void SetExpressionTree(); // 0 for none, 1 for left and 2 for right

	void setData(FEMUserData *userData, equation* equationptr, ASTNode* tree, ASTNode* leftBoundary, 
		ASTNode* rightBoundary, std::vector<EquationUnit>*, interpreter*);

	void FEM::preProcess();

	void FEM::build();

	void FEM::solve();

};