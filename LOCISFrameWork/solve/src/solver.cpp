#include "solver.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// options and output
solverOptions::solverOptions() :
    absXTol(0.0),
    relXTol(0.0),
    absFTol(0.0),
    relFTol(0.0),
    maxIter(1),
    traceLevel(0)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// puts the data in binaryFetch to notify the server to receive this binary data before proceeding
void solverOptions::putToBinaryFetch(const char* name, int dataType, void* dataPtr, unsigned int& size)
{
    std::string strName(name);
    std::string binaryName = "binary-" + strName;
    const char* binaryNameCstr = binaryName.c_str();

    if(rjw.hasMember(binaryNameCstr))
    {
        long uid;
        rjw.getInt(binaryNameCstr, uid);
        binaryData bd;
        bd.id = uid;
        bd.sizeFromFetch = &size;
        bd.ptr = dataPtr;
        bd.type = dataType;

        //call function to retrieve binary data from server
    }
}

solverOutput::solverOutput() :
    workSpaceSize(0),
    numFevals(0),
    numJacevals(0),
    numNonLinIter(0),
    funcNorm(0.0),
    xNorm(0.0)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// puts the data in binaryFetch to notify the server to receive this binary data before proceeding
void solverOutput::putToBinarySend(const char* name, int dataType, void* dataPtr, unsigned int size)
{
    std::string strName(name);
    std::string binaryName = "binary-" + strName;
    const char* binaryNameCstr = binaryName.c_str();

    if(rjw.hasMember(binaryNameCstr))
    {
        unsigned int uid = 0; //Needs to be a unique number
        binaryData bd;
        bd.id = uid;
        bd.ptr = dataPtr;
        bd.sizeFromSend = size;
        bd.type = dataType;

        //call function to retrieve binary data from server
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solver
void solver::setPEquationVec(std::vector<virtualOper> *value)
{
    pEquationVec = value;
}

solver::solver(int solverType_arg, std::string solverName_arg) :
    solverType(solverType_arg),
    solverName(solverName_arg),
    numVar(0),
    numEqu(0),
    residual(NULL),
    jacobian(NULL),
    option(NULL)
{

}

solver::~solver()
{

}

void solver::setSolveDimensions(unsigned int numEqu_arg, unsigned int numVar_arg)
{
    numEqu = numEqu_arg;
    numVar = numVar_arg;
}

void solver::setResiduaAndJacobian(genericResidual *residual_arg, genericJacobian *jacobian_arg)
{
    residual = residual_arg;
    jacobian = jacobian_arg;
}

int solver::getSolverType()
{
    return solverType;
}

std::string solver::getSolverName()
{
    return solverName;
}
