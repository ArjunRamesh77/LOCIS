#pragma once

#include "solvernonlineardae.h"
#include "ida/ida.h"
#include "ida/ida_direct.h"
#include "nvector/nvector_serial.h"
#include "sundials/sundials_dense.h"
#include "sundials/sundials_sparse.h"
#include "sunmatrix/sunmatrix_dense.h"
#include "sunmatrix/sunmatrix_sparse.h"
#include "sunlinsol/sunlinsol_dense.h"
#include "sunlinsol/sunlinsol_lapackdense.h"
#include "sunlinsol/sunlinsol_klu.h"
#include "sundials/sundials_types.h"
#include "sundials/sundials_math.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ida user supplied functions
int idaResidualFunction(double tt, N_Vector yy, N_Vector yp, N_Vector rr, void* userData);

int idaJacobianFunction(double tt, double cj, N_Vector yy, N_Vector yp, N_Vector rr, SUNMatrix Jac, void* userData,
                        N_Vector tmp1, N_Vector tmp2, N_Vector tmp3);

int idaRootFunction(double t, N_Vector y, N_Vector yp, double* gout, void* userData);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ida linear solver types
enum kinsolLinearSolverTypes
{
    LINSOLV_IDA_INTRINSIC_DENSE,
    LINSOLV_IDA_LAPACK_DENSE,
    LINSOLV_IDA_KLU_SPARSE
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IDA solver options
struct solverOptionsIda : public solverOptions
{
    IDAErrHandlerFn errCb;
    int matrixType;
    int linearSolverType;
    int maxOrd;
    long int maxNumSteps;
    double initStep;
    double maxStep;
    double stopTime;
    int maxErrorTestFails;
    int maxNonLinIters;
    int maxConvFails;
    double nonLinConvCoef;
    int suppressAlg;
    double* id;
    double* varCons;
    int nroots;
    int* rootdir;
    double* vAbsFtol;

    solverOptionsIda();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// kinsol specific output data
struct solverOutputIda : public solverOutput
{
    long int lenrw;
    long int leniw;
    long int nsteps;
    long int nlinsetups;
    long int netfails;
    int klast;
    int kcur;
    double hlast;
    double hcur;
    double hinused;
    double tcur;
    double tolsfac;
    double* eweight;
    double* ele;
    long int nncfails;

    solverOutputIda();

    void buildEweight(long int num);
    void buildEle(long int num);
    void deleteEle();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// data structure to be passed to all ida functions
struct solverIdaUserData
{
    unsigned int numVars;
    int jacType;
    double* yyOrig;
    double* ypOrig;
    genericResidual* res;
    genericJacobian* jac;
    solverSundialsIda* interf;    

    solverIdaUserData();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Contains the implementation of Sundials IDA from LLNL
class solverSundialsIda : public solverNonLinearDae
{
    void* idamem;
    N_Vector yy;
    N_Vector yp;
    double* yyGuess;
    double* ypGuess;
    SUNMatrix J;
    SUNLinearSolver LS;
    solverIdaUserData userData;
    N_Vector Nid;
    N_Vector NvarCons;
    N_Vector NvAbsFTol;
    int* rootsFound;

public:
    solverSundialsIda();
    ~solverSundialsIda();

    //interface: solver
    virtual bool setSolverParameters(solverOptionsIda *ops);
    virtual bool init();
    virtual bool exit();
    virtual bool getSolverOutput(solverOutputIda* out);

    //interface: solverNonLinearDae
    virtual int solve(double time, int itask);
};




