#include "interpreter.h"
#include "equation.h"
#include <cmath>
#include <iomanip>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Start the interpreter
void interpreter::Start()
{
	// Run the AST!
	VISIT_NODE(AST);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Visits the node
ASTNode* interpreter::VISIT_NODE(ASTNode* node)
{
	if (node)
		return node->visit(this);
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Scalar evaluation of a Qualified Named reference Node (services both interpreter and equation)
bool interpreter::evaluate(ASTQualifiedNamedReferenceNode* qRefName)
{
	// Resolve Qualified name
	ASTNamedReferenceNode* item = NULL;
	Model* search = NULL;
	ModelEntity* search_res = NULL;
	Object* next = NULL;

	for (std::vector<ASTNode*>::const_iterator it = qRefName->astvnQualifiedName.begin(); it != qRefName->astvnQualifiedName.end(); ++it)
	{
		item = dynamic_cast<ASTNamedReferenceNode*>(*it);

		// See if entity exists 
		search_res = SEARCH_IN_MODEL(search, item->sName);
		if (search_res == NULL)
		{
			semanticErr_UndeclaredEntity(item->tName);
			return false;
		}

		// check if item an FEMoneD reference
		if (search_res->isFEMOneDLine)
		{
			return evaluateFEMoneDObject(static_cast<FEMOneDLineObject*>(search_res), qRefName, it);
		}

		// Successful search (now prepare next item)
		if (item->bIsArray)
		{
			//Check if entity is a vector
			if (search_res->getDimType() != SY_VECTOR)
			{
				semanticErr_NotAVector(item->tName);
				return false;
			}

			//Check if same number of dimensions
			if (item->astvnArrayIndices.size() != search_res->getNumDims())
			{
				semanticErr_DimensionMismatch(item->tName);
				return false;
			}

			// Check if reference is allocated
			if(!initAllVector(search_res))
			{
				return false;
			}

			ASTNode* eval = NULL;
			int ival = 0;
			int fullsum = 0;
			int c = 0;
			bool success = true;
			bool temp_bInequation = false;

			for (std::vector<ASTNode*>::const_iterator it = item->astvnArrayIndices.begin(); it != item->astvnArrayIndices.end(); ++it)
			{
				temp_bInequation = bInEquation;
				bInEquation = false;
				eval = VISIT_NODE(*it);
				bInEquation = temp_bInequation;
				assert(eval);

				ival = (int)eval->value;
				if ((ival > 0) && (ival <= search_res->Dims.at(c)))
				{
					fullsum += (ival - 1)*search_res->DimsC.at(c);
				}
				else
				{
					semanticErr_IndexOutOfBounds(item->tName);
					return false;
				}
				c++;
			}

			//Now check type of entity
			if (search_res->geType() != MODEL)
			{
				//This means this is terminal as variables and parameters cannot be qualified
				//Check if this is the final element
				if (it != (qRefName->astvnQualifiedName.end() - 1))
				{
					semanticErr_InvalidQualification(item->tName);
					return false;
				}

				//Set the value
				//Check if dt term
				if (search_res->vset)
				{
					// Call from equation
					if(bInEquation)
						qRefName->equation_data = static_cast<equation*>(Eq)->getSubstitutedEntityNameVector(search_res, fullsum);
					else
						qRefName->value = search_res->Vvalue[fullsum];
				}
				else
				{
					semanticErr_UninitializedVector(item->tName);
					return false;
				}
				return true;
			}

			//Set to next
			search = &dynamic_cast<Object*>(search_res)->VModelObject[fullsum];
			assert(search);
		}
		else
		{
			//Check if entity is a scalar
			if (search_res->getDimType() != SY_SCALAR)
			{
				semanticErr_EntityIsNotAScalar(item->tName);
				return false;
			}

			//Now check type of entity
			if (search_res->geType() != MODEL)
			{
				//This means this is terminal as variables and parameters cannot be qualified
				//Check if this is the final element
				if (it != (qRefName->astvnQualifiedName.end() - 1))
				{
					semanticErr_InvalidQualification(item->tName);
					return false;
				}

				//Return the value
				if (bInEquation)
				{
					// check if tval or xval
					if (search_res->isTval)
					{
						if (static_cast<equation*>(Eq)->isInitialzation())
						{
							std::stringstream ss;
							ss << std::scientific << std::setprecision(10) << search_res->SValue;
							qRefName->equation_data = ss.str();
						}
						else
						{
							if (simuArg_iType == 1)
								qRefName->equation_data = "_t";
							else
								qRefName->equation_data = "0.0";
						}
						return true;
					}

					if (search_res->isFEMtrialFunction || search_res->isFEMxval)
					{
						qRefName->equation_data = "0.0";
						return true;
					}

					qRefName->equation_data = static_cast<equation*>(Eq)->getSubstitutedEntityNameScalar(search_res);
				}
				else
				{
					// If FEM trial function just setup the node so that it can be recognized
					if (search_res->isFEMtrialFunction || search_res->isFEMxval)
					{
						qRefName->me = search_res;
						qRefName->iNumDerivs = (*it)->iNumDerivs;
						return true;
					}
					
					qRefName->value = search_res->SValue;
				}

				return true;
			}

			//Set to next
			search = dynamic_cast<Object*>(search_res)->SModelObject;
			assert(search);
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Scalar setting of Variables and Parameters
bool interpreter::assign(ASTQualifiedNamedReferenceNode* qRefName, double &val, int &op)
{
	// Resolve Qualified name
	ASTNamedReferenceNode* item = NULL;
	Model* search = NULL;
	ModelEntity* search_res = NULL;
	Object* next = NULL;
	bool set_success_s = false;
	bool set_success_v = false;

	// fullsum needs to be declared outside the scope to get the value during substitution
	int fullsum = -1;
	for (std::vector<ASTNode*>::const_iterator it = qRefName->astvnQualifiedName.begin(); it != qRefName->astvnQualifiedName.end(); ++it)
	{
		item = dynamic_cast<ASTNamedReferenceNode*>(*it);

		// See if entity exists 
		search_res = SEARCH_IN_MODEL(search, item->sName);
		if (search_res == NULL)
		{
			semanticErr_UndeclaredEntity(item->tName);
			return false;
		}

		// Set entity token
		search_res->tok = &item->tName;

		// check if item an FEMoneD reference
		if (search_res->isFEMOneDLine)
		{
			FEMOneDLineObject* FEMobj = static_cast<FEMOneDLineObject*>(search_res);
			return assignFEMoneDObject(FEMobj, qRefName, it, val, op);
		}

		// Successful search (now prepare next item)
		if (item->bIsArray)
		{
			//Check if entity is a vector
			if (search_res->getDimType() != SY_VECTOR)
			{
				semanticErr_NotAVector(item->tName);
				return false;
			}

			//Check if same number of dimensions
			if (item->astvnArrayIndices.size() != search_res->getNumDims())
			{
				semanticErr_DimensionMismatch(item->tName);
				return false;
			}

			// Check if reference is allocated
			if (!initAllVector(search_res))
			{
				return false;
			}

			ASTNode* eval;
			int ival = 0;
			fullsum = 0;
			int c = 0;
			bool success = true;
			for (std::vector<ASTNode*>::const_iterator it = item->astvnArrayIndices.begin(); it != item->astvnArrayIndices.end(); ++it)
			{
				eval = VISIT_NODE(*it);
				assert(eval);
				ival = (int)eval->value;
				if ((ival > 0) && (ival <= search_res->Dims.at(c)))
				{
					fullsum += (ival - 1)*search_res->DimsC.at(c);
				}
				else
				{
					semanticErr_IndexOutOfBounds(item->tName);
					return false;
				}
				c++;
			}

			//Now check type of entity
			if (search_res->geType() != MODEL)
			{
				//This means this is terminal as variables and parameters cannot be qualified
				//Check if this is the final element
				if (it != (qRefName->astvnQualifiedName.end() - 1))
				{
					semanticErr_InvalidQualification(item->tName);
					return false;
				}

				//Set the value
				set_success_v = true;
				break;
			}

			//Set to next
			search = &dynamic_cast<Object*>(search_res)->VModelObject[fullsum];
			assert(search);
		}
		else
		{
			/* This check is not needed as the vector can be referenced with no array indices
			//Check if entity is a scalar
			if (search_res->getDimType() != SY_SCALAR)
			{
			//SEMANTIC ERROR:: Refernced entity is not a scalar
			break;
			}
			*/

			//Now check type of entity
			if (search_res->geType() != MODEL)
			{
				//This means this is terminal as variables and parameters cannot be qualified
				//Check if this is the final element
				if (it != (qRefName->astvnQualifiedName.end() - 1))
				{
					semanticErr_InvalidQualification(item->tName);
					return false;
				}

				//Return the value
				set_success_s = true;
				break;
			}

			//Set to next
			search = dynamic_cast<Object*>(search_res)->SModelObject;
			assert(search);
		}
	}

	// No direct assignment to iter Parameter
	if (search_res->Type == ITER)
	{
		semanticErr_IterCannotBeAssigned(item->tName);
		return false;
	}

	// Perform checks to see if correct scope
	if (IS.section_type == SCOPE_SET)
	{
		if (search_res->Type != PARAMETER)
		{
			semanticErr_OnlyParametersSet(item->tName);
			return false;
		}

		if (op != EQUALS)
		{
			semanticErr_NoBoundsAllowed(item->tName);
			return false;
		}
	}
	else if (IS.section_type == SCOPE_FIX)
	{
		if (search_res->Type != VARIABLE)
		{
			semanticErr_OnlyVariablesFix(item->tName);
			return false;
		}
		else
		{
			if (search_res->checkIsdt())
			{
				semanticErr_OnlyVariablesFix(item->tName);
				return false;
			}
		}

	}
	else if (IS.section_type == SCOPE_GUESS)
	{
		if (search_res->Type != VARIABLE)
		{
			semanticErr_OnlyVariablesGuess(item->tName);
			return false;
		}

		if (op != EQUALS)
		{
			semanticErr_NoBoundsInGuess(item->tName);
			return false;
		}
	}

	// Now set the values
	Variable* v = NULL;
	if (set_success_s)
	{
		switch (op)
		{
		case(EQUALS):
			search_res->SValue = val;
			if (IS.section_type == SCOPE_FIX)
			{
				v = static_cast<Variable*>(search_res);
				v->SFixValueToggle = SY_FIX;

				//Fix dt component too
				v = static_cast<Variable*>(search_res->other);
				v->SFixValueToggle = SY_FIX;
			}
			break;

		case(GEQUALS):
		case (GTHAN):
			v = static_cast<Variable*>(search_res);
			v->SLowerType = optochar(op);
			v->SLower = val;
			break;

		case(LEQUALS):
		case (LTHAN):
			v = static_cast<Variable*>(search_res);
			v->SUpperType = optochar(op);
			v->SUpper = val;
			break;
		}
		return true;
	}

	// Actually set the values here(Vector) 
	if (set_success_v)
	{
		switch (op)
		{
		case(EQUALS):
			if (fullsum != -1)
			{
				search_res->Vvalue[fullsum] = val;
				if (IS.section_type == SCOPE_FIX)
				{
					v = static_cast<Variable*>(search_res);
					v->VFixValueToggle[fullsum] = SY_FIX;

					// Fix dt component too
					v = static_cast<Variable*>(search_res->other);
					v->VFixValueToggle[fullsum] = SY_FIX;
				}
			}
			else
			{
				search_res->setAllto(val);
				if (IS.section_type == SCOPE_FIX)
				{
					v = static_cast<Variable*>(search_res);
					assert(v->setAlltoB(SY_VARIABLE_FT, SY_FIX, 0.0) == SY_SUCCESS);

					// Fix dt component too
					v = static_cast<Variable*>(search_res->other);
					assert(v->setAlltoB(SY_VARIABLE_FT, SY_FIX, 0.0) == SY_SUCCESS);
				}
			}
			break;

		case(GEQUALS):
		case (GTHAN):
			v = static_cast<Variable*>(search_res);
			if (fullsum != -1)
			{
				v->VLowerType[fullsum] = optochar(op);
				v->VLower[fullsum] = val;
			}
			else
			{
				assert(v->setAlltoB(SY_VARIABLE_LBT, optochar(op), val) == SY_SUCCESS);
			}
			break;

		case(LEQUALS):
		case (LTHAN):
			v = static_cast<Variable*>(search_res);
			if (fullsum != 0)
			{
				v->VUpperType[fullsum] = optochar(op);
				v->VUpper[fullsum] = val;
			}
			else
			{
				assert(v->setAlltoB(SY_VARIABLE_UBT, optochar(op), val) == SY_SUCCESS);
			}
			break;
		}
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize Vector
bool interpreter::initVector(ModelEntity* search_res)
{
	ASTNode* eval = NULL;
	int ret = 0;
	assert(search_res->build_node);
	ASTmodel_entity_declNode* node = static_cast<ASTmodel_entity_declNode*>(search_res->build_node);

	// Clear dummy dimensions
	search_res->Dims.clear();

	LOOP_OVER_NODES(node, astvnArrayIndices)
	{
		eval = VISIT_NODE(*it_astvnArrayIndices);
		assert(eval);
		ret = search_res->setDims(eval->value);
		if (ret != SY_SUCCESS)
		{
			semanticErr_IndexOutOfBounds(*search_res->tok);
			return false;
		}
	}

	// Check if Object
	if (search_res->geType() == MODEL)
	{
		Object* cob = NULL;
		Model* save = NULL;
		ASTNode* mod = NULL;
		cob = static_cast<Object*>(search_res);
		assert(cob->allocateArray() == SY_SUCCESS);

		std::string cob_getModelName = cob->getModelName();
		mod = SymTab->getModel(cob_getModelName);
		assert(mod);

		SAVE_INTERPRETER_STATE
		for (int i = 0; i < cob->Maxdim; i++)
		{
			IS.scp = &cob->VModelObject[i];
			mod->visit(this);
		}
		RESET_INTERPRETER_STATE

		search_res->vset = true;
		return true;
	}

	// Process Default
	if (node->bHasDefault)
	{
		eval = NULL;
		eval = VISIT_NODE(static_cast<ASTdefualtNode*>(node->astnDefault)->astnExpr);
		assert(eval);
		assert(search_res->allocateArray(eval->value) == SY_SUCCESS);
	}
	else
	{
		assert(ret = search_res->allocateArray(SY_DEFAULT_VAL) == SY_SUCCESS);
	}

	// Process Options
	if (node->bHasOptions)
	{
		IS.ob = search_res;
		VISIT_ALL_NODES(node, astvnOptions)
	}

	search_res->vset = true;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Completely initializes a Vector entity
bool interpreter::initAllVector(ModelEntity* search_res)
{
	if (!search_res->vset)
	{
		if (!initVector(search_res))
		{
			return false;
		}

		// For Variable initialize each other if uninitialized
		if (search_res->geType() == VARIABLE)
		{
			if (!search_res->other->vset)
			{
				if (!initVector(search_res->other))
				{
					return false;
				}
			}
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Resolve operators
char interpreter::optochar(int & op)
{
	switch (op)
	{
	case GEQUALS:
		return SY_GREATER_THAN_OR_EQUAL;
	case LEQUALS:
		return SY_LESS_THAN_OR_EQUAL;
	case GTHAN:
		return SY_GREATER_THAN;
	case LTHAN:
		return SY_LESS_THAN;
	}

	return SY_GREATER_THAN;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get all the unknowns for the specified model
void interpreter::getAllVars(Model* MainObj, std::string Name_arg)
{
	for (auto it = MainObj->getModelEntities()->begin(); it != MainObj->getModelEntities()->end(); ++it)
	{
		// Add all free Variables
		if (it->second->geType() == VARIABLE && it->second->vset && !it->second->checkIsdt())
		{
			//Once for normal/once for dt variable
			bool done = true;
			ModelEntity* mod = NULL;
			mod = it->second;
			while (done)
			{
				//Stop once dt is encountered
				if (mod->checkIsdt())
				{
					done = false;
				}

				Variable* v = static_cast<Variable*>(mod);
				if (v->getDimType() == SY_SCALAR)
				{
					VariableData vd;
					vd.fullname = Name_arg + "." + v->getName();
					vd.LoType = v->SLowerType;
					vd.UpType = v->SUpperType;
					vd.Lo = v->SLower;
					vd.Up = v->SUpper;
					vd.val = &v->SValue;

					if (v->SFixValueToggle == SY_FREE)
					{
						if (v->checkIsdt())
						{
							numdVars++;
							v->nSValue = numdVars;
							vd.index = numdVars;
							AlldVars.push_back(vd);
						}
						else
						{
							numVars++;
							v->nSValue = numVars;
							vd.index = numVars;
							AllVars.push_back(vd);
						}
					}
					else
					{
						if (v->checkIsdt())
						{
							v->SValue = 0.0;
							vd.val = &v->SValue; // Make derivative values to 0 since algebraic part is fixed
							numfdVars++;
							v->nSValue = numfdVars;
							vd.index = numfdVars;
							AllfdVars.push_back(vd);
						}
						else
						{
							numfVars++;
							v->nSValue = numfVars;
							vd.index = numfVars;
							AllfVars.push_back(vd);
						}
					}
				}
				else
				{
					for (int i = 0; i < v->getMaxDims(); ++i)
					{
						VariableData vd;
						vd.fullname = Name_arg + "." + v->getName() + v->getGetMultiDimsFromSingle(i);
						vd.LoType = v->VLowerType[i];
						vd.UpType = v->VUpperType[i];
						vd.Lo = v->VLower[i];
						vd.Up = v->VUpper[i];
						vd.val = &v->Vvalue[i];

						if (v->VFixValueToggle[i] == SY_FREE)
						{
							if (v->checkIsdt())
							{
								numdVars++;
								v->nVvalue[i] = numdVars;
								vd.index = numdVars;
								AlldVars.push_back(vd);
							}
							else
							{
								numVars++;
								v->nVvalue[i] = numVars;
								vd.index = numVars;
								AllVars.push_back(vd);
							}
						}
						else
						{
							if (v->checkIsdt())
							{
								v->Vvalue[i] = 0.0;
								vd.val = &v->Vvalue[i]; // Make derivative values to 0 since algebraic part is fixed
								numfdVars++;
								v->nVvalue[i] = numfdVars;
								vd.index = numfdVars;
								AllfdVars.push_back(vd);
							}
							else
							{
								numfVars++;
								v->nVvalue[i] = numfVars;
								vd.index = numfVars;
								AllfVars.push_back(vd);
							}
						}
					}
				}

				// Switch to Derivative variable
				std::string dollar_mod_getName = "$" + mod->getName();
				mod = MainObj->getModelEntity(NULL, dollar_mod_getName);
				//assert(mod);
			}
		}

		//Add all Parameters
		if (it->second->geType() == PARAMETER && it->second->vset)
		{
			ModelEntity* mod = NULL;
			mod = it->second;

			Parameter* p = static_cast<Parameter*>(mod);
			if (p->getDimType() == SY_SCALAR)
			{

				ParameterData pd;
				pd.fullname = Name_arg + "." + p->getName();
				pd.val = &p->SValue;

				numPars++;
				p->nSValue = numPars;
				pd.index = numdVars;
				AllPars.push_back(pd);
			}
			else
			{
				for (int i = 0; i < p->getMaxDims(); ++i)
				{

					ParameterData pd;
					pd.fullname = Name_arg + "." + p->getName() + p->getGetMultiDimsFromSingle(i);
					pd.val = &p->Vvalue[i];

					numPars++;
					p->nVvalue[i] = numPars;
					pd.index = numdVars;
					AllPars.push_back(pd);
				}
			}
		}

		//Recursive call this function on all objects in the model
		// Add all free Variables
		if (it->second->geType() == MODEL)
		{
			if (it->second->getDimType() == SY_SCALAR)
			{
				getAllVars(static_cast<Object*>(it->second)->SModelObject, Name_arg + "." + it->second->getName());
			}
			else
			{
				for (int i = 0; i < it->second->getMaxDims(); ++i)
				{
					getAllVars(&static_cast<Object*>(it->second)->VModelObject[i], Name_arg + "." + it->second->getName() + it->second->getGetMultiDimsFromSingle(i));
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Returns all the variable names
std::vector<VariableData>* interpreter::getAllVarNames()
{
	return &AllVars;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize all uninitialized variables
bool interpreter::InitAll(Model* MainObj)
{
	if (!MainObj)
		return false;
	
	for (auto it = MainObj->getModelEntities()->begin(); it != MainObj->getModelEntities()->end(); ++it)
	{
		// Initialize any uninitialized vectors (including objects)
		if (!it->second->vset)
		{
			if (!initVector(it->second))
			{
				return false;
			}
		}

		// Recursive call to all objects in model
		if (it->second->geType() == MODEL)
		{
			Object* m = NULL;
			m = static_cast<Object*>(it->second);
			assert(m);

			if (m->getDimType() == SY_SCALAR)
			{
				return InitAll(m->SModelObject);
			}
			else
			{
				for (int i = 0; i < m->getMaxDims(); ++i)
				{
					return InitAll(&m->VModelObject[i]);
				}
			}
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Return reference to MAster Model
Model* interpreter::getMasterModel()
{
	return MasterModel->SModelObject;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Add any extra symbols to the model(MUST ADD GLOBAL SYMBOL TABLE IN THE FUTURE)
bool interpreter::addExtraSymbols(Model* md)
{
	// Add trial functions as parameters
	Parameter* FEMtrialFunction = new Parameter;
	FEMtrialFunction->setName("_v");
	FEMtrialFunction->setNType(-3);
	FEMtrialFunction->setType(PARAMETER);
	FEMtrialFunction->setSType("PARAMETER");
	FEMtrialFunction->isFEMtrialFunction = true;
	FEMtrialFunction->setDimType(SY_SCALAR);
	FEMtrialFunction->SValue = 1.0;
	std::string FEMtrialFunction_getName = FEMtrialFunction->getName();
	md->insertModelEntity(FEMtrialFunction_getName, FEMtrialFunction);
	assert(md);
	
	// Add time t 
	Parameter* tval = new Parameter;
	tval->setName("_t");
	tval->setNType(-3);
	tval->setType(PARAMETER);
	tval->setSType("PARAMETER");
	tval->isTval = true;
	tval->setDimType(SY_SCALAR);
	tval->SValue = 1.0;
	std::string tval_getName = tval->getName();
	md->insertModelEntity(tval_getName, tval);
	assert(md);

	// Add spacial x 
	Parameter* xval = new Parameter;
	xval->setName("_x");
	xval->setNType(-3);
	xval->setType(PARAMETER);
	xval->setSType("PARAMETER");
	xval->isFEMxval = true;
	xval->setDimType(SY_SCALAR);
	xval->SValue = 1.0;
	std::string xval_getName = xval->getName();
	md->insertModelEntity(xval_getName, xval);
	assert(md);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sets the simulation run arguments
bool interpreter::setSimulationArgs(ASTNode* node, std::string type, double tStart, double tEnd, int numSteps, double absTol, double relTol)
{
	ASTSimulationArgs* simulationArgsNode = static_cast<ASTSimulationArgs*>(node);

	simuArg_type = type;
	simuArg_startt = tStart;
	simuArg_endt = tEnd;
	simuArg_numStepst = numSteps;
	simuArg_asbtol = absTol;
	simuArg_reltol = relTol;

	int line, pos;

	//Sanity check
	if (type == "STEADY_STATE")
	{
		if (absTol < 0.0)
		{
			//SEMANTIC ERROR::
			static_cast<ASTNumberNode*>(simulationArgsNode->simAbsTol)->tNumber.getLnAndPos(line, pos);
			errptr->SetError(1003, "SEMANTIC ERROR", line, pos);
			errptr->AddErrorLine("Absolute tolerance cannot be negative");
			errptr->AddError();
			THROW_INTERPRETER_EX(443, "Interpreter Error")
			return false;
		}

		simuArg_iType = 0;
	}
	else if (type == "DYNAMIC")
	{
		if (absTol < 0.0)
		{
			//SEMANTIC ERROR::
			static_cast<ASTNumberNode*>(simulationArgsNode->simAbsTol)->tNumber.getLnAndPos(line, pos);
			errptr->SetError(1003, "SEMANTIC ERROR", line, pos);
			errptr->AddErrorLine("Absolute error tolerance cannot be negative");
			errptr->AddError();
			THROW_INTERPRETER_EX(443, "Interpreter Error")
			return false;
		}

		if (relTol < 0.0)
		{
			//SEMANTIC ERROR::
			static_cast<ASTNumberNode*>(simulationArgsNode->simRelTol)->tNumber.getLnAndPos(line, pos);
			errptr->SetError(1003, "SEMANTIC ERROR", line, pos);
			errptr->AddErrorLine("Relative error tolerance cannot be negative");
			errptr->AddError();
			THROW_INTERPRETER_EX(443, "Interpreter Error")
			return false;
		}

		if (tEnd < tStart)
		{
			//SEMANTIC ERROR::
			static_cast<ASTNumberNode*>(simulationArgsNode->simEndt)->tNumber.getLnAndPos(line, pos);
			errptr->SetError(1003, "SEMANTIC ERROR", line, pos);
			errptr->AddErrorLine("End time must be greater than start time for integration");
			errptr->AddError();
			THROW_INTERPRETER_EX(443, "Interpreter Error")
			return false;
		}

		if (numSteps <= 0)
		{
			//SEMANTIC ERROR::
			simulationArgsNode->simNumSteps.getLnAndPos(line, pos);
			errptr->SetError(1003, "SEMANTIC ERROR", line, pos);
			errptr->AddErrorLine("There must be at least one time step for integration");
			errptr->AddError();
			THROW_INTERPRETER_EX(443, "Interpreter Error")
			return false;
		}

		simuArg_iType = 1;
	}
	else
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// check if sections in the model are correctly done
void interpreter::checkSectionConsistency(ASTNode* node)
{
	ASTmodel_sectionNode* node1 = static_cast<ASTmodel_sectionNode*>(node);

	// Set should always be behind equation/init section
	if (node1->sName == "SET")
	{
		if (bSetSection)
			semanticErr_SectionRedeclaration(node);

		if(bEquationSection || bInitSection)
			semanticErr_SectionBehindEquationInit(node);

		bSetSection = true;
	}

	// Fix must be always behind guess, init and equation section
	if (node1->sName == "FIX")
	{
		if(bFixSection)
			semanticErr_SectionRedeclaration(node);

		if (bGuessSection)
			semanticErr_SectionBehindGuess(node);

		if (bEquationSection || bInitSection)
			semanticErr_SectionBehindEquationInit(node);

		bFixSection = true;
	}

	// Guess must be always after fix section but behind equation
	if (node1->sName == "GUESS")
	{
		if(bGuessSection)
			semanticErr_SectionRedeclaration(node);

		if (bEquationSection || bInitSection)
			semanticErr_SectionBehindEquationInit(node);

		bGuessSection = true;
	}

	// Init Section must be
	if (node1->sName == "INIT")
	{
		if(bInitSection)
			semanticErr_SectionRedeclaration(node);

		bInitSection = true;
	}

	// Equation section must be
	if (node1->sName == "EQUATION")
	{
		if(bEquationSection)
			semanticErr_SectionRedeclaration(node);

		bInitSection = false;
	}
}


