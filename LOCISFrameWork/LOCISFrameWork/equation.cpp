#include "equation.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implements the equation Server ( Its a special version of the interpreter)
// Everything will be processed from the point of view of the Equation generation

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Processes the equation section
ASTNode* equation::dispatch(ASTmodel_sectionNode* node)
{
	// Only Equation Sections will be processed here
	if (node->sName == "EQUATION" || node->sName == "INIT")
	{
		if (node->sName == "EQUATION")
		{
			IS.section_type = SCOPE_EQUATION;
		}
		else
		{
			IS.section_type = SCOPE_INIT;
		}

		VISIT_ALL_NODES(node, astvnSectionStatements)
	}
	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Processes each individual equation
ASTNode* equation::dispatch(ASTequationNode* node)
{
	// Generates equations based on state
	ASTNode* LHS = NULL;
	ASTNode* RHS = NULL;

	LHS = VISIT_NODE(node->astnLHS);
	RHS = VISIT_NODE(node->astnRHS);

	assert(LHS);
	assert(RHS);

	// FEM equations are not generated here
	if (!IS.bInFEMmode)
	{
		EquationUnit EqU;
		if (IS.section_type == SCOPE_EQUATION)
		{
			EqU.sEquation.assign(LHS->equation_data + "-" + RHS->equation_data);
			EqU.sEqOp = node->sEquationTypeOp;
			EqU.iEqOp = node->iEquationTypeOp;
			Equations->push_back(EqU);
		}

		if (IS.section_type == SCOPE_INIT)
		{
			EqU.sEquation.assign(LHS->equation_data + "-" + RHS->equation_data);
			EqU.sEqOp = node->sEquationTypeOp;
			EqU.iEqOp = node->iEquationTypeOp;
			InitEquations->push_back(EqU);
		}
	}

	return node;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Expression Evaluation
ASTNode* equation::dispatch(ASTMathBinaryOpNode* node)
{
	ASTNode* left_eval = NULL;
	ASTNode* right_eval = NULL;

	//Get left and right eval
	left_eval = VISIT_NODE(node->astnLeft);
	right_eval = VISIT_NODE(node->astnRight);

	assert(left_eval);
	assert(right_eval);

	switch (node->iBinaryOp)
	{
	case PLUS:
		node->equation_data = "(" + left_eval->equation_data + "+" + right_eval->equation_data + ")";
		break;

	case MINUS:
		node->equation_data = "(" + left_eval->equation_data + "-" + right_eval->equation_data + ")";
		break;

	case MULT:
		node->equation_data = "(" + left_eval->equation_data + "*" + right_eval->equation_data + ")";
		break;

	case DIV:
		node->equation_data = "(" + left_eval->equation_data + "/" + right_eval->equation_data + ")";
		break;

	case RAISE:
		node->equation_data = "(" + left_eval->equation_data + "**" + right_eval->equation_data + ")";
	}

	return node;
}

ASTNode* equation::dispatch(ASTMathUnaryNode* node)
{
	ASTNode* right_eval = NULL;

	right_eval = VISIT_NODE(node->astnRight);
	assert(right_eval);

	switch (node->iUnaryOp)
	{
	case PLUS:
		node->equation_data = "( +" + right_eval->equation_data + ")";
		break;

	case MINUS:
		node->equation_data = "( -" + right_eval->equation_data + ")";
		break;
	}

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Simply return a number node
ASTNode* equation::dispatch(ASTNumberNode* node)
{
	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Just Evaluate and return the node
ASTNode* equation::dispatch(ASTQualifiedNamedReferenceNode* node)
{	
	if (Ip->evaluate(node)) // Use interpreter's version of evaluate
		return node;

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Expression Evaluation(logical)
ASTNode* equation::dispatch(ASTLogicalBinaryOpNode* node)
{
	return node;
}

ASTNode* equation::dispatch(ASTLogicalUnaryNode* node)
{
	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// if statement
ASTNode* equation::dispatch(ASTif_statmentNode* node)
{
	//// Cannot have an if in a for loop
	//if (IS.bInFor)
	//{
	//	//SEMANTIC ERROR:: Cannot have an if insde a for loop
	//	return NULL;
	//}

	ASTNode* cond = NULL;
	// Similar trick
	Ip->IS.scp = IS.scp;
	cond = node->astnCondition->visit(Ip);
	assert(cond);

	// Simulate actual if statement
	if (cond->lvalue)
	{
		VISIT_ALL_NODES(node, astvnifs)
	}
	else if (node->bHasElse)
	{
		VISIT_ALL_NODES(node, astvnelses)
	}
	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// performs the for loop( Exactly the same as Interpreter)
ASTNode* equation::dispatch(ASTfor_loopNode* node)
{
	ModelEntity* iter = NULL;
	iter = SEARCH_IN_MODEL(NULL, node->sName);

	if (iter)
	{
		//iter has to be an iter type variable
		if (iter->Type != ITER)
		{
			Ip->semanticErr_OnlyIterAllowedInFor(node);
			return NULL;
		}

		ASTNode* start = NULL;
		ASTNode* till = NULL;
		ASTNode* incr = NULL;
		int incr_val = 1;
		int start_val = 1;
		int till_val = 1;

		start = VISIT_NODE(node->astnStartExpr);
		till = VISIT_NODE(node->astnTillExpr);
		assert(start);
		assert(till);

		if (node->astnIncrExpr)
		{
			incr = VISIT_NODE(node->astnIncrExpr);
			assert(incr);
		}

		// See if increment present
		start_val = (int)start->value;
		iter->SValue = start_val;
		till_val = (int)till->value;
		if (incr)
		{
			incr_val = (int)incr->value;
			if (incr_val > 0.0)
			{
				if (start_val <= till_val)
				{
					IS.bInFor = true;
					for (int i = start_val; i <= till_val; i = i + incr_val, iter->SValue = iter->SValue + incr_val)
					{
						VISIT_ALL_NODES(node, astvnfors)
					}
					IS.bInFor = false;
				}
				else
				{
					Ip->semanticErr_EndlessForDetected(node);
					return NULL;
				}
			}
			else
			{
				if (start_val >= till_val)
				{
					IS.bInFor = true;
					for (int i = start_val; i >= till_val; i = i - incr_val, iter->SValue = iter->SValue - incr_val)
					{
						VISIT_ALL_NODES(node, astvnfors)
					}
					IS.bInFor = false;
				}
				else
				{
					Ip->semanticErr_EndlessForDetected(node);
					return NULL;
				}
			}
		}
		else
		{
			//For no increment case
			if (start_val <= till_val)
			{
				IS.bInFor = true;
				for (int i = start_val; i <= till_val; ++i, ++iter->SValue)
				{
					VISIT_ALL_NODES(node, astvnfors)
				}
				IS.bInFor = false;
			}
			else
			{
				IS.bInFor = true;
				for (int i = start_val; i >= till_val; --i, --iter->SValue)
				{
					VISIT_ALL_NODES(node, astvnfors)
				}
				IS.bInFor = false;
			}
		}
	}
	else
	{
		Ip->semanticErr_UndeclaredEntity(node->tName);
		return NULL;
	}

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Runs the PDE equation node
ASTNode* equation::dispatch(ASTFEMoneDEquation* node)
{
	//Evaluate the domain
	if (!Ip->evaluate(static_cast<ASTQualifiedNamedReferenceNode*>(node->astnDomain))) //Use interpreter's version of evaluate
		return NULL;

	// Get domain left and right
	int nElements = -1;
	int domainleftIndex = -1;
	int domainRightIndex = -1;
	int basisType = -1;
	double ddomainLeft = 0.0;
	double ddomainRight = 0.0;

	ModelEntity* dom = NULL;
	if (node->astnDomain->me)
	{
		if (node->astnDomain->me->isFEMOneDVariable)
		{
			if (node->astnDomain->me->currentDomain)
			{
				dom = node->astnDomain->me->currentDomain;
			}
			else
			{
				Ip->semanticErr_NoDomainAssociatedWithVar(node->astnDomain);
			}
		}
		else
		{
			Ip->semanticErr_NotAFieldVariable(node->astnDomain);
		}
	}
	else
		Ip->semanticErr_ExpectedAVariableRefInDomain(node->astnDomain);

	// Get left and right ends of the domain
	FEMOneDModelDomainData* dom1 = static_cast<FEMOneDModelDomainData*>(dom);
	if (!dom)
	{
		return NULL;
	}

	// Domain Data
	domainleftIndex = dom1->domainSize["left"];
	domainRightIndex = dom1->domainSize["right"];
	ddomainLeft = dom1->startLength;
	ddomainRight = dom1->endLength;
	nElements = dom1->iNumel;
	basisType = dom1->iBasis;

	// Evaluate to get optimized tree
	IS.bInFEMmode = true;
	node->astnEqn->visit(this);
	node->astnBoundaryLeft->visit(this);
	node->astnBoundaryRight->visit(this);
	IS.bInFEMmode = false;

	// Get FEM oneD equations
	if (!getFEMoneDEquations(node->astnEqn, node->astnBoundaryLeft, node->astnBoundaryRight, node->iLeftBoundaryType, node->iRightBoundaryType, 
		domainleftIndex, domainRightIndex, ddomainLeft, ddomainRight, nElements, basisType))
	{
		Ip->semanticErr_FEMEquationGenFail(node);
	}

	return node;
}

// Unused Functions
UNUSED_AST_NODE_FUNCTION(equation, ASTmodel_collectionNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTmodelNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTmodel_entity_decl_groupNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTmodel_entity_declNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTmodel_entities_collectionNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTgeneric_tokenNode)
UNUSED_AST_NODE_FUNCTION(equation, ASToptions_groupNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTunit_optionNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTdesc_optionNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTtype_optionNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTbounds_optionNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTdefualtNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTNamedReferenceNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTArrayIndicesNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTarray_indexNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTstatement_blockNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTassignmentNode)

UNUSED_AST_NODE_FUNCTION(equation, ASTBasisTypeNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTSegmentAttributesNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTPdevarAttributesNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTSegmentIdTypeNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTNumelNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTLengthNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTdomainOptionNode)
UNUSED_AST_NODE_FUNCTION(equation, ASTstringIndex)
UNUSED_AST_NODE_FUNCTION(equation, ASTDerivativeSingleVar)
UNUSED_AST_NODE_FUNCTION(equation, ASTBoundaryConditions)
UNUSED_AST_NODE_FUNCTION(equation, ASTSimulationArgs)