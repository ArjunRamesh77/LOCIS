#include <iostream>
#include <string>
#include "FEM1Dintegrate.h"


std::string trapezoid::compositeIntegral(std::vector<std::string> integrationStrings, double length)
{
	// add strings in vector using composite rule
	std::string integratedString, tempstr;
	std::vector<std::string> tempvec;

	std::stringstream ss;
	ss << std::scientific << std::setprecision(10) << length;

	integratedString = "0.5 * " + ss.str() + " * (";

	for (int i = 0; i < integrationStrings.size(); i++)
	{
		
		if (i == 0)
		{
			integratedString += integrationStrings.at(i) + " + ";
		}
		else if (i == integrationStrings.size() - 1)
		{
			integratedString += integrationStrings.at(i);
		}
		else
		{
			integratedString += "2 *" + integrationStrings.at(i) + " + ";
		}
	}
	integratedString += ")";
	return integratedString;

}


std::string simpson13::compositeIntegral(std::vector<std::string> integrationStrings, double length)
{
	// add strings in vector using composite rule
	std::string integratedString, tempstr;
	std::vector<std::string> tempvec;

	std::stringstream ss;
	ss << std::scientific << std::setprecision(10) << length;

	integratedString = "0.333333333333 * " + ss.str() + " * (";

	for (int i = 0; i < integrationStrings.size(); i++)
	{

		if (i == 0)
		{
			integratedString += integrationStrings.at(i) + " + ";
		}
		else if (i % 2 == 1 && i != (integrationStrings.size() - 1))
		{
			integratedString += "4 *" + integrationStrings.at(i) + " + ";
		}
		else if (i % 2 == 0 && i != 0 && i != (integrationStrings.size() - 1))
		{
			integratedString += "2 *" + integrationStrings.at(i) + " + ";
		}
		else if (i == integrationStrings.size() - 1)
		{
			integratedString += integrationStrings.at(i);
		}
		else
		{
		}
	}
	integratedString += ")";
	return integratedString;

}


std::string simpson38::compositeIntegral(std::vector<std::string> integrationStrings, double length)
{
	// add strings in vector using composite rule
	std::string integratedString, tempstr;
	std::vector<std::string> tempvec;

	std::stringstream ss;
	ss << std::scientific << std::setprecision(10) << length;

	integratedString = "0.375 * " + ss.str() + " * (";

	for (int i = 0; i < integrationStrings.size(); i++)
	{

		if (i == 0)
		{
			integratedString += integrationStrings.at(i) + " + ";
		}
		else if (i % 3 == 0 && i != 0 && i != (integrationStrings.size() - 1))
		{
			integratedString += "2 *" + integrationStrings.at(i) + " + ";
		}
		else if (i % 3 == 1 && i != 0 && i != (integrationStrings.size() - 1))
		{
			integratedString += "3 *" + integrationStrings.at(i) + " + ";
		}
		else if (i % 3 == 2 && i != 0 && i != (integrationStrings.size() - 1))
		{
			integratedString += "3 *" + integrationStrings.at(i) + " + ";
		}
		else if (i == integrationStrings.size() - 1)
		{
			integratedString += integrationStrings.at(i);
		}
		else
		{
		}
	}
	integratedString += ")";
	return integratedString;

}
