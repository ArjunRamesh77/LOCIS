#include "equation.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions
ASTNode* equation::dispatch(ASTfunctionCallNode* node)
{
	//Single parameter functions
	if (node->astvnFunctionArgs.size() == 1)
	{
		ASTNode* eval = NULL;
		if (!node->astvnFunctionArgs.at(0))
		{
			Ip->semanticErr_NoArgsFunction(node);
			return NULL;
		}
		eval = node->astvnFunctionArgs.at(0)->visit(this);
		assert(eval);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Common trigonometric functions
		if (node->sFunctionName == "COS")
		{
			node->equation_data = "cos(" + eval->equation_data + ")";
			return node;
		}

		if (node->sFunctionName == "SIN")
		{
			node->equation_data = "sin(" + eval->equation_data + ")";
			return node;
		}

		if (node->sFunctionName == "TAN")
		{
			node->equation_data = "tan(" + eval->equation_data + ")";
			return node;
		}

		if (node->sFunctionName == "ACOS")
		{
			node->equation_data = "arccos(" + eval->equation_data + ")";
			return node;
		}

		if (node->sFunctionName == "ASIN")
		{
			node->equation_data = "arcsin(" + eval->equation_data + ")";
			return node;
		}

		if (node->sFunctionName == "ATAN")
		{
			node->equation_data = "arctan(" + eval->equation_data + ")";
			return node;
		}

		if (node->sFunctionName == "ACOSH")
		{
			node->equation_data = "cosh(" + eval->equation_data + ")";
			return node;
		}

		if (node->sFunctionName == "SINH")
		{
			node->equation_data = "sinh(" + eval->equation_data + ")";
			return node;
		}

		if (node->sFunctionName == "ACOSH")
		{
			node->equation_data = "arccosh(" + eval->equation_data + ")";
			return node;
		}

		if (node->sFunctionName == "ASINH")
		{
			node->equation_data = "arcsinh(" + eval->equation_data + ")";
			return node;
		}

		if (node->sFunctionName == "ATANH")
		{
			node->equation_data = "arctanh(" + eval->equation_data + ")";
			return node;
		}

		if (node->sFunctionName == "EXP")
		{
			node->equation_data = "exp(" + eval->equation_data + ")";
			return node;
		}

		if (node->sFunctionName == "LOG")
		{
			node->equation_data = "log(" + eval->equation_data + ")";
			return node;
		}

		if (node->sFunctionName == "LOG10")
		{
			node->equation_data = "log10(" + eval->equation_data + ")";
			return node;
		}

		Ip->semanticErr_InvalidFunction(node);
		return NULL;
	}

	return NULL;
}
