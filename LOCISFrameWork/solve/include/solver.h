#pragma once

#include "genericresidual.h"
#include "genericjacobian.h"
#include "rapidjsonwrapper.h"
#include <string>
#include <math.h>

//also used with system type
enum solverTypes
{
    SOLVER_ALG_NONLINEAR,
    SOLVER_ALG_LINEAR,
    SOLVER_DAE_NONLINEAR,
    SOLVER_ODE
};

enum solverModes
{
    SOLVER_MODE_DIRECT,
    SOLVER_MODE_BLOCK_DECOMPOSITION
};

enum solverNames
{
    SOLVER_KINSOL,
    SOLVER_IDA
};

enum bufferDataType
{
    DATATYPE_DOUBLE,
    DATATYPE_INTEGER
};

struct binaryData
{
    long id;
    std::string sid;
    int type;
    unsigned int* sizeFromFetch;
    unsigned int sizeFromSend;
    void* ptr;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// generic options strucnture
struct solverOptions
{
    rapidjsonWrapper rjw;
    unsigned int numVars;
    long int matrixType;
    double absXTol;
    double relXTol;
    double absFTol;
    double relFTol;
    long int maxIter;
    long int traceLevel;

public:
    solverOptions();
    void putToBinaryFetch(const char *name, int dataType, void *dataPtr, unsigned int& size);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// generic output
struct solverOutput
{
    rapidjsonWrapper rjw;
    long int workSpaceSize;
    long int numFevals;
    long int numJacevals;
    long int numNonLinIter;
    double funcNorm;
    double xNorm;

    //solution
    double* xSol;
    unsigned int sizeX;

    double* yySol;
    unsigned int sizeYY;

    double* ypSol;
    unsigned int sizeYP;

public:
    solverOutput();
    void putToBinarySend(const char *name, int dataType, void *dataPtr, unsigned int size);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// base class for all solver types
class solver
{
protected:
    int solverType;
    std::string solverName;
    unsigned int numVar;
    unsigned int numEqu;
    std::vector<virtualOper>* pEquationVec;
    genericResidual* residual;
    genericJacobian* jacobian;
    solverOptions* option;

public:
    solver(int sovlerType_arg, std::string solverName_arg);
    ~solver();

    void setSolveDimensions(unsigned int numEqu_arg, unsigned int numVar_arg);
    void setPEquationVec(std::vector<virtualOper> *value);
    void setResiduaAndJacobian(genericResidual* residual_arg, genericJacobian* jacobian_arg);
    int getSolverType();
    std::string getSolverName();

    //interface
    virtual bool buildResidualAndJacobian(std::vector<virtualOper>* pEquationVec) = 0;
    virtual bool init(solverOptions* ops) = 0;
    virtual bool exit() = 0;
    virtual bool getSolverOutput(solverOutput* out) = 0;

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// uitility functions for JSON
bool getBinaryDataFromServer(const std::string name, int dataType, rapidjsonWrapper& rjw, void *dataPtr, unsigned int &size);
