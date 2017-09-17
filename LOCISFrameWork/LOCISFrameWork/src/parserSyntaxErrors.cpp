#include "parser.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void parser::ErrorSyntaxType1(int expected)
{
	// Only set error state.error is set
	if (state.error == false)
		return;

	token *tok;
	// Use only valid token for error reporting
	tok = &err_tok;

	int line_num;
	int pos;
	std::string sline_num;
	std::string spos;
	tok->GetLnAndPos(line_num, pos);
	sline_num = std::to_string(line_num);
	spos = std::to_string(pos);

	// Retrieve token actuals
	std::string expected_str("");
	std::string found_str("");
	GetActualTokenType(expected, expected_str);
	GetActualTokenType(tok->GetType(), found_str);

	// If Value only
	if (found_str == "")
	{
		found_str.assign(tok->GetValue());
	}
	if (expected_str == "")
	{
		isDataType(expected, expected_str);
	}

	// Put error
	parse_err->SetError(-1001, SYNTAX_ERROR, line_num, pos);
	parse_err->AddErrorLine("Expected to find " + expected_str + ", but found \"" + found_str + "\" | " + sline_num + " | " + spos);
	parse_err->AddError();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void parser::ErrorSyntaxType2(std::vector<int> &expected)
{
	if (state.error == false)
		return;

	token *tok;
	// Use only valid token for error reporting
	tok = &err_tok;

	int line_num;
	int pos;
	std::string sline_num;
	std::string spos;
	tok->GetLnAndPos(line_num, pos);
	sline_num = std::to_string(line_num);
	spos = std::to_string(pos);

	// Retrieve token actuals
	std::string expected_str("");
	std::string full_expected_str("");
	std::string found_str("");

	// Add all the expected 
	for (std::vector<int>::iterator it = expected.begin(); it != expected.end(); it++)
	{
		GetActualTokenType(*it, expected_str);

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
	GetActualTokenType(tok->GetType(), found_str);

	// If Value only
	if (found_str == "")
	{
		found_str.assign(tok->GetValue());
	}

	// Put error
	parse_err->SetError(-1001, SYNTAX_ERROR, line_num, pos);
	parse_err->AddErrorLine("Expected to find " + full_expected_str + " but found \"" + found_str + "\" | " + sline_num + " | " + spos);
	parse_err->AddError();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void parser::ErrorSyntaxType3(std::vector<std::string> &expected)
{
	if (state.error == false)
		return;

	token *tok;
	// Use only valid token for error reporting
	tok = &err_tok;

	int line_num;
	int pos;
	std::string sline_num;
	std::string spos;
	tok->GetLnAndPos(line_num, pos);
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

	GetActualTokenType(tok->GetType(), found_str);
	// If Value only
	if (found_str == "")
	{
		found_str.assign(tok->GetValue());
	}

	// Put error
	parse_err->SetError(-1001, SYNTAX_ERROR, line_num, pos);
	parse_err->AddErrorLine("Expected to find " + full_expected_str + ", but found \"" + found_str + "\" | " + sline_num + " | " + spos);
	parse_err->AddError();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void parser::ErrorSyntaxType4()
{
	if (state.error == false)
		return;

	token *tok;
	// Use only valid token for error reporting
	tok = &err_tok;

	int line_num;
	int pos;
	std::string sline_num;
	std::string spos;
	tok->GetLnAndPos(line_num, pos);
	sline_num = std::to_string(line_num);
	spos = std::to_string(pos);

	// Put error
	parse_err->SetError(-1001, SYNTAX_ERROR, line_num, pos);
	parse_err->AddErrorLine("Unexpected token \"" + tok->GetValue() + "\" | " + sline_num + " | " + spos);
	parse_err->AddError();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void parser::ErrorSyntaxType6(std::string expected_str)
{
	if (state.error == false)
		return;

	token *tok;
	// Use only valid token for error reporting
	tok = &err_tok;

	int line_num;
	int pos;
	std::string sline_num;
	std::string spos;
	tok->GetLnAndPos(line_num, pos);
	sline_num = std::to_string(line_num);
	spos = std::to_string(pos);

	// Retrieve token actuals;
	std::string found_str("");
	GetActualTokenType(tok->GetType(), found_str);
	// If Value only
	if (found_str == "")
	{
		found_str.assign(tok->GetValue());
	}

	// Put error
	parse_err->SetError(-1001, SYNTAX_ERROR, line_num, pos);
	parse_err->AddErrorLine("Expected to find " + expected_str + ", but found \"" + found_str + "\" | " + sline_num + " | " + spos);
	parse_err->AddError();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void parser::ErrorSyntaxType7(int expected)
{
	if (state.error == false)
		return;

	token *tok;
	// Use only valid token for error reporting
	tok = &err_tok;

	int line_num;
	int pos;
	std::string sline_num;
	std::string spos;
	tok->GetLnAndPos(line_num, pos);
	sline_num = std::to_string(line_num);
	spos = std::to_string(pos);

	// Retrieve token actuals;
	std::string expected_str("");
	GetActualTokenType(expected, expected_str);

	// Put error
	parse_err->SetError(-1001, SYNTAX_ERROR, line_num, pos);
	parse_err->AddErrorLine("Missing token \"" + expected_str + "\" | " + sline_num + " | " + spos);
	parse_err->AddError();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
void parser::ErrorSyntaxType8()
{
	if (state.error == false)
		return;

	token *tok;
	// Use only valid token for error reporting
	tok = &err_tok;

	int line_num;
	int pos;
	std::string sline_num;
	std::string spos;
	tok->GetLnAndPos(line_num, pos);
	sline_num = std::to_string(line_num);
	spos = std::to_string(pos);

	// Put error
	parse_err->SetError(-1001, SYNTAX_ERROR, line_num, pos);
	parse_err->AddErrorLine("Unexpected End of File encountered | " + sline_num + " | " + spos);
	parse_err->AddError();
}