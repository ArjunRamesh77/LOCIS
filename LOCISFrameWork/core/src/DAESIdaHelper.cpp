#include "DAESIdaHelper.h"
#include "DAESIda.h"
#include <regex>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Error handler function
void DAESIdaERRORFUN(int error_code, const char *module,
	const char *function, char *msg,
	void *user_data)
{

//	long ierror;
//	ierror = error_code;
	//NLSKinsolerrormsgstruct error;

	// USERDATA WILL HAVE ALL REQUIRED ERROR DATA
	DAESIdaErrorHandler* data;
	DAESIdaerrormsgstruct error;

	data = (DAESIdaErrorHandler*)user_data;

	std::string errLine, smodule(module), sfunction(function), smsg(msg);
	
	data->errVector->setError(5001, "IDA Failed", 0, 0);
	errLine = "MODULE: " + smodule;
	data->errVector->addErrorLine(errLine);
	errLine = " FUNCTION: " + sfunction;
	data->errVector->addErrorLine(errLine);
	errLine = " MESSAGE: " + smsg;
	data->errVector->addErrorLine(errLine);
	data->errVector->addError();

//	data->outPutErrorHandler(ierror, cstr);
	return;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Info handler
void DAESIdaOutputInfoHandler(void *s, int type)
{


	return;
}

void DAESIdaOutputErrorHandler(void *s, int type)
{
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OUTPUT HELPER FUNCTIONS

/* Convert double to string with specified number of places after the decimal
and left padding. */
std::string IDAprd1(std::string x, int width)
{
	int numEmptySpaces, len;
	double temp_double;
	std::string str, str_temp;
	str = "";
	len = x.length();
	if (width < len)
	{
		temp_double = stod(x);

		str += IDAprd(temp_double, 1, width);
		//str_temp = std::to_string(temp_double);
		//len = str_temp.length();
		//numEmptySpaces = width - len;
		//str += std::string(numEmptySpaces, ' ');
		//str += str_temp;

	}
	else
	{
		numEmptySpaces = width - len;

		str += std::string(numEmptySpaces, ' ');

		str += x;

	}
	return str;
}



std::string IDAprd(const double x, const int decDigits, const int width)
{
	std::stringstream ss;
	ss << std::fixed << std::right;
	ss.fill(' ');        // fill space around displayed #
	ss.width(width);     // set  width around displayed #
	ss.precision(decDigits); // set # places after decimal
	ss << x;
	return ss.str();
}