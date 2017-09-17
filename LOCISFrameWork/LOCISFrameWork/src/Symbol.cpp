#include "Symbol.h"
#include "lexer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ModelEntity
int ModelEntity::allocateArray(const double &val) { return 0; };

// Set >>>>>>>>>>>>
ModelEntity::ModelEntity()
{
	//Options
	bHasDesc = false;
	bHasUnit = false;
	bHasBounds[0] = bHasBounds[1] = bHasBounds[2] = bHasBounds[3] = false;

	//FEM one D flags
	isFEMoneDDomain = false;
	isFEMOneDdVariable = false;
	isFEMOneDLine = false;
	isFEMOneDVariable = false;
	isFEMtrialFunction = false;
	isTval = false;
	isFEMxval = false;
	currentDomain = NULL; // Stores the domain pertaining to the variable referenced

	//Assign all defaults
	other = NULL;
	nt = NULL;
	isdt = false;
	nSValue = 0; 
	nVvalue = NULL;
	iter = 0;
	vset = true;
	build_node = NULL;
	SValue = SY_DEFAULT_VAL;
	Vvalue = NULL;
	Name.assign("");
	Desc.assign("");
	SType.assign("");
	Type = SY_VAL_NOT_SET;
	NumType = SY_VAL_NOT_SET;
	DimType = SY_VAL_NOT_SET;
	Maxdim = SY_VAL_NOT_SET;
}

ModelEntity::~ModelEntity()
{
	if (Vvalue)
		delete Vvalue;
}

void ModelEntity::setName(const std::string &name_arg)
{
	Name.assign(name_arg);
}

void ModelEntity::setDesc(const std::string &desc_arg)
{
	Desc.assign(desc_arg);
}

void ModelEntity::setUnit(const std::string &unit_arg)
{
	Unit.assign(unit_arg);
}

void ModelEntity::setDimType(const int DimType_arg)
{
	DimType = DimType_arg;
}

void ModelEntity::setType(const int Type_arg)
{
	Type = Type_arg;
}

void ModelEntity::setIsdt()
{
	isdt = true;
}

void ModelEntity::setNType(const int type)
{
	NumType = type;
}

void ModelEntity::setSType(const std::string &Type)
{
	SType = Type;
}

int ModelEntity::setDims(const int dimval_arg)
{
	// Make sure its a vector
	if (DimType == SY_VECTOR)
	{
		// size specification of the dimension must be greater than 1
		if (dimval_arg > 1)
		{
			Dims.push_back(dimval_arg);
		}
		else
		{
			return SY_INDEX_GT_1;
		}
	}

	// Get Multipliers
	DimsC.clear();
	for (int i = 0; i < Dims.size(); i++)
	{
		int mult = 1;
		for (auto it = Dims.rbegin() + i; it != (Dims.rend() - 1); it++)
		{
			mult *= *it;
		}
		DimsC.push_back(mult);
	}

	return SY_SUCCESS;
}

void ModelEntity::Reset()
{
	//Assign all defaults
	Name.assign("");
	Desc.assign("");
	Type = SY_VAL_NOT_SET;
	DimType = SY_VAL_NOT_SET;
	Maxdim = SY_VAL_NOT_SET;
}

// Get >>>>>>>>>>>>
std::string ModelEntity::getName() const
{
	return Name;
}

std::string ModelEntity::getDesc() const
{
	return Desc;
}

std::string ModelEntity::getUnit() const
{
	return Unit;
}

int ModelEntity::getDimType() const
{
	return DimType;
}

bool ModelEntity::checkIsdt()
{
	return isdt;
}

int ModelEntity::geType() const
{
	return Type;
}

std::string ModelEntity::getSType() const
{
	return SType;
}

int ModelEntity::getNumDims() const
{
	return Dims.size();
}

int ModelEntity::getMaxDims() const
{
	return Maxdim;
}

std::vector<int>* ModelEntity::getDimAt()
{
	if (Dims.size() > 0)
		return &Dims;
	else
		return NULL;
}

ModelEntity* ModelEntity::ReturnObject()
{
	return this;
}

int ModelEntity::setAllto(const double &val)
{
	if (Vvalue)
	{
		for (int i = 0; i < Maxdim; i++)
		{
			Vvalue[i] = val;
		}
		return SY_SUCCESS;
	}
	else
	{
		return SY_NOT_INITIALIZED
	}

	return SY_FAIL;
}

std::string ModelEntity::getGetMultiDimsFromSingle(int &index) const
{
	std::string stempval("");
	int tempval = index;
	if (DimType == SY_VECTOR)
	{
		
		for (std::vector<int>::const_reverse_iterator it = Dims.rbegin(); it != Dims.rend(); it++)
		{	
			if (it == Dims.rend() - 1)
				stempval += std::to_string((tempval%*it) + 1);
			else
				stempval += std::to_string((tempval%*it) + 1) + ",";
			tempval = tempval / (*it);
		}
		
		return "[" + stempval + "]";
	}

	return "";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Parameter
Parameter::Parameter() :
	ModelEntity()
{
	Vvalue = NULL;
}

Parameter::~Parameter()
{
	
}

int Parameter::allocateArray(const double &val)
{
	// Valid only for vectors
	if (DimType == SY_VECTOR)
	{
		// A size has been specified
		if (Dims.size() > 0)
		{
			// It has not been assigned before
			if (Vvalue == NULL)
			{
				int fullprod = 1;
				for (std::vector<int>::iterator it = Dims.begin(); it != Dims.end(); it++)
					fullprod *= *it;

				Maxdim = fullprod;

				nVvalue = new int[fullprod];
				Vvalue = new double[fullprod];
				if (Vvalue == NULL)
				{
					return SY_VECTOR_FAILED_TO_ALLOCATE;
				}

				for (int i = 0; i < fullprod; i++)
				{
					nVvalue[i] = 0;
					Vvalue[i] = val;
				}

				return SY_SUCCESS;
			}
			else
			{
				return SY_VECTOR_ALREADY_ALLOCATED;
			}
		}
		else
		{
			return SY_DIMS_ZERO;
		}
	}
	return SY_FAIL;
}

void Parameter::Reset()
{
	//Assign all defaults
	ModelEntity::Reset();
	if (Vvalue != NULL)
	{
		delete Vvalue;
		Vvalue = NULL;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Variable
Variable::Variable() :
	ModelEntity()
{
	//Initialize
	Vvalue = NULL;
	VLowerType = NULL;
	VUpperType = NULL;
	VUpper = NULL;
	VLower = NULL;
	VFixValueToggle = NULL;

	// Scalar
	SUpperType = SY_VAL_NOT_SETC;
	SLowerType = SY_VAL_NOT_SETC;
	SUpper = SY_LARGE_POSITIVE;
	SLower = SY_LARGE_NEGATIVE;
	SFixValueToggle = SY_FREE;
}

Variable::~Variable()
{
	if (VLowerType != NULL)
		delete VLowerType;

	if (VUpperType != NULL)
		delete VUpperType;

	if (VUpper != NULL)
		delete VUpper;

	if (VLower != NULL)
		delete VLower;

	if (VFixValueToggle != NULL)
		delete VFixValueToggle;

}

int Variable::allocateArray(const double &val)
{

	// Valid only for vectors
	if (DimType == SY_VECTOR)
	{
		// A size has been specified
		if (Dims.size() > 0)
		{
			// It has not been assigned before
			if (Vvalue == NULL && VUpper == NULL && VLower == NULL && VUpperType == NULL && VLowerType == NULL && VFixValueToggle == NULL)
			{
				int fullprod = 1;
				for (std::vector<int>::iterator it = Dims.begin(); it != Dims.end(); it++)
					fullprod *= *it;

				Maxdim = fullprod;

				// Allocate all arrays for variable
				nVvalue = new int[fullprod];
				Vvalue = new double[fullprod];
				VUpper = new double[fullprod];
				VLower = new double[fullprod];
				VUpperType = new char[fullprod];
				VLowerType = new char[fullprod];
				VFixValueToggle = new char[fullprod];

				if (nVvalue == NULL)
					return SY_VECTOR_FAILED_TO_ALLOCATE;

				if (Vvalue == NULL)
					return SY_VECTOR_FAILED_TO_ALLOCATE;

				if (VUpper == NULL)
					return SY_VECTOR_FAILED_TO_ALLOCATE;

				if (VLower == NULL)
					return SY_VECTOR_FAILED_TO_ALLOCATE;

				if (VUpperType == NULL)
					return SY_VECTOR_FAILED_TO_ALLOCATE;

				if (VLowerType == NULL)
					return SY_VECTOR_FAILED_TO_ALLOCATE;

				if (VFixValueToggle == NULL)
					return SY_VECTOR_FAILED_TO_ALLOCATE;

				for (int i = 0; i < fullprod; i++)
				{
					nVvalue[i] = 0;
					Vvalue[i] = val;
					VUpper[i] = SY_LARGE_POSITIVE;
					VLower[i] = SY_LARGE_NEGATIVE;
					VUpperType[i] = SY_LESS_THAN_OR_EQUAL;
					VLowerType[i] = SY_GREATER_THAN_OR_EQUAL;
					VFixValueToggle[i] = SY_FREE;
				}

				return SY_SUCCESS;
			}
			else
			{
				return SY_VECTOR_ALREADY_ALLOCATED;
			}
		}
		else
		{
			return SY_DIMS_ZERO;
		}
	}
	return SY_FAIL;
}

int Variable::setAlltoB(const int &type, const char ctype, const double &dtype)
{
	switch (type)
	{
	case SY_VARIABLE_UBT:
		if (VUpperType && VUpper)
		{
			for (int i = 0; i < Maxdim; i++)
			{
				VUpperType[i] = ctype;
				VUpper[i] = dtype;
			}
			return SY_SUCCESS;
		}
		else
		{
			return SY_NOT_INITIALIZED
		}
		break;

	case SY_VARIABLE_LBT:
		if (VLowerType && VLower)
		{
			for (int i = 0; i < Maxdim; i++)
			{
				VLowerType[i] = ctype;
				VLower[i] = dtype;
			}
			return SY_SUCCESS;
		}
		else
		{
			return SY_NOT_INITIALIZED
		}
		break;

	case SY_VARIABLE_FT:
		if (VFixValueToggle)
		{
			for (int i = 0; i < Maxdim; i++)
			{
				VFixValueToggle[i] = ctype;
			}
			return SY_SUCCESS;
		}
		else
		{
			return SY_NOT_INITIALIZED
		}
		break;
	}

	return SY_FAIL;
}

void Variable::Reset()
{
	ModelEntity::Reset();

	//Initialize
	VLowerType = NULL;
	VUpperType = NULL;
	VUpper = NULL;
	VLower = NULL;
	VFixValueToggle = NULL;

	// Scalar
	SUpperType = SY_VAL_NOT_SETC;
	SLowerType = SY_VAL_NOT_SETC;
	SUpper = SY_LARGE_POSITIVE;
	SLower = SY_LARGE_NEGATIVE;
	SFixValueToggle = SY_VAL_NOT_SETC;

	if (VLowerType != NULL)
		delete VLowerType;

	if (VUpperType != NULL)
		delete VUpperType;

	if (VUpper != NULL)
		delete VUpper;

	if (VLower != NULL)
		delete VLower;

	if (VFixValueToggle != NULL)
		delete VFixValueToggle;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Object
Object::Object():
ModelEntity()
{
	sModelName = "";
	SModelObject = NULL;
	VModelObject = NULL;
}

Object::~Object()
{
	if (SModelObject != NULL)
		delete SModelObject;

	if (VModelObject != NULL)
		delete VModelObject;
}

void Object::setModelName(std::string name)
{
	sModelName.assign(name);
}

std::string Object::getModelName()
{
	return sModelName;
}

int Object::setScalarObject(const Model* mod)
{
	if (DimType == SY_SCALAR)
	{
		if (mod)
		{
			SModelObject = new Model(*mod);
			return SY_SUCCESS;
		}
		else
		{
			return SY_FAIL;
		}
	}
	return SY_FAIL;
}

int Object::allocateArray()
{
	// Valid only for vectors
	if (DimType == SY_VECTOR)
	{
		// A size has been specified
		if (Dims.size() > 0)
		{
			// It has not been assigned before
			if (VModelObject == NULL)
			{
				int fullprod = 1;
				for (std::vector<int>::iterator it = Dims.begin(); it != Dims.end(); it++)
					fullprod *= *it;

				Maxdim = fullprod;

				VModelObject = new Model[fullprod];
				if (VModelObject == NULL)
				{
					return SY_VECTOR_FAILED_TO_ALLOCATE;
				}

				return SY_SUCCESS;
			}
			else
			{
				return SY_VECTOR_ALREADY_ALLOCATED;
			}
		}
		else
		{
			return SY_DIMS_ZERO;
		}
	}
	return SY_FAIL;
}

void Object::Reset()
{
	SModelObject = NULL;
	VModelObject = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Model
Model::Model()
{

}

Model::~Model()
{
	for (auto it = ModelEntities.begin(); it != ModelEntities.end(); ++it)
	{
		if (it->second != NULL)
			delete it->second;
	}
}

bool Model::insertModelEntity(std::string &symbolName, ModelEntity* me)
{
	if (ModelEntities[symbolName] == NULL)
	{
		//ModelEntities.insert({ symbolName, me });
		ModelEntities[symbolName] = me;
		return true;
	}
	return false; //entity with same name already exists
}

std::unordered_map<std::string, ModelEntity*>* Model::getModelEntities()
{
	return &ModelEntities;
}

std::vector<ASTNode*>* Model::getAllEquationNodes()
{
	return &EquationNodes;
}

std::vector<ASTNode *>* Model::getAllInitEquationNodes()
{
	return &InitNodes;
}

ModelEntity* Model::getModelEntity(Model* first, std::string &symbolName)
{
	ModelEntity* ret = NULL;

	// if first is NULL this means there this is the first search with symbol Name
	if (first == NULL)
	{
		auto find = this->ModelEntities.find(symbolName);

		if (find != this->ModelEntities.end())
			ret = find->second;
		else
			return NULL;
	}
	else
	{
		auto find = first->ModelEntities.find(symbolName);

		if (find != first->ModelEntities.end())
			ret = find->second;
		else
			return NULL;
	}

	return ret;
}

void Model::insertEquationNode(ASTNode* EqNode)
{
	EquationNodes.push_back(EqNode);
}

void Model::insertInitNode(ASTNode* InNode)
{
	InitNodes.push_back(InNode);
}
