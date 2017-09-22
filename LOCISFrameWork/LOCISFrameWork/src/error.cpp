#include "error.h"
#include <sstream>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
cerrors::cerrors(LOCISErrorCallBack cb)
{
	errcb = cb;
	num_errors = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Add the error packet to the list of errors
void cerrors::AddError()
{
	errors.push_back(temp_err);
	num_errors++;

	// Format the error
	std::stringstream cLines;
	cLines << "CODE:" + std::to_string(temp_err.code) + "\n";
	cLines << "TYPE:" + temp_err.type + "\n";
	cLines << "LINE:" + std::to_string(temp_err.line_number) + "\n";
	cLines << "POS:" + std::to_string(temp_err.position) + "\n";
	//foreach(std::string line in temp_err.err_lines)
	for(auto it = temp_err.err_lines.begin(); it != temp_err.err_lines.end(); it++)
	{
		cLines << *it + "\n";
	}

	//Callback
	errcb(-1, cLines.str().c_str());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Adds a line to the current error
void cerrors::AddErrorLine(const std::string &line)
{
	temp_err.err_lines.push_back(line);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Puts in error data
void cerrors::SetError(const int &code, const std::string type, const long int &line_number, const int &position)
{
	temp_err.err_lines.clear();
	temp_err.code = code;
	temp_err.type = type;
	temp_err.line_number = line_number;
	temp_err.position = position;
}