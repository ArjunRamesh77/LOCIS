#include <iostream>
#include <sstream>
#include <fstream>
#include "LOCIS_API.h"
#include "LOCIS_API_INCLUDE.h"
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

/*
std::stringstream dynResOut;
long dynFlag;

/////////////////////////////////////////////////////////////////////////////////
// Callbacks
long errorCallback(long lErrorType, const char* cMessage)
{
    std::cout << cMessage << std::endl;
    return 0;
}

long infoCallback(long lInfoType, const char* cMessage)
{
    std::cout<< cMessage;
    return 0;
}

long steadyStateResults(long lpNum, const char* cNames, double* dpValues)
{
    std::cout<<"STEADY STATE RESULTS ::::"<<std::endl;
    std::string Name;
    std::stringstream ss(cNames);
    for(long i = 0; i < lpNum; ++i)
    {
        std::getline(ss, Name);
        std::cout<< cNames[i] << "   " << dpValues[i] << std::endl;
    }
    return 0;
}

long dynamicResults(long lColIndex, long lpNum, const char* cNames, double* dpTime, double* dpValues)
{
    dynFlag = 1;
    std::string Name;
    if(lpNum == -1)
    {
        std::stringstream ss(cNames);
        dynResOut<<"t  ";
        while(std::getline(ss, Name))
        {
            dynResOut<<Name<<"  ";
        }
        dynResOut<<std::endl;
    }
    else
    {      
        dynResOut<<*dpTime<<"   ";
        for(long i = 0; i < lpNum; ++i)
        {
            dynResOut<<dpValues[i]<<"   ";
        }
        dynResOut<<std::endl;
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Entry point
int main(int argc, const char** argv)
{   
    int b;
    long instanceId = -1;
    long result = -1;
    dynFlag = 0;

    std::stringstream ss;
    std::string line;
    std::ifstream myfile (argv[1]);
    if (myfile.is_open())
    {
        while (std::getline (myfile,line) )
        {
            ss << line << '\n';
        }
        myfile.close();

        instanceId = LOCISCreate();
        result = LOCISInit(instanceId, ss.str().c_str(), errorCallback, infoCallback, steadyStateResults, dynamicResults);
        result = LOCISSolve(instanceId);

        if(dynFlag == 1)
        {
            std::cout<<"DYNAMIC RESULTS ::::"<<std::endl;
            std::string line;
            while(std::getline(dynResOut, line))
            {
                std::cout<<line<<std::endl;
            }
        }

        result = LOCISExit(instanceId, true);
    }
    return 0;
}
*/

#include "solverkernel.h"
#include "genericresidual.h"

int main()
{
    //--------------------------------- NON-LINEAR-ALG---------------------------------------------
    //kernel JSON input
    const char kernelInput[] = "{ \"root\" : {"
                               "  \"system-type\" : \"SOLVER_ALG_NONLINEAR\","
                               "  \"num-vars\" : 2,"
                               "  \"num-equs\" : 2,"
                               "  \"solution-method\" : { \"type\" : \"SOLVER_MODE_DIRECT\","
                               "                          \"solver\" : { \"type\" : \"SOLVER_ALG_NONLINEAR\","
                               "                                       \"name\" : \"SOLVER_KINSOL\","
                               "                                       \"input\" : { \"matrixType\" : 0,"
                               "                                                     \"linearSolverType\" : 0,"
                               "                                                     \"strategy\" : 0,"
                               "                                                     \"noInitSetup\" : 0,"
                               "                                                     \"relFTol\" : 1.0e-6},"
                               "                                       \"output\" : {}}}"
                               "}"
                               "}";


//    "}";



    //residual instructions
    std::vector<virtualOper> resInstr;
    resInstr.push_back(virtualOper(VR_CONST, 3.0));
    resInstr.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)0));
    resInstr.push_back(virtualOper(VR_BIN_MUL));
    resInstr.push_back(virtualOper(VR_CONST, 9.0));
    resInstr.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)1));
    resInstr.push_back(virtualOper(VR_BIN_MUL));
    resInstr.push_back(virtualOper(VR_BIN_ADD));
    resInstr.push_back(virtualOper(VR_CONST, 3.6));
    resInstr.push_back(virtualOper(VR_BIN_ADD));
    (resInstr.end() - 1)->signal = VR_SIGNAL_LAST;
    resInstr.push_back(virtualOper(VR_CONST, 8.0));
    resInstr.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)0));
    resInstr.push_back(virtualOper(VR_BIN_MUL));
    resInstr.push_back(virtualOper(VR_CONST, 1.6));
    resInstr.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)1));
    resInstr.push_back(virtualOper(VR_BIN_MUL));
    resInstr.push_back(virtualOper(VR_BIN_ADD));
    resInstr.push_back(virtualOper(VR_CONST, 5.6));
    resInstr.push_back(virtualOper(VR_BIN_ADD));
    (resInstr.end() - 1)->signal = VR_SIGNAL_LAST;

    //solver kernel
    solverKernel sk;
    sk.setFlatEquationSet(&resInstr);
    sk.setMainSystem(kernelInput);
    sk.allocateGlobalSystemVars();

    //initialize
    double* xg = NULL;
    //set the guesses
    xg = sk.getSystem()->getVarX();
    xg[0] = 1.0;
    xg[1] = 1.0;

    if(sk.initSystem())
    {
        //solve
        sk.solveSystem();
    }

    //solution
    for(int i = 0; i < 2; ++i)
        std::cout<<" x[" << i << "] = " << xg[i];

    //--------------------------------- NON-LINEAR-DAE---------------------------------------------
    //kernel
    const char kernelInput1[] ="{ \"root\" : {"
                               "  \"system-type\" : \"SOLVER_DAE_NONLINEAR\","
                               "  \"num-vars\" : 5,"
                               "  \"num-equs\" : 5,"
                               "  \"time-start\" : 0.0,"
                               "  \"time-end\" : 100.0,"
                               "  \"time-step\" : 100,"
                               "  \"solution-method\" :{ \"type\" : \"SOLVER_MODE_DIRECT\","
                               "                        \"solver\" : { \"type\" : \"SOLVER_DAE_NONLINEAR\","
                               "                                       \"name\" : \"SOLVER_IDA\","
                               "                                       \"input\" : { \"matrixType\" : 0,"
                               "                                                     \"linearSolverType\" : 0,"
                               "                                                     \"iTask\" : 1,"
                               "                                                     \"relFTol\" : 1.0e-8,"
                               "                                                     \"absFTol\" : 1.0e-8},"
                               "                                       \"output\" : {}}},"
                               "  \"init-dae\" : { \"system-type\" : \"SOLVER_ALG_NONLINEAR\","
                               "                   \"num-vars\" : 9,"
                               "                   \"num-equs\" : 9,"
                               "                   \"solution-method\" : { \"type\" : \"SOLVER_MODE_DIRECT\","
                               "                                           \"solver\" : { \"type\" : \"SOLVER_ALG_NONLINEAR\","
                               "                                                          \"name\" : \"SOLVER_KINSOL\","
                               "                                                          \"input\" : { \"matrixType\" : 0,"
                               "                                                                        \"linearSolverType\" : 0,"
                               "                                                                        \"strategy\" : 0,"
                               "                                                                        \"noInitSetup\" : 0,"
                               "                                                                        \"relFTol\" : 1.0e-6},"
                               "                                                          \"output\" : {}}}}"
                               "}"
                               "}";

    //residual
    std::vector<virtualOper> daeRes;
    // x = 0, y = 1, v = 2, w = 3, lam = 4
    //m*(v*v + w*w - g*y) - 2*lam*(x*x + y*y)
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)2));
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)2));
    daeRes.push_back(virtualOper(VR_BIN_MUL));
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)3));
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)3));
    daeRes.push_back(virtualOper(VR_BIN_MUL));
    daeRes.push_back(virtualOper(VR_BIN_ADD));
    daeRes.push_back(virtualOper(VR_CONST, 9.8));
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)1));
    daeRes.push_back(virtualOper(VR_BIN_MUL));
    daeRes.push_back(virtualOper(VR_BIN_SUB));
    daeRes.push_back(virtualOper(VR_CONST, 1.0));
    daeRes.push_back(virtualOper(VR_BIN_MUL));
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)0));
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)0));
    daeRes.push_back(virtualOper(VR_BIN_MUL));
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)1));
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)1));
    daeRes.push_back(virtualOper(VR_BIN_MUL));
    daeRes.push_back(virtualOper(VR_BIN_ADD));
    daeRes.push_back(virtualOper(VR_CONST, 2.0));
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)4));
    daeRes.push_back(virtualOper(VR_BIN_MUL));
    daeRes.push_back(virtualOper(VR_BIN_MUL));
    daeRes.push_back(virtualOper(VR_BIN_SUB));
    (daeRes.end() - 1)->signal = VR_SIGNAL_LAST;

    //$x = v;
    daeRes.push_back(virtualOper(VR_VAR2_INDEX, (unsigned int)0));
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)2));
    daeRes.push_back(virtualOper(VR_BIN_SUB));
    (daeRes.end() - 1)->signal = VR_SIGNAL_LAST;

    //$y = w;
    daeRes.push_back(virtualOper(VR_VAR2_INDEX, (unsigned int)1));
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)3));
    daeRes.push_back(virtualOper(VR_BIN_SUB));
    (daeRes.end() - 1)->signal = VR_SIGNAL_LAST;

    //m*$v + 2*x*lam = 0;
    daeRes.push_back(virtualOper(VR_CONST, 1.0));
    daeRes.push_back(virtualOper(VR_VAR2_INDEX, (unsigned int)2));
    daeRes.push_back(virtualOper(VR_BIN_MUL));
    daeRes.push_back(virtualOper(VR_CONST, 2.0));
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)0));
    daeRes.push_back(virtualOper(VR_BIN_MUL));
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)4));
    daeRes.push_back(virtualOper(VR_BIN_MUL));
    daeRes.push_back(virtualOper(VR_BIN_ADD));
    (daeRes.end() - 1)->signal = VR_SIGNAL_LAST;

    //m*$w + m*g + 2*y*lam = 0;
    daeRes.push_back(virtualOper(VR_CONST, 1.0));
    daeRes.push_back(virtualOper(VR_VAR2_INDEX, (unsigned int)3));
    daeRes.push_back(virtualOper(VR_BIN_MUL));
    daeRes.push_back(virtualOper(VR_CONST, 1.0));
    daeRes.push_back(virtualOper(VR_CONST, 9.8));
    daeRes.push_back(virtualOper(VR_BIN_MUL));
    daeRes.push_back(virtualOper(VR_BIN_ADD));
    daeRes.push_back(virtualOper(VR_CONST, 2.0));
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)1));
    daeRes.push_back(virtualOper(VR_BIN_MUL));
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)4));
    daeRes.push_back(virtualOper(VR_BIN_MUL));
    daeRes.push_back(virtualOper(VR_BIN_ADD));
    (daeRes.end() - 1)->signal = VR_SIGNAL_LAST;

    //init(12 instr)
    //x = 0;
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)0));
    daeRes.push_back(virtualOper(VR_CONST, 0.0));
    daeRes.push_back(virtualOper(VR_BIN_SUB));
    (daeRes.end() - 1)->signal = VR_SIGNAL_LAST;

    //y + s = 0;
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)1));
    daeRes.push_back(virtualOper(VR_CONST, 1.0));
    daeRes.push_back(virtualOper(VR_BIN_ADD));
    (daeRes.end() - 1)->signal = VR_SIGNAL_LAST;

    //v = 0.01;
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)2));
    daeRes.push_back(virtualOper(VR_CONST, 0.01));
    daeRes.push_back(virtualOper(VR_BIN_SUB));
    (daeRes.end() - 1)->signal = VR_SIGNAL_LAST;

    //w = 0;
    daeRes.push_back(virtualOper(VR_VAR1_INDEX, (unsigned int)3));
    daeRes.push_back(virtualOper(VR_CONST, 0.0));
    daeRes.push_back(virtualOper(VR_BIN_SUB));
    (daeRes.end() - 1)->signal = VR_SIGNAL_LAST;

    //solver kernel
    solverKernel sk1;
    sk1.setFlatEquationSet(&daeRes);
    sk1.setNumInstrInit(12);
    sk1.setMainSystem(kernelInput1);
    sk1.allocateGlobalSystemVars();

    //set the guesses
    double* yyg = NULL;
    double* ypg = NULL;
    yyg = sk1.getSystem()->getVarYY();
    ypg = sk1.getSystem()->getVarYP();

    yyg[0] = 0;
    yyg[1] = -1;
    yyg[2] = 1;
    yyg[3] = 0;
    yyg[4] = 1*(1 + 1*9.8)/(2*1*1);
    ypg[0] = 1;
    ypg[1] = 1;
    ypg[2] = 1;
    ypg[3] = 1;
    ypg[4] = 1;

    //initialize
    if(sk1.initSystem())
    {
        sk1.solveSystem();
    }

    //---------------------------------BLOCK DECOMPOSITION-SIMPLE---------------------------------------------
    const char kernelInput2[] ="{\"root\" : {"
                               "  \"system-type\" : \"SOLVER_ALG_NONLINEAR\","
                               "  \"num-vars\" : 6,"
                               "  \"num-equs\" : 6,"
                               "  \"solution-method\":{ \"type\" : \"SOLVER_MODE_BLOCK_DECOMPOSITION\","
                               "                        \"solver-dae\" : { \"type\" : \"SOLVER_DAE_NONLINEAR\","
                               "                                           \"name\" : \"SOLVER_IDA\","
                               "                                           \"input\" : { \"matrixType\" : 0,"
                               "                                                        \"linearSolverType\" : 0,"
                               "                                                        \"iTask\" : 1,"
                               "                                                        \"relFTol\" : 1.0e-8,"
                               "                                                        \"absFTol\" : 1.0e-8},"
                               "                                           \"output\" : {}},"
                               "                        \"solver-alg\" : {  \"type\" : \"SOLVER_ALG_NON_LINEAR\","
                               "                                            \"name\" : \"SOLVER_KINSOL\","
                               "                                            \"input\" : { \"matrixType\" : 0,"
                               "                                                          \"linearSolverType\" : 0,"
                               "                                                          \"strategy\" : 0,"
                               "                                                          \"noInitSetup\" : 0,"
                               "                                                          \"relFTol\" : 1.0e-10},"
                               "                                            \"output\" : {}}}"
                               "}"
                               "}";

    double C1 = 1.0;
    double C2 = 2.0;
    double C3 = 3.0;

    #define X(index) (unsigned int)(index - 1)

    std::vector<virtualOper> bdRes;
    bdRes.push_back(virtualOper(VR_CONST, C1));
    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(1)));
    bdRes.push_back(virtualOper(VR_BIN_MUL));
    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(3)));
    bdRes.push_back(virtualOper(VR_BIN_ADD));
    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(5)));
    bdRes.push_back(virtualOper(VR_BIN_ADD));
    bdRes.push_back(virtualOper(VR_CONST, C1));
    bdRes.push_back(virtualOper(VR_BIN_SUB));
    bdRes.push_back(virtualOper(VR_CONST, C2));
    bdRes.push_back(virtualOper(VR_BIN_SUB));
    bdRes.push_back(virtualOper(VR_CONST, 1.0));
    bdRes.push_back(virtualOper(VR_BIN_SUB));
    (bdRes.end() - 1)->signal = VR_SIGNAL_LAST;

    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(1)));
    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(3)));
    bdRes.push_back(virtualOper(VR_BIN_ADD));
    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(4)));
    bdRes.push_back(virtualOper(VR_BIN_ADD));
    bdRes.push_back(virtualOper(VR_CONST, 2.0));
    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(6)));
    bdRes.push_back(virtualOper(VR_BIN_MUL));
    bdRes.push_back(virtualOper(VR_BIN_ADD));
    bdRes.push_back(virtualOper(VR_CONST, 4.0));
    bdRes.push_back(virtualOper(VR_BIN_SUB));
    bdRes.push_back(virtualOper(VR_CONST, C2));
    bdRes.push_back(virtualOper(VR_BIN_SUB));
    (bdRes.end() - 1)->signal = VR_SIGNAL_LAST;

    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(1)));
    bdRes.push_back(virtualOper(VR_CONST, 2.0));
    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(3)));
    bdRes.push_back(virtualOper(VR_BIN_MUL));
    bdRes.push_back(virtualOper(VR_BIN_ADD));
    bdRes.push_back(virtualOper(VR_CONST, C3));
    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(5)));
    bdRes.push_back(virtualOper(VR_BIN_MUL));
    bdRes.push_back(virtualOper(VR_BIN_ADD));
    bdRes.push_back(virtualOper(VR_CONST, 1.0));
    bdRes.push_back(virtualOper(VR_BIN_SUB));
    bdRes.push_back(virtualOper(VR_CONST, 2.0));
    bdRes.push_back(virtualOper(VR_CONST, C2));
    bdRes.push_back(virtualOper(VR_BIN_MUL));
    bdRes.push_back(virtualOper(VR_BIN_SUB));
    bdRes.push_back(virtualOper(VR_CONST, C3));
    bdRes.push_back(virtualOper(VR_BIN_SUB));
    (bdRes.end() - 1)->signal = VR_SIGNAL_LAST;

    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(2)));
    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(3)));
    bdRes.push_back(virtualOper(VR_BIN_ADD));
    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(4)));
    bdRes.push_back(virtualOper(VR_BIN_ADD));
    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(5)));
    bdRes.push_back(virtualOper(VR_BIN_ADD));
    bdRes.push_back(virtualOper(VR_CONST, 2.0));
    bdRes.push_back(virtualOper(VR_BIN_SUB));
    bdRes.push_back(virtualOper(VR_CONST, C2));
    bdRes.push_back(virtualOper(VR_BIN_SUB));
    (bdRes.end() - 1)->signal = VR_SIGNAL_LAST;

    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(1)));
    bdRes.push_back(virtualOper(VR_CONST, C2));
    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(3)));
    bdRes.push_back(virtualOper(VR_BIN_MUL));
    bdRes.push_back(virtualOper(VR_BIN_SUB));
    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(5)));
    bdRes.push_back(virtualOper(VR_BIN_ADD));
    bdRes.push_back(virtualOper(VR_CONST, 2.0));
    bdRes.push_back(virtualOper(VR_BIN_SUB));
    bdRes.push_back(virtualOper(VR_CONST, C2*C2));
    bdRes.push_back(virtualOper(VR_BIN_ADD));
    (bdRes.end() - 1)->signal = VR_SIGNAL_LAST;

    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(1)));
    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(3)));
    bdRes.push_back(virtualOper(VR_BIN_SUB));
    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(4)));
    bdRes.push_back(virtualOper(VR_BIN_ADD));
    bdRes.push_back(virtualOper(VR_VAR1_INDEX, X(6)));
    bdRes.push_back(virtualOper(VR_BIN_SUB));
    bdRes.push_back(virtualOper(VR_CONST, 1.0));
    bdRes.push_back(virtualOper(VR_BIN_SUB));
    bdRes.push_back(virtualOper(VR_CONST, C2));
    bdRes.push_back(virtualOper(VR_BIN_ADD));
    (bdRes.end() - 1)->signal = VR_SIGNAL_LAST;

    //solver kernel
    solverKernel sk2;
    sk2.setFlatEquationSet(&bdRes);
    sk2.setMainSystem(kernelInput2);
    sk2.allocateGlobalSystemVars();

    //initialize
    double* xg2 = NULL;
    //set the guesses
    xg2 = sk2.getSystem()->getVarX();
    xg2[0] = 1.0;
    xg2[1] = 2.0;
    xg2[2] = 3.0;
    xg2[3] = 4.0;
    xg2[4] = 5.0;
    xg2[5] = 6.0;

    if(sk2.initSystem())
    {
        //solve
        sk2.solveSystem();
    }

    //solution
    for(int i = 0; i < 6; ++i)
        std::cout<<" x[" << i << "] = " << xg2[i];


    //---------------------------------BLOCK DECOMPOSITION---------------------------------------------
    const char kernelInput3[] ="{ \"root\" : {"
                               "  \"system-type\" : \"SOLVER_DAE_NONLINEAR\","
                               "  \"num-vars\" : 2,"
                               "  \"num-equs\" : 2,"
                               "  \"solution-method\" { \"type\" : \"BLOCK_DECOMPOSITION\","
                               "                        \"solver-dae\" : { \"type\" : \"SOLVER_DAE_NONLINEAR\","
                               "                                            \"name\" : \"SOLVER_IDA\","
                               "                                            \"input\" : { \"matrixType\" : 0,"
                               "                                                     \"linearSolverType\" : 0,"
                               "                                                     \"iTask\" : 1,"
                               "                                                     \"relFTol\" : 1.0e-8,"
                               "                                                     \"absFTol\" : 1.0e-8},"
                               "                                            \"output\" : {}},"
                               "                        \"solver-alg\" : {  \"type\" : \"SOLVER_ALG_NON_LINEAR\","
                               "                                            \"input\" : { \"matrixType\" : 0,"
                               "                                                      \"linearSolverType\" : 0,"
                               "                                                      \"strategy\" : 0,"
                               "                                                      \"noInitSetup\" : 0,"
                               "                                                      \"relFTol\" : 1.0e-6},"
                               "                                            \"output\" : {}}},"
                               "  \"init-dae\" : { \"system-type\" : \"SOLVER_ALG_NONLINEAR\","
                               "                   \"num-vars\" : 9,"
                               "                   \"num-equs\" : 9,"
                               "                   \"solution-method\" : { \"type\" : \"DIRECT\","
                               "                                           \"solver\" : { \"type\" : \"SOLVER_ALG_NON_LINEAR\","
                               "                                                          \"input\" : { \"matrixType\" : 0,"
                               "                                                                        \"linearSolverType\" : 0,"
                               "                                                                        \"strategy\" : 0,"
                               "                                                                        \"noInitSetup\" : 0,"
                               "                                                                        \"relFTol\" : 1.0e-6},"
                               "                                                          \"output\" : {}}}}"
                               " }"
                               "}";


   return 0;
}
