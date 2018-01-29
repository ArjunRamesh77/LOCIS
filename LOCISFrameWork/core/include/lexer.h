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

// Lexer error codes
#define LEXER_EOF -1
#define LEXER_SUCCESS 0
#define LEXER_FAIL 1

extern const std::string keywords[NUM_KEYWORDS];
extern const std::string operators[NUM_OPERATORS];

// LOCIS Keywords enums
enum token_keywords {
    KW_MODEL = KEYWORDS_OFFSET,
    KW_PARAMETER,
    KW_VARIABLE,
    KW_EQUATION,
    KW_REAL,
    KW_INTEGER,
    KW_UNIT,
    KW_TYPE,
    KW_DESC,
    KW_FOR,
    KW_SET,
    KW_FIX,
    KW_INIT,
    KW_OBJECT,
    KW_pIF,
    KW_pELSE,
    KW_ITER,
    KW_GUESS,
    KW_LINE,
    KW_BASIS,
    KW_SEGMENT,
    KW_ID,
    KW_NUMEL,
    KW_LENGTH,
    KW_pDOMAIN,
    KW_PDEVAR,
    KW_PDE,
    KW_VALUE,
    KW_FLUX
};

// LOCIS Keywords data types
enum token_datatypes {
    DT_REAL_VALUE = DATATYPES_OFFSET,
    DT_INTEGER_VALUE,
    DT_EXPO_VALUE,
    DT_IDENT,
    DT_STRING
};

// Token Enums
enum token_operators {
    OP_DEQUALS = OPERATORS_OFFEST,
    OP_GEQUALS,
    OP_LEQUALS,
    OP_NEQUALS,
    OP_CEQUALS,
    OP_DCOLON,
    OP_AND,
    OP_OR,
    OP_LCIRCLE,
    OP_RCIRCLE,
    OP_LSQUARE,
    OP_RSQUARE,
    OP_LCURLY,
    OP_RCURLY,
    OP_QUESTION,
    OP_SEMICOLON,
    OP_BSLASH,
    OP_COLON,
    OP_DOT,
    OP_GTHAN,
    OP_LTHAN,
    OP_EQUALS,
    OP_PLUS,
    OP_MINUS,
    OP_DIV,
    OP_MULT,
    OP_RAISE,
    OP_NOT,
    OP_COMMA,
    OP_DOLLAR,
    OP_APOS
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOCIS token class
class token
{
private:
    int type;
    std::string value;
    long int lineNum;
    int pos;

public:
    token();
    void setToken(int type, std::string value, long int lineNum, int pos);
    int getType();
    std::string getValue();
    void getLnAndPos(int &line_number, int &pos1);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOCIS lexer
class lexer
{
private:
    //file data
    std::stringstream file;

    //line data
    std::string line;
    long int lineNum;
    int currentPos;
    std::string::iterator currentIt;
    bool FEOF;
    bool enableGenericError;

    //Save data
    std::string::iterator currentItSave;
    int currentPosSave;

    //Error
    cerrors *lexErr;

public:
    lexer();
    lexer(const char* source, cerrors *errorptr);
    ~lexer();

    //Control Functions
    void setSource(const char* cpSource);
    void setErrorHandler(cerrors *errorptr);
    int getNextToken(token *tok);
    void saveIter();
    void resetIter();
    void incIter(bool nl = false);
    void getNewLine(bool force = false);
    void tryRecover();

    //isFunctions
    bool isTabOrSpaceOrEnd();
    bool isInteger(token *tok);
    bool isReal(token *tok);
    bool isKeyword(token *tok);
    bool isOperator(token *tok);
    bool isString(token *tok);
    bool isExpo(token *tok);
    bool isIdentifier(token *tok);
    bool isMultiLineComment(token *tok);
    bool isSingleLineComment(token *tok);
    void isEscapeSeq();
};

#endif // ! LOCIS_LEXER_H
