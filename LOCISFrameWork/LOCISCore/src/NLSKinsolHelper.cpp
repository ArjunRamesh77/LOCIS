#include "NLSKinsolHelper.h"
#include "NLSKinsol.h"
#include <regex>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Info handler function
void NLSKinsolIHFUN(const char *module, const char *function, char *msg, void *user_data)
{
	//// USERDATA WILL HAVE ALL REQUIRED OUTPUT DATA
	NLSKinsolInfoHandler* data;
	//long ierror;
	//ierror = 0;
	NLSKinsolinfomsgstruct info;
	//std::stringstream cLines;
	//cLines << "MODULE: " + std::to_string(*module) + "\n";
	//cLines << "FUNCTION: " + std::to_string(*function) + "\n";
	//cLines << "MESSAGE: " + std::to_string(*msg) + "\n";
	info.module = module;
	info.function = function;
	info.msg = msg;

	//std::string tmp = cLines.str();
	//const char* cstr = tmp.c_str();
	data = (NLSKinsolInfoHandler*)user_data;
	info.totalStringout = data->stringOut;

	info.outPutInfoHandler = data->outPutInfoHandler;
	data->InfoFormatFn(&info, 0); // this calls function in this file NLSKinsolOutputInfoHandler
	//return;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Error handler function
void NLSKinsolERRORFUN(int error_code, const char *module,
	const char *function, char *msg,
	void *user_data)
{
	// USERDATA WILL HAVE ALL REQUIRED ERROR DATA
	NLSKinsolErrorHandler* data;
//	long ierror;
//	ierror = error_code;	

	std::string errLine, smodule(module), sfunction(function), smsg(msg);

	data = (NLSKinsolErrorHandler*)user_data;

	data->errVector->setError(4001, "Kinsol Failed", 0, 0);
	errLine = "MODULE: " + smodule;
	data->errVector->addErrorLine(errLine);
	errLine = " FUNCTION: " + sfunction;
	data->errVector->addErrorLine(errLine);
	errLine = " MESSAGE: " + smsg;
	data->errVector->addErrorLine(errLine);
	data->errVector->addError();

	//data->outPutErrorHandler(ierror, cstr);
	return;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Info handler
void NLSKinsolOutputInfoHandler(void *s, int type)
{
	NLSKinsolinfomsgstruct* info;
	info = (NLSKinsolinfomsgstruct *)s;

	LOCISInfoCallBack infoCallBackFn;
	infoCallBackFn = info->outPutInfoHandler;

	std::string copy_str, mod_str;
	std::regex reg("="); // searches for #u, #Du, #v, #Dv
	std::smatch matches;

	copy_str = info->msg;

//	std::string totalOutputString;
//	totalOutputString = "";
	std::regex sep("[ ]+");
	std::sregex_token_iterator tokens(copy_str.cbegin(), copy_str.cend(), sep, -1);
	std::sregex_token_iterator end;
	std::string print_str, temp_str;

	bool print_this, enter_first, correct_str, is_pnorm;
	double temp;
	is_pnorm = false;
	enter_first = true;
	correct_str = false;

	std::stringstream cLines;
	std::string tmp;
	const char* cstr;
	for (; tokens != end; ++tokens)
	{
		if (correct_str)
		{
			// do things to print
			if (*tokens == "=")
			{
				print_this = true;
			}
			else
			{
				if (print_this)
				{
					print_this = false;
					if (enter_first)
					{
						if (is_pnorm)
						{
							temp_str = *tokens;
							//std::cout << KINprd1(temp_str, 13) << " ";
							*(info->totalStringout) += KINprd1(temp_str, 15) + " ";
							//totalOutputString += KINprd1(temp_str, 13) + " ";
							//cLines << KINprd1(temp_str, 13) + " ";

							//tmp = cLines.str();
							//cstr = tmp.c_str();
							//infoCallBackFn(0, cstr);
							//cLines.str("");
							//cLines.clear();

						}
						else
						{
							enter_first = false;
							temp_str = *tokens;
							//std::cout << KINprd1(temp_str, 13);
							*(info->totalStringout) += KINprd1(temp_str, 15);
//							totalOutputString += KINprd1(temp_str, 13);
//							cLines << KINprd1(temp_str, 13);

							//tmp = cLines.str();
							//cstr = tmp.c_str();
							//infoCallBackFn(0, cstr);
							//cLines.str("");
							//cLines.clear();

						}

					}
					else
					{
						temp_str = *tokens;
						//std::cout << "   " << KINprd1(temp_str, 13) << " ";
						*(info->totalStringout) += KINprd1(temp_str, 15) + " ";
//						totalOutputString += KINprd1(temp_str, 13) + " ";
//						cLines << KINprd1(temp_str, 13) + " ";

						//tmp = cLines.str();
						//cstr = tmp.c_str();
						//infoCallBackFn(0, cstr);
						//cLines.str("");
						//cLines.clear();
					}
				}
			}

		}
		else
		{
			if (*tokens == "nni" || *tokens == "pnorm")
			{
				correct_str = true;
				if (*tokens == "pnorm")
				{
					is_pnorm = true;
				}
			}
		}
	}
	if (is_pnorm)
	{
		//std::cout << "\n";
		
		cLines << *(info->totalStringout) << "\n";
		//cLines << "\n";
		tmp = cLines.str();
		cstr = tmp.c_str();
		infoCallBackFn(0, cstr);
		cLines.str("");
		cLines.clear();
		*(info->totalStringout) = ""; // clear out the string

	}
	
	return;
}

void NLSKinsolOutputErrorHandler(void *s, int type)
{
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OUTPUT HELPER FUNCTIONS

/* Convert double to string with specified number of places after the decimal
and left padding. */
std::string KINprd1(std::string x, int width) 
{
	int numEmptySpaces, len;
	double temp_double;
	std::string str, str_temp;
	str = "";
	len = x.length();
	if (width < len)
	{
		temp_double = stod(x);

		str += KINprd(temp_double, 1, width);

	}
	else
	{
		numEmptySpaces = width - len;

		str += std::string(numEmptySpaces, ' ');

		str += x;

	}
	return str;
}



std::string KINprd(const double x, const int decDigits, const int width)
{
	std::stringstream ss;
	ss << std::fixed << std::right;
	ss.fill(' ');        // fill space around displayed #
	ss.width(width);     // set  width around displayed #
	ss.precision(decDigits); // set # places after decimal
	ss << x;
	return ss.str();
}