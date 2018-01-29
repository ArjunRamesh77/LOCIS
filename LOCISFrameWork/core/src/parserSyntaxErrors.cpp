#include "parser.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void parser::syntaxErrorExpectedType(int expected)
{
	// Only set error state.error is set
	if (state.error == false)
		return;

    token *tok(NULL);
	// Use only valid token for error reporting
	tok = &errTok;

    int line_num(0);
    int pos(0);
    std::string sline_num("");
    std::string spos("");
    tok->getLnAndPos(line_num, pos);
	sline_num = std::to_string(line_num);
	spos = std::to_string(pos);

	// Retrieve token actuals
	std::string expected_str("");
	std::string found_str("");
    getActualTokenType(expected, expected_str);
    getActualTokenType(tok->getType(), found_str);

	// If Value only
	if (found_str == "")
	{
        found_str.assign(tok->getValue());
	}
	if (expected_str == "")
	{
		isDataType(expected, expected_str);
	}

	// Put error
    parseErr->setError(-1001, SYNTAX_ERROR, line_num, pos);
    parseErr->addErrorLine("Expected to find " + expected_str + ", but found \"" + found_str);
    parseErr->addError();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void parser::syntaxErrorExpectedAnyofType(std::vector<int> &expected)
{
	if (state.error == false)
		return;

    token *tok(NULL);
	// Use only valid token for error reporting
	tok = &errTok;

    int line_num(0);
    int pos(0);
    std::string sline_num("");
    std::string spos("");
    tok->getLnAndPos(line_num, pos);
	sline_num = std::to_string(line_num);
	spos = std::to_string(pos);

	// Retrieve token actuals
	std::string expected_str("");
	std::string full_expected_str("");
	std::string found_str("");

	// Add all the expected 
	for (std::vector<int>::iterator it = expected.begin(); it != expected.end(); it++)
	{
        getActualTokenType(*it, expected_str);

		// If Value only
		if (expected_str == "")
		{
			isDataType(*it, expected_str);
		}

		if (it != (expected.end() - 1))
		{
			full_expected_str += expected_str + ", ";
		}
		else
		{
			full_expected_str += expected_str;
		}
	}
    getActualTokenType(tok->getType(), found_str);

	// If Value only
	if (found_str == "")
	{
        found_str.assign(tok->getValue());
	}

	// Put error
    parseErr->setError(-1001, SYNTAX_ERROR, line_num, pos);
    parseErr->addErrorLine("Expected to find " + full_expected_str + " but found \"" + found_str);
    parseErr->addError();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void parser::syntaxErrorExpectedString(std::string expected_str)
{
    if (state.error == false)
        return;

    token *tok(NULL);
    // Use only valid token for error reporting
    tok = &errTok;

    int line_num(0);
    int pos(0);
    std::string sline_num("");
    std::string spos("");
    tok->getLnAndPos(line_num, pos);
    sline_num = std::to_string(line_num);
    spos = std::to_string(pos);

    // Retrieve token actuals;
    std::string found_str("");
    getActualTokenType(tok->getType(), found_str);
    // If Value only
    if (found_str == "")
    {
        found_str.assign(tok->getValue());
    }

    // Put error
    parseErr->setError(-1001, SYNTAX_ERROR, line_num, pos);
    parseErr->addErrorLine("Expected to find " + expected_str + ", but found \"" + found_str);
    parseErr->addError();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void parser::syntaxErrorExpectedAnyofString(std::vector<std::string> &expected)
{
	if (state.error == false)
		return;

    token *tok(NULL);
	// Use only valid token for error reporting
	tok = &errTok;

    int line_num(0);
    int pos(0);
    std::string sline_num("");
    std::string spos("");
    tok->getLnAndPos(line_num, pos);
	sline_num = std::to_string(line_num);
	spos = std::to_string(pos);

	// Retrieve token actuals
	std::string expected_str("");
	std::string full_expected_str("");
	std::string found_str("");

	// Add all the expected 
	for (std::vector<std::string>::iterator it = expected.begin(); it != expected.end(); it++)
	{
		if (it != (expected.end() - 1))
		{
			full_expected_str += *it + ", ";
		}
		else
		{
			full_expected_str += *it;
		}
	}

    getActualTokenType(tok->getType(), found_str);
	// If Value only
	if (found_str == "")
	{
        found_str.assign(tok->getValue());
	}

	// Put error
    parseErr->setError(-1001, SYNTAX_ERROR, line_num, pos);
    parseErr->addErrorLine("Expected to find " + full_expected_str + ", but found \"" + found_str);
    parseErr->addError();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void parser::syntaxErrorUnexpected()
{
	if (state.error == false)
		return;

    token *tok(NULL);
	// Use only valid token for error reporting
	tok = &errTok;

    int line_num(0);
    int pos(0);
    std::string sline_num("");
    std::string spos("");
    tok->getLnAndPos(line_num, pos);
	sline_num = std::to_string(line_num);
	spos = std::to_string(pos);

	// Put error
    parseErr->setError(-1001, SYNTAX_ERROR, line_num, pos);
    parseErr->addErrorLine("Unexpected token \"" + tok->getValue());
    parseErr->addError();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void parser::syntaxErrorMissing(int expected)
{
	if (state.error == false)
		return;

    token *tok(NULL);
	// Use only valid token for error reporting
	tok = &errTok;

    int line_num(0);
    int pos(0);
    std::string sline_num("");
    std::string spos("");
    tok->getLnAndPos(line_num, pos);
	sline_num = std::to_string(line_num);
	spos = std::to_string(pos);

	// Retrieve token actuals;
	std::string expected_str("");
    getActualTokenType(expected, expected_str);

	// Put error
    parseErr->setError(-1001, SYNTAX_ERROR, line_num, pos);
    parseErr->addErrorLine("Missing token \"" + expected_str);
    parseErr->addError();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void parser::syntaxErrorUnexpectedEof()
{
	if (state.error == false)
		return;

    token *tok(NULL);
	// Use only valid token for error reporting
	tok = &errTok;

    int line_num(0);
    int pos(0);
    std::string sline_num("");
    std::string spos("");
    tok->getLnAndPos(line_num, pos);
	sline_num = std::to_string(line_num);
	spos = std::to_string(pos);

	// Put error
    parseErr->setError(-1001, SYNTAX_ERROR, line_num, pos);
    parseErr->addErrorLine("Unexpected End of File encountered ");
    parseErr->addError();
}

