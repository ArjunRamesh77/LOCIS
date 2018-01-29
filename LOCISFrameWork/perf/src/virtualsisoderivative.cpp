#include "virtualdependencytree.h"

int virtualDependencyTree::getSISOFunctionDerivative(virtualOper *fnIt, unsigned int wIndexR)
{
    switch(fnIt->index)
    {
    case VR_FUNC_COS:
        instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
        instr->push_back(virtualOper(VR_FUNCTION_SISO, (unsigned int)VR_FUNC_SIN));
        instr->push_back(virtualOper(VR_UNA_SUB));
        break;

    case VR_FUNC_SIN:
        instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
        instr->push_back(virtualOper(VR_FUNCTION_SISO, (unsigned int)VR_FUNC_COS));
        instr->push_back(virtualOper(VR_UNA_SUB));
        break;

    case VR_FUNC_TAN:
        instr->push_back(virtualOper(VR_CONST, 1.0));
        instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
        instr->push_back(virtualOper(VR_FUNCTION_SISO, (unsigned int)VR_FUNC_COS));
        instr->push_back(virtualOper(VR_CONST, 2.0));
        instr->push_back(virtualOper(VR_BIN_RAISE));
        instr->push_back(virtualOper(VR_BIN_DIV));
        break;

    case VR_FUNC_ACOS:
        instr->push_back(virtualOper(VR_CONST, -1.0));
        instr->push_back(virtualOper(VR_CONST, 1.0));
        instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
        instr->push_back(virtualOper(VR_CONST, 2.0));
        instr->push_back(virtualOper(VR_BIN_RAISE));
        instr->push_back(virtualOper(VR_BIN_SUB));
        instr->push_back(virtualOper(VR_FUNC_SQRT));
        instr->push_back(virtualOper(VR_BIN_DIV));
        break;

    case VR_FUNC_ASIN:
        instr->push_back(virtualOper(VR_CONST, 1.0));
        instr->push_back(virtualOper(VR_CONST, 1.0));
        instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
        instr->push_back(virtualOper(VR_CONST, 2.0));
        instr->push_back(virtualOper(VR_BIN_RAISE));
        instr->push_back(virtualOper(VR_BIN_SUB));
        instr->push_back(virtualOper(VR_FUNC_SQRT));
        instr->push_back(virtualOper(VR_BIN_DIV));
        break;

    case VR_FUNC_ATAN:
        instr->push_back(virtualOper(VR_CONST, 1.0));
        instr->push_back(virtualOper(VR_CONST, 1.0));
        instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
        instr->push_back(virtualOper(VR_CONST, 2.0));
        instr->push_back(virtualOper(VR_BIN_RAISE));
        instr->push_back(virtualOper(VR_BIN_ADD));
        instr->push_back(virtualOper(VR_BIN_DIV));
        break;

    case VR_FUNC_COSH:
        instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
        instr->push_back(virtualOper(VR_FUNC_ASIN));
        break;

    case VR_FUNC_SINH:
        instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
        instr->push_back(virtualOper(VR_FUNC_ACOS));
        break;

    case VR_FUNC_TANH:
        instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
        instr->push_back(virtualOper(VR_FUNC_ASIN));
        break;

    case VR_FUNC_ACOSH:
        instr->push_back(virtualOper(VR_CONST, 1.0));
        instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
        instr->push_back(virtualOper(VR_CONST, 2.0));
        instr->push_back(virtualOper(VR_BIN_RAISE));
        instr->push_back(virtualOper(VR_CONST, 1.0));
        instr->push_back(virtualOper(VR_BIN_SUB));
        instr->push_back(virtualOper(VR_FUNC_SQRT));
        instr->push_back(virtualOper(VR_BIN_DIV));
        break;

    case VR_FUNC_ASINH:
        instr->push_back(virtualOper(VR_CONST, 1.0));
        instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
        instr->push_back(virtualOper(VR_CONST, 2.0));
        instr->push_back(virtualOper(VR_BIN_RAISE));
        instr->push_back(virtualOper(VR_CONST, 1.0));
        instr->push_back(virtualOper(VR_BIN_ADD));
        instr->push_back(virtualOper(VR_FUNC_SQRT));
        instr->push_back(virtualOper(VR_BIN_DIV));
        break;

    case VR_FUNC_ATANH:
        instr->push_back(virtualOper(VR_CONST, 1.0));
        instr->push_back(virtualOper(VR_CONST, 1.0));
        instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
        instr->push_back(virtualOper(VR_CONST, 2.0));
        instr->push_back(virtualOper(VR_BIN_RAISE));
        instr->push_back(virtualOper(VR_BIN_SUB));
        instr->push_back(virtualOper(VR_BIN_DIV));
        break;

    case VR_FUNC_EXP:
        instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
        instr->push_back(virtualOper(VR_FUNC_EXP));
        break;

    case VR_FUNC_LOG:
        instr->push_back(virtualOper(VR_CONST, 1.0));
        instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
        instr->push_back(virtualOper(VR_BIN_DIV));
        break;

    case VR_FUNC_LOG10:
        instr->push_back(virtualOper(VR_CONST, 1.0));
        instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
        instr->push_back(virtualOper(VR_CONST, CONST_LOG10));
        instr->push_back(virtualOper(VR_BIN_MUL));
        instr->push_back(virtualOper(VR_BIN_DIV));
        break;

    case VR_FUNC_SQRT:
        instr->push_back(virtualOper(VR_CONST, -1.0));
        instr->push_back(virtualOper(VR_CONST, 2.0));
        instr->push_back(virtualOper(VR_INTER_INDEX, wIndexR));
        instr->push_back(virtualOper(VR_FUNC_SQRT));
        instr->push_back(virtualOper(VR_BIN_MUL));
        instr->push_back(virtualOper(VR_BIN_DIV));
        break;
    }

    //assign to intermediate
    instr->push_back(virtualOper(VR_ASSIGN, wIndex));
    wIndex++;

    return 0;
}
