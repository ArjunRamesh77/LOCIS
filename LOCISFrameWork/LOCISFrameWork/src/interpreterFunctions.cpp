#include "interpreter.h"
#include <cmath>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions
ASTNode* interpreter::dispatch(ASTfunctionCallNode* node)
{
	//Single parameter functions
	if (node->astvnFunctionArgs.size() == 1)
	{
		ASTNode* eval = NULL;
		if (!node->astvnFunctionArgs.at(0))
		{
			semanticErr_NoArgsFunction(node);
			return NULL;
		}
		eval = VISIT_NODE(node->astvnFunctionArgs.at(0));
		assert(eval);

		double dval = eval->value;

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Common trigonometric functions
		if (node->sFunctionName == "COS")
		{
			node->value = cos(dval);
			return node;
		}

		if (node->sFunctionName == "SIN")
		{
			node->value = sin(dval);
			return node;
		}

		if (node->sFunctionName == "TAN")
		{
			node->value = tan(dval);
			return node;
		}

		if (node->sFunctionName == "ACOS")
		{
			node->value = acos(dval);
			return node;
		}

		if (node->sFunctionName == "ASIN")
		{
			node->value = asin(dval);
			return node;
		}

		if (node->sFunctionName == "ATAN")
		{
			node->value = atan(dval);
			return node;
		}

		if (node->sFunctionName == "COSH")
		{
			node->value = cosh(dval);
			return node;
		}

		if (node->sFunctionName == "SINH")
		{
			node->value = sinh(dval);
			return node;
		}

		if (node->sFunctionName == "ACOSH")
		{
			node->value = acosh(dval);
			return node;
		}

		if (node->sFunctionName == "ASINH")
		{
			node->value = asinh(dval);
			return node;
		}

		if (node->sFunctionName == "ATANH")
		{
			node->value = atanh(dval);
			return node;
		}

		if (node->sFunctionName == "EXP")
		{
			node->value = exp(dval);
			return node;
		}

		if (node->sFunctionName == "LOG")
		{
			node->value = log(dval);
			return node;
		}

		if (node->sFunctionName == "LOG10")
		{
			node->value = log10(dval);
			return node;
		}

		semanticErr_InvalidFunction(node);
		return NULL;
	}

	return NULL;
}