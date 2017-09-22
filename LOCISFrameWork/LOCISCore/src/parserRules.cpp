#include "parser.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_model_collection()
{
	AST_DEC_VEC(all_models)

	START_RULE(model_collection)

	FOR_ALL_WHILE
		ITEM_E(MODEL)
			DO_RULE(model) AST_SAV_VEC(all_models) 
	END_WHILE
	
	AST_RETURN_NODE(ASTmodel_collectionNode)(astvn_all_models);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_model()
{
	AST_DEC_TOK(model_name)
	AST_DEC_VEC(all_model_statements)
	AST_DEC_NODE(simArgs)
	
	START_RULE(model)

	EXPECT_TOKEN_ELSE(MODEL)
		SYNTAX_ERROR_EXPECTED_TYPE(MODEL)
	END_EXPECT

	AST_SAV_TOK(model_name)

	EXPECT_TOKEN_ELSE(IDENT)
		SYNTAX_ERROR_EXPECTED_STRING("Indetifier")
	END_EXPECT

	IF(LTHAN)
		DO_RULE(simulation_args) AST_SAV_NODE(simArgs)
	END_IF

	EXPECT_TOKEN_ELSE(LCURLY)
		SYNTAX_ERROR_EXPECTED_TYPE(LCURLY)
	END_EXPECT

	FOR_ALL_WHILE
		!ITEM_E(RCURLY)
			DO_RULE(model_statement) AST_SAV_VEC(all_model_statements) 
	END_WHILE
		
	EXPECT_TOKEN_ELSE(RCURLY)
		SYNTAX_ERROR_MISSING(RCURLY)
	END_EXPECT

	AST_RETURN_NODE(ASTmodelNode)(&t_model_name, astvn_all_model_statements, astn_simArgs);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_model_statement()
{
	START_RULE(model_statement)

	FOR_ALL_IF
		ITEM_S(PARAMETER)
		ITEM_S(VARIABLE)
		ITEM_S(ITER)
		ITEM_S(IDENT)
		ITEM_E(LINE)
			DO_RULE(model_entity_decl)
	ELSE
		FOR_ALL_IF
			ITEM_S(SET)
			ITEM_S(FIX)
			ITEM_S(INIT)
			ITEM_S(GUESS)
			ITEM_E(EQUATION)
				DO_RULE(model_section)
		ELSE
			SYNTAX_ERROR_UNEXPECTED
		END_IF
	END_IF

	AST_RETURN_NO_CREATE
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_model_entity_decl()
{
	AST_DEC_NODE(model_entity_type)
	AST_DEC_NODE(options)
	AST_DEC_NODE(numtype)
	AST_DEC_NODE(model_entities)

	START_RULE(model_entity_decl)

	DO_RULE(model_entity_type) AST_SAV_NODE(model_entity_type) 

	FOR_ALL_IF
		ITEM_S(REAL)
		ITEM_E(INTEGER)
			DO_RULE(numtype) AST_SAV_NODE(numtype) 
	END_IF

	DO_RULE(model_entities) AST_SAV_NODE(model_entities) 

	EXPECT_TOKEN_ELSE(SEMICOLON)
		SYNTAX_ERROR_MISSING(SEMICOLON)
	END_EXPECT

	AST_RETURN_NODE(ASTmodel_entity_decl_groupNode)(astn_model_entity_type, astn_numtype, astn_model_entities);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_model_section()
{
	AST_DEC_NODE(section_name)
	AST_DEC_NODE(all_statements)
	AST_DEC_NODE(options)

	START_RULE(model_section)

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONTEXT FIND
	context = NONE;
	FOR_ALL_IF
		ITEM_S(SET)
		ITEM_S(FIX)
		ITEM_E(GUESS)
			context = FOR_NONEQ_TYPE;
	ELSE
		FOR_ALL_IF
			ITEM_S(INIT)
			ITEM_E(EQUATION)
				context = FOR_EQ_TYPE;
		END_IF
	END_IF
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONTEXT FIND

	DO_RULE(section_type) AST_SAV_NODE(section_name) 

	IF(DCOLON)
		DO_RULE(options) AST_SAV_NODE(options) 
	END_IF

	DO_RULE(single_or_compound_statement) AST_SAV_NODE(all_statements) 

	AST_RETURN_NODE(ASTmodel_sectionNode)(astn_section_name, astn_options, astn_all_statements);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_section_type()
{
	AST_DEC_TOK(section_name)
	AST_SAV_TOK(section_name)

	START_RULE(section_type)

	SWITCH
		CASE(SET)
		CASE(FIX)
		CASE(GUESS)
		CASE(INIT)
		CASE(EQUATION)
		LAST
			FORCE_ERROR
				std::vector<int> possible_states;
				possible_states.push_back(SET);
				possible_states.push_back(FIX);
				possible_states.push_back(GUESS);
				possible_states.push_back(INIT);
				possible_states.push_back(EQUATION);
				SYNTAX_ERROR_EXPECTED_ANYOF_TYPE(possible_states)
			UNFORCE_ERROR
	END_SWITCH

	AST_RETURN_NODE(ASTgeneric_tokenNode)(&t_section_name);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_options()
{
	START_RULE(options)

	EXPECT_TOKEN_ELSE(DCOLON)
		SYNTAX_ERROR_EXPECTED_TYPE(DCOLON)
	END_EXPECT

	EXPECT_TOKEN_ELSE(LCIRCLE)
		SYNTAX_ERROR_EXPECTED_TYPE(LCIRCLE)
	END_EXPECT

	DO_RULE(options_space)

	EXPECT_TOKEN_ELSE(RCIRCLE)
		SYNTAX_ERROR_MISSING(RCIRCLE)
	END_EXPECT

	AST_RETURN_NO_CREATE
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_options_space()
{
	AST_DEC_VEC(all_options)

	START_RULE(options_space)

	DO_RULE(option) AST_SAV_VEC(all_options) 

	FOR_ALL_WHILE
		ITEM_E(COMMA)
			EAT(0)
			DO_RULE(option) AST_SAV_VEC(all_options) 
	END_WHILE

	AST_RETURN_NODE(ASToptions_groupNode)(astvn_all_options);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_option()
{
	START_RULE(option)

	SWITCH
		CASE_RULE(UNIT, unit)
		CASE_RULE(DESC, desc)
		CASE_RULE(GTHAN, bounds)
		CASE_RULE(LTHAN, bounds)
		CASE_RULE(GEQUALS, bounds)
		CASE_RULE(LEQUALS, bounds)
		CASE_RULE(BASIS, basis_type)
		CASE_RULE(SEGMENT, segment_def)
		CASE_RULE(PDEVAR, pdevar_def)
		LAST
			FORCE_ERROR
				std::vector<int> possible_states;
				possible_states.push_back(UNIT);
				possible_states.push_back(DESC);
				possible_states.push_back(GTHAN);
				possible_states.push_back(LTHAN);
				possible_states.push_back(GEQUALS);
				possible_states.push_back(LEQUALS);
				possible_states.push_back(BASIS);
				possible_states.push_back(SEGMENT);
				possible_states.push_back(PDEVAR);
				SYNTAX_ERROR_EXPECTED_ANYOF_TYPE(possible_states)
			UNFORCE_ERROR
	END_SWITCH

	AST_RETURN_NO_CREATE
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_unit()
{
	AST_DEC_TOK(value)

	START_RULE(unit)

	EXPECT_TOKEN_ELSE(UNIT)
		SYNTAX_ERROR_EXPECTED_TYPE(UNIT)
	END_EXPECT

	EXPECT_TOKEN_ELSE(EQUALS)
		SYNTAX_ERROR_EXPECTED_TYPE(EQUALS)
	END_EXPECT

	AST_SAV_TOK(value)
	EXPECT_TOKEN_ELSE(STRING)
		SYNTAX_ERROR_EXPECTED_TYPE(STRING)
	END_EXPECT

	AST_RETURN_NODE(ASTunit_optionNode)(&t_value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_desc()
{
	AST_DEC_TOK(value)

	START_RULE(desc)

	EXPECT_TOKEN_ELSE(DESC)
		SYNTAX_ERROR_EXPECTED_TYPE(DESC)
	END_EXPECT

	EXPECT_TOKEN_ELSE(EQUALS)
		SYNTAX_ERROR_EXPECTED_TYPE(EQUALS)
	END_EXPECT

	AST_SAV_TOK(value)
	EXPECT_TOKEN_ELSE(STRING)
		SYNTAX_ERROR_EXPECTED_TYPE(STRING)
	END_EXPECT

	AST_RETURN_NODE(ASTdesc_optionNode)(&t_value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_numtype()
{
	AST_DEC_TOK(numtype)
	AST_SAV_TOK(numtype)

	START_RULE(numtype)

	SWITCH
		CASE(REAL)
		CASE(INTEGER)
		LAST
			FORCE_ERROR
				std::vector<int> possible_states;
				possible_states.push_back(REAL);
				possible_states.push_back(INTEGER);
				SYNTAX_ERROR_EXPECTED_ANYOF_TYPE(possible_states)
			UNFORCE_ERROR
	END_SWITCH

	AST_RETURN_NODE(ASTgeneric_tokenNode)(&t_numtype);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_bounds()
{
	AST_DEC_NODE(inequality_op)
	AST_DEC_NODE(expression)
		
	START_RULE(bounds)

	DO_RULE(inequality) AST_SAV_NODE(inequality_op) 

	DO_RULE(expression) AST_SAV_NODE(expression) 

	AST_RETURN_NODE(ASTbounds_optionNode)(astn_inequality_op, astn_expression);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_inequality()
{
	AST_DEC_TOK(inequality_op)
	AST_SAV_TOK(inequality_op)

	START_RULE(inequality)

	SWITCH
		CASE(GTHAN)
		CASE(LTHAN)
		CASE(GEQUALS)
		CASE(LEQUALS)
		LAST
			FORCE_ERROR
				std::vector<int> possible_states;
				possible_states.push_back(GTHAN);
				possible_states.push_back(LTHAN);
				possible_states.push_back(GEQUALS);
				possible_states.push_back(LEQUALS);
				SYNTAX_ERROR_EXPECTED_ANYOF_TYPE(possible_states)
			UNFORCE_ERROR
	END_SWITCH

	AST_RETURN_NODE(ASTgeneric_tokenNode)(&t_inequality_op);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_noneq_statement()
{
	START_RULE(noneq_statment)

	IF(FOR)
		DO_RULE(for_loop)
	ELIF(pIF)
		DO_RULE(if_statement)
	ELSE
		DO_RULE(assignment)

		EXPECT_TOKEN_ELSE(SEMICOLON)
			SYNTAX_ERROR_EXPECTED_TYPE(SEMICOLON)
		END_EXPECT
	END_IF

	AST_RETURN_NO_CREATE
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_eq_statement()
{
	START_RULE(eq_statment)

	IF(FOR)
		DO_RULE(for_loop)
	ELIF(pIF)
		DO_RULE(if_statement)
	ELIF(PDE)
		DO_RULE(fem1d_equation)
	ELSE
		DO_RULE(equation)

		EXPECT_TOKEN_ELSE(SEMICOLON)
			SYNTAX_ERROR_EXPECTED_TYPE(SEMICOLON)
		END_EXPECT
	END_IF

	AST_RETURN_NO_CREATE
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_model_entity_type()
{
	AST_DEC_TOK(model_entity_type)
	AST_SAV_TOK(model_entity_type)

	START_RULE(model_entity_type)

	SWITCH
		CASE(PARAMETER)
		CASE(VARIABLE)
		CASE(ITER)
		CASE(IDENT)
		CASE(LINE)
		LAST
			FORCE_ERROR
				std::vector<int> possible_states;
				possible_states.push_back(PARAMETER);
				possible_states.push_back(VARIABLE);
				possible_states.push_back(ITER);
				possible_states.push_back(IDENT);
				possible_states.push_back(LINE);
				SYNTAX_ERROR_EXPECTED_ANYOF_TYPE(possible_states)
			UNFORCE_ERROR
	END_SWITCH

	AST_RETURN_NODE(ASTgeneric_tokenNode)(&t_model_entity_type);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_model_entities()
{
	AST_DEC_VEC(all_model_entities)

	START_RULE(model_entities)

	DO_RULE(model_entity) AST_SAV_VEC(all_model_entities) 

	FOR_ALL_WHILE
		ITEM_E(COMMA)
			EAT(0)
			DO_RULE(model_entity) AST_SAV_VEC(all_model_entities) 
	END_WHILE

	AST_RETURN_NODE(ASTmodel_entities_collectionNode)(astvn_all_model_entities);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_model_entity()
{
	AST_DEC_NODE(type_name)
	AST_DEC_NODE(pdefault)
	AST_DEC_NODE(options)

	START_RULE(model_entity)

	DO_RULE(type_name) AST_SAV_NODE(type_name) 

	FOR_ALL_IF
		ITEM_E(EQUALS)
			DO_RULE(default) AST_SAV_NODE(pdefault) 
	END_IF

	IF(DCOLON)
		DO_RULE(options) AST_SAV_NODE(options) 
	END_IF

	AST_RETURN_NODE(ASTmodel_entity_declNode)(astn_type_name, astn_options, astn_pdefault);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_default()
{
	AST_DEC_TOK(equal_type_op)
	AST_DEC_NODE(expression)

	START_RULE(default)

	FOR_ALL_IF
		ITEM_E(EQUALS)
			AST_SAV_TOK(equal_type_op)
			EAT(0)		
	ELSE
		FORCE_ERROR
			std::vector<int> possible_states;
			possible_states.push_back(EQUALS);
			SYNTAX_ERROR_EXPECTED_ANYOF_TYPE(possible_states)
		UNFORCE_ERROR
	END_IF

	DO_RULE(expression) AST_SAV_NODE(expression) 

	AST_RETURN_NODE(ASTdefualtNode)(&t_equal_type_op, astn_expression);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_equation()
{
	AST_DEC_NODE(LHS)
	AST_DEC_NODE(equation_op)
	AST_DEC_NODE(RHS)

	START_RULE(equationn)

	DO_RULE(expression) AST_SAV_NODE(LHS) 

	DO_RULE(equation_type_operator) AST_SAV_NODE(equation_op) 

	DO_RULE(expression) AST_SAV_NODE(RHS) 

	AST_RETURN_NODE(ASTequationNode)(astn_LHS, astn_equation_op, astn_RHS);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_expression()
{
	ASTNode* node1 = NULL;
	ASTNode* node2 = NULL;

	AST_DEC_NODE(term1)
	AST_DEC_NODE(term2)
	AST_DEC_TOK(bin_op)

	START_RULE(expression)

	DO_RULE(term) AST_SAV_NODE(term1) 
	node1 = astn_term1;

	FOR_ALL_WHILE
		ITEM_S(PLUS)
		ITEM_E(MINUS)		
			AST_SAV_TOK(bin_op)
			EAT(0)

			DO_RULE(term) AST_SAV_NODE(term2) 
				
			node2 = new ASTMathBinaryOpNode;
			node1 = static_cast<ASTMathBinaryOpNode*>(node2)->CreateNode(node1, &t_bin_op, astn_term2);
	END_WHILE

	return node1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_term()
{
	ASTNode* node1 = NULL;
	ASTNode* node2 = NULL;

	AST_DEC_NODE(factor1)
	AST_DEC_NODE(factor2)
	AST_DEC_TOK(bin_op)

	START_RULE(term)

	DO_RULE(factor) AST_SAV_NODE(factor1) 
	node1 = astn_factor1;

	FOR_ALL_WHILE
		ITEM_S(MULT)
		ITEM_E(DIV)
			AST_SAV_TOK(bin_op)
			EAT(0)
				
			DO_RULE(factor) AST_SAV_NODE(factor2) 

			node2 = new ASTMathBinaryOpNode;
			node1 = static_cast<ASTMathBinaryOpNode*>(node2)->CreateNode(node1, &t_bin_op, astn_factor2);
	END_WHILE

	return node1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_factor()
{

	AST_DEC_NODE(primary1)
	AST_DEC_NODE(primary2)
	AST_DEC_TOK(raise_to)

	START_RULE(factor)

	DO_RULE(primary) AST_SAV_NODE(primary1) 

	IF(RAISE)
		AST_SAV_TOK(raise_to)
		EAT(1)

		DO_RULE(primary) AST_SAV_NODE(primary2) 

		AST_RETURN_NODE(ASTMathBinaryOpNode)(astn_primary1, &t_raise_to, astn_primary2);
	END_IF

	AST_RETURN_NO_CREATE
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_primary()
{
	AST_DEC_TOK(un_op)
	bool unary_op_enabled = false;
	
	START_RULE(primary)

	FOR_ALL_IF
		ITEM_S(PLUS)
		ITEM_E(MINUS)
			AST_SAV_TOK(un_op)
			EAT(0)
			unary_op_enabled = true;
	END_IF

	FOR_ALL_IF
		ITEM_S(REAL_VALUE)
		ITEM_S(INTEGER_VALUE)
		ITEM_E(EXPO_VALUE)
			DO_RULE(real_number)
		ELIF(LCIRCLE)
			EAT(0)
			DO_RULE(logical_expression)
			EXPECT_TOKEN_ELSE(RCIRCLE)
				SYNTAX_ERROR_EXPECTED_TYPE(RCIRCLE)
			END_EXPECT
	ELIF(IDENT)
		LookAhead(1);
		if (LAtoks.at(0).GetType() == LCIRCLE)
		{
			DO_RULE(function_name)
		}
		else
		{
			DO_RULE(full_type_name)
		}
	ELIF(DOLLAR)
		DO_RULE(full_type_name)
	ELSE
		FORCE_ERROR
			std::vector<std::string> possible_states;
			possible_states.push_back("const number");
			possible_states.push_back("array reference");
			possible_states.push_back("function call");
			possible_states.push_back("expression");
			SYNTAX_ERROR_EXPECTED_ANYOF_STRING(possible_states)
		UNFORCE_ERROR
	END_IF

	if(unary_op_enabled)
	{ 
		AST_RETURN_NODE(ASTMathUnaryNode)(&t_un_op, ASTtempNode);
	}
	else
		AST_RETURN_NO_CREATE
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_real_number()
{
	AST_DEC_TOK(number)
	AST_SAV_TOK(number)

	START_RULE(real_number)

	SWITCH
		CASE(REAL_VALUE)
		CASE(INTEGER_VALUE)
		CASE(EXPO_VALUE)
		LAST
			FORCE_ERROR
				std::vector<std::string> possible_states;
				possible_states.push_back("Real");
				possible_states.push_back("Integer");
				SYNTAX_ERROR_EXPECTED_ANYOF_STRING(possible_states)
			UNFORCE_ERROR
	END_SWITCH

	AST_RETURN_NODE(ASTNumberNode)(&t_number);	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_equation_type_operator()
{
	AST_DEC_TOK(eq_type_op)
	AST_SAV_TOK(eq_type_op)

	START_RULE(equation_type_operator)

	SWITCH
		CASE(EQUALS)
		CASE(GTHAN)
		CASE(LTHAN)
		CASE(GEQUALS)
		CASE(LEQUALS)
		LAST
			FORCE_ERROR
				std::vector<int> possible_states;
				possible_states.push_back(EQUALS);
				possible_states.push_back(GTHAN);
				possible_states.push_back(LTHAN);
				possible_states.push_back(GEQUALS);
				possible_states.push_back(LEQUALS);
				SYNTAX_ERROR_EXPECTED_ANYOF_TYPE(possible_states)
			UNFORCE_ERROR
	END_SWITCH

	AST_RETURN_NODE(ASTgeneric_tokenNode)(&t_eq_type_op);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_comparison_type_operator()
{
	AST_DEC_TOK(cmp_type_op)
	AST_SAV_TOK(cmp_type_op)

	START_RULE(comparison_type_operator)

	SWITCH
		CASE(DEQUALS)
		CASE(GTHAN)
		CASE(LTHAN)
		CASE(GEQUALS)
		CASE(LEQUALS)
		LAST
			FORCE_ERROR
				std::vector<int> possible_states;
				possible_states.push_back(DEQUALS);
				possible_states.push_back(GTHAN);
				possible_states.push_back(LTHAN);
				possible_states.push_back(GEQUALS);
				possible_states.push_back(LEQUALS);
				SYNTAX_ERROR_EXPECTED_ANYOF_TYPE(possible_states)
			UNFORCE_ERROR
	END_SWITCH

	AST_RETURN_NODE(ASTgeneric_tokenNode)(&t_cmp_type_op);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_type_name()
{
	AST_DEC_TOK(dt)
	AST_DEC_TOK(type_name)
	AST_DEC_NODE(array_indices)
	AST_DEC_NODE(deriv)

	START_RULE(type_name)

	IF(DOLLAR)
		AST_SAV_TOK(dt)
		EAT(0)
	END_IF

	AST_SAV_TOK(type_name)
	EXPECT_TOKEN_ELSE(IDENT)
		SYNTAX_ERROR_EXPECTED_STRING("Identifier")
	END_EXPECT

	FOR_ALL_IF
		ITEM_E(LSQUARE)
			EAT(1)
			DO_RULE(array_indices) AST_SAV_NODE(array_indices) 
			EXPECT_TOKEN_ELSE(RSQUARE)
				SYNTAX_ERROR_MISSING(RSQUARE)
			END_EXPECT
	END_IF

	IF(APOS)
		DO_RULE(derivative_sigle_var) AST_SAV_NODE(deriv)
	END_IF

	AST_RETURN_NODE(ASTNamedReferenceNode)(&t_dt, &t_type_name, astn_array_indices, astn_deriv);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_array_indices()
{
	AST_DEC_VEC(all_array_indices)

	START_RULE(array_indices)

	DO_RULE(array_index) AST_SAV_VEC(all_array_indices) 

	FOR_ALL_WHILE
		ITEM_E(COMMA)
			EAT(0)
			DO_RULE(array_index) AST_SAV_VEC(all_array_indices) 
	END_WHILE

	AST_RETURN_NODE(ASTArrayIndicesNode)(astvn_all_array_indices);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_array_index()
{
	START_RULE(array_index)

	IF(STRING)
		DO_RULE(string_index)
	ELSE
		DO_RULE(expression)
	END_IF

	AST_RETURN_NO_CREATE
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_full_type_name()
{
	AST_DEC_TOK(dt)
	AST_DEC_VEC(full_type_name)

	START_RULE(full_type_name)
		
	DO_RULE(type_name) AST_SAV_VEC(full_type_name) 

	FOR_ALL_WHILE
		ITEM_E(DOT)
			EAT(1)
			DO_RULE(type_name) AST_SAV_VEC(full_type_name) 
	END_WHILE

	AST_RETURN_NODE(ASTQualifiedNamedReferenceNode)(astvn_full_type_name);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_function_name()
{
	AST_DEC_TOK(functionName)
	AST_SAV_TOK(functionName)
	AST_DEC_VEC(functionArgs)

	START_RULE(functionCall)

	EAT(0)
	EXPECT_TOKEN_ELSE(LCIRCLE)
		SYNTAX_ERROR_EXPECTED_TYPE(LCIRCLE)
	END_EXPECT
		
	DO_RULE(expression) AST_SAV_VEC(functionArgs) 

	FOR_ALL_WHILE
		ITEM_E(COMMA)
			EAT(1)
			DO_RULE(expression) AST_SAV_VEC(functionArgs) 
	END_WHILE

	EXPECT_TOKEN_ELSE(RCIRCLE)
		SYNTAX_ERROR_EXPECTED_TYPE(RCIRCLE)
	END_EXPECT

	AST_RETURN_NODE(ASTfunctionCallNode)(&t_functionName, astvn_functionArgs);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_logical_expression()
{
	ASTNode* node1 = NULL;
	ASTNode* node2 = NULL;

	AST_DEC_NODE(lterm1)
	AST_DEC_NODE(lterm2)
	AST_DEC_TOK(lbin_op)

	START_RULE(logical_expression)

	DO_RULE(lterm) AST_SAV_NODE(lterm1) 
	node1 = astn_lterm1;

	FOR_ALL_WHILE
		ITEM_S(AND)
		ITEM_E(OR)
			AST_SAV_TOK(lbin_op)
			EAT(0)

			DO_RULE(lterm) AST_SAV_NODE(lterm2) 

			node2 = new ASTLogicalBinaryOpNode;
			node1 = static_cast<ASTLogicalBinaryOpNode*>(node2)->CreateNode(node1, &t_lbin_op, astn_lterm2);
	END_WHILE

	return node1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_lterm()
{
	bool unary_enabled = false;
	AST_DEC_TOK(not)

	START_RULE(lterm)

	IF(NOT)
		unary_enabled = true;
	AST_SAV_TOK(not)
		EAT(0)
	END_IF

	DO_RULE(lfactor)

	if(unary_enabled)
	{
		AST_RETURN_NODE(ASTLogicalUnaryNode)(&t_not, ASTtempNode);
	}
	else
		AST_RETURN_NO_CREATE
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_lfactor()
{
	ASTNode* node1 = NULL;
	ASTNode* node2 = NULL;

	AST_DEC_NODE(expression1)
	AST_DEC_NODE(expression2)
	AST_DEC_TOK(lrel_op)

	START_RULE(lfactor)

	DO_RULE(expression) AST_SAV_NODE(expression1) 
	node1 = astn_expression1;

	FOR_ALL_WHILE
		ITEM_S(DEQUALS)
		ITEM_S(GTHAN)
		ITEM_S(LTHAN)
		ITEM_S(GEQUALS)
		ITEM_E(LEQUALS)
			AST_SAV_TOK(lrel_op)
			DO_RULE(comparison_type_operator)

			DO_RULE(expression) AST_SAV_NODE(expression2) 

			node2 = new ASTLogicalBinaryOpNode;
			node1 = static_cast<ASTLogicalBinaryOpNode*>(node2)->CreateNode(node1, &t_lrel_op, astn_expression2);
	END_WHILE

	return node1;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_for_loop()
{
	AST_DEC_TOK(iter_name)
	AST_DEC_NODE(start_expr)
	AST_DEC_NODE(till_expr)
	AST_DEC_NODE(incr_expr)
	AST_DEC_NODE(for_body)

	START_RULE(for_loop)

	EXPECT_TOKEN_ELSE(FOR)
		SYNTAX_ERROR_EXPECTED_TYPE(FOR)
	END_EXPECT

	EXPECT_TOKEN_ELSE(LCIRCLE)
		SYNTAX_ERROR_EXPECTED_TYPE(LCIRCLE)
	END_EXPECT

	AST_SAV_TOK(iter_name)
	EXPECT_TOKEN_ELSE(IDENT)
		SYNTAX_ERROR_EXPECTED_STRING("Identifier")
	END_EXPECT

	EXPECT_TOKEN_ELSE(EQUALS)
		SYNTAX_ERROR_EXPECTED_TYPE(EQUALS)
	END_EXPECT

	DO_RULE(expression) AST_SAV_NODE(start_expr) 
		 
	EXPECT_TOKEN_ELSE(COMMA)
		SYNTAX_ERROR_EXPECTED_TYPE(COMMA)
	END_EXPECT

	DO_RULE(expression) AST_SAV_NODE(till_expr) 

	IF(COMMA)
		EAT(5)
		DO_RULE(expression) AST_SAV_NODE(incr_expr) 
	END_IF

	EXPECT_TOKEN_ELSE(RCIRCLE)
		SYNTAX_ERROR_MISSING(RCIRCLE)
	END_EXPECT

	DO_RULE(single_or_compound_statement) AST_SAV_NODE(for_body) 

	AST_RETURN_NODE(ASTfor_loopNode)(&t_iter_name, astn_start_expr, astn_till_expr, astn_incr_expr, astn_for_body);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_single_or_compound_statement()
{
	AST_DEC_VEC(all_statements)

	START_RULE(single_or_compound_statement)

	EXPECT_TOKEN_ELSE(LCURLY)
		SYNTAX_ERROR_EXPECTED_TYPE(LCURLY)
	END_EXPECT

	FOR_ALL_WHILE
		!ITEM_E(RCURLY)
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONTEXT FIND
			if (context == FOR_EQ_TYPE)
			{ 
				DO_RULE(eq_statement) AST_SAV_VEC(all_statements) 
			}
			else if (context == FOR_NONEQ_TYPE)
			{
				DO_RULE(noneq_statement) AST_SAV_VEC(all_statements) 
			}
			else
			{
				//error
			}
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONTEXT FIND	
				
	END_WHILE

	EXPECT_TOKEN_ELSE(RCURLY)
		SYNTAX_ERROR_MISSING(RCURLY)
	END_EXPECT

	AST_RETURN_NODE(ASTstatement_blockNode)(astvn_all_statements);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_if_statement()
{
	AST_DEC_NODE(condition)
	AST_DEC_NODE(then_body)
	AST_DEC_NODE(else_body)

	START_RULE(if_statement)

	EXPECT_TOKEN_ELSE(pIF)
		SYNTAX_ERROR_EXPECTED_TYPE(FOR)
	END_EXPECT

	EXPECT_TOKEN_ELSE(LCIRCLE)
		SYNTAX_ERROR_EXPECTED_TYPE(LCIRCLE)
	END_EXPECT

	DO_RULE(logical_expression) AST_SAV_NODE(condition) 

	EXPECT_TOKEN_ELSE(RCIRCLE)
		SYNTAX_ERROR_MISSING(RCIRCLE)
	END_EXPECT

	DO_RULE(single_or_compound_statement) AST_SAV_NODE(then_body) 

	IF(pELSE)
		EAT(3)
		DO_RULE(single_or_compound_statement) AST_SAV_NODE(else_body) 
	END_IF

	AST_RETURN_NODE(ASTif_statmentNode)(astn_condition, astn_then_body, astn_else_body);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_assignment()
{
	AST_DEC_NODE(LHS)
	AST_DEC_TOK(asg_op)
	AST_DEC_NODE(RHS)

	START_RULE(assignment)

	DO_RULE(full_type_name) AST_SAV_NODE(LHS) 

	AST_SAV_TOK(asg_op) 
	DO_RULE(equation_type_operator) 

	DO_RULE(expression) AST_SAV_NODE(RHS) 

	AST_RETURN_NODE(ASTassignmentNode)(astn_LHS, &t_asg_op, astn_RHS);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_simulation_args()
{
	AST_DEC_TOK(simType)
	AST_DEC_NODE(simStartt)
	AST_DEC_NODE(simEndt)
	AST_DEC_TOK(simNumSteps)
	AST_DEC_NODE(simAbsTol)
	AST_DEC_NODE(simRelTol)

	START_RULE(simulation_args)

	EXPECT_TOKEN_ELSE(LTHAN)
		SYNTAX_ERROR_MISSING(LTHAN)
	END_EXPECT

	AST_SAV_TOK(simType)

	EXPECT_TOKEN_ELSE(IDENT)
		SYNTAX_ERROR_MISSING(IDENT)
	END_EXPECT

	DO_RULE(real_number) AST_SAV_NODE(simStartt)

	DO_RULE(real_number) AST_SAV_NODE(simEndt)

	AST_SAV_TOK(simNumSteps)

	EXPECT_TOKEN_ELSE(INTEGER_VALUE)
		SYNTAX_ERROR_MISSING(INTEGER_VALUE)
	END_EXPECT

	DO_RULE(real_number) AST_SAV_NODE(simAbsTol)

	DO_RULE(real_number) AST_SAV_NODE(simRelTol)

	EXPECT_TOKEN_ELSE(GTHAN)
		SYNTAX_ERROR_MISSING(GTHAN)
	END_EXPECT

	AST_RETURN_NODE(ASTSimulationArgs)(&t_simType, astn_simStartt, astn_simEndt, &t_simNumSteps, astn_simAbsTol, astn_simRelTol);
}