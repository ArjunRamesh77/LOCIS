#include "Symbol.h"
#include "lexer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ModelEntity
modelEntity::modelEntity(int type_arg) :
    bHasDesc(false),
    bHasUnit(false),
    bHasBounds{false, false, false, false},

    meFEMContext(),
    isFEMoneDDomain(false),
    isFEMOneDLine(false),
    isFEMOneDVariable(false),
    isFEMOneDdVariable(false),
    isFEMtrialFunction(false),
    isTval(false),
    isFEMxval(false),
    currentDomain(NULL),

    other(NULL),
    isdt(false),

    sValue(SY_DEFAULT_VAL),
    vValue(NULL),

    nSValue(0),
    nVValue(NULL),

    vectorIsInitialized(true),
    buildNode(NULL),

    tok(NULL),
    name(""),
    desc(""),
    unit(""),

    type(type_arg),
    sType(""),
    nType(SY_VAL_NOT_SET),

    dimType(SY_VAL_NOT_SET),
    dims(),
    dimsC(),
    maxdim(0)
{

}

modelEntity::~modelEntity()
{
    if (vValue)
        delete vValue;
}

void modelEntity::setBHasDesc(bool value)
{
    bHasDesc = value;
}

void modelEntity::setBHasUnit(bool value)
{
    bHasUnit = value;
}

void modelEntity::setBHasBounds(int index, bool value)
{
    bHasBounds[index] = value;
}

void modelEntity::setOther(modelEntity *value)
{
    other = value;
}

void modelEntity::setVectorIsInitialized(bool value)
{
    vectorIsInitialized = value;
}

void modelEntity::setBuildNode(ASTNode *value)
{
    buildNode = value;
}

void modelEntity::setTok(token *value)
{
    tok = value;
}

void modelEntity::setName(const std::string value)
{
    name.assign(value);
}

void modelEntity::setDesc(const std::string value)
{
    desc.assign(value);
}

void modelEntity::setUnit(const std::string value)
{
    unit.assign(value);
}

void modelEntity::setDimType(const int value)
{
    dimType = value;
}

void modelEntity::setType(const int value)
{
    type = value;
}

void modelEntity::setIsdt()
{
	isdt = true;
}

void modelEntity::setNType(const int value)
{
    nType = value;
}

void modelEntity::setSType(const std::string value)
{
    sType = value;
}

int modelEntity::setDims(const int value)
{
	// Make sure its a vector
    if (dimType == SY_VECTOR)
	{
		// size specification of the dimension must be greater than 1
        if (value > 1)
		{
            dims.push_back(value);
		}
		else
		{
			return SY_INDEX_GT_1;
		}
	}

	// Get Multipliers
    dimsC.clear();
    for (int i = 0; i < dims.size(); i++)
	{
		int mult = 1;
        for (auto it = dims.rbegin() + i; it != (dims.rend() - 1); it++)
		{
			mult *= *it;
		}
        dimsC.push_back(mult);
	}

    return SY_SUCCESS;
}

// Get >>>>>>>>>>>>
bool modelEntity::getBHasDesc() const
{
    return bHasDesc;
}

bool modelEntity::getBHasUnit() const
{
    return bHasUnit;
}

bool modelEntity::getBHasBounds(int index)
{
    return bHasBounds[index];
}

modelEntity *modelEntity::getOther() const
{
    return other;
}

bool modelEntity::getVectorIsInitialized() const
{
    return vectorIsInitialized;
}

ASTNode *modelEntity::getBuildNode() const
{
    return buildNode;
}

token *modelEntity::getTok() const
{
    return tok;
}

std::string modelEntity::getName()
{
    return name;
}

std::string modelEntity::getDesc()
{
    return desc;
}

std::string modelEntity::getUnit()
{
    return unit;
}

int modelEntity::getDimType()
{
    return dimType;
}

bool modelEntity::checkIsdt()
{
	return isdt;
}

int modelEntity::geType()
{
    return type;
}

std::string modelEntity::getSType()
{
    return sType;
}

int modelEntity::getNType()
{
    return nType;
}

int modelEntity::getNumDims()
{
    return dims.size();
}

int modelEntity::getMaxDims()
{
    return maxdim;
}

void modelEntity::clearDims()
{
    dims.clear();
}

int modelEntity::getDimsAt(int index)
{
    if (dims.size() > 0)
        return dims.at(index);
	else
        return 0;
}

int modelEntity::getDimsCAt(int index) const
{
    if (dimsC.size() > 0)
        return dimsC.at(index);
    else
        return 0;
}

void modelEntity::setDummyIndex()
{
    dims.push_back(0);
}

modelEntity* modelEntity::ReturnObject()
{
	return this;
}

int modelEntity::setAllto(const double &val)
{
    if (vValue)
	{
        for (unsigned int i = 0; i < maxdim; i++)
		{
            vValue[i] = val;
		}
		return SY_SUCCESS;
	}
	else
	{
		return SY_NOT_INITIALIZED
	}

    return SY_FAIL;
}

int modelEntity::allocateArray(const double &val)
{
    return 0;
}

std::string modelEntity::getGetMultiDimsFromSingle(int &index)
{
	std::string stempval("");
	int tempval = index;
    if (dimType == SY_VECTOR)
	{
		
        for (std::vector<int>::const_reverse_iterator it = dims.rbegin(); it != dims.rend(); it++)
		{	
            if (it == dims.rend() - 1)
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
parameter::parameter() :
    modelEntity(PARAMETER)
{
    vValue = NULL;
}

parameter::~parameter()
{
	
}

int parameter::allocateArray(const double &val)
{
	// Valid only for vectors
    if (getDimType() == SY_VECTOR)
	{
		// A size has been specified
        if (dims.size() > 0)
		{
			// It has not been assigned before
            if (vValue == NULL)
			{
                unsigned int fullprod = 1;
                for (std::vector<int>::iterator it = dims.begin(); it != dims.end(); it++)
					fullprod *= *it;

                maxdim = fullprod;

                nVValue = new int[fullprod];
                vValue = new double[fullprod];
                if (vValue == NULL)
				{
					return SY_VECTOR_FAILED_TO_ALLOCATE;
				}

				for (int i = 0; i < fullprod; i++)
				{
                    nVValue[i] = 0;
                    vValue[i] = val;
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Variable
variable::variable() : modelEntity(VARIABLE),
    vLowerType(NULL),
    vUpperType(NULL),
    vUpperValue(NULL),
    vLowerValue(NULL),
    vFixValueToggle(NULL),
    sUpperType(SY_VAL_NOT_SETC),
    sLowerType(SY_VAL_NOT_SETC),
    sUpperValue(SY_LARGE_POSITIVE),
    sLowerValue(SY_LARGE_NEGATIVE),
    sFixValueToggle(SY_FREE)
{

}

variable::~variable()
{
    if (vLowerType)
        delete vLowerType;

    if (vUpperType)
        delete vUpperType;

    if (vUpperValue)
        delete vUpperValue;

    if (vLowerValue)
        delete vLowerValue;

    if (vFixValueToggle)
        delete vFixValueToggle;

}

int variable::allocateArray(const double &val)
{
	// Valid only for vectors
    if (dimType == SY_VECTOR)
	{
		// A size has been specified
        if (dims.size() > 0)
		{
			// It has not been assigned before
            if (vValue == NULL && vUpperValue == NULL && vLowerValue == NULL && vUpperType == NULL && vLowerType == NULL && vFixValueToggle == NULL)
			{
				int fullprod = 1;
                for (std::vector<int>::iterator it = dims.begin(); it != dims.end(); it++)
					fullprod *= *it;

                maxdim = fullprod;

				// Allocate all arrays for variable
                nVValue = new int[fullprod];
                vValue = new double[fullprod];
                vUpperValue = new double[fullprod];
                vLowerValue = new double[fullprod];
                vUpperType = new char[fullprod];
                vLowerType = new char[fullprod];
                vFixValueToggle = new char[fullprod];

                if (nVValue == NULL)
					return SY_VECTOR_FAILED_TO_ALLOCATE;

                if (vValue == NULL)
					return SY_VECTOR_FAILED_TO_ALLOCATE;

                if (vUpperValue == NULL)
					return SY_VECTOR_FAILED_TO_ALLOCATE;

                if (vLowerValue == NULL)
					return SY_VECTOR_FAILED_TO_ALLOCATE;

                if (vUpperType == NULL)
					return SY_VECTOR_FAILED_TO_ALLOCATE;

                if (vLowerType == NULL)
					return SY_VECTOR_FAILED_TO_ALLOCATE;

                if (vFixValueToggle == NULL)
					return SY_VECTOR_FAILED_TO_ALLOCATE;

				for (int i = 0; i < fullprod; i++)
				{
                    nVValue[i] = 0;
                    vValue[i] = val;
                    vUpperValue[i] = SY_LARGE_POSITIVE;
                    vLowerValue[i] = SY_LARGE_NEGATIVE;
                    vUpperType[i] = SY_LESS_THAN_OR_EQUAL;
                    vLowerType[i] = SY_GREATER_THAN_OR_EQUAL;
                    vFixValueToggle[i] = SY_FREE;
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

int variable::setAlltoB(const int &type, const char ctype, const double &dtype)
{
	switch (type)
	{
	case SY_VARIABLE_UBT:
        if (vUpperType && vUpperValue)
		{
            for (int i = 0; i < maxdim; i++)
			{
                vUpperType[i] = ctype;
                vUpperValue[i] = dtype;
			}
			return SY_SUCCESS;
		}
		else
		{
			return SY_NOT_INITIALIZED
		}
		break;

	case SY_VARIABLE_LBT:
        if (vLowerType && vLowerValue)
		{
            for (int i = 0; i < maxdim; i++)
			{
                vLowerType[i] = ctype;
                vLowerValue[i] = dtype;
			}
			return SY_SUCCESS;
		}
		else
		{
			return SY_NOT_INITIALIZED
		}
		break;

	case SY_VARIABLE_FT:
        if (vFixValueToggle)
		{
            for (int i = 0; i < maxdim; i++)
			{
                vFixValueToggle[i] = ctype;
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Object
object::object(): modelEntity(MODEL),
  sModelObject(NULL),
  vModelObject(NULL),
  sModelName("")
{

}

object::~object()
{
    if (sModelObject)
        delete sModelObject;

    if (vModelObject)
        delete vModelObject;
}

void object::setModelName(std::string name)
{
	sModelName.assign(name);
}

std::string object::getModelName()
{
	return sModelName;
}

int object::allocateArray()
{
	// Valid only for vectors
    if (dimType == SY_VECTOR)
	{
		// A size has been specified
        if (dims.size() > 0)
		{
			// It has not been assigned before
            if (vModelObject == NULL)
			{
				int fullprod = 1;
                for (std::vector<int>::iterator it = dims.begin(); it != dims.end(); it++)
					fullprod *= *it;

                maxdim = fullprod;

                vModelObject = new model[fullprod];
                if (vModelObject == NULL)
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Model
model::model() :
    model(NULL)
{

}

model::model(globalSymbolTable *gstPtr) :
    gst(gstPtr),
    allModelEntities(),
    allEquationNodes(),
    allInitNodes()
{

}

model::~model()
{
    for (auto it = allModelEntities.begin(); it != allModelEntities.end(); ++it)
	{
		if (it->second != NULL)
			delete it->second;
	}
}

bool model::insertModelEntity(std::string &symbolName, modelEntity* me)
{
    auto find = allModelEntities.find(symbolName);
    if (find == allModelEntities.end())
	{
        allModelEntities[symbolName] = me;
		return true;
	}
	return false; //entity with same name already exists
}

std::unordered_map<std::string, modelEntity*>* model::getAllModelEntities()
{
    return &allModelEntities;
}

std::vector<ASTNode*>* model::getAllEquationNodes()
{
    return &allEquationNodes;
}

std::vector<ASTNode *>* model::getAllInitEquationNodes()
{
    return &allInitNodes;
}

modelEntity* model::getModelEntity(model* first, std::string &symbolName)
{
	modelEntity* ret = NULL;

	// if first is NULL this means there this is the first search with symbol Name
	if (first == NULL)
	{
        auto find = this->allModelEntities.find(symbolName);

        if (find != this->allModelEntities.end())
        {
			ret = find->second;
        }
		else
        {
            // Search in gst second
            if(gst)
                ret = gst->getSymbol(symbolName);
            return ret;
        }
	}
	else
	{
        auto find = first->allModelEntities.find(symbolName);

        if (find != first->allModelEntities.end())
        {
			ret = find->second;
        }
		else
        {
            // Search in gst second
            if(gst)
                ret = gst->getSymbol(symbolName);
            return ret;
        }
	}

	return ret;
}

void model::insertEquationNode(ASTNode* EqNode)
{
    allEquationNodes.push_back(EqNode);
}

void model::insertInitNode(ASTNode* InNode)
{
    allInitNodes.push_back(InNode);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Global Symbol Tree
globalSymbolTable::globalSymbolTable() :
    gst()
{

}

void globalSymbolTable::clearAll()
{
    gst.clear();
}

modelEntity *globalSymbolTable::getSymbol(std::string value)
{
    gst["std"] = NULL;
    auto find = gst.find(value);
    if(find != gst.end())
    {
        return find->second;
    }
    return NULL;
}

bool globalSymbolTable::addSymbol(std::string name, modelEntity *value)
{
    auto find = gst.find(name);
    if(find == gst.end())
    {
        gst[name] = value;
        return true;
    }
    return false;
}

bool globalSymbolTable::deleteSymbol(std::string value)
{
    auto find = gst.find(value);
    if(find != gst.end())
    {
        gst.erase(find);
        return true;
    }
    return false;
}
