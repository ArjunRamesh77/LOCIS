#ifndef  LOCIS_ERROR_H
#define  LOCIS_ERROR_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOCIS error includes
#include <string>
#include <vector>
#include "LOCIS_API_INCLUDE.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOCIS error class
struct error
{
	int code;
	std::string type;
	long int line_number;
	int position;
	std::vector<std::string> err_lines;
};

class cerrors
{
private:
	LOCISErrorCallBack errcb;
	error temp_err;
	int num_errors;

public:
	std::vector<error> errors;
	cerrors(LOCISErrorCallBack cb);

	//Sets the type of error
	void SetError(const int &code, const std::string type, const long int &line_number, const int &position);

	//Adds a single line of error (can be called multiple times)
	void AddErrorLine(const std::string &line);

	//Adds the error to the list
	void AddError();

};

#endif // ! LOCIS_ERROR_H