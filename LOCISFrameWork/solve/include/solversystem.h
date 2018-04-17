#pragma once

#include "virtualinstruction.h"
#include "blockdecomposition.h"
#include "genericresidual.h"
#include "genericjacobian.h"

//solver specific includes
#include "solversundialskinsol.h"
#include "solversundialsida.h"
#include <memory>

//utility
int getSolverTypeFromString(std::string val);
int getSolveModeFromString(std::string val);
int getSolverNamefromString(std::string val);
solver* getSolverfromSolverName(int val);
solverOptions* getSolverOptionsFromSolverName(int val);
solverOutput* getSolverOutputFromSolverName(int val);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// encapsulates all information about a system of equations for the solverkernel
class solverSystem
{
    //direct
    solverSystem* daeInitSystem;
    int systemType;
    int solveMode;
    int solverType;
    int solverName;
    rapidjson::Value* solveInput;
    rapidjson::Value* solveOutput;
    unsigned int numVar;
    unsigned int numEqu;
    std::vector<virtualOper>* pEquationVec; //direct mode

    //time
    double tStart;
    double tEnd;
    unsigned int numSteps;

    //variables(Central)
    double* varx;
    double* varyy;
    double* varyp;

    //solver
    solver* solv;
    solverOptions* sops;
    solverOutput* sout;
    int numRoots;

    //block decomposition
    solverSystem* root;
    unsigned int blockId;
    unsigned int numBlocks;
    std::vector<solverSystem*> solveBlocks; //only the root node holds all the sub-blocks
    std::vector<unsigned int>* mapVarX;
    std::vector<unsigned int>* mapVarYY;
    std::vector<unsigned int>* mapVarYP;
    int blockAlgSolver;
    int blockDaeSolver;
    rapidjson::Value* blockSolverInputAlg;
    rapidjson::Value* blockSolverOutputAlg;
    rapidjson::Value* blockSolverInputDae;
    rapidjson::Value* blockSolverOutputDae;

public:
    solverSystem();
    ~solverSystem();

    //setter
    void setSystemType(int val);
    void setSolveMode(int val);
    void setSolverType(int val);
    void setSolverName(int val);
    void setSolverInputAndOutput(rapidjson::Value *in, rapidjson::Value *out);
    void setDaeInitSystem(solverSystem* daeInitSystem_arg);
    void setTimeRange(double tStart_arg, double tEnd_arg, unsigned int nSteps_arg);
    bool setSystemDims(unsigned int numVar_arg, unsigned int numEqu_arg);
    bool allocateSystemDims();
    void setEquationVector(std::vector<virtualOper>* pEquationVec_arg);
    void setSolver(solver *solver_arg, solverOptions* sops_arg, solverOutput* sout_arg);
    void setBlockId(unsigned int blid);
    void setNumBlocks(unsigned int blnum);
    void addBlock(solverSystem* blsystem);
    void setBlockOptions(rapidjson::Value* val);
    void setBlockMapVarX(std::vector<unsigned int>* mapVarX_arg);
    void setBlockMapVarYY(std::vector<unsigned int>* mapVarYY_arg);
    void setBlockMapVarYP(std::vector<unsigned int>* mapVarYP_arg);
    void setBlockSolvers(int alg, int dae);
    void setBlockAlgInputOutput(rapidjson::Value* in, rapidjson::Value* out);
    void setBlockDaeInputOutput(rapidjson::Value* in, rapidjson::Value* out);

    //getter
    solverSystem *getRoot();
    solverSystem* getDaeInitSystem();
    int getSystemType();
    int getSolveMode();
    int getSolverName();
    rapidjson::Value* getSolveInput();
    rapidjson::Value *getSolveOutput();
    unsigned int getNumVar();
    unsigned int getNumEqu();
    std::vector<virtualOper> *getPEquationVec();
    int getBlockSolverAlgName();
    int getBlockSolverDaeName();
    rapidjson::Value* getBlockSolveAlgInput();
    rapidjson::Value* getBlockSolveAlgOutput();
    rapidjson::Value* getBlockSolveDaeInput();
    rapidjson::Value* getBlockSolveDaeOutput();

    //time
    double getTStart();
    double getTEnd();
    unsigned int getNumSteps();

    double* getVarX();
    double* getVarYY();
    double* getVarYP();

    solver* getSolver();
    solverOptions* getSolverOptions();
    solverOutput* getSolverOutput();

    int getNumRoots();
    genericResidual* getRootResidual();

    unsigned int getBlockId();
    unsigned int getNumBlocks();
    solverSystem* getBlockAt(unsigned int id);
    solverOptions* getBlockSolverOptions();

    //logic
    void copyBlockVarXToRoot();
    void copyBlockVarYYToRoot();
    void copyBlockVarYPToRoot();
};
