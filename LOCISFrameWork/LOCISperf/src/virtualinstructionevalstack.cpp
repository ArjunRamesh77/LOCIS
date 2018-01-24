#include "virtualinstruction.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main evaluation function
void virtualInstructionStack::evalStackBased(std::vector<virtualOper>::const_iterator oper)
{
    switch(oper->operType)
    {

    case VR_CONST:
        instStack.push(oper->value);
        break;

    case VR_INTER_INDEX:
        instStack.push(instInter.at(oper->index));
        break;

    case VR_BIN_ADD:
        val1 = instStack.top();
        instStack.pop();
        val2 = instStack.top();
        instStack.pop();

        val1 = val1 + val2;
        instStack.push(val1);
        break;

    case VR_BIN_SUB:
        val1 = instStack.top();
        instStack.pop();
        val2 = instStack.top();
        instStack.pop();

        val1 = val2 - val1;
        instStack.push(val1);
        break;

    case VR_BIN_MUL:
        val1 = instStack.top();
        instStack.pop();
        val2 = instStack.top();
        instStack.pop();

        val1 = val1 * val2;
        instStack.push(val1);
        break;

    case VR_BIN_DIV:
        val1 = instStack.top();
        instStack.pop();
        val2 = instStack.top();
        instStack.pop();

        val1 = val2 / val1;
        instStack.push(val1);
        break;

    case VR_BIN_RAISE:
        val1 = instStack.top();
        instStack.pop();
        val2 = instStack.top();
        instStack.pop();

        val1 = std::pow(val1, val2);
        instStack.push(val1);
        break;

    case VR_UNA_ADD:
        val1 = instStack.top();
        instStack.pop();

        val1 = +val1;
        instStack.push(val1);
        break;

    case VR_UNA_SUB:
        val1 = instStack.top();
        instStack.pop();

        val1 = -val1;
        instStack.push(val1);
        break;

    case VR_ASSIGN:
        val1 = instStack.top();
        instStack.pop();
        instInter.push_back(val1);
        break;

    case VR_LOAD_CONST:
        instInter.push_back(oper->value);
        break;

    case VR_FUNCTION_SISO:
        val1 = instStack.top();
        instStack.pop();

        val1 = g_virtualFunctionLoader.fpSISOarr[oper->index](val1);
        instStack.push(val1);
        break;

    case VR_FUNCTION_DISO:
        val1 = instStack.top();
        instStack.pop();

        val2 = instStack.top();
        instStack.pop();

        val1 = g_virtualFunctionLoader.fpDISOarr[oper->index](val2,val1);
        instStack.push(val1);
        break;

    case VR_NOOP:
        break;
    }
}
