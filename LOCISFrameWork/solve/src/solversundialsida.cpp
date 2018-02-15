#include "solversundialsida.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set all the solver parameters for IDA
solverOptionsIda::solverOptionsIda() :
    errCb(NULL),
    maxOrd(5),
    maxNumSteps(0),
    initStep(0.0),
    maxStep(0.0),
    stopTime(0.0),
    maxErrorTestFails(7),
    maxNonLinIters(3),
    maxConvFails(10),
    nonLinConvCoef(0.33),
    suppressAlg(SUNFALSE),
    id(NULL),
    varCons(NULL),
    nroots(0),
    rootdir(NULL),
    vAbsFtol(NULL)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solverOutputIda
solverOutputIda::solverOutputIda() :
    lenrw(0),
    leniw(0),
    nsteps(0),
    nlinsetups(0),
    netfails(0),
    klast(0),
    kcur(0),
    hlast(0.0),
    hcur(0.0),
    hinused(0.0),
    tcur(0.0),
    tolsfac(0.0),
    eweight(NULL),
    ele(NULL),
    nncfails(0)
{

}

void solverOutputIda::buildEweight(long num)
{
    if(eweight)
    {
        delete eweight;
        eweight = NULL;
    }
    eweight = new double[num];
}

void solverOutputIda::buildEle(long num)
{
    if(ele)
    {
        delete ele;
        ele = NULL;
    }
    ele = new double[num];
}

void solverOutputIda::deleteVectorData()
{
    if(eweight)
    {
        delete eweight;
        eweight = NULL;
    }

    if(ele)
    {
        delete ele;
        ele = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solverIdaUserData
solverIdaUserData::solverIdaUserData() :
    numVars(0),
    jacType(MATRIX_DENSE),
    yyOrig(NULL),
    ypOrig(NULL),
    res(NULL),
    jac(NULL),
    interf(NULL)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set all the solver parameters for IDA
solverSundialsIda::solverSundialsIda() :
    idamem(NULL),
    yy(NULL),
    yp(NULL),
    yyGuess(NULL),
    ypGuess(NULL),
    J(NULL),
    LS(NULL),
    userData(),
    Nid(NULL),
    NvarCons(NULL),
    NvAbsFTol(NULL),
    rootsFound(NULL)
{

}

solverSundialsIda::~solverSundialsIda()
{
    exit();
}

bool solverSundialsIda::setSolverParameters(solverOptionsIda *ops)
{
    //populate base
    if(!solver::setSolverParameters(static_cast<solverOptions*>(ops)))
    {
        return false;
    }

    //check sanity
    switch (ops->linearSolverType)
    {
    case LINSOLV_IDA_INTRINSIC_DENSE:
    case LINSOLV_IDA_KLU_SPARSE:
    case LINSOLV_IDA_LAPACK_DENSE:
         break;

    default:
        return false;
    }

    if(ops->maxOrd <= 0)
        return false;

    if(ops->maxNumSteps < 0)
        return false;

    if(ops->initStep < 0.0)
        return false;

    if(ops->maxStep < 0.0)
        return false;

    if(ops->stopTime < 0.0)
        return false;

    if(ops->maxErrorTestFails <= 0)
        return false;

    if(ops->maxNonLinIters <= 0)
        return false;

    if(ops->maxConvFails <= 0)
        return false;

    if(ops->nonLinConvCoef <= 0.0)
        return false;

    switch(ops->suppressAlg)
    {
    case SUNTRUE:
    case SUNFALSE:
        break;

    default:
        return false;
    }

    if(ops->id)
    {
        for(unsigned int i = 0; i < ops->numVars; ++i)
        {
            if(!(ops->id[i] == 1.0 || ops->id[i] == 0.0))
                return false;
        }
    }

    if(ops->varCons)
    {
        for(unsigned int i = 0; i < ops->numVars; ++i)
        {
            if(!(ops->varCons[i] == 1.0 || ops->varCons[i] == 0.0 || ops->varCons[i] == -1.0 || ops->varCons[i] == -2.0))
                return false;
        }
    }

    if(ops->nroots < 0)
        return false;

    if(ops->rootdir)
    {
        for(unsigned int i = 0; i < ops->nroots; ++i)
        {
            if(!(ops->rootdir[i] == 0 || ops->rootdir[i] == 1 || ops->rootdir[i] == -1))
                return false;
        }

        //setup roots found
        if(rootsFound)
        {
            delete rootsFound;
            rootsFound = NULL;
        }
        rootsFound = new int[ops->nroots];
    }

    if(ops->vAbsFtol)
    {
        for(unsigned int i = 0; i < ops->numVars; ++i)
        {
            if(!(ops->vAbsFtol[i] < 0.0))
                return false;
        }
    }

    option = ops;
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// finalize the IDA solver
bool solverSundialsIda::exit()
{
    if(idamem)
    {
        IDAFree(idamem);
        idamem = NULL;
    }

    if(yy)
    {
        N_VDestroy(yy);
        yy = NULL;
    }

    if(yp)
    {
        N_VDestroy(yp);
        yp = NULL;
    }

    if(Nid)
    {
        N_VDestroy(Nid);
        Nid = NULL;
    }

    if(NvarCons)
    {
        N_VDestroy(NvarCons);
        NvarCons = NULL;
    }

    if(NvAbsFTol)
    {
        N_VDestroy(NvAbsFTol);
        NvAbsFTol = NULL;
    }

    if(rootsFound)
    {
        delete rootsFound;
        rootsFound = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// initialize the IDA solver
bool solverSundialsIda::init()
{
    int retval = 0;
    solverOptionsIda* ops = static_cast<solverOptionsIda*>(option);

    yy = N_VMake_Serial(numVar, yyGuess);
    yp = N_VMake_Serial(numVar, ypGuess);

    //create IDA memory
    idamem = IDACreate();

    //setup userData
    userData.numVars = numVar;
    userData.res = residual;
    userData.jac = jacobian;
    userData.jacType = matrixType;
    userData.yyOrig = yyOrig;
    userData.ypOrig = ypOrig;
    userData.interf = this;

    //callbacks
    if(ops->errcb)
    {
        retval = IDASetErrHandlerFn(idamem, ops->errcb, &userData);
        if(retval != IDA_SUCCESS)
            return false;
    }

    //set all options
    retval = IDASetUserData(idamem, &userData);
    if(retval != IDA_SUCCESS)
        return false;

    retval = IDASetMaxOrd(idamem, ops->maxOrd);
    if(retval != IDA_SUCCESS)
        return false;

    retval = IDASetMaxNumSteps(idamem, ops->maxNumSteps);
    if(retval != IDA_SUCCESS)
        return false;

    retval = IDASetInitStep(idamem, ops->initStep);
    if(retval != IDA_SUCCESS)
        return false;

    retval = IDASetMaxStep(idamem, ops->maxStep);
    if(retval != IDA_SUCCESS)
        return false;

    retval = IDASetMaxErrTestFails(idamem, ops->maxErrorTestFails);
    if(retval != IDA_SUCCESS)
        return false;

    retval = IDASetMaxNonlinIters(idamem, ops->maxNonLinIters);
    if(retval != IDA_SUCCESS)
        return false;

    retval = IDASetMaxConvFails(idamem, ops->maxConvFails);
    if(retval != IDA_SUCCESS)
        return false;

    retval = IDASetNonlinConvCoef(idamem, ops->nonLinConvCoef);
    if(retval != IDA_SUCCESS)
        return false;

    retval = IDASetSuppressAlg(idamem, ops->suppressAlg);
    if(retval != IDA_SUCCESS)
        return false;

    if(ops->id)
    {
        Nid = N_VMake_Serial(numVar, ops->id);
        retval = IDASetId(idamem, Nid);
        if(retval != IDA_SUCCESS)
            return false;
    }

    if(ops->varCons)
    {
        NvarCons = N_VMake_Serial(numVar, ops->varCons);
        retval = IDASetConstraints(idamem,  NvarCons);
        if(retval != IDA_SUCCESS)
            return false;
    }

    //init
    retval = IDAInit(idamem, idaResidualFunction, t0, yy, yp);
    if(retval != IDA_SUCCESS)
        return false;

    //specify tolerances
    retval = IDASStolerances(idamem, ops->relFTol, ops->absFTol);
    if(retval != IDA_SUCCESS)
        return false;

    if(ops->vAbsFtol)
    {
        NvAbsFTol = N_VMake_Serial(numVar, ops->vAbsFtol);
        retval = IDASVtolerances(idamem, ops->relFTol, ops->NvAbsFTol);
        if(retval != IDA_SUCCESS)
            return false;
    }

    //set matrix/linear solver type
    //Get matrix type
    switch(ops->matrixType)
    {
    case MATRIX_DENSE:
        J = SUNDenseMatrix(numVar, numEqu);
        break;

    case MATRIX_SPARSE_CSR:
        J = SUNSparseMatrix(numVar, numEqu, jacobian->getNNZ(), CSR_MAT);
        break;

    case MATRIX_SPARSE_CSC:
        J = SUNSparseMatrix(numVar, numEqu, jacobian->getNNZ(), CSC_MAT);
        break;

    }

    //Get linear solver type
    switch (ops->linearSolverType)
    {
    case LINSOLV_IDA_INTRINSIC_DENSE:
        if(matrixType == MATRIX_DENSE)
            LS = SUNDenseLinearSolver(yy, J);
        else
            return false;

    case LINSOLV_IDA_LAPACK_DENSE:
        if(matrixType == MATRIX_DENSE)
            LS = SUNLapackDense(yy, J);
        else
            return false;

    case LINSOLV_IDA_KLU_SPARSE:
        if(matrixType == MATRIX_SPARSE_CSR || matrixType == MATRIX_SPARSE_CSC)
            LS = SUNKLU(yy, J);
        else
            return false;
    }

    retval = IDADlsSetLinearSolver(idamem, LS, J);
    if(retval != IDA_SUCCESS)
        return false;

    retval = IDADlsSetJacFn(idamem, idaJacobianFunction);
    if(retval != IDA_SUCCESS)
        return false;

    //root-finding (needed for event tracking)
    if(ops->nroots > 0)
    {
        retval = IDARootInit(idamem, ops->nRoots, idaRootFunction);
        if(retval != IDA_SUCCESS)
            return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solve a time step
int solverSundialsIda::solve(double time, int itask, int* roots)
{
    roots = NULL;
    int retval = 0;
    double tret = 0.0;
    retval = IDASolve(idamem, time, tret, yy, yp, itask);

    if(retval == IDA_ROOT_RETURN)
    {
       int retval1 = 0;
       retval1 = IDAGetRootInfo(idamem, rootsFound);
       if(retval1 == IDA_SUCCESS)
       {
            return rootsFound;
       }
    }

    return retval;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get the solver output
bool solverSundialsIda::getSolverOutput(solverOutputIda *out)
{
    int retval = 0;

    retval = IDAGetWorkSpace(idamem, out->lenrw, out->leniw);
    if(retval != IDA_SUCCESS)
        return false;

    retval = IDADlsGetWorkSpace(idamem, out->lenrwLS, out->leniwLS);
    if(retval != IDA_SUCCESS)
        return false;

    out->workSpaceSize = out->lenrw + out->leniw + out->lenrwLS + out->leniwLS;

    retval = IDAGetIntegratorStats(idamem, out->nsteps, out->numFevals, out->nlinsetups, out->netfails, out->klast, out->kcur, out->hinused,
                                            out->hlast, out->hcur, out->tcur);
    if(retval != IDA_SUCCESS)
        return false;

    retval = IDAGetTolScaleFactor(idamem, out->tolsfac);
    if(retval != IDA_SUCCESS)
        return false;

    if(out->eweight)
    {
        N_Vector Neweight = N_VMake_Serial(numVar, out->eweight);
        retval = IDAGetErrWeights(idamem, Neweight);
        if(retval != IDA_SUCCESS)
            return false;
        N_VDestroy(Neweight);
    }

    if(out->ele)
    {
        N_Vector Nele = N_VMake_Serial(numVar, out->ele);
        retval = IDAGetEstLocalErrors(idamem, Nele);
        if(retval != IDA_SUCCESS)
            return false;
        N_VDestroy(Nele);
    }

    retval = IDAGetNonlinSolvStats(idamem, out->numNonLinIter, out->nncfails);
    if(retval != IDA_SUCCESS)
        return false;

    retval = IDAGetNumGEvals(idamem, out->ngevals);
    if(retval != IDA_SUCCESS)
        return false;

    return true;
}












