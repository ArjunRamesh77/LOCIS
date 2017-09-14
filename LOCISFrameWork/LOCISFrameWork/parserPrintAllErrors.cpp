#include "parser.h"
#include <iostream>
#include <fstream>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void parser::PrintAllErrors()
{
	std::ofstream efile("C:\\ARJUN\\compiler_errors.txt");
	if (efile.is_open())
	{
		for (std::vector<error>::iterator it = parse_err->errors.begin(); it != parse_err->errors.end(); it++)
		{
			// Error Header
			efile << "  \n";
			efile << "     Type: " << it->type << "\n";
			efile << "     Code: " << it->code << "\n";
			efile << "     Line: " << it->line_number << "\n";
			efile << "     Pos: " << it->position << "\n";
			
			// Error Lines
			for (std::vector<std::string>::iterator line = it->err_lines.begin(); line != it->err_lines.end(); line++)
			{
				efile << "          " << *line << "\n";
			}
		}
	}

	efile << "    \n NO ERRORS, YOU CAN SMILE OR CRY ACTUALLY!! \n";
	efile.close();
}