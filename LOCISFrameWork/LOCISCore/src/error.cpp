#include "error.h"
#include <sstream>

error::error() :
    code(0),
    type(""),
    lineNumber(-1),
    position(-1),
    errLines()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
cerrors::cerrors(LOCISErrorCallBack cb)
{
	errcb = cb;
    numErrors = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Add the error packet to the list of errors
void cerrors::addError()
{
    errors.push_back(tempErr);
    numErrors++;

	// Format the error
	std::stringstream cLines;
    cLines << "CODE:" + std::to_string(tempErr.code) + "\n";
    cLines << "TYPE:" + tempErr.type + "\n";
    cLines << "LINE:" + std::to_string(tempErr.lineNumber) + "\n";
    cLines << "POS:" + std::to_string(tempErr.position) + "\n";
	//foreach(std::string line in temp_err.err_lines)
    for(auto it = tempErr.errLines.begin(); it != tempErr.errLines.end(); it++)
	{
		cLines << *it + "\n";
	}

	//Callback
	errcb(-1, cLines.str().c_str());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Adds a line to the current error
void cerrors::addErrorLine(const std::string &line)
{
    tempErr.errLines.push_back(line);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Puts in error data
void cerrors::setError(const int &code, const std::string type, const long int &line_number, const int &position)
{
    tempErr.errLines.clear();
    tempErr.code = code;
    tempErr.type = type;
    tempErr.lineNumber = line_number;
    tempErr.position = position;
}

