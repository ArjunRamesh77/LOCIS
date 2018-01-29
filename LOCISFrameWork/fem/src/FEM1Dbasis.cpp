//#include "linearBasis1D.h"
#include "FEM1Dbasis.h"
#include <iostream>
using namespace FEM1D;
std::string linearBasis1D::get_phi(int n, int i, double length)// n  is basis function number, i is the node number
{
	if ((n == i))
	{
		return "1";
	}
	else
	{
		return "0";
	}

}

std::string linearBasis1D::get_dphi(int n, int i, double length)// n  is basis function number, i is the node number
{
	std::stringstream ss;
	ss << std::scientific << std::setprecision(10) << length;

	// In linear basis function, node number doesn't matter
	if (n == 1)
	{
		return "(-1.0/" + ss.str() + ")";
	}
	else if (n == 2)
	{
		return "(1.0/" + ss.str() + ")";
	}
	else
	{
		return "[" + std::to_string(n) + ", " + std::to_string(i) + "]"; // this should display an error
	}
}

//****************************QUADRATIC************************************************************

std::string quadraticBasis1D::get_phi(int n, int i, double length)// n  is basis function number, i is the node number
{
	if ((n == i))
	{
		return "1";
	}
	else
	{
		return "0";
	}
}

std::string quadraticBasis1D::get_dphi(int n, int i, double length)// n  is basis function number, i is the node number
{
	/*
	In linear basis function, node number doesn't matter
	xeta = (x - x_i)/(x_i+1 - x_i)
	all x_i are end points of the element
	there are 3 quadratic basis functions in an element as follows:
	1: (1 - 3xeta + 2xeta^2)
	2: (4xeta - 4xeta^2)
	3: (-xeta + 2xeta^2)
	we are only interested in points x = {0, 0.5, 1.0}
	d(xeta) = 1/(length of element)
	d(xeta^2) = 2 * (x - x_i)/(length of element)^2 = {0, 1/length, 2/length)
	*/

	std::stringstream ss;
	ss << std::scientific << std::setprecision(10) << length;

	if (n == 1)
	{
		if (i == 1)
		{
			return  "((-3.0)/" + ss.str() + ")";
		}
		else if (i == 2)
		{
			return  "((-1.0)/" + ss.str() + ")";
		}
		else if (i == 3)
		{
			return  "((1.0)/" + ss.str() + ")";
		}
		else
		{
			return "[" + std::to_string(n) + ", " + std::to_string(i) + "]";

		}
	}
	else if (n == 2)
	{
		if (i == 1)
		{
			return  "((4.0)/" + ss.str() + ")";
		}
		else if (i == 2)
		{
			return  "0";
		}
		else if (i == 3)
		{
			return  "((-4.0)/" + ss.str() + ")";
		}
		else
		{
			return "[" + std::to_string(n) + ", " + std::to_string(i) + "]";

		}
	}
	else if (n == 3)
	{
		if (i == 1)
		{
			return  "((-1.0)/" + ss.str() + ")";
		}
		else if (i == 2)
		{
			return  "((1.0)/" + ss.str() + ")";
		}
		else if (i == 3)
		{
			return  "((3.0)/" + ss.str() + ")";
		}
		else
		{
			return "[" + std::to_string(n) + ", " + std::to_string(i) + "]";
		}
	}
	else
	{
		return "[" + std::to_string(n) + ", " + std::to_string(i) + "]"; // this should display an error
	}
}
//**********************************CUBIC**************************************************************
std::string cubicBasis1D::get_phi(int n, int i, double length)// n  is basis function number, i is the node number
{
	if ((n == i))
	{
		return "1";
	}
	else
	{
		return "0";
	}

}

std::string cubicBasis1D::get_dphi(int n, int i, double length)// n  is basis function number, i is the node number
{
	/*
	In linear basis function, node number doesn't matter
	xeta = (x - x_i)/(x_i+1 - x_i)
	all x_i are end points of the element
	there are 4 cubic basis functions in an element as follows:
	1: (1 - 3xeta^2 + 2xeta^3)
	2: (xeta - 2xeta^2 + xeta^3)
	3: (3xeta^2 - 2xeta^3)
	4: (-xeta^2 + xeta^3)
	we are only interested in points x = {0, 1/3, 2/3, 1.0}
	d(xeta) = 1/(length of element)
	d(xeta^2) = 2 * xeta /(length of element) = {0, 2/(3*length), 4/(3 *length), 2/length)
	d(xeta^3) = 3 xeta^2 /(length of element) = {0, 1/(3 *length), 4/(3 *length), 3/length}
	*/

	std::stringstream ss;
	ss << std::scientific << std::setprecision(10) << length;
	if (n == 1)
	{
		if (i == 1)
		{
			return "((-11./2.) /" + ss.str() + ")";
		}
		else if (i == 2)
		{
			return "((-1.0) /" + ss.str() + ")";
		}
		else if (i == 3)
		{
			return "((1.0/2.0) /" + ss.str() + ")";
		}
		else if (i == 4)
		{
			return "((-1) / " + ss.str() + ")";
		}
		else
		{
			return "ERROR";
		}
	}
	else if (n == 2)
	{
		if (i == 1)
		{
			return "((9.0) /" + ss.str() + ")";
		}
		else if (i == 2)
		{
			return "((-3./2.) /" + ss.str() + ")" ;
		}
		else if (i == 3)
		{
			return "((-3.0) / " + ss.str() + ")";
		}
		else if (i == 4)
		{
			return "((9./2.) /" + ss.str() + ")";
		}
		else
		{
			return "ERROR";
		}
	}
	else if (n == 3)
	{
		if (i == 1)
		{
			return "((-9./2.) /" + ss.str() + ")";
		}
		else if (i == 2)
		{
			return "((3.0) / " + ss.str() + ")";
		}
		else if (i == 3)
		{
			return "((3.0/2.0) / " + ss.str() + ")";
		}
		else if (i == 4)
		{
			return "((-9.0) / " + ss.str() + ")";
		}
		else
		{
			return "ERROR";
		}
	}
	else if (n == 4)
	{
		if (i == 1)
		{
			return "(1. / " + ss.str() + ")";
			;
		}
		else if (i == 2)
		{
			return "((-1.0/2.0) / " + ss.str() + ")";
		}
		else if (i == 3)
		{
			return "(1.0 / " + ss.str() + ")";

		}
		else if (i == 4)
		{
			return "((11.0/2.0) / " + ss.str() + ")";
		}
		else
		{
			return "ERROR";
		}
	}
	else
	{
		return "[" + std::to_string(n) + ", " + std::to_string(i) + "]"; // this should display an error
	}


}
