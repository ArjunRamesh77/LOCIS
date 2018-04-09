#include "solversundialsida.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set all the solver parameters for IDA
solverOptionsIda::solverOptionsIda() :
    solverOptions(),
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
    solverOutput(),
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
    sizeEweight = num;
}

void solverOutputIda::buildEle(long num)
{
    if(ele)
    {
        delete ele;
        ele = NULL;
    }
    ele = new double[num];
    sizeEle = num;
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
    solverNonLinearDae(SOLVER_ALG_NONLINEAR, "SOLVER_IDA"),
    idamem(NULL),
    yy(NULL),
    yp(NULL),
    J(NULL),
    LS(NULL),
    userData(),
    rootsFound(NULL)
{

}

solverSundialsIda::~solverSundialsIda()
{
    exit();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// initialize the IDA solver
bool solverSundialsIda::init(solverOptions *ops1)
{
    int retval = 0;

    yy = N_VMake_Serial(numVar, yyGuess);
    yp = N_VMake_Serial(numVar, ypGuess);

    //create IDA memory
    idamem = IDACreate();

    //set all options
    solverOptionsIda* ops = static_cast<solverOptionsIda*>(ops1);

    if(ops->rjw.hasMember("errcb"))
    {
        if(!ops->errCb)
            return false;

        retval = IDASetErrHandlerFn(idamem, ops->errCb, &userData);
        if(retval != IDA_SUCCESS)
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

        }
    }

    if(RJ_GETINT(linearSolverType))
    {
        switch (ops->linearSolverType)
        {
        case LINSOLV_IDA_INTRINSIC_DENSE:
            if(ops->matrixType == MATRIX_DENSE)
                LS = SUNDenseLinearSolver(yy, J);
            else
                return false;

//        case LINSOLV_IDA_LAPACK_DENSE:
//            if(matrixType == MATRIX_DENSE)
//                LS = SUNLapackDense(yy, J);
//            else
//                return false;

//        case LINSOLV_IDA_KLU_SPARSE:
//            if(matrixType == MATRIX_SPARSE_CSR || matrixType == MATRIX_SPARSE_CSC)
//                LS = SUNKLU(yy, J);
//            else
//                return false;
        }
    }

    if(RJ_GETINT(maxOrd))
    {
        if(ops->maxOrd <= 0)
            return false;

        retval = IDASetMaxOrd(idamem, ops->maxOrd);
        if(retval != IDA_SUCCESS)
            return false;
    }

    if(RJ_GETINT(maxNumSteps))
    {
        if(ops->maxNumSteps < 0)
            return false;

        retval = IDASetMaxNumSteps(idamem, ops->maxNumSteps);
        if(retval != IDA_SUCCESS)
            return false;
    }

    if(RJ_GETDOUBLE(initStep))
    {
        if(ops->initStep < 0.0)
            return false;

        retval = IDASetInitStep(idamem, ops->initStep);
        if(retval != IDA_SUCCESS)
            return false;
    }

    if(RJ_GETDOUBLE(maxStep))
    {
        if(ops->maxStep < 0.0)
            return false;

        retval = IDASetMaxStep(idamem, ops->maxStep);
        if(retval != IDA_SUCCESS)
            return false;
    }

    if(RJ_GETDOUBLE(stopTime))
    {
        if(ops->stopTime < 0.0)
            return false;
    }

    if(RJ_GETINT(maxErrorTestFails))
    {
        if(ops->maxErrorTestFails <= 0)
            return false;

        retval = IDASetMaxErrTestFails(idamem, ops->maxErrorTestFails);
        if(retval != IDA_SUCCESS)
            return false;
    }

    if(RJ_GETINT(maxNonLinIters))
    {
        if(ops->maxNonLinIters <= 0)
            return false;

        retval = IDASetMaxNonlinIters(idamem, ops->maxNonLinIters);
        if(retval != IDA_SUCCESS)
            return false;
    }

    if(RJ_GETINT(maxConvFails))
    {
        if(ops->maxConvFails <= 0)
            return false;

        retval = IDASetMaxConvFails(idamem, ops->maxConvFails);
        if(retval != IDA_SUCCESS)
            return false;
    }

    if(RJ_GETDOUBLE(nonLinConvCoef))
    {
        if(ops->nonLinConvCoef <= 0.0)
            return false;

        retval = IDASetNonlinConvCoef(idamem, ops->nonLinConvCoef);
        if(retval != IDA_SUCCESS)
            return false;
    }

    if(RJ_GETINT(suppressAlg))
    {
        switch(ops->suppressAlg)
        {
        case SUNTRUE:
        case SUNFALSE:
            break;

        default:
            return false;
        }

        retval = IDASetSuppressAlg(idamem, ops->suppressAlg);
        if(retval != IDA_SUCCESS)
            return false;
    }

//    if(ops->id)
//    {
//        for(unsigned int i = 0; i < ops->numVars; ++i)
//        {
//            if(!(ops->id[i] == 1.0 || ops->id[i] == 0.0))
//                return false;
//        }
//    }
//    if(ops->id)
//    {
//        Nid = N_VMake_Serial(numVar, ops->id);
//        retval = IDASetId(idamem, Nid);
//        if(retval != IDA_SUCCESS)
//            return false;
//    }

    ops->varCons = NULL;
    if(!RJ_GET_DOUBLE_ARRAY(varCons, ops->sizeVarcons))
    {
        ops->putToBinaryFetch("varCons", DATATYPE_DOUBLE, ops->varCons, ops->sizeVarcons);
    }
    if(ops->varCons)
    {
        for(unsigned int i = 0; i < ops->numVars; ++i)
        {
            if(!(ops->varCons[i] == 1.0 || ops->varCons[i] == 0.0 || ops->varCons[i] == -1.0 || ops->varCons[i] == -2.0))
                return false;
        }

        ops->NvarCons = N_VMake_Serial(numVar, ops->varCons);
        retval = IDASetConstraints(idamem,  ops->NvarCons);
        if(retval != IDA_SUCCESS)
            return false;
    }

    if(RJ_GETINT(nroots))
    {
        if(ops->nroots < 0)
            return false;
    }

    ops->rootdir = NULL;

    if(ops->rjw.getIntArray("rootdir", ops->sizeRootdir, ops->rootdir))
    //if(!RJ_GET_INT_ARRAY(rootdir, ops->sizeRootdir))
    {
        ops->putToBinaryFetch("rootdir", DATATYPE_DOUBLE, ops->rootdir, ops->sizeRootdir);
    }
    if(ops->rootdir)
    {
        if(ops->sizeRootdir != ops->nroots)
            return false;

        if(ops->nroots == 0)
            return false;

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

    if(RJ_GETDOUBLE(absFTol))
    {
        if(ops->absFTol < 0.0)
            return false;
    }

    if(RJ_GETDOUBLE(relFTol))
    {
        if(ops->relFTol < 0.0)
            return false;
    }

    ops->vAbsFtol = NULL;
    if(!RJ_GET_DOUBLE_ARRAY(vAbsFtol, ops->sizeVAbsFtol))
    {
        ops->putToBinaryFetch("vAbsFtol", DATATYPE_DOUBLE, ops->vAbsFtol, ops->sizeVAbsFtol);
    }
    if(ops->vAbsFtol)
    {
        for(unsigned int i = 0; i < ops->numVars; ++i)
        {
            if(!(ops->vAbsFtol[i] < 0.0))
                return false;
        }
        ops->NvAbsFTol = N_VMake_Serial(ops->sizeVAbsFtol, ops->vAbsFtol);
    }

    if(RJ_GETINT(iTask))
    {
        switch(ops->iTask)
        {
        case IDA_NORMAL:
        case IDA_ONE_STEP:
            iTask = ops->iTask;
            break;

        default:
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
    userData.jacType = ops->matrixType;
    userData.yyOrig = yyOrig;
    userData.ypOrig = ypOrig;
    userData.interf = this;

    retval = IDASetUserData(idamem, &userData);
    if(retval != IDA_SUCCESS)
        return false;

    //init
    double t0 = 0.0;
    retval = IDAInit(idamem, idaResidualFunction, t0, yy, yp);
    if(retval != IDA_SUCCESS)
        return false;

    //specify tolerances
    retval = IDASStolerances(idamem, ops->relFTol, ops->absFTol);
    if(retval != IDA_SUCCESS)
        return false;

    if(ops->vAbsFtol)
    {
        retval = IDASVtolerances(idamem, ops->relFTol, ops->NvAbsFTol);
        if(retval != IDA_SUCCESS)
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
        retval = IDARootInit(idamem, ops->nroots, idaRootFunction);
        if(retval != IDA_SUCCESS)
            return false;

        retval = IDASetRootDirection(idamem, ops->rootdir);
        if(retval != IDA_SUCCESS)
            return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// solve a time step
bool solverSundialsIda::solve(double time)
{
    int retval = 0;
    double tret = 0.0;
    bGotRoot = false;
    retval = IDASolve(idamem, time, &tret, yy, yp, static_cast<solverOptionsIda*>(option)->iTask);

    if(retval == IDA_ROOT_RETURN)
    {
       int retval1 = 0;
       retval1 = IDAGetRootInfo(idamem, rootsFound);
       if(retval1 == IDA_SUCCESS)
       {
            bGotRoot = true;
       }
       return true;
    }

    if(retval == IDA_SUCCESS)
    {
        return true;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get the solver output
bool solverSundialsIda::getSolverOutput(solverOutput *out1)
{
    solverOutputIda* out = static_cast<solverOutputIda*>(out1);
    int retval = 0;
    out->rjw.createNewObjectValue();

    if(RJ_OUT_HAS(lenrw) || RJ_OUT_HAS(leniw))
    {
        retval = IDAGetWorkSpace(idamem, &out->lenrw, &out->leniw);
        if(retval == IDA_SUCCESS)
        {
            RJ_ADDINT(lenrw);
            RJ_ADDINT(leniw);
        }
    }

    if(RJ_OUT_HAS(lenrwLS) || RJ_OUT_HAS(leniwLS))
    {
        retval = IDADlsGetWorkSpace(idamem, &out->lenrwLS, &out->leniwLS);
        if(retval != IDA_SUCCESS)
        {
            RJ_ADDINT(lenrwLS);
            RJ_ADDINT(leniwLS);
        }
    }

    if(RJ_OUT_HAS(workSpaceSize))
    {
        out->workSpaceSize = out->lenrw + out->leniw + out->lenrwLS + out->leniwLS;
        RJ_ADDINT(workSpaceSize);
    }

    if(RJ_OUT_HAS(nsteps))
    {
        retval = IDAGetIntegratorStats(idamem, &out->nsteps, &out->numFevals, &out->nlinsetups, &out->netfails, &out->klast, &out->kcur, &out->hinused,
                                                &out->hlast, &out->hcur, &out->tcur);
        if(retval == IDA_SUCCESS)
        {
            RJ_ADDINT(nsteps);
            RJ_ADDINT(numFevals);
            RJ_ADDINT(nlinsetups);
            RJ_ADDINT(netfails);
            RJ_ADDINT(klast);
            RJ_ADDINT(kcur);
            RJ_ADDDOUBLE(hinused);
            RJ_ADDDOUBLE(hlast);
            RJ_ADDDOUBLE(hcur);
            RJ_ADDDOUBLE(tcur);
        }
    }

    if(RJ_OUT_HAS(out->tolsfac))
    {
        retval = IDAGetTolScaleFactor(idamem, &out->tolsfac);
        if(retval == IDA_SUCCESS)
            RJ_ADDDOUBLE(tolsfac);
    }

    if(RJ_OUT_HAS(eweight))
    {
        if(!out->eweight)
            out->buildEweight(numVar);

        if(out->eweight)
        {
            N_Vector Neweight = N_VMake_Serial(numVar, out->eweight);
            retval = IDAGetErrWeights(idamem, Neweight);
            if(retval == IDA_SUCCESS)
            {
                if(numVar < MAX_ARRAY_SIZE_IN_JSON)
                    RJ_ADDDOUBLE_ARRAY(eweight, numVar);
                else
                    out->putToBinarySend("eweight", DATATYPE_DOUBLE, out->eweight, numVar);
            }
            N_VDestroy(Neweight);
        }
    }

    if(RJ_OUT_HAS(ele))
    {
        if(!out->ele)
            out->buildEle(numVar);

        if(out->ele)
        {
            N_Vector Nele = N_VMake_Serial(numVar, out->ele);
            retval = IDAGetEstLocalErrors(idamem, Nele);
            if(retval == IDA_SUCCESS)
            {
                if(numVar < MAX_ARRAY_SIZE_IN_JSON)
                    RJ_ADDDOUBLE_ARRAY(ele, numVar);
                else
                    out->putToBinarySend("ele", DATATYPE_DOUBLE, out->ele, numVar);
            }
            N_VDestroy(Nele);
        }
    }

    if(RJ_OUT_HAS(nncfails) || RJ_OUT_HAS(numNonLinIter))
    {
        retval = IDAGetNonlinSolvStats(idamem, &out->numNonLinIter, &out->nncfails);
        if(retval == IDA_SUCCESS)
        {
            RJ_ADDINT(numNonLinIter);
            RJ_ADDINT(nncfails);
        }
    }

    if(RJ_OUT_HAS(gevals))
    {
        retval = IDAGetNumGEvals(idamem, &out->gevals);
        if(retval == IDA_SUCCESS)
        {
            RJ_ADDINT(gevals);
        }
    }

    //solution
    out->yySol = yyGuess;
    out->ypSol = ypGuess;
    out->sizeYY = numVar;
    out->sizeYP = numVar;

    if(numVar < MAX_ARRAY_SIZE_IN_JSON)
    {
        RJ_ADDDOUBLE_ARRAY(yySol, numVar);
        RJ_ADDDOUBLE_ARRAY(ypSol, numVar);
    }
    else
    {
        out->putToBinarySend("yySol", DATATYPE_DOUBLE, out->yySol, numVar);
        out->putToBinarySend("ypSol", DATATYPE_DOUBLE, out->ypSol, numVar);
    }

    out->rjw.addObjectToDoc("SOLVER_OUTPUT_IDA");

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// finalize the IDA solver
bool solverSundialsIda::exit()
{
    if(idamem)
    {
        IDAFree(&idamem);
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

    if(rootsFound)
    {
        delete rootsFound;
        rootsFound = NULL;
    }

    return true;
}
