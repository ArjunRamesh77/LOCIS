#include "interpreter.h"
#include <cmath>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Process Node that holds all the models
ASTNode* interpreter::dispatch(ASTmodel_collectionNode* node)
{
	// First Pass (model collection)
	VISIT_ALL_NODES(node, astvnModelCollection)

	//Check if pass successful (abort if not)
	if (IS.pass_err)
	{
		semanticErr_FirstPassConsistencyCheck();
	}

	// Second Pass (create full OOP structure)
	IS.pass = 2;
	MasterModel = new Object;
	MasterModel->SModelObject = new Model;
	MasterModel->setType(MODEL);
	MasterModel->setSType("MODEL");
	MasterModel->setName("Master");
	MasterModel->setDimType(SY_SCALAR);
	IS.scp = MasterModel->SModelObject;

	// Pass master to the last model in list(only way to currently identify master model)
	(*(node->astvnModelCollection.end() - 1))->visit(this);

	// Initialized any initialized Vectors
	if (InitAll(MasterModel->SModelObject))
	{
		// Get all the Variables (should always work)
		getAllVars(MasterModel->SModelObject, MasterModel->getName());
	}
	else
	{
		//SEMANTIC ERROR:: 
		semanticErr_FailedToInitializeAll();
	}

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Process a model
ASTNode* interpreter::dispatch(ASTmodelNode* node)
{
	// In the first pass just collect all the models and prevent circular referencing
	if (IS.pass == 1)
	{
		// Run through only object statements
		ASTmodel_entity_decl_groupNode* temp = NULL;
		LOOP_OVER_NODES(node, astvnModelStatements)
		{
			// Catch only declaration statements
			temp = dynamic_cast<ASTmodel_entity_decl_groupNode*>(*it_astvnModelStatements);
			if (temp)
			{
				// Check if its an object declaration
				if (temp->iDeclType == IDENT)
				{
					if (!(SymTab->getModel(temp->sModelBaseName)))
					{
						//SEMANTIC ERROR:: 
						semanticErr_ModelNoExist(temp); 
						IS.pass_err = true;
					}
				}
			}
		}

		// If no error insert the model (Prevents model being inserted if erroneous)
		if (!SymTab->insertModel(node->sName, node))
		{
			//SEMANTIC ERROR:: 
			semanticErr_MultipleModelDefinitions(node);
			IS.pass_err = true;
		}
	}

	// In the second pass build the damn thing!!
	if (IS.pass == 2)
	{
		// Get Simulation Parameters
		if (node->astnSimulationArgs)
		{
			ASTSimulationArgs* simArgs = static_cast<ASTSimulationArgs*>(node->astnSimulationArgs);
			if (!setSimulationArgs(node->astnSimulationArgs,
				simArgs->simType.getValue(),
				simArgs->dsimStartt,
				simArgs->dsimEndt,
				std::stoi(simArgs->simNumSteps.getValue()),
				simArgs->dsimAbsTol,
				simArgs->dsimRelTol))
			{
				return NULL;
			}
		}

		// Reset sections
		bSetSection = false;
		bFixSection = false;
		bGuessSection = false;
		bInitSection = false;
		bEquationSection = false;

		// Add any extra symbols to the model(should always work)
		assert(IS.scp);
		addExtraSymbols(IS.scp);

		// Run through all model statements
		VISIT_ALL_NODES(node, astvnModelStatements)
	}

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Process a group of model entities
ASTNode* interpreter::dispatch(ASTmodel_entity_decl_groupNode* node)
{
	// Model entities can be only be declared at the top of the model
	if (bSetSection || bFixSection || bEquationSection || bInitSection || bGuessSection)
	{
		semanticErr_EntityWrongPositionDecl(node);
	}
	
	//What type of entity
	IS.node = node;
	IS.dectype = node->iDeclType;
	if (node->iDeclType == IDENT)
	{
		IS.modelName = node->sModelBaseName;
	}
	else
	{
		IS.numtype = node->iNumType;
	}

	// Declare all
	VISIT_ALL_NODES(node, astvnModelEntities)

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Process each entity declared
ASTNode* interpreter::dispatch(ASTmodel_entity_declNode* node)
{
	ModelEntity* ob = NULL;
	ModelEntity* ob2 = NULL;
	ASTNode* mod = NULL;
	Model* save = NULL;

	// Find type of entity
	switch (IS.dectype)
	{
		// Regular LOCIS objects
		case IDENT:
			ob = new Object;
			mod = SymTab->getModel(IS.modelName);
			if (!mod)
			{
				semanticErr_ModelUndefined(node);
			}
			static_cast<Object*>(ob)->setModelName(IS.modelName);
			ob->setName(node->sName);
			ob->setType(MODEL);
			ob->setSType("MODEL");
			break;

		// FEMoneD line objects (built like variables/parameters)
		case LINE:
			initFEMLine(node);
			return node;

		case PARAMETER:
			ob = new Parameter;
			ob->setName(node->sName);
			ob->setNType(IS.numtype);
			ob->setType(PARAMETER);
			ob->setSType("PARAMETER");
			break;

		case VARIABLE:

			// Regular
			ob = new Variable;
			ob->setNType(IS.numtype);
			ob->setName(node->sName);
			ob->setType(VARIABLE);
			ob->setSType("VARIABLE");

			// Time dependent
			ob2 = new Variable;
			ob2->setNType(IS.numtype);
			ob2->setName("$" + node->sName);
			ob2->setType(VARIABLE);
			ob2->setSType("VARIABLE");
			ob2->setIsdt();
			
			// Give each others reference to each other
			ob2->other = ob; 
			ob->other = ob2; 
			break;

		case ITER:
			ob = new Parameter;
			ob->setName(node->sName);
			ob->setType(ITER);
			ob->setSType("ITER");
			break;
	}

	// Save the token for the entity
	ob->tok = &node->tName;
	IS.ob_tok = node->tName;

	// Cannot declare dt variables
	if (node->bIsdt)
	{
		semanticErr_dtVariableDefined(node);
	}

	//Check if iteration variable
	if (IS.dectype == ITER)
	{
		ob->setDimType(SY_SCALAR); //Force to a scalar
		if(node->bIsArray)
		{
			semanticErr_EntityTypeNoVector(node);
		}

		if (node->bHasDefault)
		{
			semanticErr_EntityTypeNoDefault("iteration", node);
		}

		if (node->bHasOptions)
		{
			semanticErr_EntityTypeNoOptions("iteration", node);
		}
		goto insert;
	}

	// Object declaration errors
	if (ob->geType() == MODEL)
	{
		if (node->bHasDefault)
		{
			semanticErr_EntityTypeNoDefault("object", node);
		}

		if (node->bHasOptions)
		{
			semanticErr_EntityTypeNoOptions("object", node);
		}
	}
	
	ASTNode* eval;
	int ret = 0;
	if(node->bIsArray)
	{	
		// Vectors of normal entities are built only at first assignment/evaluation
		ob->setDimType(SY_VECTOR);
		ob->vset = false;
		ob->build_node = node;

		// Has to set dummy values for the array indices for validation during assignment
		LOOP_OVER_NODES(node, astvnArrayIndices)
		{
			ob->Dims.push_back(0);
		}

		// Create dt variable the same way as variable
		if (ob->geType() == VARIABLE)
		{
			ob2->setDimType(SY_VECTOR);
			ob2->vset = false;
			ob2->build_node = node;

			LOOP_OVER_NODES(node, astvnArrayIndices)
			{
				ob2->Dims.push_back(0);
			}
		}

		// Default values are assigned on first occurrence of vectors
	}
	else
	{
		ob->setDimType(SY_SCALAR);
		if (ob->geType() == VARIABLE)
		{
			ob2->setDimType(SY_SCALAR);
		}

		if (IS.dectype == IDENT)
		{
			SAVE_INTERPRETER_STATE
			Object* cob = NULL;
			cob = static_cast<Object*>(ob);
			cob->SModelObject = new Model;
			IS.scp = cob->SModelObject;
			mod->visit(this);
			RESET_INTERPRETER_STATE
		}
		else
		{
			// Default values for variables/Parameters
			if(node->bHasDefault)
			{
				eval = NULL;
				eval = static_cast<ASTdefualtNode*>(node->astnDefault)->astnExpr->visit(this);
				assert(eval);
				ob->SValue = eval->value;
			}
			else
			{
				ob->SValue = SY_DEFAULT_VAL;
			}

			// Process Options
			if(node->bHasOptions)
			{
				IS.ob = ob;
				VISIT_ALL_NODES(node, astvnOptions)
			}
		}
	}

	insert:

	// Insert into symbol table only if declaration successful
	// this also prevents self inference 
	if (!(INSERT_NEW_ENTITY_IN_MODEL(node->sName, ob)))
	{
		if (ob)
			delete ob;

		if (ob->geType() == VARIABLE)
		{
			delete ob2;
		}

		semanticErr_EntityRedeclaration(node);
	}

	// For Variable type insert dt type too
	if (ob->geType() == VARIABLE)
	{
		std::string entityName = "$" + node->sName;
		INSERT_NEW_ENTITY_IN_MODEL(entityName, ob2);
	}

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Process each section
ASTNode* interpreter::dispatch(ASTmodel_sectionNode* node)
{
	// Check section consistency
	checkSectionConsistency(node);
	
	//Process Set section (to set the values of parameters)
	if (node->sName == "SET")
		IS.section_type = SCOPE_SET;

	//Process Set section (to set the values of parameters)
	if (node->sName == "FIX")
		IS.section_type = SCOPE_FIX;
	
	//Process Guess Section (to set guess values for variables)
	if (node->sName == "GUESS")
		IS.section_type = SCOPE_GUESS;

	//Simply evaluate and save the node
	if (node->sName == "EQUATION")
	{
		IS.scp->insertEquationNode(node); //Insert all Equation nodes for easy access in Equation Server
		IS.section_type = SCOPE_EQUATION;
	}

	//Same as equation Node
	if (node->sName == "INIT")
	{
		IS.scp->insertInitNode(node); //Insert all Equation nodes for easy access in Equation Server
		IS.section_type = SCOPE_INIT;
	}

	VISIT_ALL_NODES(node, astvnSectionStatements)

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Declare Unit for entity
ASTNode* interpreter::dispatch(ASTunit_optionNode* node)
{
	//Check if Description already given
	if (IS.ob->bHasUnit)
	{
		semanticErr_optionRedefined("Unit", IS.ob);
		return NULL;
	}

	IS.ob->setUnit(node->sValue);
	IS.ob->bHasUnit = true;

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Declare Description for entity
ASTNode* interpreter::dispatch(ASTdesc_optionNode* node)
{
	//Check if Description already given
	if (IS.ob->bHasDesc)
	{
		semanticErr_optionRedefined("Description", IS.ob);
		return NULL;
	}

	IS.ob->setDesc(node->sValue);
	IS.ob->bHasDesc = true;

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Declare bounds for entity of type Variable only!
ASTNode* interpreter::dispatch(ASTbounds_optionNode * node)
{
	int ret = 0;

	// Map to char(this was done to conserve memory)
	if (IS.ob->geType() == VARIABLE)
	{
		char op = ' ';
		switch (node->iInequalityOp)
		{
		case GTHAN:
			if (IS.ob->bHasBounds[0]) { semanticErr_optionRedefined(">", IS.ob); return NULL; }
			op = SY_GREATER_THAN;
			IS.ob->bHasBounds[0] = true;
			break;

		case LTHAN:
			if (IS.ob->bHasBounds[1]) { semanticErr_optionRedefined("<", IS.ob); return NULL; }
			op = SY_LESS_THAN;
			IS.ob->bHasBounds[1] = true;
			break;

		case GEQUALS:
			if (IS.ob->bHasBounds[2]) { semanticErr_optionRedefined(">=", IS.ob); return NULL; }
			op = SY_GREATER_THAN_OR_EQUAL;
			IS.ob->bHasBounds[2] = true;
			break;

		case LEQUALS:
			if (IS.ob->bHasBounds[3]) { semanticErr_optionRedefined("<=", IS.ob); return NULL; }
			op = SY_LESS_THAN_OR_EQUAL;
			IS.ob->bHasBounds[3] = true;
			break;
		}

		//Evaluate expression
		ASTNode* bval = NULL;
		bval = node->astnExpr->visit(this);
		assert(bval);

		Variable* var = NULL;
		assert(IS.ob);
		var	= static_cast<Variable*>(IS.ob);

		//Check if scalar or vector
		if (IS.ob->getDimType() == SY_SCALAR)
		{
			if (op == SY_GREATER_THAN || op == SY_GREATER_THAN_OR_EQUAL)
			{
				var->SLowerType = op;
				var->SLower = bval->value;
			}
			else
			{
				var->SUpperType = op;
				var->SUpper = bval->value;
			}
			return node;
		}
		else
		{
			if (op == SY_GREATER_THAN || op == SY_GREATER_THAN_OR_EQUAL)
			{
				assert(var->setAlltoB(SY_VARIABLE_LBT, op, bval->value) == SY_SUCCESS);
			}
			else
			{
				assert(var->setAlltoB(SY_VARIABLE_UBT, op, bval->value) == SY_SUCCESS);
			}
		}
	}
	else
	{
		semanticErr_BoundsOnlyForVariables(node);
	}

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// return the evaluated default value
ASTNode* interpreter::dispatch(ASTdefualtNode* node)
{
	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Equation node (This is simply evaluated without using the returns, just to catch semantic errors)
ASTNode* interpreter::dispatch(ASTequationNode* node)
{
	ASTNode* LHS = NULL;
	ASTNode* RHS = NULL;

	LHS = VISIT_NODE(node->astnLHS);
	RHS = VISIT_NODE(node->astnRHS);

	assert(LHS);
	assert(RHS);

	if (node->iEquationTypeOp != EQUALS)
	{
		semanticErr_OnlyEqualityInEquation(node);
	}

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Expression Evaluation
ASTNode* interpreter::dispatch(ASTMathBinaryOpNode* node)
{
	ASTNode* left_eval = NULL;
	ASTNode* right_eval = NULL;

	//Get left and right eval
	left_eval = VISIT_NODE(node->astnLeft);
	right_eval = VISIT_NODE(node->astnRight);

	assert(left_eval);
	assert(right_eval);

	// Type of binary operation
	switch (node->iBinaryOp)
	{
	case PLUS:
		node->value = left_eval->value + right_eval->value;
		break;

	case MINUS:
		node->value = left_eval->value - right_eval->value;
		break;

	case MULT:
		node->value = left_eval->value * right_eval->value;
		break;

	case DIV:
		node->value = left_eval->value / right_eval->value;
		break;

	case RAISE:
		node->value = pow(left_eval->value, right_eval->value);
	}

	return node;
}

ASTNode* interpreter::dispatch(ASTMathUnaryNode* node)
{
	ASTNode* right_eval = NULL;

	right_eval = VISIT_NODE(node->astnRight);
	assert(right_eval);

	// Type of unary operation
	switch (node->iUnaryOp)
	{
	case PLUS:
		node->value = right_eval->value;
		break;

	case MINUS:
		node->value = -right_eval->value;
		break;
	}

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Simply return a number node
ASTNode* interpreter::dispatch(ASTNumberNode* node)
{
	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Just Evaluate and return the node
ASTNode* interpreter::dispatch(ASTQualifiedNamedReferenceNode* node)
{
	if (evaluate(node))
		return node;

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Expression Evaluation(logical)
ASTNode* interpreter::dispatch(ASTLogicalBinaryOpNode* node)
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
	case AND:
		node->lvalue = left_eval->lvalue && right_eval->lvalue;
		break;

	case OR:
		node->lvalue = left_eval->lvalue || right_eval->lvalue;
		break;

	case GTHAN:
		node->lvalue = left_eval->value > right_eval->value;
		break;

	case LTHAN:
		node->lvalue = left_eval->value < right_eval->value;
		break;

	case GEQUALS:
		node->lvalue = left_eval->value >= right_eval->value;
		break;

	case LEQUALS:
		node->lvalue = left_eval->value <= right_eval->value;
		break;

	case DEQUALS:
		node->lvalue = left_eval->value == right_eval->value;
		break;
	}

	return node;
}

ASTNode* interpreter::dispatch(ASTLogicalUnaryNode* node)
{
	ASTNode* right_eval = NULL;

	right_eval = VISIT_NODE(node->astnRight);
	assert(right_eval);

	switch (node->iUnaryOp)
	{
	case NOT:
		node->lvalue = !right_eval->lvalue;
		break;
	}

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// if statement
ASTNode* interpreter::dispatch(ASTif_statmentNode* node)
{
	ASTNode* cond = NULL;
	cond = VISIT_NODE(node->astnCondition);
	assert(cond);

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
// performs the for loop
ASTNode* interpreter::dispatch(ASTfor_loopNode* node)
{
	ModelEntity* iter = NULL;
	iter = SEARCH_IN_MODEL(NULL, node->sName);

	if (iter)
	{
		//iter has to be an iter type variable
		if (iter->Type != ITER)
		{
			semanticErr_OnlyIterAllowedInFor(node);
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
		
		// Increment is optional
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
					for (int i = start_val; i <= till_val; i = i + incr_val, iter->SValue = iter->SValue + incr_val)
					{
						VISIT_ALL_NODES(node, astvnfors)
					}
				}
				else
				{
					semanticErr_EndlessForDetected(node);
				}
			}
			else
			{
				if (start_val >= till_val)
				{
					for (int i = start_val; i >= till_val; i = i - incr_val, iter->SValue = iter->SValue - incr_val)
					{
						VISIT_ALL_NODES(node, astvnfors)
					}
				}
				else
				{
					semanticErr_EndlessForDetected(node);
				}
			}
		}
		else
		{
			//For no increment case
			if (start_val <= till_val)
			{
				for (int i = start_val; i <= till_val; ++i, ++iter->SValue)
				{
					VISIT_ALL_NODES(node, astvnfors)
				}
			}
			else
			{
				for (int i = start_val; i >= till_val; --i, --iter->SValue)
				{
					VISIT_ALL_NODES(node, astvnfors)
				}
			}
		}
	}
	else
	{
		semanticErr_UndeclaredEntity(node->tName);
	}

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Assigns values
ASTNode* interpreter::dispatch(ASTassignmentNode* node)
{
	//get LHS
	ASTQualifiedNamedReferenceNode* LHS = NULL;
	ASTNode* RHS = NULL;
	
	LHS = static_cast<ASTQualifiedNamedReferenceNode*>(node->astnLHS);
	RHS = VISIT_NODE(node->astnRHS);
	assert(LHS);
	assert(RHS);
	
	if (assign(LHS, RHS->value, node->iEquationTypeOp))
	{
		return node;
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Runs the PDE equation node
ASTNode* interpreter::dispatch(ASTFEMoneDEquation* node)
{
	//Evaluate the domain
	evaluate(static_cast<ASTQualifiedNamedReferenceNode*>(node->astnDomain));

	// Fix Neumann nodes as required
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
				semanticErr_NoDomainAssociatedWithVar(node->astnDomain);
			}
		}
		else
		{
			semanticErr_NotAFieldVariable(node->astnDomain);
		}
	}
	else
		semanticErr_ExpectedAVariableRefInDomain(node->astnDomain);

	assert(dom);
	FEMOneDModelDomainData* dom1 = static_cast<FEMOneDModelDomainData*>(dom);
	if (node->iLeftBoundaryType == VALUE)
	{
		static_cast<Variable*>(node->astnDomain->me)->VFixValueToggle[dom1->domainNuemannSize["left"]] = SY_FIX;
		static_cast<Variable*>(node->astnDomain->me->other)->VFixValueToggle[dom1->domainNuemannSize["left"]] = SY_FIX;
	}
	if (node->iRightBoundaryType == VALUE)
	{
		static_cast<Variable*>(node->astnDomain->me)->VFixValueToggle[dom1->domainNuemannSize["right"]] = SY_FIX;
		static_cast<Variable*>(node->astnDomain->me->other)->VFixValueToggle[dom1->domainNuemannSize["right"]] = SY_FIX;
	}
	
	// Evaluate the equation
	VISIT_NODE(node->astnEqn);

	// Evaluate the boundaries
	VISIT_NODE(node->astnBoundaryLeft);
	VISIT_NODE(node->astnBoundaryRight);

	return node;
}

// UNUSED 
UNUSED_AST_NODE_FUNCTION(interpreter, ASTmodel_entities_collectionNode)
UNUSED_AST_NODE_FUNCTION(interpreter, ASTgeneric_tokenNode)
UNUSED_AST_NODE_FUNCTION(interpreter, ASToptions_groupNode)
UNUSED_AST_NODE_FUNCTION(interpreter, ASTtype_optionNode)
UNUSED_AST_NODE_FUNCTION(interpreter, ASTNamedReferenceNode)
UNUSED_AST_NODE_FUNCTION(interpreter, ASTArrayIndicesNode)
UNUSED_AST_NODE_FUNCTION(interpreter, ASTarray_indexNode)
UNUSED_AST_NODE_FUNCTION(interpreter, ASTstatement_blockNode)

UNUSED_AST_NODE_FUNCTION(interpreter, ASTBasisTypeNode)
UNUSED_AST_NODE_FUNCTION(interpreter, ASTSegmentAttributesNode)
UNUSED_AST_NODE_FUNCTION(interpreter, ASTPdevarAttributesNode)
UNUSED_AST_NODE_FUNCTION(interpreter, ASTSegmentIdTypeNode)
UNUSED_AST_NODE_FUNCTION(interpreter, ASTNumelNode)
UNUSED_AST_NODE_FUNCTION(interpreter, ASTLengthNode)
UNUSED_AST_NODE_FUNCTION(interpreter, ASTdomainOptionNode)
UNUSED_AST_NODE_FUNCTION(interpreter, ASTstringIndex)
UNUSED_AST_NODE_FUNCTION(interpreter, ASTDerivativeSingleVar)
UNUSED_AST_NODE_FUNCTION(interpreter, ASTBoundaryConditions)
UNUSED_AST_NODE_FUNCTION(interpreter, ASTSimulationArgs)