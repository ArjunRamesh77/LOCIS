#include "solversundialskinsol.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solverOptionsKinsol
solverOptionsKinsol::solverOptionsKinsol() :
    solverOptions(),
    errCb(NULL),
    infoCb(NULL),
    strategy(KIN_NONE),
    matrixType(MATRIX_DENSE),
    linearSolverType(LINSOLV_KINSOL_INTRINSIC_DENSE),
    noInitSetup(SUNFALSE),
    noResMon(SUNFALSE),
    maxSetupCalls(10),
    maxSubSetupCalls(5),
    etaChoice(KIN_ETACHOICE1),
    etaConst(0.1),
    gamma(0.9),
    alpha(2.0),
    omegaConst(0.9),
    omegaMin(0.00001),
    omegaMax(0.9),
    noMineps(SUNFALSE),
    maxNewtonStep(0.0),
    maxBetaFails(0.0),
    relErrFunc(0.0),
    scStepTol(0),
    varCons(NULL),
    maa(0)
{
    maxIter = 200;
    traceLevel = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solverOutputKinsol
solverOutputKinsol::solverOutputKinsol() :
    solverOptions(),
    lenrw(0),
    leniw(0),
    nbcfails(0),
    nbacktr(0),
    stepLength(0.0),
    lenrwLS(0),
    leniwLS(0),
    nfevalsLS(0),
    lsflag(0)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solverUserDataKinsol
solverKinsolUserData::solverKinsolUserData() :
    numVars(0),
    jacType(MATRIX_DENSE),
    xOrig(NULL),
    res(NULL),
    jac(NULL)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solverSundialsKinsol
solverSundialsKinsol::solverSundialsKinsol() :
    solverNonLinearAlg(SOLVER_ALG_NONLINEAR, SOLVER_KINSOL),
    kmem(NULL),
    xGuess(NULL),
    y(NULL),
    scale(NULL),
    J(NULL),
    LS(NULL),
    userData()
{

}

solverSundialsKinsol::~solverSundialsKinsol()
{
    exit();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set the parameters for kinsol
bool solverSundialsKinsol::setSolverParameters(solverOptionsKinsol* ops)
{
    //populate base
    if(!solver::setSolverParameters(static_cast<solverOptions*>(ops)))
    {
        return false;
    }

    //check sanity
    switch (ops->matrixType)
    {
    case MATRIX_DENSE:
    case MATRIX_SPARSE_CSR:
    case MATRIX_SPARSE_CSC:
        break;

    default:
        return false
    }

    switch (ops->linearSolverType)
    {
    case LINSOLV_KINSOL_INTRINSIC_DENSE:
    case LINSOLV_KINSOL_KLU_SPARSE:
    case LINSOLV_KINSOL_LAPACK_DENSE:
         break;

    default:
        return false;
    }

    switch(ops->strategy)
    {
    case KIN_NONE:
    case KIN_LINESEARCH:
    case KIN_FP:
    case KIN_PICARD:
        break;

    default:
        return false;

    }

    switch(ops->traceLevel)
    {
    case 0:
    case 1:
    case 2:
    case 3:
        break;

    default:
        return false;

    }

    switch(ops->noInitSetup)
    {
    case SUNFALSE:
    case SUNTRUE:
        break;

    default:
        return false;
    }

    switch (ops->noResMon)
    {
    case SUNFALSE:
    case SUNTRUE:
        break;

    default:
        return false;
    }

    if(ops->maxSetupCalls < 0)
        return false;

    if(ops->noResMon == SUNFALSE)
    {
        if(ops->maxSubSetupCalls < 0)
            return false;
    }

    switch (ops->etaChoice)
    {
    case KIN_ETACHOICE1:
    case KIN_ETACHOICE2:
    case KIN_ETACONSTANT:
        break;

    default:
        return false;
    }

    if(ops->etaChoice == KIN_ETACONSTANT)
    {
        if(!(ops->etaConst > 0.0 && ops->etaConst <= 1.0))
            return false;
    }

    if(ops->etaChoice == KIN_ETACHOICE2)
    {
        if(!((ops->gamma > 0.0 && ops->gamma <= 1.0) && (ops->alpha > 1.0 && ops->alpha <= 2.0)))
            return false;
    }

    if(ops->noResMon == SUNFALSE)
    {
        if(!(ops->omegaConst > 0.0 && ops->omegaConst < 1.0))
            return false;
    }

    if(ops->noResMon == SUNFALSE)
    {
        if(!(ops->omegaMin > 0.0 && ops->omegaMin < 1.0))
            return false;

        if(!(ops->omegaMax > ops->omegaMin && ops->omegaMax < 1.0))
            return false;
    }

    switch(ops->noMineps)
    {
    case SUNFALSE:
    case SUNTRUE:
        break;
    default:
        return false;
    }

    if(ops->maxNewtonStep < 0.0)
        return false;

    if(ops->maxBetaFails < 0.0)
        return false;

    if(ops->relErrFunc < 0.0)
        return false;

    if(ops->relFTol < 0.0)
        return false;

    if(ops->scStepTol < 0.0)
        return false;

    if(ops->varCons)
    {
        for(unsigned int i = 0; i < ops->numVars; ++i)
        {
            if(!(ops->varCons[i] == 1.0 || ops->varCons[i] == 0.0 || ops->varCons[i] == -1.0 || ops->varCons[i] == -2.0))
                return false;
        }
    }

    if(ops->strategy == KIN_PICARD || ops->strategy == KIN_FP)
    {
        if(ops->maa < 0 || ops->maa >= ops->maxIter)
            return false;
    }

    //populate kinsol specific
    option = ops;
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// initialize kinsol
bool solverSundialsKinsol::init()
{
    int retval;
    solverOptionsKinsol* ops = static_cast<solverOptionsKinsol*>(option);

    //set the guess
    y = N_VMake_Serial(numVar, xGuess);
    //y = N_VNew_Serial(numVar);
    scale = N_VNew_Serial(numVar);

    //set unit scale
    double* scaleData = N_VGetArrayPointer(scale);
    for(unsigned int i = 0; i < numVar; ++i)
    {
        scaleData[i] = 1.0;
    }

    //create kinsol memory
    kmem = KINCreate();

    //setup userData
    userData.numVars = numVar;
    userData.res = residual;
    userData.jac = jacobian;
    userData.jacType = matrixType;
    userData.xOrig = xOrig;

    //set all options
    if(ops->errCb)
    {
        retval = KINSetErrHandlerFn(kmem, ops->errcb, userData);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(ops->infoCb)
    {
        retval = KINSetInfoHandlerFn(kmem, ops->infoCb, userData);
        if(retval != KIN_SUCCESS)
            return false;
    }

    retval = KINSetPrintLevel(kmem, ops->traceLevel);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINSetUserData(kmem, &userData);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINSetNumMaxIters(kmem, ops->maxIter);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINSetNoInitSetup(kmem, ops->noInitStep);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINSetNoResMon(kmem, ops->noResMon);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINSetMaxSetupCalls(kmem, ops->maxSetupCalls);
    if(retval != KIN_SUCCESS)
        return false;

    if(ops->noResMon == SUNFALSE)
    {
        retval = KINSetMaxSubSetupCalls(kmem, ops->maxSubSetupCalls);
        if(retval != KIN_SUCCESS)
            return false;
    }

    retval = KINSetEtaForm(kmem, ops->etaChoice);
    if(retval != KIN_SUCCESS)
        return false;

    if(ops->etaChoice == KIN_ETACONSTANT)
    {
        retval = KINSetEtaConstValue(kmem, ops->etaConst);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(ops->etaChoice == KIN_ETACHOICE2)
    {
        retval = KINSetEtaParams(kmem, ops->gamma, ops->alpha);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(ops->noResMon == SUNFALSE)
    {
        retval = KINSetResMonConstValue(kmem, ops->omegaConst);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(ops->noResMon == SUNFALSE)
    {
        retval = KINSetResMonParams(kmem, ops->omegaMin, ops->omegaMax);
        if(retval != KIN_SUCCESS)
            return false;
    }

    retval = KINSetNoMinEps(kmem, ops->noMineps);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINSetMaxNewtonStep(kmem, ops->maxNewtonStep);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINSetMaxBetaFails(kmem, ops->maxBetaFails);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINSetRelErrFunc(kmem, ops->relErrFunc);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINSetFuncNormTol(kmem, ops->relFTol);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINSetScaledStepTol(kmem, ops->scStepTol);
    if(retval != KIN_SUCCESS)
        return false;

    if(enableConstraintsOnVars)
    {
        retval = KINSetConstraints(kmem, ops->varCons);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(strategy == KIN_PICARD || strategy == KIN_FP)
    {
        if(ops->maa >= ops->maxIter)
            return false;

        retval = KINSetMAA(kmem, ops->maa);
        if(retval != KIN_SUCCESS)
            return false;
    }

    //init
    retval = KINInit(kmem, kinsolResidualFunction, y);
    if(retval != KIN_SUCCESS)
        return false;

    //Get matrix type
    switch(matrixType)
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
    switch (linearSolverType)
    {
    case LINSOLV_KINSOL_INTRINSIC_DENSE:
        if(matrixType == MATRIX_DENSE)
            LS = SUNDenseLinearSolver(y, J);
        else
            return false;

    case LINSOLV_KINSOL_LAPACK_DENSE:
        if(matrixType == MATRIX_DENSE)
            LS = SUNLapackDense(y, J);
        else
            return false;

    case LINSOLV_KINSOL_KLU_SPARSE:
        if(matrixType == MATRIX_SPARSE_CSR || matrixType == MATRIX_SPARSE_CSC)
            LS = SUNKLU(y, J);
        else
            return false;

    }

    //Set the linear solver
    retval = KINDlsSetLinearSolver(kmem, LS, J);
    if(retval != KINDLS_SUCCESS)
        return false;

    //set jacobian function
    retval = KINDlsSetJacFn(kmem, kisnsolJacobianFunction);
    if(retval != KINDLS_SUCCESS)
        return false;

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// finalize kinsol
bool solverSundialsKinsol::exit()
{
    if(kmem)
    {
        KINFree(kmem);
        kmem = NULL;
    }

    if(J)
    {
        SUNMatDestroy(J);
        J = NULL;
    }

    if(LS)
    {
        SUNLinSolFree(LS);
        LS = NULL;
    }

    if(scale)
    {
        N_VDestroy_Serial(scale);
        scale = NULL;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solve with kinsol
int solverSundialsKinsol::solve()
{
    int retval = 0;
    retval = KINSol(kmem, y, strategy, scale, scale);
    return retval;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get Output
bool solverSundialsKinsol::getSolverOutput(solverOutputKinsol *out)
{
    //output
    retval = KINGetWorkSpace(kmem, out->lenrw, out->leniw);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINGetNumFuncEvals(kmem, out->numFevals);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINGetNumBetaCondFails(kmem, out->nbcfails);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINGetNumBacktrackOps(kmem, out->nbacktr);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINGetFuncNorm(kmem, out->funcNorm);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINGetStepLength(kmem, out->xNorm);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINDlsGetWorkSpace(kmem, out->lenrwLS, out->leniwLS);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINDlsGetNumJacEvals(kmem, out->numJacevals);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINDlsGetNumFuncEvals(kmem, out->nfevalsLS);
    if(retval != KIN_SUCCESS)
        return false;

    retval = KINDlsGetLastFlag(kmem, out->lsflag);
    if(retval != KIN_SUCCESS)
        return false;

    out->workSpaceSize = out->lenrw + out->leniw + out->lenrwLS + out->leniwLS;

    return true;
}




