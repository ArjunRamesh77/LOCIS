#include "lexer.h"

/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOCIS lexer constructor ( File source )
lexer::lexer(std::string name, cerrors *errorptr)
{
	bIsFile = true;
	fname = name;
	lex_err = errorptr;
	file.open(fname, std::ifstream::in);
	std::getline(file, line);
	current_it = line.begin();
	current_pos = 0;
	FEOF = false;
	enable_generic_error = true;
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOCIS lexer constructor ( Char source )
lexer::lexer(const char* source, cerrors *errorptr)
{
	bIsFile = false;
	lex_err = errorptr;
	file << source;				// Read character source as stringstream
	std::getline(file, line);
	current_it = line.begin();
	current_pos = 0;
	FEOF = false;
	enable_generic_error = true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
lexer::~lexer()
{
	//Close file
	/*
	if (file.is_open())
	{
		file.close();
	}
	*/
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void lexer::checkNewLine()
{
	if (current_it == line.end())
	{
		current_read = false;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void lexer::getNewLine(bool force)
{
	if (current_it == line.end() || force)
	{
		if (!std::getline(file, line))
		{
			FEOF = true;
			return;
		}
		line_num++;
		current_pos = 0;
		current_it = line.begin();

		// Skip Blank lines
		while (!FEOF && current_it == line.end())
		{
			if (!std::getline(file, line))
			{
				FEOF = true;
				return;
			}
			line_num++;
			current_pos = 0;
			current_it = line.begin();
		}

		return;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
int lexer::getNextToken(token *tok)
{

start:

	//Set Invalid Token
	tok->set_token(-100, "EOF", -1, -1);

	int retval = 0;

	// Ignore Multiline comment
	retval = isMultiLineComment(tok);
	if (retval == 1)
		goto start;

	// Ignore singleline comment
	retval = isSingleLineComment(tok);
	if (retval == 1)
		goto start;

	// Try Keywords
	retval = isKeyword(tok);
	if (retval == 1)
	{
		return 1;
	}

	// Try String
	retval = isString(tok);
	if (retval == 1)
	{
		return 1;
	}

	// Try Identifier
	retval = isIdentifier(tok);
	if (retval == 1)
	{
		return 1;
	}

	// Try Operator
	retval = isOperator(tok);
	if (retval == 1)
	{
		return 1;
	}

	// Try Exponential
	retval = isExpo(tok);
	if (retval == 1)
	{
		return 1;
	}

	// Try Real
	retval = isReal(tok);
	if (retval == 1)
	{
		return 1;
	}

	// Try Integer
	retval = isInteger(tok);
	if (retval == 1)
	{
		return 1;
	}

	// Get out if no tokens identified and EOF
	if (retval == 0 && FEOF)
	{
		return -1;
	}

	// If token not reco
	if (enable_generic_error)
	{
		lex_err->SetError(1113, "Lexer Error", line_num, current_pos);
		lex_err->AddErrorLine(" Unidentified token found ");
		lex_err->AddError();
		enable_generic_error = true;
	}

	// Recover
	tryRecover();
	if (FEOF)
		return -1;
	goto start;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void lexer::incIter(bool nl)
{
	if (current_it != line.end())
	{
		current_it++;
		current_pos++;
	}
	if (nl)
		getNewLine();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void lexer::resetIter()
{
	current_it = current_it_save;
	current_pos = current_pos_save;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void lexer::saveIter()
{
	current_it_save = current_it;
	current_pos_save = current_pos;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
int lexer::tryRecover()
{
	//Skip to Next Readable token
	while (current_it != line.end())
	{
		if (*current_it == '\t' || *current_it == ' ')
		{
			return 1;
		}
		incIter();
	}

	return 0;
}