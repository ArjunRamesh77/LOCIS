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
    long int lineNum;
    int pos;

public:
    token();
    void set_token(int type, std::string value, long int lineNum, int pos);
    int GetType();
    std::string GetValue();
    void GetLnAndPos(int &line_number, int &pos1);
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

    lexer(const char* source, cerrors *errorptr);
    ~lexer();

    //Control Functions
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
