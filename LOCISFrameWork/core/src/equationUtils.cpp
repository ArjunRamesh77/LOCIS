#include "equation.h"
#include <iomanip>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Visits the node
ASTNode* equation::VISIT_NODE(ASTNode* node)
{
	if (node)
		return node->visit(this);
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Triggers the equation mode for interpreter 
void equation::setEquationModeForInterpreter(bool state)
{
	Ip->bInEquation = state;
	Ip->Eq = this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set Equation Vector
void equation::setEquations(std::vector<EquationUnit>* Equations_arg, std::vector<EquationUnit>* InitEquations_arg)
{
	Equations = Equations_arg;
	InitEquations = InitEquations_arg;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sets the simulation type (Steady_state or Dynamic)
void equation::setSimulationType(int type)
{
	iSimulationType = type;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sets the initialization mode
void equation::setInitializationMode(bool val, stateVars* initHelpers_arg)
{
	bInitialization = val;
	initHelpers = initHelpers_arg;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Given a state of variables (v and dv) retrieve new equation set
int equation::getEquationsRecursive(model* mod)
{
	// Loop through all Equations
	Ip->IS.scp = mod;
	IS.scp = mod;

	// set t = t0
	if (bInitialization)
	{
		modelEntity* t0Val = NULL;
		std::string sym_t = "_t";
		t0Val = mod->getModelEntity(NULL, sym_t); //works only for regular models
		if (t0Val)
			t0Val->sValue = t0_init_val;
	}

	// Equation section
	for (auto eqn = mod->getAllEquationNodes()->begin(); eqn != mod->getAllEquationNodes()->end(); ++eqn)
	{
		VISIT_NODE(*eqn);
	}

	// Init Section
	for (auto eqn = mod->getAllInitEquationNodes()->begin(); eqn != mod->getAllInitEquationNodes()->end(); ++eqn)
	{
		VISIT_NODE(*eqn);
	}

	// Loop through all objects in this object (Recursive call)
	for (auto obj = mod->getAllModelEntities()->begin(); obj != mod->getAllModelEntities()->end(); ++obj)
	{
		if (obj->second->geType() == KW_MODEL)
		{
			if (obj->second->getDimType() == SY_SCALAR)
			{
				getEquationsRecursive(static_cast<object*>(obj->second)->sModelObject);
			}
			else
			{
				for (int i = 0; i < obj->second->getMaxDims(); i++)
				{
					getEquationsRecursive(&static_cast<object*>(obj->second)->vModelObject[i]);
				}
			}
		}
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper function to Make Variable substitutions(Scalar)
std::string equation::getSubstitutedEntityNameScalar(modelEntity* me)
{
	// build the value based on index
	if (me->geType() == KW_VARIABLE)
	{
		variable* mev = static_cast<variable*>(me);

		if (me->checkIsdt())
		{
			if (iSimulationType == EQ_STEADY_STATE)
			{
				return "0";
			}
			else
			{
				if (mev->sFixValueToggle == SY_FREE)
				{
					if (bInitialization)
					{
						if (initHelpers->initdVindex[mev->nSValue] == -1)
						{
							initHelpers->initIncr++;
							initHelpers->initdVindex[mev->nSValue] = initHelpers->initIncr;
							return "V[" + std::to_string(initHelpers->initIncr) + "]";
						}
						else
						{
							return "V[" + std::to_string(initHelpers->initdVindex[mev->nSValue]) + "]";
						}
					}
					else
					{
						if (occurdV.find(mev->nSValue) == occurdV.end())
						{
							numDiffVars++;
							occurdV[mev->nSValue] = 1;
						}
						return "DV[" + std::to_string(mev->nSValue) + "]";
					}
				}
				else
				{
					std::stringstream ss;
					ss << std::scientific << std::setprecision(10) << mev->sValue;
					return ss.str();
					//return std::to_string(mev->SValue);
				}
			}
		}
		else
		{
			if (mev->sFixValueToggle == SY_FREE)
			{
				if (bInitialization)
				{
					if (initHelpers->initVindex[mev->nSValue] == -1)
					{
						initHelpers->initIncr++;
						initHelpers->initVindex[mev->nSValue] = initHelpers->initIncr;
						return "V[" + std::to_string(initHelpers->initIncr) + "]";
					}
					else
					{
						return "V[" + std::to_string(initHelpers->initVindex[mev->nSValue]) + "]";
					}
				}
				else
				{
					if (occurV.find(mev->nSValue) == occurV.end())
					{
						occurV[mev->nSValue] = 1;
						numAlgVars++;
					}
						
					return "V[" + std::to_string(mev->nSValue) + "]";
				}
			}
			else
			{
				std::stringstream ss;
				ss << std::scientific << std::setprecision(10) << mev->sValue;
				return ss.str();
				//return std::to_string(mev->SValue);
			}
		}
	}
	else if (me->geType() == KW_PARAMETER || me->geType() == KW_ITER)
	{
		std::stringstream ss;
		ss << std::scientific << std::setprecision(10) << me->sValue;
		return ss.str();
		//return std::to_string(me->SValue);
	}

	assert(false);
	return "<f!@k>";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper function to Make Variable substitutions(vector)
std::string equation::getSubstitutedEntityNameVector(modelEntity* me, int &index)
{
	// build the value based on index
	if (me->geType() == KW_VARIABLE)
	{
		variable* mev = static_cast<variable*>(me);
		if (me->checkIsdt())
		{
			if (iSimulationType == EQ_STEADY_STATE)
			{
				return "0";
			}
			else
			{
				if (mev->vFixValueToggle[index] == SY_FREE)
				{
					if (bInitialization)
					{
						if (initHelpers->initdVindex[me->nVValue[index]] == -1)
						{
							initHelpers->initIncr++;
							initHelpers->initdVindex[me->nVValue[index]] = initHelpers->initIncr;
							return "V[" + std::to_string(initHelpers->initIncr) + "]";
						}
						else
						{
							return "V[" + std::to_string(initHelpers->initdVindex[me->nVValue[index]]) + "]";
						}
					}
					else
					{
						if (occurdV.find(me->nVValue[index]) == occurdV.end())
						{
							numDiffVars++;
							occurdV[me->nVValue[index]] = 1;
						}
						return "DV[" + std::to_string(me->nVValue[index]) + "]";
					}
				}
				else
				{
					std::stringstream ss;
					ss << std::scientific << std::setprecision(10) << mev->vValue[index];
					return ss.str();
					//return std::to_string(mev->Vvalue[index]);
				}
			}
		}
		else
		{
			if (mev->vFixValueToggle[index] == SY_FREE)
			{
				if (bInitialization)
				{
					if (initHelpers->initVindex[me->nVValue[index]] == -1)
					{
						initHelpers->initIncr++;
						initHelpers->initVindex[me->nVValue[index]] = initHelpers->initIncr;
						return "V[" + std::to_string(initHelpers->initIncr) + "]";
					}
					else
					{
						return "V[" + std::to_string(initHelpers->initVindex[me->nVValue[index]]) + "]";
					}
				}
				else
				{
					if (occurV.find(me->nVValue[index]) == occurV.end())
					{
						numAlgVars++;
						occurV[me->nVValue[index]] = 1;
					}
					return "V[" + std::to_string(me->nVValue[index]) + "]";
				}
			}
			else
			{
				std::stringstream ss;
				ss << std::scientific << std::setprecision(10) << mev->vValue[index];
				return ss.str();
				//return std::to_string(mev->Vvalue[index]);
			}
		}
	}
	else if (me->geType() == KW_PARAMETER)
	{
		std::stringstream ss;
		ss << std::scientific << std::setprecision(10) << me->vValue[index];
		return ss.str();
		//return std::to_string(me->Vvalue[index]);
	}

	assert(false);
	return "<f!@k>";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maps from Internal->State
stateVars* equation::MapToStateVector()
{
	stateVars* PortableVars = NULL;
	PortableVars = new stateVars(Ip->AllVars.size());

	assert(Ip->AlldVars.size() <= Ip->AllVars.size());

	//Populate Vars
	int c = 0;
	for (auto it = Ip->AllVars.begin(); it != Ip->AllVars.end(); ++it)
	{
		PortableVars->Vars[c] = *it->val;
		c++;
	}

	c = 0;
	for (auto it = Ip->AlldVars.begin(); it != Ip->AlldVars.end(); ++it)
	{
		PortableVars->dVars[c] = *it->val;
		c++;
	}

	return PortableVars;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maps from State->Internal
bool equation::MapFromStateVector(stateVars* Pv)
{
	if (Pv)
	{
		if (Pv->NumVars == Ip->AllVars.size())
		{
			auto iv = Ip->AllVars.begin();

			//do Vars
			for (int i = 0; i < Pv->NumVars; ++i)
			{
				*iv->val = Pv->Vars[i];
				iv++;
			}

			//do dVars
			iv = Ip->AlldVars.begin();
			for (int i = 0; i < Pv->NumVars; ++i)
			{
				*iv->val = Pv->dVars[i];
				iv++;
			}
		}
		else
		{
			return false;
		}
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get New Equations from state vector (Forces Generation of Equations regardless)
bool equation::getNewEquationsFromState(stateVars* Pv)
{
	if (Equations)
	{
		//Reset Variable count
		numAlgVars = numDiffVars = 0;

		Equations->clear();
		if (Pv)
			MapFromStateVector(Pv);
		getEquationsRecursive(MasterModel);
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get total number of occurring variables
void equation::getOccuringVariablesNumbers(int &Alg, int &Diff)
{
	Alg = numAlgVars;
	Diff = numDiffVars;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sets the initialization value for dynamic systems
void equation::setT0Value(double val)
{
	t0_init_val = val;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if system is initializing
bool equation::isInitialzation()
{
	return bInitialization;
}


