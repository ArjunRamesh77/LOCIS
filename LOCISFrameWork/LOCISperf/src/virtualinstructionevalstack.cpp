#include "virtualinstruction.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main evaluation function
void virtualInstructionStack::evalStackBased(std::vector<virtualOper>::const_iterator oper, std::stack<double>& stackRef, std::vector<double> &inter)
{
    //locals
    double val1(0);
    double val2(0);

    switch(oper->operType)
    {

    case VR_BIN_ADD:
        val1 = stackRef.top();
        stackRef.pop();
        val2 = stackRef.top();
        stackRef.pop();

        val1 = val1 + val2;
        stackRef.push(val1);
        break;

    case VR_BIN_SUB:
        val1 = stackRef.top();
        stackRef.pop();
        val2 = stackRef.top();
        stackRef.pop();

        val1 = val1 - val2;
        stackRef.push(val1);
        break;

    case VR_BIN_MUL:
        val1 = stackRef.top();
        stackRef.pop();
        val2 = stackRef.top();
        stackRef.pop();

        val1 = val1 * val2;
        stackRef.push(val1);
        break;

    case VR_BIN_DIV:
        val1 = stackRef.top();
        stackRef.pop();
        val2 = stackRef.top();
        stackRef.pop();

        val1 = val2 / val1;
        stackRef.push(val1);
        break;

    case VR_BIN_RAISE:
        break;

    case VR_UNA_ADD:
        val1 = stackRef.top();
        stackRef.pop();

        val1 = +val1;
        stackRef.push(val1);
        break;

    case VR_UNA_SUB:
        val1 = stackRef.top();
        stackRef.pop();

        val1 = -val1;
        stackRef.push(val1);
        break;

    case VR_ASSIGN:
        val1 = stackRef.top();
        stackRef.pop();
        inter.push_back(val1);
        break;

    case VR_FUNCTION_SISO:
        val1 = stackRef.top();
        stackRef.pop();

        val1 = g_virtualFunctionLoader.fpSISOarr[oper->index](val1);
        stackRef.push(val1);
        break;

    case VR_FUNCTION_DISO:
        val1 = stackRef.top();
        stackRef.pop();

        val2 = stackRef.top();
        stackRef.pop();

        val1 = g_virtualFunctionLoader.fpDISOarr[oper->index](val2,val1);
        stackRef.push(val1);
        break;
    }
}
