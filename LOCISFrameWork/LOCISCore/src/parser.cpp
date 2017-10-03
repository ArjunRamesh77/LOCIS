#include "parser.h"
#include "lexer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Parser 
parser::parser(lexer* lex_arg, cerrors *errorptr) :
    ex(),
    state(),
    lex(lex_arg),
    parseErr(errorptr),
    curTok(),
    errTok(),
    LAtoks(),
    FEOF(false),
    tokenTypeBeforeEOF(-1),
    context(-1)
{

}

parser::~parser()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Consumes a single token
void parser::consume(int type, bool is_terminal)
{
	ex.type = type;
	tokenTypeBeforeEOF = type;
    if (curTok.getType() != type)
	{
		// To show error after first fail
		state.error = 1;
        errTok = curTok;
	}
	else
	{
		state.error = false;
        getNextToken();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gets the string value of a given non generic token type
void parser::getActualTokenType(const int type, std::string &out)
{
	out.assign("");
	// Keywords
	if (type >= 1000 && type < 2000)
	{
		out.assign(keywords[(type - 1000)]);
	}

	// Operators
	if (type >= 2000 && type < 3000)
	{
		out.assign(operators[(type - 2000)]);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gets the next token to process
void parser::getNextToken()
{
    int retval = 0;
	// Throw exception after a consume tries to consume a EOF token
	if (FEOF)
	{
        syntaxErrorExpectedType(tokenTypeBeforeEOF);
		ex.type = FILE_ENDs;
		throw &ex;
	}

	if (!LAtoks.empty())
	{
        curTok = LAtoks.front();
        if (curTok.getType() == -100)
		{
			FEOF = true;
		}
		LAtoks.pop_back();
	}
	else
	{
        retval = lex->getNextToken(&curTok);
        if (retval == LEXER_EOF)
		{
			FEOF = true;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gets the string value for Data type
bool parser::isDataType(int type, std::string &val)
{
	switch (type)
	{
	case REAL_VALUE:
	case EXPO_VALUE:
		val.assign("Real");
		return true;
		break;

	case INTEGER_VALUE:
		val.assign("Integer");
		return true;
		break;

	case IDENT:
		val.assign("Identifier");
		return true;
		break;

	case STRING:
		val.assign("String");
		return true;
		break;

	default:
		return false;
	}
	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// for LL(k) grammars
void parser::lookAhead(int num)
{
    int retval = 0;
	token latok;
	for (int i = 1; i <= num; i++)
	{
		retval = lex->getNextToken(&latok);
		if (retval == -1)
		{
            latok.setToken(-100, NULL, -1, -1);
			LAtoks.push_back(latok);
		}
		LAtoks.push_back(latok);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Checks for equality of token type and given type
bool parser::Teq(token *tok, int val)
{

    if (tok->getType() == val)
	{
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// To start a rule
ASTNode* parser::startRule(int rule)
{
	try
	{
		// Get first token
        getNextToken();

		switch (rule)
		{
		case 0:  return P_model_collection();
		case 1:  return P_model();
		case 2:  return P_options();
		case 3:  return P_options_space();
		case 4:  return P_option();
		case 5:  return P_unit();
		case 6:  return P_desc();
		case 7:  return P_bounds();
		case 8:  return P_inequality();
		case 9:  return P_equation();
		case 10:  return P_equation_type_operator();
		case 11:  return P_numtype();
		case 12:  return P_type_name();
		case 13:  return P_array_indices();
		case 14:  return P_array_index();
		case 15:  return P_full_type_name();
		case 16:  return P_function_name();
		case 17:  return P_default();
		case 18:  return P_expression();
		case 19:  return P_term();
		case 20:  return P_factor();
		case 21:  return P_primary();
		case 22:  return P_real_number();
		case 23:  return P_logical_expression();
		case 24:  return P_lterm();
		case 25:  return P_lfactor();
		case 26:  return P_for_loop();
		case 27:  return P_single_or_compound_statement();
		case 28:  return P_if_statement();
		case 29:  return P_assignment();
		case 30:  return P_model_entity_decl();
		case 31:  return P_model_entity_type();
		case 32:  return P_model_entities();
		case 33:  return P_model_entity();
		case 34:  return P_model_section();
		case 35:  return P_section_type();
		case 36:  return P_model_statement();
		case 37:  return P_noneq_statement();
		case 38:  return P_eq_statement();
		case 39:  return P_comparison_type_operator();
		default:  return NULL;
		}
	}

	// Catch all parser exceptions
	catch(parserException* pex)
	{
		return NULL;
	}
	return NULL;
}




