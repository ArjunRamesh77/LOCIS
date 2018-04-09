#include "solversundialsida.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IDA residual function
int idaResidualFunction(double tt, N_Vector yy, N_Vector yp, N_Vector rr, void *userData)
{
    solverIdaUserData* udata = static_cast<solverIdaUserData*>(userData);
    double* dyy = NV_DATA_S(yy);
    double* dyp = NV_DATA_S(yp);
    double* dr = NV_DATA_S(rr);
    udata->res->evalResidual2StackBased(udata->yyOrig, dyy, udata->ypOrig, dyp, tt, dr);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IDA jacobian function
int idaJacobianFunction(double tt, double cj, N_Vector yy, N_Vector yp, N_Vector rr, SUNMatrix Jac, void *userData,
                        N_Vector tmp1, N_Vector tmp2, N_Vector tmp3)
{
    solverIdaUserData* udata = static_cast<solverIdaUserData*>(userData);
    double* dyy = NV_DATA_S(yy);
    double* dyp = NV_DATA_S(yp);
    double* j = NULL;

    //chose the type of jacobian
    switch(udata->jacType)
    {
    case MATRIX_DENSE:
        j = static_cast<SUNMatrixContent_Dense>(Jac->content)->data;
        udata->jac->evalDenseJacobian2StackBased(cj, udata->yyOrig, dyy, udata->ypOrig, dyp, j);
        break;

    case MATRIX_SPARSE_CSC:
        break;

    case MATRIX_SPARSE_CSR:
        break;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IDA root function
int idaRootFunction(double t, N_Vector yy, N_Vector yp, double *gout, void *userData)
{
    solverIdaUserData* udata = static_cast<solverIdaUserData*>(userData);
    double* dyy = NV_DATA_S(yy);
    double* dyp = NV_DATA_S(yp);
    udata->interf->evalRootResidual(dyy, dyp, t, gout);
    return 0;
}
