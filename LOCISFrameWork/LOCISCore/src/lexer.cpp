#include "lexer.h"

///////////////////////////////////////////////// <KEYWORDS> //////////////////////////////////////////////////////////////////////
// LOCIS Keywords
const std::string keywords[NUM_KEYWORDS] = { "MODEL",
                                                    "PARAMETER",
                                                    "VARIABLE",
                                                    "EQUATION",
                                                    "REAL",
                                                    "INTEGER",
                                                    "UNIT",
                                                    "TYPE",
                                                    "DESC",
                                                    "FOR",
                                                    "SET",
                                                    "FIX",
                                                    "INIT",
                                                    "OBJECT",
                                                    "IF",
                                                    "ELSE",
                                                    "ITER",
                                                    "GUESS",
                                                    "LINE",
                                                    "BASIS",
                                                    "SEGMENT",
                                                    "ID",
                                                    "NUMEL",
                                                    "LENGTH",
                                                    "DOMAIN",
                                                    "PDEVAR",
                                                    "PDE",
                                                    "VALUE",
                                                    "FLUX"
                                                  };



///////////////////////////////////////////////// <OPERATORS> //////////////////////////////////////////////////////////////////////
const std::string operators[NUM_OPERATORS] = { "==",
                                                      ">=",
                                                      "<=",
                                                      "!=",
                                                      ":=",
                                                      "::",
                                                      "&&",
                                                      "||",
                                                      "(",
                                                      ")",
                                                      "[",
                                                      "]",
                                                      "{",
                                                      "}",
                                                      "?",
                                                      ";",
                                                      "\\",
                                                      ":",
                                                      ".",
                                                      ">",
                                                      "<",
                                                      "=",
                                                      "+",
                                                      "-",
                                                      "/",
                                                      "*",
                                                      "^",
                                                      "!",
                                                      ",",
                                                      "$",
                                                      "'"};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// token
token::token() :
 type(-1),
 value(""),
 lineNum(-1),
 pos(-1)
{

}

void token::set_token(int type_arg, std::string value_arg, long lineNum_arg, int pos_arg)
{
    type = type_arg;
    value = value_arg;
    lineNum = lineNum_arg;
    pos = pos_arg;
}

int token::GetType()
{
    return type;
}

std::string token::GetValue()
{
    return value;
}

void token::GetLnAndPos(int &lineNumber_arg, int &pos_arg)
{
    lineNumber_arg = lineNum;
    pos_arg = pos;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOCIS lexer constructor ( Char source )
lexer::lexer(const char* source, cerrors *errorptr) :
    file(source),
    line(""),
    lineNum(0),
    currentPos(0),
    currentIt(NULL),
    FEOF(false),
    enableGenericError(true),
    currentItSave(NULL),
    currentPosSave(0),
    lexErr(errorptr)
{
	std::getline(file, line);
    currentIt = line.begin();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
lexer::~lexer()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void lexer::getNewLine(bool force)
{
    if (currentIt == line.end() || force)
	{
		if (!std::getline(file, line))
		{
			FEOF = true;
			return;
		}
        lineNum++;
        currentPos = 0;
        currentIt = line.begin();

		// Skip Blank lines
        while (!FEOF && currentIt == line.end())
		{
			if (!std::getline(file, line))
			{
				FEOF = true;
				return;
			}
            lineNum++;
            currentPos = 0;
            currentIt = line.begin();
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
    if (enableGenericError)
	{
        lexErr->SetError(1113, "Lexer Error", lineNum, currentPos);
        lexErr->AddErrorLine(" Unidentified token found ");
        lexErr->AddError();
        enableGenericError = true;
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
    if (currentIt != line.end())
	{
        currentIt++;
        currentPos++;
	}
	if (nl)
		getNewLine();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void lexer::resetIter()
{
    currentIt = currentItSave;
    currentPos = currentPosSave;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void lexer::saveIter()
{
    currentItSave = currentIt;
    currentPosSave = currentPos;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
int lexer::tryRecover()
{
	//Skip to Next Readable token
    while (currentIt != line.end())
	{
        if (*currentIt == '\t' || *currentIt == ' ')
		{
			return 1;
		}
		incIter();
	}

	return 0;
}


