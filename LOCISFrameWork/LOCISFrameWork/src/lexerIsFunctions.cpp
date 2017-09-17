#include "lexer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Escape Sequence
void lexer::isEscapeSeq()
{
	if (*current_it == '\\')
	{
		incIter();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Exponential number
int lexer::isExpo(token *tok)
{
	if (FEOF)
		return 0;

	std::string real_value("");
	bool inloop = false;

	// Ignore starting space or tabs
	while (isTabOrSpaceOrEnd());
	if (FEOF)
		return 0;

	saveIter();

	//Get First integer if it exists
	while (current_it != line.end() && isdigit(*current_it))
	{
		real_value += *current_it;
		incIter();
	}

	//Get dot
	if (current_it != line.end() && *current_it == '.')
	{
		// Put dot
		real_value += *current_it;
		incIter();

		// Get Integer
		while (current_it != line.end() && isdigit(*current_it))
		{
			real_value += *current_it;
			incIter();
			inloop = true;
		}
		if (inloop)
		{
			//Get E
			if (current_it != line.end() && (*current_it == 'e' || *current_it == 'E'))
			{
				real_value += *current_it;
				incIter();

				//Get Integer
				if (current_it != line.end() && (*current_it == '+' || *current_it == '-'))
				{
					real_value += *current_it;
					incIter();
				}
				while (current_it != line.end() && isdigit(*current_it))
				{
					real_value += *current_it;
					incIter();
					inloop = true;
				}
				if (inloop)
				{
					tok->set_token(EXPO_VALUE, real_value, line_num, current_pos_save);
					return 1;
				}
				else
				{
					//LEX_ERROR
					lex_err->SetError(1113, "Lexer Error", line_num, current_pos);
					lex_err->AddErrorLine(" Missing exponent ");
					lex_err->AddError();
					enable_generic_error = false;
				}
			}
		}
	}

	resetIter();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Indentifier
int lexer::isIdentifier(token *tok)
{
	if (FEOF)
		return 0;

	std::string identifier("");
	bool inloop = false;

	// Ignore starting space or tabs
	while (isTabOrSpaceOrEnd());
	if (FEOF)
		return 0;

	saveIter();

	//Check if first character is a alphabet
	if (current_it != line.end() && (isalpha(*current_it) || (*current_it == '_')))
	{
		identifier += *current_it;
		incIter();
		inloop = true;
	}

	// Get Full name
	if (inloop)
	{
		//Check if it has more characters
		while (current_it != line.end() && (isalnum(*current_it) || *current_it == '_'))
		{
			identifier += *current_it;
			incIter();
		}

		tok->set_token(IDENT, identifier, line_num, current_pos_save);
		return 1;
	}

	resetIter();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Integer
int lexer::isInteger(token *tok)
{
	if (FEOF)
		return 0;

	std::string integer_value("");
	bool inloop = false;

	// Ignore starting space or tabs
	while (isTabOrSpaceOrEnd());
	if (FEOF)
		return 0;

	saveIter();

	//Check if first character is a digit
	if (current_it != line.end() && !isdigit(*current_it))
	{
		return 0;
	}

	// Get Integer
	while (current_it != line.end() && isdigit(*current_it))
	{
		integer_value += *current_it;
		incIter();
		inloop = true;
	}
	if (inloop)
	{
		tok->set_token(INTEGER_VALUE, integer_value, line_num, current_pos_save);
		return 1;
	}

	resetIter();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function loops over all the keywords and returns a keyword token if encountered
int lexer::isKeyword(token *tok)
{
	if (FEOF)
		return 0;

	std::string key;

	// Ignore starting space or tabs
	while (isTabOrSpaceOrEnd());
	if (FEOF)
		return 0;

	saveIter();

	//Check if first character alphabet
	if (current_it != line.end() && !isalpha(*current_it))
	{
		return 0;
	}


	// Main loop for all keywords
	std::string::iterator it2;
	for (int i = 0; i < NUM_KEYWORDS; i++)
	{
		resetIter();

		//Keyword data
		it2 = key.begin();
		key = keywords[i];

		while (current_it != line.end() && it2 != key.end() && toupper(*current_it) == *it2)
		{
			it2++;
			incIter();
		}

		//Check if successful
		if (it2 == key.end())
		{
			if (isTabOrSpaceOrEnd() || !isalnum(*current_it))
			{
				tok->set_token(KEYWORDS_OFFSET + i, keywords[i], line_num, current_pos_save);
				return 1;
			}
		}
	}

	resetIter();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Multiline Comment
int lexer::isMultiLineComment(token *tok)
{
	if (FEOF)
		return 0;

	bool inloop = false;

	// Ignore starting space or tabs
	while (isTabOrSpaceOrEnd());
	if (FEOF)
		return 0;

	saveIter();

	// Get Comment
	if (current_it != line.end() && *current_it == '#')
	{
		incIter();
		if (current_it != line.end() && *current_it == '*')
		{
			incIter(true);
			while (current_it != line.end() && !FEOF && (*current_it != '*'))
			{
				incIter(true);
				inloop = true;
			}
			if (current_it != line.end() && *current_it == '*')
			{
				incIter();
				if (current_it != line.end() && *current_it == '#')
				{
					incIter();
					return 1;
				}
				else
				{
					//ERROR
					lex_err->SetError(1113, "Lexer Error", line_num, current_pos);
					lex_err->AddErrorLine(" Missing * for comment end ");
					lex_err->AddError();
					enable_generic_error = false;
					return 0;
				}
			}
			else
			{
				//ERROR
				lex_err->SetError(1113, "Lexer Error", line_num, current_pos);
				lex_err->AddErrorLine(" Missing # for comment end ");
				lex_err->AddError();
				enable_generic_error = false;
				return 0;
			}
		}
		else
		{
			resetIter();
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function loops over all the keywords and returns a keyword token if encountered
int lexer::isOperator(token *tok)
{
	if (FEOF)
		return 0;

	std::string op;

	// Ignore starting space or tabs
	while (isTabOrSpaceOrEnd());
	if (FEOF)
		return 0;

	saveIter();

	// Main loop for all keywords
	std::string::iterator it2;
	for (int i = 0; i < NUM_OPERATORS; i++)
	{
		resetIter();

		//Operator data
		op = operators[i];
		it2 = op.begin();
		while (current_it != line.end() && it2 != op.end() && *current_it == *it2)
		{
			it2++;
			incIter();
		}

		//Check if successful
		if (it2 == op.end())
		{
			tok->set_token(OPERATORS_OFFEST + i, operators[i], line_num, current_pos_save);
			return 1;
		}
	}

	resetIter();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Real number
int lexer::isReal(token *tok)
{
	if (FEOF)
		return 0;

	std::string real_value("");
	bool inloop = false;

	// Ignore starting space or tabs
	while (isTabOrSpaceOrEnd());
	if (FEOF)
		return 0;

	saveIter();

	//Get first integer if exists
	while (current_it != line.end() && isdigit(*current_it))
	{
		real_value += *current_it;
		incIter();
	}

	//Get dot
	if (current_it != line.end() && *current_it == '.')
	{
		// Put dot
		real_value += *current_it;
		incIter();

		// Get Integer
		while (current_it != line.end() && isdigit(*current_it))
		{
			real_value += *current_it;
			incIter();
			inloop = true;
		}
		if (inloop)
		{
			tok->set_token(REAL_VALUE, real_value, line_num, current_pos_save);
			return 1;
		}
		else
		{
			//LEX_ERROR
			lex_err->SetError(1113, "Lexer Error", line_num, current_pos);
			lex_err->AddErrorLine(" Missing decimal point ");
			lex_err->AddError();
			enable_generic_error = false;
		}
	}

	resetIter();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Single line comment
int lexer::isSingleLineComment(token *tok)
{
	if (FEOF)
		return 0;

	// Ignore starting space or tabs
	while (isTabOrSpaceOrEnd());
	if (FEOF)
		return 0;

	saveIter();

	// Get Comment
	if (current_it != line.end() && *current_it == '#')
	{
		getNewLine(true);
		return 1;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String
int lexer::isString(token *tok)
{
	if (FEOF)
		return 0;

	char qtype;

	std::string string_value("");

	// Ignore starting space or tabs
	while (isTabOrSpaceOrEnd());
	if (FEOF)
		return 0;

	saveIter();

	// Find open quotes
	if (current_it != line.end() && *current_it == '\"')
	{
		// Get string
		qtype = *current_it;
		incIter();
		while (!FEOF && *current_it != qtype)
		{
			isEscapeSeq();
			string_value += *current_it;
			incIter(true);
		}

		if (current_it != line.end() && *current_it == qtype)
		{
			incIter();
			tok->set_token(STRING, string_value, line_num, current_pos_save);
			return 1;
		}
		else
		{
			//LEX_ERROR
			lex_err->SetError(1113, "Lexer Error", line_num, current_pos);
			lex_err->AddErrorLine(" Missing end quotes ");
			lex_err->AddError();
			enable_generic_error = false;
		}
	}

	resetIter();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tab/Space/End
int lexer::isTabOrSpaceOrEnd()
{
	if (FEOF)
		return 0;

	if (current_it == line.end())
	{
		getNewLine();
		if (FEOF)
			return 0;
		return 1;
	}

	if (*current_it == '\t' || *current_it == ' ')
	{
		incIter();
		return 1;
	}
	return 0;
}
