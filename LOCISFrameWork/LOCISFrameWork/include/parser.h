#ifndef LOCIS_PARSER_H
#define LOCIS_PARSER_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOCIS Parser includes
#include "lexer.h"
#include "AST.h"
#include "ASTFEMoneD.h"
#include <vector>
#include<queue>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOCIS Parse builder macros
enum parserExecptions { FILE_ENDs, TERMINAL_ERR };

// Initiate beginning of a production rule
#define START_RULE(rule)  ASTReturnNode = NULL; ASTtempNode = NULL;												

// End of a production rule
#define END_RULE }

// Checks whether a syntax error should be thrown
#define SHOW_ERROR CheckStateError() && CheckEnableError()

#define SYNTAX_ERROR "Syntax Error"

// Main construct to match tokens
#define EXPECT_TOKEN_ELSE(TOK) 		Consume(TOK);															\
									if (SHOW_ERROR)															\
									{																		\

// Main construct to end matching tokens
#define END_EXPECT						state.error = false;												\
										ex.type = TERMINAL_ERR;												\
										throw &ex;															\
									}																		


// For loop construct for matching tokens
#define FOR_ALL_WHILE  while(
#define ITEM_S(TOK)		  Teq(&cur_tok,TOK) ||
#define ITEM_E(TOK)		  Teq(&cur_tok, TOK)) {
#define END_WHILE       }

// If statement for matching different type tokens
#define FOR_ALL_IF		  if(
#define ITEM_S(TOK)		  Teq(&cur_tok,TOK) ||
#define ITEM_E(TOK)		  Teq(&cur_tok, TOK)) {
#define END_IF			  }

// Call a new production rule
#define DO_RULE(value)	  ASTtempNode = P_##value(); 

// If statement for matching a single type of token
#define IF(TOK)		if(Teq(&cur_tok, TOK)) {			
#define ELIF(TOK)	} else if(Teq(&cur_tok, TOK)) {
#define ELSE        } else {

// Switch statement for matching tokens
#define	SWITCH switch(cur_tok.GetType()) {

#define CASE(TOK) case TOK:				    \
				  EXPECT_TOKEN_ELSE(TOK)		\
				  END_EXPECT				        \
				  break;
#define CASE_RULE(TOK, value) case TOK:			    \
							  DO_RULE(value)		\
							  break;

#define LAST	default:

#define END_SWITCH }

// eats a token
#define EAT(posi)   Consume(cur_tok.GetType());												
	
// Syntax Error Types
#define SYNTAX_ERROR_EXPECTED_TYPE(type) ErrorSyntaxType1(type);

#define SYNTAX_ERROR_EXPECTED_ANYOF_TYPE(possible_states) ErrorSyntaxType2(possible_states);

#define SYNTAX_ERROR_EXPECTED_STRING(string) ErrorSyntaxType6(string);

#define SYNTAX_ERROR_EXPECTED_ANYOF_STRING(possible_states) ErrorSyntaxType3(possible_states);

#define SYNTAX_ERROR_UNEXPECTED	ErrorSyntaxType4();

#define SYNTAX_ERROR_MISSING(type) ErrorSyntaxType7(type);

#define SYNTAX_ERROR_UNEXPECTED_EOF ErrorSyntaxType8();

// Forces/Unforces error reporting
#define FORCE_ERROR state.error = true; err_tok = cur_tok;
#define UNFORCE_ERROR state.error = false;	ex.type = TERMINAL_ERR;	throw &ex;	

// Context switches
#define NONE 0
#define FOR_EQ_TYPE 1
#define FOR_NONEQ_TYPE  2

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//AST BUILD HELPERS
#define AST_DEC_TOK(name) token t_##name; t_##name.set_token(-1, "", -1, -1);
#define AST_DEC_VEC(name) std::vector<ASTNode*> astvn_##name;
#define AST_DEC_NODE(name) ASTNode* astn_##name = NULL;
#define AST_SAV_TOK(name) t_##name = cur_tok;
#define AST_SAV_NODE(name) astn_##name = ASTtempNode;
#define AST_SAV_VEC(name) astvn_##name.push_back(ASTtempNode);
#define AST_RETURN_NODE(name)	ASTReturnNode = new name;				\
								return static_cast<name*>(ASTReturnNode)->CreateNode
#define AST_RETURN_NO_CREATE return ASTtempNode;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOCIS Parser Structures
struct parserState
{
	bool error;

	//Initialize state
	parserState()
	{
		// state
		error = false;
	}
};

struct parserException
{
	int code;
	int type;

	parserException()
	{
		type = -1;
		code = -1;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOCIS Parser class
class parser
{
private:
	parserException ex;
	lexer* lex;
	cerrors *parse_err;
	token cur_tok;
	token err_tok;
	parserState state;
	ASTNode *ASTtempNode;
	ASTNode* ASTReturnNode;
	std::deque<token> LAtoks;

	bool FEOF;
	int tokenTypeBeforeEOF;
	bool enable_error_write;
	int context;
	int retval;

private:
	//Control
	void GetNextTokenP();
	void Consume(const int &type, bool is_terminal = false);
	bool Teq(token *tok, const int &val);
	bool CheckStateError();
	bool CheckEnableError();
	void LookAhead(const int &num);
	void GetActualTokenType(int type, std::string &out);
	bool isDataType(int &type, std::string &val);
	void PrintAllErrors();

	//Errors
	void ErrorSyntaxType1(int expected);
	void ErrorSyntaxType2(std::vector<int> &expected);
	void ErrorSyntaxType3(std::vector<std::string> &expected);
	void ErrorSyntaxType4();
	void ErrorSyntaxType6(std::string expected);
	void ErrorSyntaxType7(int expected);
	void ErrorSyntaxType8();

public:
	parser(lexer* lex0, cerrors *errorptr);
	~parser();
	ASTNode* startRule(int rule);

	//Rules
	ASTNode* P_model_collection();							
	ASTNode* P_model();										
	ASTNode* P_options();									
	ASTNode* P_options_space();								
	ASTNode* P_option();									
	ASTNode* P_unit();										
	ASTNode* P_desc();										
	ASTNode* P_bounds();									
	ASTNode* P_inequality();								
	ASTNode* P_equation();									
	ASTNode* P_equation_type_operator();					
	ASTNode* P_numtype();									
	ASTNode* P_type_name();									
	ASTNode* P_array_indices();								
	ASTNode* P_array_index();								
	ASTNode* P_full_type_name();							
	ASTNode* P_function_name();								
	ASTNode* P_default();									
	ASTNode* P_expression();								
	ASTNode* P_term();										
	ASTNode* P_factor();									
	ASTNode* P_primary();									
	ASTNode* P_real_number();								
	ASTNode* P_logical_expression();						
	ASTNode* P_lterm();										
	ASTNode* P_lfactor();									
	ASTNode* P_for_loop();									
	ASTNode* P_single_or_compound_statement();				
	ASTNode* P_if_statement();								
	ASTNode* P_assignment();								
	ASTNode* P_model_entity_decl();							
	ASTNode* P_model_entity_type();							
	ASTNode* P_model_entities();							
	ASTNode* P_model_entity();								
	ASTNode* P_model_section();								
	ASTNode* P_section_type();								
	ASTNode* P_model_statement();							
	ASTNode* P_noneq_statement();							
	ASTNode* P_eq_statement();								
	ASTNode* P_comparison_type_operator();	
	ASTNode* P_simulation_args();
	
	// FEMoneD
	ASTNode* P_basis_type();
	ASTNode* P_segment_def();
	ASTNode* P_segment_attributes();
	ASTNode* P_segment_attribute();
	ASTNode* P_segment_id_type();
	ASTNode* P_segment_numel();
	ASTNode* P_segment_length();
	ASTNode* P_domain_option();
	ASTNode* P_fem1d_equation();
	ASTNode* P_string_index();
	ASTNode* P_pdevar_def();
	ASTNode* P_pdevar_attributes();
	ASTNode* P_derivative_sigle_var();
	ASTNode* P_boundary_type();
};

#endif // !LOCIS_PARSER_H