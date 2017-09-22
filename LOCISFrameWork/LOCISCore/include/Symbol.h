#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "lexer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// All defines
#define SY_SCALAR			 0
#define SY_VECTOR			 1
#define SY_TYPE_REAL		 0
#define SY_TYPE_INT		 1
#define SY_TYPE_OBJECT		 2
#define SY_TYPE_MODEL		 3
#define SY_VAL_NOT_SET	     -1
#define SY_VAL_NOT_SETC	 'N'
#define SY_LARGE_NEGATIVE   1.0e-300
#define SY_LARGE_POSITIVE   1.0e300
#define SY_DEFAULT_VAL		 0.0
#define SY_INDEX_OUT_OF_RANGE -1
#define SY_INVALID_INDEX_POSITION -2;
#define SY_DIMS_ZERO -3;
#define SY_VECTOR_NOT_ALLOCATED -4
#define SY_VECTOR_FAILED_TO_ALLOCATE -5
#define SY_VECTOR_ALREADY_ALLOCATED -6
#define SY_INDEX_GT_1 -7;
#define SY_NOT_INITIALIZED -8;
#define SY_FAIL 0;
#define SY_SUCCESS 1
#define SY_PARAMETER_TYPE 1
#define SY_VARIABLE_TYPE 2
#define SY_OBJECT_TYPE 3
#define SY_FIX		1
#define SY_FREE    2
#define SY_LESS_THAN_OR_EQUAL 'A'
#define SY_GREATER_THAN_OR_EQUAL 'B'
#define SY_LESS_THAN 'C'
#define SY_GREATER_THAN 'D'
#define SY_VARIABLE_UBT 0
#define SY_VARIABLE_LBT 1
#define SY_VARIABLE_FT 2

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Define all class types so that can be used in any order
class modelCollection;
class ModelEntity;
class Parameter;
class Variable;
class Object;
class Model;
class interpreter;
class ASTNode;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Abstract class types for a basic entity in a simulation
class ModelEntity
{
public:

	//Option flags
	bool bHasDesc;
	bool bHasUnit;
	bool bHasBounds[4];

	//FEM one D flags
	bool isFEMoneDDomain;
	bool isFEMOneDLine;
	bool isFEMOneDVariable;
	bool isFEMOneDdVariable;
	bool isFEMtrialFunction;
	bool isTval;
	bool isFEMxval;
	ModelEntity* currentDomain;

	// Primitives
	ModelEntity* other;
	ModelEntity* nt;
	int iter;
	bool isdt;
	double SValue;
	double *Vvalue;

	//Numbering for Variables
	int nSValue;
	int *nVvalue;

	//Late binding support for vectors
	bool vset;
	ASTNode* build_node;

	// Generic
	token* tok;       //Stores the token for the Symbol
	std::string Name;
	std::string Desc;
	std::string Unit;
	std::string SType;
	int Type;
	int NumType;

	// Dimensions
	int DimType;
	std::vector<int> Dims;
	std::vector<int> DimsC;
	long int Maxdim;

public:
	ModelEntity();
	~ModelEntity();

	// Set
	void setName(const std::string &name);
	void setDesc(const std::string &name);
	void setUnit(const std::string &name);
	void setDimType(const int DimType);
	void setType(const int Type);
	void setSType(const std::string &Type);
	void setNType(const int type);
	void setIsdt();
	int setDims(const int dimval);

	// Get
	std::string getName() const;
	std::string getSType() const;
	std::string getDesc() const;
	std::string getUnit() const;
	bool checkIsdt();
	int getDimType() const;
	int geType() const;
	int getNumDims() const;
	int getMaxDims() const;
	std::string getGetMultiDimsFromSingle(int &index) const;
	std::vector<int>* getDimAt();
	ModelEntity* ReturnObject();

	int setAllto(const double &val);

	// Virtual
	virtual int allocateArray(const double &val);
	virtual void Reset();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Parameter Class
class Parameter : public ModelEntity
{
public:
	Parameter();
	~Parameter();

	//Set
	int allocateArray(const double &val);
	void Reset();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Variable Class 
class Variable : public ModelEntity
{
public:

	// Vector
	char* VLowerType;
	char* VUpperType;
	double* VUpper;
	double* VLower;
	char* VFixValueToggle;

	// Scalar
	char SUpperType;
	char SLowerType;
	double SUpper;
	double SLower;
	char SFixValueToggle;
	
public:
	Variable();
	~Variable();

	//Vector operations
	int allocateArray(const double &val);
	void Reset();

	//Setting allto
	int setAlltoB(const int &type, const char ctype, const double &dtype);

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Object 
class Object : public ModelEntity
{
public:

	std::string sModelName;
	Model* SModelObject;
	Model* VModelObject;

public:
	Object();
	~Object();

	void setModelName(std::string name);
	std::string getModelName();
	int setScalarObject(const Model* mod);
	int allocateArray();
	void Reset();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Model (Class stores the prototype of a model)
class Model 
{

	std::unordered_map<std::string, ModelEntity*> ModelEntities;
	std::vector<ASTNode*> EquationNodes;
	std::vector<ASTNode*> InitNodes;

public:
	Model();
	~Model();

	std::unordered_map<std::string, ModelEntity*>* getModelEntities();
	std::vector<ASTNode*>* getAllEquationNodes();
	std::vector<ASTNode *>* getAllInitEquationNodes();
	bool insertModelEntity(std::string &symbolName, ModelEntity* me);
	ModelEntity* getModelEntity(Model* first, std::string &symbolName);
	void insertEquationNode(ASTNode* EqNode);
	void insertInitNode(ASTNode* node);
};