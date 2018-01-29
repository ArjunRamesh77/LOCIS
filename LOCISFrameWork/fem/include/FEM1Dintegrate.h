#pragma once
#include <vector>
#include<iomanip>
#include<sstream>
namespace FEM1D
{
class integration1D
{
protected:
	int npts; // number of points required by integration method (2 for trapezoidal)

	std::vector<std::string> beforeIntegrationVec; // Size of this vector will be number of equations

public:

    int getNumPoints()
	{
		return npts;
	};

	virtual std::string compositeIntegral(std::vector<std::string> integrationStrings, double length)
	{
		return "";
	};

};

class trapezoid:public integration1D
{

public:
	std::string compositeIntegral(std::vector<std::string> integrationStrings, double length);

};

class simpson13 :public integration1D
{

public:
	std::string compositeIntegral(std::vector<std::string> integrationStrings, double length);

};

class simpson38 :public integration1D
{

public:
	std::string compositeIntegral(std::vector<std::string> integrationStrings, double length);

};
}
