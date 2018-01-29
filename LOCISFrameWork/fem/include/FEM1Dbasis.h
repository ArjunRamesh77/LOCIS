#pragma once
#include <vector>
#include<string>
#include <iomanip>
#include<sstream>

// all functions that are 'same' for all inherited classes are public functions in base class
// all functions that are in every inherited class but are different for each one are virtual
// functions that are specific to one class are public in that particular class
namespace FEM1D
{
class basis1D
{
protected:
	int numNodes;

public:
    int getnumNodes()
	{
		return numNodes;
	};

	virtual std::string get_phi(int n, int i, double length) // n  is basis function number, i is the node number
	{
		return "";
	}

	virtual std::string get_dphi(int n, int i, double length)
	{
		return "";
	}
};

class linearBasis1D : public basis1D
{
	// there is no additional info that linear basis needs to store!
public:
	
	std::string get_phi(int n, int i, double length); // n  is basis function number, i is the node number

	std::string get_dphi(int n, int i, double length);

	linearBasis1D()
	{
		numNodes = 2;
	};
};

class quadraticBasis1D : public basis1D
{
	// there is no additional info that linear basis needs to store!
public:

	std::string get_phi(int n, int i, double length); // n  is basis function number, i is the node number

	std::string get_dphi(int n, int i, double length);

	quadraticBasis1D()
	{
		numNodes = 3;
	};
};

class cubicBasis1D : public basis1D
{
	// there is no additional info that linear basis needs to store!
public:
	std::string get_phi(int n, int i, double length); // n  is basis function number, i is the node number

	std::string get_dphi(int n, int i, double length);

	cubicBasis1D()
	{
		numNodes = 4;
	};
};
}
