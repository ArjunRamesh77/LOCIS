#include "lexer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Escape Sequence
void lexer::isEscapeSeq()
{
    if (*currentIt == '\\')
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
    while (currentIt != line.end() && isdigit(*currentIt))
	{
        real_value += *currentIt;
		incIter();
	}

	//Get dot
    if (currentIt != line.end() && *currentIt == '.')
	{
		// Put dot
        real_value += *currentIt;
		incIter();

		// Get Integer
        while (currentIt != line.end() && isdigit(*currentIt))
		{
            real_value += *currentIt;
			incIter();
			inloop = true;
		}
		if (inloop)
		{
			//Get E
            if (currentIt != line.end() && (*currentIt == 'e' || *currentIt == 'E'))
			{
                real_value += *currentIt;
				incIter();

				//Get Integer
                if (currentIt != line.end() && (*currentIt == '+' || *currentIt == '-'))
				{
                    real_value += *currentIt;
					incIter();
				}
                while (currentIt != line.end() && isdigit(*currentIt))
				{
                    real_value += *currentIt;
					incIter();
					inloop = true;
				}
				if (inloop)
				{
                    tok->set_token(EXPO_VALUE, real_value, lineNum, currentPosSave);
					return 1;
				}
				else
				{
					//LEX_ERROR
                    lexErr->SetError(1113, "Lexer Error", lineNum, currentPos);
                    lexErr->AddErrorLine(" Missing exponent ");
                    lexErr->AddError();
                    enableGenericError = false;
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
    if (currentIt != line.end() && (isalpha(*currentIt) || (*currentIt == '_')))
	{
        identifier += *currentIt;
		incIter();
		inloop = true;
	}

	// Get Full name
	if (inloop)
	{
		//Check if it has more characters
        while (currentIt != line.end() && (isalnum(*currentIt) || *currentIt == '_'))
		{
            identifier += *currentIt;
			incIter();
		}

        tok->set_token(IDENT, identifier, lineNum, currentPosSave);
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
    if (currentIt != line.end() && !isdigit(*currentIt))
	{
		return 0;
	}

	// Get Integer
    while (currentIt != line.end() && isdigit(*currentIt))
	{
        integer_value += *currentIt;
		incIter();
		inloop = true;
	}
	if (inloop)
	{
        tok->set_token(INTEGER_VALUE, integer_value, lineNum, currentPosSave);
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
    if (currentIt != line.end() && !isalpha(*currentIt))
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

        while (currentIt != line.end() && it2 != key.end() && toupper(*currentIt) == *it2)
		{
			it2++;
			incIter();
		}

		//Check if successful
		if (it2 == key.end())
		{
            if (isTabOrSpaceOrEnd() || !isalnum(*currentIt))
			{
                tok->set_token(KEYWORDS_OFFSET + i, keywords[i], lineNum, currentPosSave);
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
    if (currentIt != line.end() && *currentIt == '#')
	{
		incIter();
        if (currentIt != line.end() && *currentIt == '*')
		{
			incIter(true);
            while (currentIt != line.end() && !FEOF && (*currentIt != '*'))
			{
				incIter(true);
				inloop = true;
			}
            if (currentIt != line.end() && *currentIt == '*')
			{
				incIter();
                if (currentIt != line.end() && *currentIt == '#')
				{
					incIter();
					return 1;
				}
				else
				{
					//ERROR
                    lexErr->SetError(1113, "Lexer Error", lineNum, currentPos);
                    lexErr->AddErrorLine(" Missing * for comment end ");
                    lexErr->AddError();
                    enableGenericError = false;
					return 0;
				}
			}
			else
			{
				//ERROR
                lexErr->SetError(1113, "Lexer Error", lineNum, currentPos);
                lexErr->AddErrorLine(" Missing # for comment end ");
                lexErr->AddError();
                enableGenericError = false;
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
        while (currentIt != line.end() && it2 != op.end() && *currentIt == *it2)
		{
			it2++;
			incIter();
		}

		//Check if successful
		if (it2 == op.end())
		{
            tok->set_token(OPERATORS_OFFEST + i, operators[i], lineNum, currentPosSave);
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
    while (currentIt != line.end() && isdigit(*currentIt))
	{
        real_value += *currentIt;
		incIter();
	}

	//Get dot
    if (currentIt != line.end() && *currentIt == '.')
	{
		// Put dot
        real_value += *currentIt;
		incIter();

		// Get Integer
        while (currentIt != line.end() && isdigit(*currentIt))
		{
            real_value += *currentIt;
			incIter();
			inloop = true;
		}
		if (inloop)
		{
            tok->set_token(REAL_VALUE, real_value, lineNum, currentPosSave);
			return 1;
		}
		else
		{
			//LEX_ERROR
            lexErr->SetError(1113, "Lexer Error", lineNum, currentPos);
            lexErr->AddErrorLine(" Missing decimal point ");
            lexErr->AddError();
            enableGenericError = false;
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
    if (currentIt != line.end() && *currentIt == '#')
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
    if (currentIt != line.end() && *currentIt == '\"')
	{
		// Get string
        qtype = *currentIt;
		incIter();
        while (!FEOF && *currentIt != qtype)
		{
			isEscapeSeq();
            string_value += *currentIt;
			incIter(true);
		}

        if (currentIt != line.end() && *currentIt == qtype)
		{
			incIter();
            tok->set_token(STRING, string_value, lineNum, currentPosSave);
			return 1;
		}
		else
		{
			//LEX_ERROR
            lexErr->SetError(1113, "Lexer Error", lineNum, currentPos);
            lexErr->AddErrorLine(" Missing end quotes ");
            lexErr->AddError();
            enableGenericError = false;
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

    if (currentIt == line.end())
	{
		getNewLine();
		if (FEOF)
			return 0;
		return 1;
	}

    if (*currentIt == '\t' || *currentIt == ' ' || *currentIt == '\r')
	{
		incIter();
		return 1;
	}
	return 0;
}
