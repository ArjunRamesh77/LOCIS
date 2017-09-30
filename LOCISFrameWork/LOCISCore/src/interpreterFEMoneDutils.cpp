#include "interpreter.h"
#include "equation.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// utility function to evaluate an FEM object
bool interpreter::evaluateFEMoneDObject(FEMOneDLineObject* FEMobj, ASTQualifiedNamedReferenceNode* qRefName, std::vector<ASTNode*>::const_iterator it_in)
{
	// create the line object if not
	if (!FEMobj->init)
	{
		if (!createFEMoneDline(FEMobj))
			return false;
	}
	
	// Now evaluate
	ASTNamedReferenceNode* item1 = NULL;
	ASTNamedReferenceNode* item2 = NULL;
	ASTNamedReferenceNode* item3 = NULL;
    modelEntity* FEMvarRef = NULL;
    model* search = NULL;
    modelEntity* search_res = NULL;
    object* next = NULL;
	bool temp_bInequation = false;

	// First check if line object is a scalar or vector
	item1 = dynamic_cast<ASTNamedReferenceNode*>(*it_in);
	if (FEMobj->getDimType() == SY_SCALAR)
	{
		search = FEMobj->SModelObject;
		assert(search);

		if (item1->bIsArray)
		{
			semanticErr_NotAVector(item1->tName);
			return false;
		}
	}
	else
	{
		// Navigate to correct Line object
		if (item1->bIsArray)
		{			
			//Check if same number of dimensions
			if (item1->astvnArrayIndices.size() != search_res->getNumDims())
			{
				semanticErr_DimensionMismatch(item1->tName);
				return false;
			}

			ASTNode* eval = NULL;
			int ival = 0;
			int fullsum = 0;
			int c = 0;
			bool success = true;
			for (std::vector<ASTNode*>::const_iterator it = item1->astvnArrayIndices.begin(); it != item1->astvnArrayIndices.end(); ++it)
			{
				temp_bInequation = bInEquation;
				bInEquation = false;
				eval = VISIT_NODE(*it);
				bInEquation = temp_bInequation;
				assert(eval);
	
				ival = (int)eval->value;
                if ((ival > 0) && (ival <= search_res->getDimsAt(c)))
				{
                    fullsum += (ival - 1)*search_res->getDimsCAt(c);
				}
				else
				{
					semanticErr_IndexOutOfBounds(item1->tName);
					return false;
				}
				c++;
			}

            search = &dynamic_cast<object*>(search_res)->VModelObject[fullsum];
			assert(search);
		}
		else
		{
			semanticErr_EntityIsNotAScalar(item1->tName);
			return false;
		}
	}
		
	// Now get a domain or an FEM variable
	if (it_in == (qRefName->astvnQualifiedName.end() - 1))
	{
		semanticErr_MissingReferenceToDomOrVar(item1);
		return false;
	}

	item2 = dynamic_cast<ASTNamedReferenceNode*>(*(it_in + 1));
	search_res = SEARCH_IN_MODEL(search, item2->sName);
	if (search_res == NULL)
	{
		semanticErr_UndeclaredEntity(item2->tName);
		return false;
	}

	//Check if entity is a domain
	FEMOneDModelDomainData* dom = NULL;
	bool bGotVariableFromDomain = false;
	bool bGotVariable = false;

	// Convert to domain and search in domains
	dom = static_cast<FEMOneDModelDomainData*>(search_res);

	if (search_res->isFEMoneDDomain)
	{
		if (item2->bIsArray)
		{
			//domains can have string indices
			for (std::vector<ASTNode*>::const_iterator it = item2->astvnArrayIndices.begin(); it != item2->astvnArrayIndices.end(); ++it)
			{
				// String indices
				if ((*it)->bIsStringIndex)
				{
					std::string stringIndex = static_cast<ASTstringIndex*>(*it)->sString;
					auto f = dom->domainSize.find(stringIndex);
					if (f != dom->domainSize.end())
					{
						if (bInEquation)
						{
							if (item2->iNumDerivs == 0)
								qRefName->equation_data = std::to_string(dom->domainSize[stringIndex]);
							else
								qRefName->equation_data = std::to_string(dom->domainNuemannSize[stringIndex]); // Reference to Neumann boundary
							return true;
						}
						else
						{
							if (item2->iNumDerivs == 0)
								qRefName->value = dom->domainSize[stringIndex] + 1; // Plus one for using 1 based indexing
							else
								qRefName->value = dom->domainNuemannSize[stringIndex] + 1; // Reference to Neumann boundary
							return true;
						}
					}
					else
					{	
						semanticErr_InvalidStringIndex(*it);
						return false;
					}
				}
				else
				{
					semanticErr_IndexInDomainOnlyString(item2);
					return false;
				}
			}
		}

		// Check whether domain is the last entry and return reference
		if ((it_in + 1) == (qRefName->astvnQualifiedName.end() - 1))
		{
			// Return the reference to the domain
			qRefName->meType = AST_DOMAIN_NODE;
			qRefName->me = search_res;
			return true;
		}
		else
		{
			//Find Variable specified after domain
			item3 = static_cast<ASTNamedReferenceNode*>(*(it_in + 2));
			search_res = SEARCH_IN_MODEL(search, item3->sName);
			if (search_res != NULL)
			{
				bGotVariableFromDomain = true;
			}
			else
			{
				semanticErr_UndeclaredEntity(item3->tName);
				return false;
			}
		}
	}
	else if (search_res->isFEMOneDVariable || search_res->isFEMOneDdVariable)
	{
		item3 = item2;
		bGotVariable = true;
	}
	else 
	{
		assert(false);
	}

	//check if variable exists
	if (bGotVariable)
	{
		search_res = SEARCH_IN_MODEL(search, item3->sName);
		if (search_res == NULL)
		{
			semanticErr_UndeclaredEntity(item3->tName);
			return false;
		}
	}

	// If here that means we need to process variable

	// If Variable has array reference treat like normal variable(also process string index)
	if (item3->bIsArray)
	{
		if (bGotVariable && !bInEquation)
		{
			semanticErr_DirectVarWithoutDomain(item3);
			return false;
		}

		ASTNode* eval = NULL;
		int ival = 0;
		int fullsum = 0;
		int c = 0;
		bool success = true;
		for (std::vector<ASTNode*>::const_iterator it = item3->astvnArrayIndices.begin(); it != item3->astvnArrayIndices.end(); ++it)
		{
			// String indices
			if ((*it)->bIsStringIndex)
			{					
				std::string stringIndex = static_cast<ASTstringIndex*>(*it)->sString;
				auto f = dom->domainSize.find(stringIndex);
				if (f != dom->domainSize.end())
				{
					if (bInEquation)
					{
						if (item3->iNumDerivs == 0)
							qRefName->equation_data = static_cast<equation*>(Eq)->getSubstitutedEntityNameVector(search_res, dom->domainSize[stringIndex]);
						else
							qRefName->equation_data = static_cast<equation*>(Eq)->getSubstitutedEntityNameVector(search_res, dom->domainNuemannSize[stringIndex]);
						return true;
					}
					else
					{
						if (item3->iNumDerivs == 0)
                            qRefName->value = search_res->vValue[dom->domainSize[stringIndex]];
						else
                            qRefName->value = search_res->vValue[dom->domainNuemannSize[stringIndex]]; // Reference to Neumann boundary
						return true;
					}
				}
				else
				{
					semanticErr_InvalidStringIndex(*it);
					return false;
				}
			}
			else
			{
				// Numerical indices
				temp_bInequation = bInEquation;
				bInEquation = false;
				eval = VISIT_NODE(*it);
				bInEquation = temp_bInequation;
				assert(eval);
				
				ival = (int)eval->value;
                if ((ival > 0) && (ival <= search_res->getDimsAt(c)))
				{
                    fullsum += (ival - 1)*search_res->getDimsCAt(c);
				}
				else
				{
					semanticErr_IndexOutOfBounds(item3->tName);
					return false;
				}
			}
			c++;
		}

		if(bInEquation)
			qRefName->equation_data = static_cast<equation*>(Eq)->getSubstitutedEntityNameVector(search_res, fullsum);
		else
            qRefName->value = search_res->vValue[fullsum];
		return true;
	}
	else
	{
		//if Array dimensions not specified this should be treated like an FEM variable for Atharv's implementation(Makes sense only on Equation processor)
		qRefName->me = search_res;
		qRefName->iNumDerivs = item3->iNumDerivs;
		qRefName->me->currentDomain = dom;
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// utility function to guess FEM one D node values
bool interpreter::assignFEMoneDObject(FEMOneDLineObject* FEMobj, ASTQualifiedNamedReferenceNode* qRefName, std::vector<ASTNode*>::const_iterator it_in,
	double &val, int &op)
{
	// create the line object if not
	if (!FEMobj->init)
	{
		if (!createFEMoneDline(FEMobj))
			return false;
	}

	// Now evaluate
	ASTNamedReferenceNode* item1 = NULL;
	ASTNamedReferenceNode* item2 = NULL;
	ASTNamedReferenceNode* item3 = NULL;
    modelEntity* FEMvarRef = NULL;
    model* search = NULL;
    modelEntity* search_res = NULL;
    object* next = NULL;

	item1 = dynamic_cast<ASTNamedReferenceNode*>(*it_in);
	// Only GUESS section allowed for assignment type operation 
	if (IS.section_type != SCOPE_GUESS)
	{
		semanticErr_OnlyGuessForFEMVars(item1);
		return false;
	}

	// First check if line object is a scalar or vector
	if (FEMobj->getDimType() == SY_SCALAR)
	{
		search = FEMobj->SModelObject;
	}
	else
	{
		// Navigate to correct Line object
		if (item1->bIsArray)
		{
			//Check if same number of dimensions
			if (item1->astvnArrayIndices.size() != search_res->getNumDims())
			{
				semanticErr_DimensionMismatch(item1->tName);
				return false;
			}

			ASTNode* eval = NULL;
			int ival = 0;
			int fullsum = 0;
			int c = 0;
			bool success = true;
			for (std::vector<ASTNode*>::const_iterator it = item1->astvnArrayIndices.begin(); it != item1->astvnArrayIndices.end(); ++it)
			{
				eval = VISIT_NODE(*it);
				assert(eval);

				ival = (int)eval->value;
                if ((ival > 0) && (ival <= search_res->getDimsAt(c)))
				{
                    fullsum += (ival - 1)*search_res->getDimsCAt(c);
				}
				else
				{
					semanticErr_IndexOutOfBounds(item1->tName);
					return false;
				}
				c++;
			}

            search = &dynamic_cast<object*>(search_res)->VModelObject[fullsum];
			assert(search);
		}
		else
		{
			semanticErr_EntityIsNotAScalar(item1->tName);
			return false;
		}
	}

	// Now get a domain or an FEM variable
	if (it_in == (qRefName->astvnQualifiedName.end() - 1))
	{
		semanticErr_MissingReferenceToDomOrVar(item1);
		return false;
	}
	item2 = dynamic_cast<ASTNamedReferenceNode*>(*(it_in + 1));
	search_res = SEARCH_IN_MODEL(search, item2->sName);
	if (search_res == NULL)
	{
		semanticErr_UndeclaredEntity(item2->tName);
		return false;
	}

	//Check if entity is a domain
	FEMOneDModelDomainData* dom = NULL;
	bool bGotVariableFromDomain = false;
	bool bGotVariable = false;

	// Convert to domain and search in domains
	dom = static_cast<FEMOneDModelDomainData*>(search_res);

	if (search_res->isFEMoneDDomain)
	{
		if (item2->bIsArray)
		{
			//domains can have string indices
			for (std::vector<ASTNode*>::const_iterator it = item2->astvnArrayIndices.begin(); it != item2->astvnArrayIndices.end(); ++it)
			{
				// String indices
				if ((*it)->bIsStringIndex)
				{
					std::string stringIndex = static_cast<ASTstringIndex*>(*it)->sString;
					auto f = dom->domainSize.find(stringIndex);
					if (f != dom->domainSize.end())
					{
						if (item2->iNumDerivs == 0)
							qRefName->value = dom->domainSize[stringIndex];
						else
							qRefName->value = dom->domainNuemannSize[stringIndex]; // Reference to Neumann boundary
						return true;
					}
					else
					{
						semanticErr_InvalidStringIndex(*it);
						return false;
					}
				}
				else
				{
					semanticErr_IndexInDomainOnlyString(item2);
					return false;
				}
			}
		}

		// Check whether domain is the last entry and return reference
		if ((it_in + 1) == (qRefName->astvnQualifiedName.end() - 1))
		{
			// Return the reference to the domain
			qRefName->meType = AST_DOMAIN_NODE;
			qRefName->me = search_res;
			return true;
		}
		else
		{
			//Find Variable specified after domain
			item3 = static_cast<ASTNamedReferenceNode*>(*(it_in + 2));
			search_res = SEARCH_IN_MODEL(search, item3->sName);
			if (search_res != NULL)
			{
				bGotVariableFromDomain = true;
			}
			else
			{
				semanticErr_UndeclaredEntity(item3->tName);
				return false;
			}
		}
	}
	else if (search_res->isFEMOneDVariable || search_res->isFEMOneDdVariable)
	{
		item3 = item2;
		bGotVariable = true;
	}
	else 
	{
		assert(false);
	}

	//check if variable exists
	if (bGotVariable)
	{
		search_res = SEARCH_IN_MODEL(search, item3->sName);
		if (search_res == NULL)
		{
			semanticErr_UndeclaredEntity(item3->tName);
			return false;
		}
	}

	// If Variable has array reference treat like normal variable(also process string index)
	if (item3->bIsArray)
	{
		if (bGotVariable)
		{
			//This is allowed for assignment
			//return false;
		}

		ASTNode* eval = NULL;
		int ival = 0;
		int fullsum = 0;
		int c = 0;
		bool success = true;
		for (std::vector<ASTNode*>::const_iterator it = item3->astvnArrayIndices.begin(); it != item3->astvnArrayIndices.end(); ++it)
		{
			// String indices
			if ((*it)->bIsStringIndex)
			{
				std::string stringIndex = static_cast<ASTstringIndex*>(*it)->sString;
				auto f = dom->domainSize.find(stringIndex);
				if (f != dom->domainSize.end())
				{
					if (item3->iNumDerivs == 0)
                        search_res->vValue[dom->domainSize[stringIndex]] = val;
					else
                        search_res->vValue[dom->domainNuemannSize[stringIndex]] = val; // Reference to Neumann boundary
					return true;
				}
				else
				{
					semanticErr_InvalidStringIndex(*it);
					return false;
				}
			}
			else
			{
				// Numerical indices
				eval = VISIT_NODE(*it);
				assert(eval);

				ival = (int)eval->value;
                if ((ival > 0) && (ival <= search_res->getDimsAt(c)))
				{
                    fullsum += (ival - 1)*search_res->getDimsCAt(c);
				}
				else
				{
					semanticErr_IndexOutOfBounds(item3->tName);
					return false;
				}
			}
			c++;
		}

        search_res->vValue[fullsum] = val;
		return true;
	}
	else
	{
		//if Array dimensions not specified, set guesses  to selection of nodes
		if (bGotVariable)
		{
			//Set all the same guess to all nodes
            for (int i = 0; i < search_res->getMaxDims() - 2; i++)
                search_res->vValue[i] = val;
		}
		else if (bGotVariableFromDomain)
		{
			//Set guess only in bounding domain
			for (int i = dom->domainSize["left"]; i <= dom->domainSize["right"]; i++)
                search_res->vValue[i] = val;
		}
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// utility function to create the FEM oneD line object once encountered
bool interpreter::createFEMoneDline(object* FEMobj)
{
	// First convert to line object
	FEMOneDLineObject* FEMob = static_cast<FEMOneDLineObject*>(FEMobj);

	// Loop through all the domains to evaluate length and number of elements
	int c = 0;
	int si = 1; int ei = -1;
	double comlen = 0.0; double startLen = 0.0; double endLen = 0.0;
	for (auto dom = FEMob->idomains.begin(); dom != FEMob->idomains.end(); dom++)
	{
		ASTNode* eval = NULL;

		// get Length
		eval = VISIT_NODE((*dom)->astnLength);
		assert(eval);

		(*dom)->dLength = eval->value;

		startLen = comlen;
		comlen = comlen + (*dom)->dLength;
		endLen = comlen;
		(*dom)->startLength = startLen;
		(*dom)->endLength = endLen;

		// get Numel
		eval = NULL;
		eval = VISIT_NODE((*dom)->astnNumel);
		assert(eval);

		(*dom)->iNumel = eval->value;

		//Set boundary nodes
		if (FEMob->calculateBoundaryNodeNumbers(c, si, ei))
		{
			// Regular end nodes
			(*dom)->domainSize["left"] = si;
			(*dom)->domainSize["right"] = ei;

			// Neumann Boundary nodes
			(*dom)->domainNuemannSize["left"] = ei + 1;
			(*dom)->domainNuemannSize["right"] = ei + 2;
		}
		else
		{
            semanticErr_CalculatingBoundaryValues(*FEMobj->getTok());
			return false;
		}

		c++;
	}

	// Check if array of lines
	if (FEMob->getDimType() == SY_SCALAR)
	{
        FEMob->SModelObject = new model;
	}
	else
	{
		// Allocate array of lines
		ASTNode* eval = NULL;
		int ret;
        ASTmodel_entity_declNode* node = static_cast<ASTmodel_entity_declNode*>(FEMob->getBuildNode());
		LOOP_OVER_NODES(node, astvnArrayIndices)
		{
			eval = VISIT_NODE(*it_astvnArrayIndices);

			ret = FEMob->setDims(eval->value);
			if (ret != SY_SUCCESS)
			{
                semanticErr_IndexOutOfBounds(*FEMobj->getTok());
				return false;
			}
		}

		assert(FEMob->allocateArray() == SY_SUCCESS);
	}

	c = 0;
start: 	

	//temp
	std::string varname("");
	std::string domname("");

	// Add the domains to the line too
	for (auto dom = FEMob->domains.begin(); dom != FEMob->domains.end(); dom++)
	{
		
		FEMOneDModelDomainData* dom_actual = dom->second; // Just pass the reference to the one found already in the object(just add the extra stuff)
		dom_actual->setNType(REAL);
		dom_actual->setName(dom->first);
		dom_actual->setType(pDOMAIN);
		dom_actual->setSType("DOMAIN");
		dom_actual->isFEMoneDDomain = true;
		dom_actual->setDimType(SY_SCALAR);
		dom_actual->iBasis = FEMob->ibasisType;

		domname = dom->first;

		// Add to line
		if (FEMob->getDimType() == SY_SCALAR)
		{
			if (!FEMob->SModelObject->insertModelEntity(domname, dom_actual))
			{
				delete dom_actual;
				assert(false);
				return false;
			}
		}
		else
		{
			// For vectors do the above operation again and again
			if (!FEMob->VModelObject[c].insertModelEntity(domname, dom_actual))
			{
				delete dom_actual;
				assert(false);
				return false;
			}
		}
	}

	// Loop through all the variables that need to be added
	for (auto var = FEMob->PDEvars.begin(); var != FEMob->PDEvars.end(); var++)
	{
		// Now Calculate the size of the Variable
		int total_numel_with_nuemann = 0;
		for (auto dom = FEMob->idomains.begin(); dom != FEMob->idomains.end(); dom++)
		{
			total_numel_with_nuemann += FEMob->getNumNodesFromElementType((*dom)->iNumel) + 2 /*2 because additional variables for Nuemann boundary condition */;
		}

		varname = var->first;

		// Create and add the variable
		// Regular
        variable* v = new variable;
		v->setNType(REAL);
		v->setName(var->first);
		v->setType(VARIABLE);
		v->setSType("VARIABLE");
		v->isFEMOneDVariable = true;

		// Time dependent
        variable* dv = new variable;
		dv->setNType(REAL);
		dv->setName("$" + var->first);
		dv->setType(VARIABLE);
		dv->setSType("VARIABLE");
		dv->setIsdt();
		dv->isFEMOneDdVariable = true;

		// Give each others reference to each other
        dv->setOther(v);
        v->setOther(dv);

		// PDE variables are always vectors
		v->setDimType(SY_VECTOR);
		dv->setDimType(SY_VECTOR);

		// Set size and allocate
		v->setDims(total_numel_with_nuemann);
		dv->setDims(total_numel_with_nuemann);
		assert(v->allocateArray(0.0) == SY_SUCCESS);
		assert(dv->allocateArray(0.0) == SY_SUCCESS);

		// Set vector flags
        v->setVectorIsInitialized(true);
        dv->setVectorIsInitialized(true);

		/* Fix all dt components of Neumann nodes(always fixed, no use)
		for (auto alldoms = FEMob->domains.begin(); alldoms != FEMob->domains.end(); alldoms++)
		{
			dv->VFixValueToggle[alldoms->second->domainSize["left"] + 1] = SY_FIX;
			dv->VFixValueToggle[alldoms->second->domainSize["left"] + 2] = SY_FIX;
		}
		*/

		// Reset all removes[ if domains specified for the variable]
		if (FEMob->PDEvars[var->first].dNames.size() > 0)
		{
			for (auto alldoms = FEMob->domains.begin(); alldoms != FEMob->domains.end(); alldoms++)
			{
				alldoms->second->mark = false;
			}

			// Mark domains to remove
			for (auto presentInDomain = FEMob->PDEvars[var->first].dNames.begin(); presentInDomain != FEMob->PDEvars[var->first].dNames.end(); presentInDomain++)
			{
				FEMob->domains[*presentInDomain]->mark = true;
			}

			// Fix all Variables
			for (auto alldoms = FEMob->domains.begin(); alldoms != FEMob->domains.end(); alldoms++)
			{
				if (!alldoms->second->mark)
				{
					for (int i = alldoms->second->domainSize["left"]; i <= alldoms->second->domainSize["right"]; i++)
					{
						v->VFixValueToggle[i] = SY_FIX;
						dv->VFixValueToggle[i] = SY_FIX;
					}
				}
			}
		}

		// Add to line
		if (FEMob->getDimType() == SY_SCALAR)
		{
			std::string dollar_varname = "$" + varname;
			if (!(FEMob->SModelObject->insertModelEntity(varname, v) && FEMob->SModelObject->insertModelEntity(dollar_varname, dv)))
			{
				delete v;
				delete dv;
				assert(false);
				return false;
			}	
		}
		else
		{
			// For vectors do the above operation again and again
			std::string dollar_varname = "$" + varname;
			if (!(FEMob->VModelObject[c].insertModelEntity(varname, v) && FEMob->VModelObject[c].insertModelEntity(dollar_varname, dv)))
			{
				delete v;
				delete dv;
				assert(false);
				return false;
			}
		}
	}

	// For vectors
    if ((FEMob->getDimType() == SY_VECTOR) && (c < FEMob->getMaxDims()))
	{
		c++;
		goto start;
	}

	// Set the initialization flag for the line
	FEMob->init = true;
	return true;
}
