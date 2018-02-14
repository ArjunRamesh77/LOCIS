#include "solversundialskinsol.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KINSOL residual function
int kinsolResidualFunction(N_Vector u, N_Vector fval, void *userData)
{
    solverKinsolUserData* udata = static_cast<solverKinsolUserData*>(userData);
    double* x = NV_DATA_S(u);
    double* r = NV_DATA_S(fval);
    udata->res->evalResidual1StackBased(udata->xOrig, x, r);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KINSOL jacobian function
int kisnsolJacobianFunction(N_Vector u, N_Vector fu, SUNMatrix J, void *userData, N_Vector tmp1, N_Vector tmp2)
{
    solverKinsolUserData* udata = static_cast<solverKinsolUserData*>(userData);
    double* x = NV_DATA_S(u);

    //chose the type of jacobian
    switch(udata->jacType)
    {
    case MATRIX_DENSE:
        double* j = static_cast<SUNMatrixContent_Dense>(J->content)->data;
        udata->jac->evalDenseJacobian1StackBased(udata->xOrig, x, j);
        break;

    case MATRIX_SPARSE_CSC:
        break;

    case MATRIX_SPARSE_CSR:
        break;


    default:

    }

    return 0;
}
