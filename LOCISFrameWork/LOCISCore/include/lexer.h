#ifndef LOCIS_LEXER_H
#define LOCIS_LEXER_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOCIS lexer includes
#include <fstream>
#include "error.h"
#include <string>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOCIS lexer defines
// Keyword
#define NUM_KEYWORDS 29
#define KEYWORDS_OFFSET 1000
#define DATATYPES_OFFSET 3000

// Operators
#define NUM_OPERATORS 31
#define OPERATORS_OFFEST 2000

extern const std::string keywords[NUM_KEYWORDS];
extern const std::string operators[NUM_OPERATORS];

// LOCIS Keywords enums
enum token_keywords {
    MODEL = KEYWORDS_OFFSET,
    PARAMETER,
    VARIABLE,
    EQUATION,
    REAL,
    INTEGER,
    UNIT,
    TYPE,
    DESC,
    FOR,
    SET,
    FIX,
    INIT,
    OBJECT,
    pIF,
    pELSE,
    ITER,
    GUESS,
    LINE,
    BASIS,
    SEGMENT,
    ID,
    NUMEL,
    LENGTH,
    pDOMAIN,
    PDEVAR,
    PDE,
    VALUE,
    FLUX
};

// LOCIS Keywords data types
enum token_datatypes {
    REAL_VALUE = DATATYPES_OFFSET,
    INTEGER_VALUE,
    EXPO_VALUE,
    IDENT,
    STRING
};

// Token Enums
enum token_operators {
    DEQUALS = OPERATORS_OFFEST,
    GEQUALS,
    LEQUALS,
    NEQUALS,
    CEQUALS,
    DCOLON,
    AND,
    OR,
    LCIRCLE,
    RCIRCLE,
    LSQUARE,
    RSQUARE,
    LCURLY,
    RCURLY,
    QUESTION,
    SEMICOLON,
    BSLASH,
    COLON,
    DOT,
    GTHAN,
    LTHAN,
    EQUALS,
    PLUS,
    MINUS,
    DIV,
    MULT,
    RAISE,
    NOT,
    COMMA,
    DOLLAR,
    APOS
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOCIS token class
class token
{
private:
	int type;
	std::string value;
	long int line_num;
	int pos;

public:
	//Assign
	void set_token(int type, std::string value, long int line_num, int pos)
	{
		this->type = type;
		this->value = value;
		this->line_num = line_num;
		this->pos = pos;
	}

	//Return type
	int GetType()
	{
		return type;
	}

	//Return Value
	std::string GetValue()
	{
		return value;
	}

	//Return Line number
	void GetLnAndPos(int &line_number, int &pos1)
	{
		line_number = line_num;
		pos1 = pos;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOCIS lexer
class lexer
{
private:
	//file data
	std::string fname;
	std::stringstream file;

	//line data
	std::string line;
	long int line_num;
	bool current_read;
	int current_pos;
	std::string::iterator current_it;
	bool FEOF;
	bool enable_generic_error;

	//Save data
	std::string::iterator current_it_save;
	int current_pos_save;

	//Error
	cerrors *lex_err;

public:
	
	lexer(std::string name, cerrors *errorptr);
	lexer(const char* source, cerrors *errorptr);
	~lexer();

	//Control Functions
	int getNextToken(token *tok);
	void checkNewLine();
	void saveIter();
	void resetIter();
	void incIter(bool nl = false);
	void getNewLine(bool force = false);
	int tryRecover();

	//isFunctions
	int isTabOrSpaceOrEnd();
	int isInteger(token *tok);
	int isReal(token *tok);
	int isKeyword(token *tok);
	int isOperator(token *tok);
	int isString(token *tok);
	int isExpo(token *tok);
	int isIdentifier(token *tok);
	int isMultiLineComment(token *tok);
	int isSingleLineComment(token *tok);
	void isEscapeSeq();
private:
	bool bIsFile;
};

#endif // ! LOCIS_LEXER_H
