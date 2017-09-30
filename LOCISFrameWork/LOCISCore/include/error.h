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
    long int lineNumber;
	int position;
    std::vector<std::string> errLines;

    error();
};

class cerrors
{
	LOCISErrorCallBack errcb;
    error tempErr;
    int numErrors;

public:
	std::vector<error> errors;
	cerrors(LOCISErrorCallBack cb);

    void setError(const int &code, const std::string type, const long int &line_number, const int &position);
    void addErrorLine(const std::string &line);
    void addError();
};

#endif // ! LOCIS_ERROR_H
