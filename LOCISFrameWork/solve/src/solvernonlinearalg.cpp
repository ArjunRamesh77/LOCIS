#include "solvernonlinearalg.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solverNonLinearAlg
solverNonLinearAlg::solverNonLinearAlg(int type, std::string name) :
    solver(type, name),
    xGuess(NULL),
    xOrig(NULL)
{

}

solverNonLinearAlg::~solverNonLinearAlg()
{

}

void solverNonLinearAlg::setXGuess(double *xGuess_arg)
{
    xGuess = xGuess_arg;
}

void solverNonLinearAlg::setXOrig(double* xOrig_arg)
{
    xOrig = xOrig_arg;
}

bool solverNonLinearAlg::buildResidualAndJacobian(std::vector<virtualOper> *pEquationVec)
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
        jacobian->generateFullJacobianInstr(VDT_NORMAL);
    }
    else
    {
        return false;
    }

    return true;
}


