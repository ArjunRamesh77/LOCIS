#include "lexer.h"

#define GET_NEXT_READABLE_INPUT  if (FEOF)                      \
                                     return false;              \
                                 while (isTabOrSpaceOrEnd());   \
                                 if (FEOF)                      \
                                    return false;

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
bool lexer::isExpo(token *tok)
{
    GET_NEXT_READABLE_INPUT

	std::string real_value("");
    bool inloop(false);

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
                    tok->setToken(EXPO_VALUE, real_value, lineNum, currentPosSave);
                    return true;
				}
				else
				{
					//LEX_ERROR
                    lexErr->SetError(1001, "Lexer Error", lineNum, currentPos);
                    lexErr->AddErrorLine(" Missing exponent ");
                    lexErr->AddError();
                    enableGenericError = false;
				}
			}
		}
	}

	resetIter();
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Indentifier
bool lexer::isIdentifier(token *tok)
{
    GET_NEXT_READABLE_INPUT

	std::string identifier("");
    bool inloop(false);

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

        tok->setToken(IDENT, identifier, lineNum, currentPosSave);
        return true;
	}

	resetIter();
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Integer
bool lexer::isInteger(token *tok)
{
    GET_NEXT_READABLE_INPUT

	std::string integer_value("");
    bool inloop(false);

	saveIter();

	//Check if first character is a digit
    if (currentIt != line.end() && !isdigit(*currentIt))
	{
        return false;
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
        tok->setToken(INTEGER_VALUE, integer_value, lineNum, currentPosSave);
        return true;
	}

	resetIter();
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function loops over all the keywords and returns a keyword token if encountered
bool lexer::isKeyword(token *tok)
{
    GET_NEXT_READABLE_INPUT

    std::string key("");

	saveIter();

	//Check if first character alphabet
    if (currentIt != line.end() && !isalpha(*currentIt))
	{
        return false;
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
                tok->setToken(KEYWORDS_OFFSET + i, keywords[i], lineNum, currentPosSave);
                return true;
			}
		}
	}

	resetIter();
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Multiline Comment
bool lexer::isMultiLineComment(token *tok)
{
    GET_NEXT_READABLE_INPUT

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
			}
            if (currentIt != line.end() && *currentIt == '*')
			{
				incIter();
                if (currentIt != line.end() && *currentIt == '#')
				{
					incIter();
                    return true;
				}
				else
				{
					//ERROR
                    lexErr->SetError(1002, "Lexer Error", lineNum, currentPos);
                    lexErr->AddErrorLine(" Missing * for comment end ");
                    lexErr->AddError();
                    enableGenericError = false;
                    return false;
				}
			}
			else
			{
				//ERROR
                lexErr->SetError(1003, "Lexer Error", lineNum, currentPos);
                lexErr->AddErrorLine(" Missing # for comment end ");
                lexErr->AddError();
                enableGenericError = false;
                return false;
			}
		}
		else
		{
			resetIter();
		}
	}

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function loops over all the keywords and returns a keyword token if encountered
bool lexer::isOperator(token *tok)
{
    GET_NEXT_READABLE_INPUT

    std::string op("");

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
            tok->setToken(OPERATORS_OFFEST + i, operators[i], lineNum, currentPosSave);
            return true;
		}
	}

	resetIter();
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Real number
bool lexer::isReal(token *tok)
{
    GET_NEXT_READABLE_INPUT

	std::string real_value("");
    bool inloop(false);

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
            tok->setToken(REAL_VALUE, real_value, lineNum, currentPosSave);
            return true;
		}
		else
		{
			//LEX_ERROR
            lexErr->SetError(1004, "Lexer Error", lineNum, currentPos);
            lexErr->AddErrorLine(" Missing decimal point ");
            lexErr->AddError();
            enableGenericError = false;
		}
	}

	resetIter();
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Single line comment
bool lexer::isSingleLineComment(token *tok)
{
    GET_NEXT_READABLE_INPUT

	saveIter();

	// Get Comment
    if (currentIt != line.end() && *currentIt == '#')
	{
		getNewLine(true);
        return true;
	}

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String
bool lexer::isString(token *tok)
{
    GET_NEXT_READABLE_INPUT

    char qtype('\0');
	std::string string_value("");

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
            tok->setToken(STRING, string_value, lineNum, currentPosSave);
            return true;
		}
		else
		{
			//LEX_ERROR
            lexErr->SetError(1005, "Lexer Error", lineNum, currentPos);
            lexErr->AddErrorLine(" Missing end quotes ");
            lexErr->AddError();
            enableGenericError = false;
		}
	}

	resetIter();
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tab/Space/End
bool lexer::isTabOrSpaceOrEnd()
{
	if (FEOF)
        return false;

    if (currentIt == line.end())
	{
		getNewLine();
		if (FEOF)
            return false;
        return true;
	}

    if (*currentIt == '\t' || *currentIt == ' ' || *currentIt == '\r')
	{
		incIter();
        return true;
	}
    return false;
}
