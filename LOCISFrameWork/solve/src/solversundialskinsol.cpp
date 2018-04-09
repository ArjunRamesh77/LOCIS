#include "solversundialskinsol.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solverOptionsKinsol
solverOptionsKinsol::solverOptionsKinsol() :
    solverOptions(),
    errCb(NULL),
    infoCb(NULL),
    strategy(KIN_NONE),
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
    solverOutput(),
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
    solverNonLinearAlg(SOLVER_ALG_NONLINEAR, "SOLVER_KINSOL"),
    kmem(NULL),
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
// initialize kinsol
bool solverSundialsKinsol::init(solverOptions *ops1)
{
    int retval = 0;

    //set the guess
    y = N_VMake_Serial(numVar, xGuess);
    scale = N_VNew_Serial(numVar);

    //set unit scale
    double* scaleData = N_VGetArrayPointer(scale);
    for(unsigned int i = 0; i < numVar; ++i)
    {
        scaleData[i] = 1.0;
    }

    //create kinsol memory
    kmem = KINCreate();

    //set all options
    solverOptionsKinsol* ops = static_cast<solverOptionsKinsol*>(ops1);
    if(ops->rjw.hasMember("errCb"))
    {
        if(!ops->errCb)
            return false;

        retval = KINSetErrHandlerFn(kmem, ops->errCb, &userData);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(ops->rjw.hasMember("infoCb"))
    {
        if(!ops->infoCb)
            return false;

        retval = KINSetInfoHandlerFn(kmem, ops->infoCb, &userData);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(RJ_GETINT(matrixType))
    {
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

        default:
            return false;

        }
    }

    if(RJ_GETINT(linearSolverType))
    {
        switch (ops->linearSolverType)
        {
        case LINSOLV_KINSOL_INTRINSIC_DENSE:
            if(ops->matrixType == MATRIX_DENSE)
                LS = SUNDenseLinearSolver(y, J);
            else
                return false;

//        case LINSOLV_KINSOL_LAPACK_DENSE:
//            if(ops->matrixType == MATRIX_DENSE)
//                LS = SUNLapackDense(y, J);
//            else
//                return false;

//        case LINSOLV_KINSOL_KLU_SPARSE:
//            if(ops->matrixType == MATRIX_SPARSE_CSR || matrixType == MATRIX_SPARSE_CSC)
//                LS = SUNKLU(y, J);
//            else
//                return false;
        }
    }

    if(RJ_GETINT(strategy))
    {
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
    }

    if(RJ_GETINT(traceLevel))
    {
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

        retval = KINSetPrintLevel(kmem, ops->traceLevel);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(RJ_GETINT(maxIter))
    {
        retval = KINSetNumMaxIters(kmem, ops->maxIter);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(RJ_GETINT(noInitSetup))
    {
        switch(ops->noInitSetup)
        {
        case SUNFALSE:
        case SUNTRUE:
            break;

        default:
            return false;
        }

        retval = KINSetNoInitSetup(kmem, ops->noInitSetup);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(RJ_GETINT(noResMon))
    {
        switch (ops->noResMon)
        {
        case SUNFALSE:
        case SUNTRUE:
            break;

        default:
            return false;
        }

        retval = KINSetNoResMon(kmem, ops->noResMon);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(RJ_GETINT(maxSetupCalls))
    {
        if(ops->maxSetupCalls < 0)
            return false;

        retval = KINSetMaxSetupCalls(kmem, ops->maxSetupCalls);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(RJ_GETINT(maxSubSetupCalls))
    {
        if(ops->noResMon == SUNFALSE)
        {
            if(ops->maxSubSetupCalls < 0)
                return false;

            retval = KINSetMaxSubSetupCalls(kmem, ops->maxSubSetupCalls);
            if(retval != KIN_SUCCESS)
                return false;
        }
    }

    if(RJ_GETINT(etaChoice))
    {
        switch (ops->etaChoice)
        {
        case KIN_ETACHOICE1:
        case KIN_ETACHOICE2:
        case KIN_ETACONSTANT:
            break;

        default:
            return false;
        }

        retval = KINSetEtaForm(kmem, ops->etaChoice);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(RJ_GETDOUBLE(etaConst))
    {
        if(ops->etaChoice == KIN_ETACONSTANT)
        {
            if(!(ops->etaConst > 0.0 && ops->etaConst <= 1.0))
                return false;

            retval = KINSetEtaConstValue(kmem, ops->etaConst);
            if(retval != KIN_SUCCESS)
                return false;
        }
    }

    if(RJ_GETDOUBLE(gamma) && RJ_GETDOUBLE(alpha))
    {
        if(ops->etaChoice == KIN_ETACHOICE2)
        {
            if(!((ops->gamma > 0.0 && ops->gamma <= 1.0) && (ops->alpha > 1.0 && ops->alpha <= 2.0)))
                return false;
        }

        if(ops->etaChoice == KIN_ETACHOICE2)
        {
            retval = KINSetEtaParams(kmem, ops->gamma, ops->alpha);
            if(retval != KIN_SUCCESS)
                return false;
        }
    }

    if(RJ_GETDOUBLE(omegaConst))
    {
        if(ops->noResMon == SUNFALSE)
        {
            if(!(ops->omegaConst > 0.0 && ops->omegaConst < 1.0))
                return false;

            retval = KINSetResMonConstValue(kmem, ops->omegaConst);
            if(retval != KIN_SUCCESS)
                return false;
        }
    }

    if(RJ_GETDOUBLE(omegaMin) && RJ_GETDOUBLE(omegaMax))
    {
        if(ops->noResMon == SUNFALSE)
        {
            if(!(ops->omegaMin > 0.0 && ops->omegaMin < 1.0))
                return false;

            if(!(ops->omegaMax > ops->omegaMin && ops->omegaMax < 1.0))
                return false;

            retval = KINSetResMonParams(kmem, ops->omegaMin, ops->omegaMax);
            if(retval != KIN_SUCCESS)
                return false;
        }
    }

    if(RJ_GETINT(noMineps))
    {
        switch(ops->noMineps)
        {
        case SUNFALSE:
        case SUNTRUE:
            break;
        default:
            return false;
        }

        retval = KINSetNoMinEps(kmem, ops->noMineps);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(RJ_GETDOUBLE(maxNewtonStep))
    {
        if(ops->maxNewtonStep < 0.0)
            return false;

        retval = KINSetMaxNewtonStep(kmem, ops->maxNewtonStep);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(RJ_GETINT(maxBetaFails))
    {
        if(ops->maxBetaFails < 0.0)
            return false;

        retval = KINSetMaxBetaFails(kmem, ops->maxBetaFails);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(RJ_GETDOUBLE(relErrFunc))
    {
        if(ops->relErrFunc < 0.0)
            return false;

        retval = KINSetRelErrFunc(kmem, ops->relErrFunc);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(RJ_GETDOUBLE(relFTol))
    {
        if(ops->relFTol < 0.0)
            return false;

        retval = KINSetFuncNormTol(kmem, ops->relFTol);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(RJ_GETDOUBLE(scStepTol))
    {
        if(ops->scStepTol < 0.0)
            return false;

        retval = KINSetScaledStepTol(kmem, ops->scStepTol);
        if(retval != KIN_SUCCESS)
            return false;
    }

    ops->varCons = NULL;
    if(!RJ_GET_DOUBLE_ARRAY(varCons, ops->size_varCons))
    {
        ops->putToBinaryFetch("varCons", DATATYPE_DOUBLE, ops->varCons, ops->size_varCons);
    }
    if(ops->varCons)
    {
        for(unsigned int i = 0; i < ops->numVars; ++i)
        {
            if(!(ops->varCons[i] == 1.0 || ops->varCons[i] == 0.0 || ops->varCons[i] == -1.0 || ops->varCons[i] == -2.0))
                return false;
        }

        ops->NvarCons = N_VMake_Serial(numVar, ops->varCons);
        retval = KINSetConstraints(kmem, ops->NvarCons);
        if(retval != KIN_SUCCESS)
            return false;
    }

    if(RJ_GETINT(maa))
    {
        if(ops->strategy == KIN_PICARD || ops->strategy == KIN_FP)
        {
            if(ops->maa < 0 || ops->maa >= ops->maxIter)
                return false;

            if(ops->maa >= ops->maxIter)
                return false;

            retval = KINSetMAA(kmem, ops->maa);
            if(retval != KIN_SUCCESS)
                return false;
        }
    }

    option = ops;

    //setup jacobian and residual
    buildResidualAndJacobian(pEquationVec);

    //setup userData
    userData.numVars = numVar;
    userData.res = residual;
    userData.jac = jacobian;
    userData.jacType = option->matrixType;
    userData.xOrig = xOrig;

    retval = KINSetUserData(kmem, &userData);
    if(retval != KIN_SUCCESS)
        return false;

    //init
    retval = KINInit(kmem, kinsolResidualFunction, y);
    if(retval != KIN_SUCCESS)
        return false;

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
// solve with kinsol
bool solverSundialsKinsol::solve()
{
    int retval = 0;
    retval = KINSol(kmem, y, static_cast<solverOptionsKinsol*>(option)->strategy, scale, scale);
    if(retval == KIN_SUCCESS)
        return true;
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get Output
bool solverSundialsKinsol::getSolverOutput(solverOutput *out1)
{
    //output
    solverOutputKinsol* out = static_cast<solverOutputKinsol*>(out1);
    int retval = 0;
    out->rjw.createNewObjectValue();

    if(RJ_OUT_HAS(lenrw) || RJ_OUT_HAS(leniw))
    {
        retval = KINGetWorkSpace(kmem, &out->lenrw, &out->leniw);
        if(retval == KIN_SUCCESS)
        {
            RJ_ADDINT(lenrw);
            RJ_ADDINT(leniw);
        }
    }

    if(RJ_OUT_HAS(numFevals))
    {
        retval = KINGetNumFuncEvals(kmem, &out->numFevals);
        if(retval == KIN_SUCCESS)
            RJ_ADDINT(numFevals);
    }

    if(RJ_OUT_HAS(nbcfails))
    {
        retval = KINGetNumBetaCondFails(kmem, &out->nbcfails);
        if(retval == KIN_SUCCESS)
            RJ_ADDINT(nbcfails);
    }

    if(RJ_OUT_HAS(nbacktr))
    {
        retval = KINGetNumBacktrackOps(kmem, &out->nbacktr);
        if(retval == KIN_SUCCESS)
            RJ_ADDINT(nbacktr);
    }

    if(RJ_OUT_HAS(funcNorm))
    {
        retval = KINGetFuncNorm(kmem, &out->funcNorm);
        if(retval == KIN_SUCCESS)
            RJ_ADDDOUBLE(funcNorm);
    }

    if(RJ_OUT_HAS(xNorm))
    {
        retval = KINGetStepLength(kmem, &out->xNorm);
        if(retval == KIN_SUCCESS)
            RJ_ADDDOUBLE(xNorm);
    }

    if(RJ_OUT_HAS(lenrwLS) || RJ_OUT_HAS(leniwLS))
    {
        retval = KINDlsGetWorkSpace(kmem, &out->lenrwLS, &out->leniwLS);
        if(retval == KIN_SUCCESS)
        {
            RJ_ADDINT(lenrwLS);
            RJ_ADDINT(leniwLS);
        }
    }

    if(RJ_OUT_HAS(numJacevals))
    {
        retval = KINDlsGetNumJacEvals(kmem, &out->numJacevals);
        if(retval == KIN_SUCCESS)
            RJ_ADDINT(numJacevals);
    }

    if(RJ_OUT_HAS(nfevalsLS))
    {
        retval = KINDlsGetNumFuncEvals(kmem, &out->nfevalsLS);
        if(retval == KIN_SUCCESS)
            RJ_ADDINT(nfevalsLS);
    }

    if(RJ_OUT_HAS(lsflag))
    {
        retval = KINDlsGetLastFlag(kmem, &out->lsflag);
        if(retval == KIN_SUCCESS)
            RJ_ADDINT(lsflag);
    }

    if(RJ_OUT_HAS(workSpaceSize))
    {
        out->workSpaceSize = out->lenrw + out->leniw + out->lenrwLS + out->leniwLS;
        RJ_ADDINT(workSpaceSize);
    }

    //solution
    out->xSol = xGuess;
    out->sizeX = numVar;

    if(numVar < MAX_ARRAY_SIZE_IN_JSON)
        RJ_ADDDOUBLE_ARRAY(xSol, out->sizeX);
    else
        out->putToBinarySend("xSol", DATATYPE_DOUBLE, out->xSol, out->sizeX);

    out->rjw.addObjectToDoc("SOLVER_OUTPUT_KINSOL");

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// finalize kinsol
bool solverSundialsKinsol::exit()
{
    if(kmem)
    {
        KINFree(&kmem);
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

