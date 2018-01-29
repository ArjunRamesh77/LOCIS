#include "solvernonlinearalg.h"

//Kinsol specifics
#include "kinsol/kinsol.h"
#include "kinsol/kinsol_dense.h"
#include "nvector/nvector_serial.h"
#include "sundials/sundials_types.h"
#include "sundials/sundials_math.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// data structure to be passed to all kinsol functions
struct solverKinsolUserData
{
    unsigned int numVars;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Contains the implementation of Sundials KINSOL from LLNL
class solverSundialsKinsol : public solverNonLinearAlg
{
    void* kinsolMemPointer;
    N_Vector y;
    N_Vector scale;
    solverKinsolUserData userData;

public:
    solverKinsolUserData();
    ~solverKinsolUserData();

    //kinsol

    //interface
    void setGuess(double* xGuess_arg);
    int solve(double* xSol);
};
