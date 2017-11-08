#pragma once

#include <stdio.h>
#include "virtualinstruction.h"
#include <map>
#include <assert.h>
#include "lociscommon.h"

// these are aid classes for implementing the reverse mode automatic differentiation

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dependency tree node
class virtualDependencyTreeNode
{
    __int8_t nodeType;
    __int8_t wIndex;
    std::vector<virtualOper>::const_iterator oper;
    std::vector<virtualDependencyTreeNode*> to;

public:
    virtualDependencyTreeNode(std::vector<virtualOper>::const_iterator oper_arg);
    virtualDependencyTreeNode(__int8_t nodeType_arg, __int8_t wIndex_arg, std::vector<virtualOper>::const_iterator oper_arg);
    ~virtualDependencyTreeNode();

    void connectTo(virtualDependencyTreeNode* node);
    std::vector<virtualDependencyTreeNode *> &getAllTo();
    __int8_t getNodeType() const;
    __int8_t getWIndex() const;
    std::vector<virtualOper>::const_iterator getOper() const;
    void setNodeType(const __int8_t &value);
    void setWIndex(const __int8_t &value);
    void setOper(const std::vector<virtualOper>::const_iterator &value);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// builds the dependency tree from a residual function stack
class virtualDependencyTree
{
    std::map<unsigned int, virtualDependencyTreeNode*> indepVariables;
    std::vector<virtualOper>* instr;
    std::vector<std::vector<virtualOper>::const_iterator> interBegin;
    std::vector<virtualOper>* residualFunctionInstr;
    unsigned int wIndex;

public:
    virtualDependencyTree();
    virtualDependencyTree(std::vector<virtualOper>* residualFunctionInstr_arg);
    ~virtualDependencyTree();

    //builds the dependency tree and returns the residual function evaluation in w form
    virtualDependencyTreeNode* buildTree(std::vector<virtualOper>::const_iterator iterFucntion);
    int getReverseModeAutoDiffInst();
    int addIntermediateDerivativeInstr(virtualDependencyTreeNode* item, virtualDependencyTreeNode* itemPrev);
};


