#include "solvernonlineardae.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solverNonLinearDae
solverNonLinearDae::solverNonLinearDae(int type, std::string name) :
    solver(type, name),
    yyGuess(NULL),
    ypGuess(NULL),
    yyOrig(NULL),
    ypOrig(NULL),
    numRoots(0),
    rootResidual(NULL)
{

}

solverNonLinearDae::~solverNonLinearDae()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set the generic root residual, will be used for event tracking
void solverNonLinearDae::setRootResidual(genericResidual *rootRes, unsigned int numRoots_arg)
{
    rootResidual = rootRes;
    numRoots = numRoots_arg;
}

void solverNonLinearDae::setGuessYyandYp(double *yy, double *yp)
{
    yyGuess = yy;
    ypGuess = yp;
}

void solverNonLinearDae::setYyOrigAndYpOrig(double *yyOrig_arg, double *ypOrig_arg)
{
    yyOrig = yyOrig_arg;
    ypOrig = ypOrig_arg;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// evaluate the root residual
int solverNonLinearDae::evalRootResidual(double *yy, double *yp, double t, double *root)
{
    rootResidual->evalResidual2StackBased(yyOrig, yy, ypOrig, yp, t, root);
    return 0;
}

bool solverNonLinearDae::checkFoundRoot()
{
    return bGotRoot;
}

bool solverNonLinearDae::buildResidualAndJacobian(std::vector<virtualOper> *pEquationVec)
{
    if(pEquationVec)
    {
        if(residual)
            delete residual;
        residual = new genericResidual;
        residual->createNewInstructionStack(pEquationVec);

        switch(option->matrixType)
        {
        case MATRIX_DENSE:
            if(jacobian)
                delete jacobian;
            jacobian = new genericJacobian;
            jacobian->createNewInstructionStack(pEquationVec);
            jacobian->setJacobianStyle(option->matrixType);
            break;

        case MATRIX_SPARSE_CSC:
            break;

        case MATRIX_SPARSE_CSR:
            break;
        }

        jacobian->setNVar(numVar);
        jacobian->generateDualPartJacobian();
    }
    else
    {
        return false;
    }

    return true;
}




