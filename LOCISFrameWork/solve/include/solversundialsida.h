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
//#include "sunlinsol/sunlinsol_lapackdense.h"
//#include "sunlinsol/sunlinsol_klu.h"
#include "sundials/sundials_types.h"
#include "sundials/sundials_math.h"

#include "rapidjsonwrapper.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ida user supplied functions
int idaResidualFunction(double tt, N_Vector yy, N_Vector yp, N_Vector rr, void* userData);

int idaJacobianFunction(double tt, double cj, N_Vector yy, N_Vector yp, N_Vector rr, SUNMatrix Jac, void* userData,
                        N_Vector tmp1, N_Vector tmp2, N_Vector tmp3);

int idaRootFunction(double t, N_Vector yy, N_Vector yp, double* gout, void* userData);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ida linear solver types
enum idaLinearSolverTypes
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
    long int linearSolverType;
    long int maxOrd;
    long int maxNumSteps;
    double initStep;
    double maxStep;
    double stopTime;
    long int maxErrorTestFails;
    long int maxNonLinIters;
    long int maxConvFails;
    double nonLinConvCoef;
    long int suppressAlg;
    double* id;
    N_Vector Nid;
    unsigned int sizeId;
    double* varCons;
    N_Vector NvarCons;
    unsigned int sizeVarcons;
    long int nroots;
    int* rootdir;
    unsigned int sizeRootdir;
    double* vAbsFtol;
    N_Vector NvAbsFTol;
    unsigned int sizeVAbsFtol;
    long int iTask;

public:
    solverOptionsIda();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// kinsol specific output data
struct solverOutputIda : public solverOutput
{
    long int lenrw;
    long int leniw;
    long int leniwLS;
    long int lenrwLS;
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
    unsigned int sizeEweight;
    double* ele;
    unsigned int sizeEle;
    long int nncfails;
    long int gevals;

public:
    solverOutputIda();
    void buildEweight(long int num);
    void buildEle(long int num);
    void deleteVectorData();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// data structure to be passed to all ida functions
class solverSundialsIda;
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
    SUNMatrix J;
    SUNLinearSolver LS;
    solverIdaUserData userData;
    int* rootsFound;
    long int iTask;

public:
    solverSundialsIda();
    ~solverSundialsIda();

    //ida

    //interface: solver
    virtual bool init(solverOptions *ops1);
    virtual bool exit();
    virtual bool getSolverOutput(solverOutput* out1);

    //interface: solverNonLinearDae
    virtual bool solve(double time);
};




