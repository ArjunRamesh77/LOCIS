#pragma once

#include <stdio.h>
#include "virtualinstruction.h"
#include <map>
#include <assert.h>
#include "lociscommon.h"

// these are aid classes for implementing the reverse mode automatic differentiation

#define VDT_NORMAL 0
#define VDT_INDEX1 2
#define VDT_INDEX2 3

enum matrixTypes
{
    MATRIX_DENSE,
    MATRIX_SPARSE_CSR,
    MATRIX_SPARSE_CSC
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dependency tree node
class virtualDependencyTreeNode
{
    __int8_t nodeType;
    unsigned int wIndex;
    std::vector<virtualDependencyTreeNode*> to;
    unsigned int* wIndexDeriv;

public:
    virtualDependencyTreeNode();
    virtualDependencyTreeNode(__int8_t nodeType_arg, unsigned int wIndex_arg);
    ~virtualDependencyTreeNode();

    void connectTo(virtualDependencyTreeNode* node);
    std::vector<virtualDependencyTreeNode *> &getAllTo();
    __int8_t getNodeType() const;
    unsigned int getWIndex() const;
    void setNodeType(const __int8_t &value);
    void setWIndex(const unsigned int &value);
    unsigned int *getWIndexDeriv() const;
    void setWIndexDeriv(unsigned int *value);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// builds the dependency tree from a residual function stack
class virtualDependencyTree
{
    std::map<unsigned int, virtualDependencyTreeNode*> indepVariables;
    std::vector<virtualOper>* instr;
    std::map<unsigned int, unsigned int> interBegin;
    std::vector<virtualOper>* residualFunctionInstr;
    int jacType;
    unsigned int wIndex;
    bool bZeroPi;
    unsigned int numTreeInstr;
    unsigned int numVars;

public:
    virtualDependencyTree();
    virtualDependencyTree(std::vector<virtualOper>* residualFunctionInstr_arg);
    ~virtualDependencyTree();

    //builds the dependency tree and returns the residual function evaluation in w form
    void setInstr(std::vector<virtualOper> *value);
    virtualDependencyTreeNode* buildTree(std::vector<virtualOper>::const_iterator &iterFucntion);
    int addIntermediateDerivativeInstr(virtualDependencyTreeNode* item, virtualDependencyTreeNode* itemPrev, bool root = false);
    int getReverseModeAutoDiffInst();
    int getSISOFunctionDerivative(virtualOper* fnIt, unsigned int wIndexR);
    std::vector<virtualOper> *getInstr() const;
    int getJacType() const;
    void setJacType(int value);
    void setNumVars(unsigned int val);
    int addFinalDerivative(std::vector<std::vector<unsigned int>*>& psnum);
};


