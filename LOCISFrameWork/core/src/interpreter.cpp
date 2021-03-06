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
    MasterModel = new object;
    MasterModel->sModelObject = new model;
    MasterModel->setType(KW_MODEL);
	MasterModel->setSType("MODEL");
	MasterModel->setName("Master");
	MasterModel->setDimType(SY_SCALAR);
    IS.scp = MasterModel->sModelObject;

	// Pass master to the last model in list(only way to currently identify master model)
	(*(node->astvnModelCollection.end() - 1))->visit(this);

	// Initialized any initialized Vectors
    if (InitAll(MasterModel->sModelObject))
	{
		// Get all the Variables (should always work)
        getAllVars(MasterModel->sModelObject, MasterModel->getName());
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
                if (temp->iDeclType == DT_IDENT)
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
    if (node->iDeclType == DT_IDENT)
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
    modelEntity* ob = NULL;
    modelEntity* ob2 = NULL;
	ASTNode* mod = NULL;
    model* save = NULL;

	// Find type of entity
	switch (IS.dectype)
	{
		// Regular LOCIS objects
        case DT_IDENT:
            ob = new object;
			mod = SymTab->getModel(IS.modelName);
			if (!mod)
			{
				semanticErr_ModelUndefined(node);
			}
            static_cast<object*>(ob)->setModelName(IS.modelName);
			ob->setName(node->sName);
            ob->setType(KW_MODEL);
			ob->setSType("MODEL");
			break;

		// FEMoneD line objects (built like variables/parameters)
        case KW_LINE:
			initFEMLine(node);
			return node;

        case KW_PARAMETER:
            ob = new parameter;
			ob->setName(node->sName);
			ob->setNType(IS.numtype);
            ob->setType(KW_PARAMETER);
			ob->setSType("PARAMETER");
			break;

        case KW_VARIABLE:

			// Regular
            ob = new variable;
			ob->setNType(IS.numtype);
			ob->setName(node->sName);
            ob->setType(KW_VARIABLE);
			ob->setSType("VARIABLE");

			// Time dependent
            ob2 = new variable;
			ob2->setNType(IS.numtype);
			ob2->setName("$" + node->sName);
            ob2->setType(KW_VARIABLE);
			ob2->setSType("VARIABLE");
			ob2->setIsdt();
			
			// Give each others reference to each other
            ob2->setOther(ob);
            ob->setOther(ob2);
			break;

        case KW_ITER:
            ob = new parameter;
			ob->setName(node->sName);
            ob->setType(KW_ITER);
			ob->setSType("ITER");
			break;
	}

	// Save the token for the entity
    ob->setTok(&node->tName);
	IS.ob_tok = node->tName;

	// Cannot declare dt variables
	if (node->bIsdt)
	{
		semanticErr_dtVariableDefined(node);
	}

	//Check if iteration variable
    if (IS.dectype == KW_ITER)
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
    if (ob->geType() == KW_MODEL)
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
        ob->setVectorIsInitialized(false);
        ob->setBuildNode(node);

		// Has to set dummy values for the array indices for validation during assignment
		LOOP_OVER_NODES(node, astvnArrayIndices)
		{
            ob->setDummyIndex();
		}

		// Create dt variable the same way as variable
        if (ob->geType() == KW_VARIABLE)
		{
			ob2->setDimType(SY_VECTOR);
            ob2->setVectorIsInitialized(false);
            ob2->setBuildNode(node);

			LOOP_OVER_NODES(node, astvnArrayIndices)
			{
                ob2->setDummyIndex();
			}
		}

		// Default values are assigned on first occurrence of vectors
	}
	else
	{
		ob->setDimType(SY_SCALAR);
        if (ob->geType() == KW_VARIABLE)
		{
			ob2->setDimType(SY_SCALAR);
		}

        if (IS.dectype == DT_IDENT)
		{
			SAVE_INTERPRETER_STATE
            object* cob = NULL;
            cob = static_cast<object*>(ob);
            cob->sModelObject = new model;
            IS.scp = cob->sModelObject;
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
                ob->sValue = eval->value;
			}
			else
			{
                ob->sValue = SY_DEFAULT_VAL;
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

        if (ob->geType() == KW_VARIABLE)
		{
			delete ob2;
		}

		semanticErr_EntityRedeclaration(node);
	}

	// For Variable type insert dt type too
    if (ob->geType() == KW_VARIABLE)
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
    if (IS.ob->getBHasUnit())
	{
		semanticErr_optionRedefined("Unit", IS.ob);
		return NULL;
	}

	IS.ob->setUnit(node->sValue);
    IS.ob->setBHasUnit(true);

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Declare Description for entity
ASTNode* interpreter::dispatch(ASTdesc_optionNode* node)
{
	//Check if Description already given
    if (IS.ob->getBHasDesc())
	{
		semanticErr_optionRedefined("Description", IS.ob);
		return NULL;
	}

	IS.ob->setDesc(node->sValue);
    IS.ob->setBHasDesc(true);

	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Declare bounds for entity of type Variable only!
ASTNode* interpreter::dispatch(ASTbounds_optionNode * node)
{
	int ret = 0;

	// Map to char(this was done to conserve memory)
    if (IS.ob->geType() == KW_VARIABLE)
	{
		char op = ' ';
		switch (node->iInequalityOp)
		{
        case OP_GTHAN:
            if (IS.ob->getBHasBounds(0)) { semanticErr_optionRedefined(">", IS.ob); return NULL; }
			op = SY_GREATER_THAN;
            IS.ob->setBHasBounds(0,true);
			break;

        case OP_LTHAN:
            if (IS.ob->getBHasBounds(1)) { semanticErr_optionRedefined("<", IS.ob); return NULL; }
			op = SY_LESS_THAN;
            IS.ob->setBHasBounds(1,true);
			break;

        case OP_GEQUALS:
            if (IS.ob->getBHasBounds(2)) { semanticErr_optionRedefined(">=", IS.ob); return NULL; }
			op = SY_GREATER_THAN_OR_EQUAL;
            IS.ob->setBHasBounds(2,true);
			break;

        case OP_LEQUALS:
            if (IS.ob->getBHasBounds(3)) { semanticErr_optionRedefined("<=", IS.ob); return NULL; }
			op = SY_LESS_THAN_OR_EQUAL;
            IS.ob->setBHasBounds(3,true);
			break;
		}

		//Evaluate expression
		ASTNode* bval = NULL;
		bval = node->astnExpr->visit(this);
		assert(bval);

        variable* var = NULL;
		assert(IS.ob);
        var	= static_cast<variable*>(IS.ob);

		//Check if scalar or vector
		if (IS.ob->getDimType() == SY_SCALAR)
		{
			if (op == SY_GREATER_THAN || op == SY_GREATER_THAN_OR_EQUAL)
			{
                var->sLowerType = op;
                var->sLowerValue = bval->value;
			}
			else
			{
                var->sUpperType = op;
                var->sUpperValue = bval->value;
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

    if (node->iEquationTypeOp != OP_EQUALS)
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
    case OP_PLUS:
		node->value = left_eval->value + right_eval->value;
		break;

    case OP_MINUS:
		node->value = left_eval->value - right_eval->value;
		break;

    case OP_MULT:
		node->value = left_eval->value * right_eval->value;
		break;

    case OP_DIV:
		node->value = left_eval->value / right_eval->value;
		break;

    case OP_RAISE:
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
    case OP_PLUS:
		node->value = right_eval->value;
		break;

    case OP_MINUS:
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
    case OP_AND:
		node->lvalue = left_eval->lvalue && right_eval->lvalue;
		break;

    case OP_OR:
		node->lvalue = left_eval->lvalue || right_eval->lvalue;
		break;

    case OP_GTHAN:
		node->lvalue = left_eval->value > right_eval->value;
		break;

    case OP_LTHAN:
		node->lvalue = left_eval->value < right_eval->value;
		break;

    case OP_GEQUALS:
		node->lvalue = left_eval->value >= right_eval->value;
		break;

    case OP_LEQUALS:
		node->lvalue = left_eval->value <= right_eval->value;
		break;

    case OP_DEQUALS:
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
    case OP_NOT:
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
    modelEntity* iter = NULL;
	iter = SEARCH_IN_MODEL(NULL, node->sName);

	if (iter)
	{
		//iter has to be an iter type variable
        if (iter->geType() != KW_ITER)
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
        iter->sValue = start_val;
		till_val = (int)till->value;
		if (incr)
		{
			incr_val = (int)incr->value;
			if (incr_val > 0.0)
			{
				if (start_val <= till_val)
				{
                    for (int i = start_val; i <= till_val; i = i + incr_val, iter->sValue = iter->sValue + incr_val)
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
                    for (int i = start_val; i >= till_val; i = i - incr_val, iter->sValue = iter->sValue - incr_val)
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
                for (int i = start_val; i <= till_val; ++i, ++iter->sValue)
				{
					VISIT_ALL_NODES(node, astvnfors)
				}
			}
			else
			{
                for (int i = start_val; i >= till_val; --i, --iter->sValue)
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
    modelEntity* dom = NULL;
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
    if (node->iLeftBoundaryType == KW_VALUE)
	{
        static_cast<variable*>(node->astnDomain->me)->vFixValueToggle[dom1->domainNuemannSize["left"]] = SY_FIX;
        static_cast<variable*>(node->astnDomain->me->getOther())->vFixValueToggle[dom1->domainNuemannSize["left"]] = SY_FIX;
	}
    if (node->iRightBoundaryType == KW_VALUE)
	{
        static_cast<variable*>(node->astnDomain->me)->vFixValueToggle[dom1->domainNuemannSize["right"]] = SY_FIX;
        static_cast<variable*>(node->astnDomain->me->getOther())->vFixValueToggle[dom1->domainNuemannSize["right"]] = SY_FIX;
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
