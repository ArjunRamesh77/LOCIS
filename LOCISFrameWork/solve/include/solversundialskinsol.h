#include "solvernonlinearalg.h"

//Kinsol specifics
#include "kinsol/kinsol.h"
#include "kinsol/kinsol_direct.h"
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
// kinsol user supplied functions
int kinsolResidualFunction(N_Vector u, N_Vector fval, void *userData);
int kisnsolJacobianFunction(N_Vector u, N_Vector fu, SUNMatrix J, void* userData, N_Vector tmp1, N_Vector tmp2);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// kinsol linear solver types
enum kinsolLinearSolverTypes
{
    LINSOLV_KINSOL_INTRINSIC_DENSE,
    LINSOLV_KINSOL_LAPACK_DENSE,
    LINSOLV_KINSOL_KLU_SPARSE
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// kinsol specific user data
struct solverOptionsKinsol : public solverOptions
{
    KINErrHandlerFn errCb;
    KINInfoHandlerFn infoCb;
    int strategy;
    int matrixType;
    int linearSolverType;
    int noInitSetup;
    int noResMon;
    long int maxSetupCalls;
    long int maxSubSetupCalls;
    int etaChoice;
    double etaConst;
    double gamma;
    double alpha;
    double omegaConst;
    double omegaMin;
    double omegaMax;
    int noMineps;
    double maxNewtonStep;
    double maxBetaFails;
    double relErrFunc;
    double scStepTol;
    double* varCons;
    long int maa;

    solverOptionsKinsol();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// kinsol specific output data
struct solverOutputKinsol : public solverOutput
{
    long int lenrw;
    long int leniw;
    long nbcfails;
    long nbacktr;
    double stepLength;
    long int lenrwLS;
    long int leniwLS;
    long int nfevalsLS;
    long int lsflag;

    solverOutputKinsol();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// data structure to be passed to all kinsol functions
struct solverKinsolUserData
{
    unsigned int numVars;
    int jacType;
    double* xOrig;
    genericResidual* res;
    genericJacobian* jac;

    solverKinsolUserData();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Contains the implementation of Sundials KINSOL from LLNL
class solverSundialsKinsol : public solverNonLinearAlg
{
    void* kmem;
    N_Vector y;
    N_Vector scale;
    SUNMatrix J;
    SUNLinearSolver LS;
    solverKinsolUserData userData;

public:
    solverSundialsKinsol();
    ~solverSundialsKinsol();

    //interface: solver
    virtual bool setSolverParameters(solverOptionsKinsol *ops);
    virtual bool init();
    virtual bool exit();
    virtual bool getSolverOutput(solverOutputKinsol* out);

    //interface: solverNonLinearAlg
    virtual int solve();
};
