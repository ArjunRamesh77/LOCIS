#include "PyFunctionServer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Builds a line of code with the Python Language
std::string PyFunctionServer::buildSimpleCodeLine(const int &num_tabs, const std::string& line)
{
	std::string tabs("");
	for (int i = 1; i <= num_tabs; i++)
	{
		tabs += "\t";
	}

	return tabs + line + "\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Builds a line of Residual
std::string PyFunctionServer::buildResidualCodeLine(const int &num_tabs, const int & res_num, const std::string& line)
{
	std::string tabs("");
	for (int i = 1; i <= num_tabs; i++)
	{
		tabs += "\t";
	}

	return tabs + "r[" + std::to_string(res_num) + "] = " + line + "\n";
}


