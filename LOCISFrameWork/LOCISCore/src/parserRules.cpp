#include "parser.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_model_collection()
{
	AST_DEC_VEC(all_models)

    START_RULE

	FOR_ALL_WHILE
        ITEM_E(KW_MODEL)
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
	
    START_RULE

    EXPECT_TOKEN_ELSE(KW_MODEL)
        syntaxErrorExpectedType(KW_MODEL);
	END_EXPECT

	AST_SAV_TOK(model_name)

    EXPECT_TOKEN_ELSE(DT_IDENT)
        syntaxErrorExpectedString("Indetifier");
	END_EXPECT

    IF(OP_LTHAN)
		DO_RULE(simulation_args) AST_SAV_NODE(simArgs)
	END_IF

    EXPECT_TOKEN_ELSE(OP_LCURLY)
        syntaxErrorExpectedType(OP_LCURLY);
	END_EXPECT

	FOR_ALL_WHILE
        !ITEM_E(OP_RCURLY)
			DO_RULE(model_statement) AST_SAV_VEC(all_model_statements) 
	END_WHILE
		
    EXPECT_TOKEN_ELSE(OP_RCURLY)
        syntaxErrorMissing(OP_RCURLY);
	END_EXPECT

	AST_RETURN_NODE(ASTmodelNode)(&t_model_name, astvn_all_model_statements, astn_simArgs);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_model_statement()
{
    START_RULE

	FOR_ALL_IF
        ITEM_S(KW_PARAMETER)
        ITEM_S(KW_VARIABLE)
        ITEM_S(KW_ITER)
        ITEM_S(DT_IDENT)
        ITEM_E(KW_LINE)
			DO_RULE(model_entity_decl)
	ELSE
		FOR_ALL_IF
            ITEM_S(KW_SET)
            ITEM_S(KW_FIX)
            ITEM_S(KW_INIT)
            ITEM_S(KW_GUESS)
            ITEM_E(KW_EQUATION)
				DO_RULE(model_section)
		ELSE
            syntaxErrorUnexpected();
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

    START_RULE

	DO_RULE(model_entity_type) AST_SAV_NODE(model_entity_type) 

	FOR_ALL_IF
        ITEM_S(KW_REAL)
        ITEM_E(KW_INTEGER)
			DO_RULE(numtype) AST_SAV_NODE(numtype) 
	END_IF

	DO_RULE(model_entities) AST_SAV_NODE(model_entities) 

    EXPECT_TOKEN_ELSE(OP_SEMICOLON)
        syntaxErrorMissing(OP_SEMICOLON);
	END_EXPECT

	AST_RETURN_NODE(ASTmodel_entity_decl_groupNode)(astn_model_entity_type, astn_numtype, astn_model_entities);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_model_section()
{
	AST_DEC_NODE(section_name)
	AST_DEC_NODE(all_statements)
	AST_DEC_NODE(options)

    START_RULE

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONTEXT FIND
    context = CONTEXT_NONE;
	FOR_ALL_IF
        ITEM_S(KW_SET)
        ITEM_S(KW_FIX)
        ITEM_E(KW_GUESS)
            context = CONTEXT_NONEQ_TYPE;
	ELSE
		FOR_ALL_IF
            ITEM_S(KW_INIT)
            ITEM_E(KW_EQUATION)
                context = CONTEXT_EQ_TYPE;
		END_IF
	END_IF
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONTEXT FIND

	DO_RULE(section_type) AST_SAV_NODE(section_name) 

    IF(OP_DCOLON)
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

    START_RULE

	SWITCH
        CASE(KW_SET)
        CASE(KW_FIX)
        CASE(KW_GUESS)
        CASE(KW_INIT)
        CASE(KW_EQUATION)
		LAST
			FORCE_ERROR
				std::vector<int> possible_states;
                possible_states.push_back(KW_SET);
                possible_states.push_back(KW_FIX);
                possible_states.push_back(KW_GUESS);
                possible_states.push_back(KW_INIT);
                possible_states.push_back(KW_EQUATION);
                syntaxErrorExpectedAnyofType(possible_states);
			UNFORCE_ERROR
	END_SWITCH

	AST_RETURN_NODE(ASTgeneric_tokenNode)(&t_section_name);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_options()
{
    START_RULE

    EXPECT_TOKEN_ELSE(OP_DCOLON)
        syntaxErrorExpectedType(OP_DCOLON);
	END_EXPECT

    EXPECT_TOKEN_ELSE(OP_LCIRCLE)
        syntaxErrorExpectedType(OP_LCIRCLE);
	END_EXPECT

	DO_RULE(options_space)

    EXPECT_TOKEN_ELSE(OP_RCIRCLE)
        syntaxErrorMissing(OP_RCIRCLE);
	END_EXPECT

	AST_RETURN_NO_CREATE
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_options_space()
{
	AST_DEC_VEC(all_options)

    START_RULE

	DO_RULE(option) AST_SAV_VEC(all_options) 

	FOR_ALL_WHILE
        ITEM_E(OP_COMMA)
            EAT
			DO_RULE(option) AST_SAV_VEC(all_options) 
	END_WHILE

	AST_RETURN_NODE(ASToptions_groupNode)(astvn_all_options);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_option()
{
    START_RULE

	SWITCH
        CASE_RULE(KW_UNIT, unit)
        CASE_RULE(KW_DESC, desc)
        CASE_RULE(OP_GTHAN, bounds)
        CASE_RULE(OP_LTHAN, bounds)
        CASE_RULE(OP_GEQUALS, bounds)
        CASE_RULE(OP_LEQUALS, bounds)
        CASE_RULE(KW_BASIS, basis_type)
        CASE_RULE(KW_SEGMENT, segment_def)
        CASE_RULE(KW_PDEVAR, pdevar_def)
		LAST
			FORCE_ERROR
				std::vector<int> possible_states;
                possible_states.push_back(KW_UNIT);
                possible_states.push_back(KW_DESC);
                possible_states.push_back(OP_GTHAN);
                possible_states.push_back(OP_LTHAN);
                possible_states.push_back(OP_GEQUALS);
                possible_states.push_back(OP_LEQUALS);
                possible_states.push_back(KW_BASIS);
                possible_states.push_back(KW_SEGMENT);
                possible_states.push_back(KW_PDEVAR);
                syntaxErrorExpectedAnyofType(possible_states);
			UNFORCE_ERROR
	END_SWITCH

	AST_RETURN_NO_CREATE
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_unit()
{
	AST_DEC_TOK(value)

    START_RULE

    EXPECT_TOKEN_ELSE(KW_UNIT)
        syntaxErrorExpectedType(KW_UNIT);
	END_EXPECT

    EXPECT_TOKEN_ELSE(OP_EQUALS)
        syntaxErrorExpectedType(OP_EQUALS);
	END_EXPECT

	AST_SAV_TOK(value)
    EXPECT_TOKEN_ELSE(DT_STRING)
        syntaxErrorExpectedType(DT_STRING);
	END_EXPECT

	AST_RETURN_NODE(ASTunit_optionNode)(&t_value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_desc()
{
	AST_DEC_TOK(value)

    START_RULE

    EXPECT_TOKEN_ELSE(KW_DESC)
        syntaxErrorExpectedType(KW_DESC);
	END_EXPECT

    EXPECT_TOKEN_ELSE(OP_EQUALS)
        syntaxErrorExpectedType(OP_EQUALS);
	END_EXPECT

	AST_SAV_TOK(value)
    EXPECT_TOKEN_ELSE(DT_STRING)
        syntaxErrorExpectedType(DT_STRING);
	END_EXPECT

	AST_RETURN_NODE(ASTdesc_optionNode)(&t_value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_numtype()
{
	AST_DEC_TOK(numtype)
	AST_SAV_TOK(numtype)

    START_RULE

	SWITCH
        CASE(KW_REAL)
        CASE(KW_INTEGER)
		LAST
			FORCE_ERROR
				std::vector<int> possible_states;
                possible_states.push_back(KW_REAL);
                possible_states.push_back(KW_INTEGER);
                syntaxErrorExpectedAnyofType(possible_states);
			UNFORCE_ERROR
	END_SWITCH

	AST_RETURN_NODE(ASTgeneric_tokenNode)(&t_numtype);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_bounds()
{
	AST_DEC_NODE(inequality_op)
	AST_DEC_NODE(expression)
		
    START_RULE

	DO_RULE(inequality) AST_SAV_NODE(inequality_op) 

	DO_RULE(expression) AST_SAV_NODE(expression) 

	AST_RETURN_NODE(ASTbounds_optionNode)(astn_inequality_op, astn_expression);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_inequality()
{
	AST_DEC_TOK(inequality_op)
	AST_SAV_TOK(inequality_op)

    START_RULE

	SWITCH
        CASE(OP_GTHAN)
        CASE(OP_LTHAN)
        CASE(OP_GEQUALS)
        CASE(OP_LEQUALS)
		LAST
			FORCE_ERROR
				std::vector<int> possible_states;
                possible_states.push_back(OP_GTHAN);
                possible_states.push_back(OP_LTHAN);
                possible_states.push_back(OP_GEQUALS);
                possible_states.push_back(OP_LEQUALS);
                syntaxErrorExpectedAnyofType(possible_states);
			UNFORCE_ERROR
	END_SWITCH

	AST_RETURN_NODE(ASTgeneric_tokenNode)(&t_inequality_op);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_noneq_statement()
{
    START_RULE

    IF(KW_FOR)
		DO_RULE(for_loop)
    ELIF(KW_pIF)
		DO_RULE(if_statement)
	ELSE
		DO_RULE(assignment)

        EXPECT_TOKEN_ELSE(OP_SEMICOLON)
            syntaxErrorExpectedType(OP_SEMICOLON);
		END_EXPECT
	END_IF

	AST_RETURN_NO_CREATE
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_eq_statement()
{
    START_RULE

    IF(KW_FOR)
		DO_RULE(for_loop)
    ELIF(KW_pIF)
		DO_RULE(if_statement)
    ELIF(KW_PDE)
		DO_RULE(fem1d_equation)
	ELSE
		DO_RULE(equation)

        EXPECT_TOKEN_ELSE(OP_SEMICOLON)
            syntaxErrorExpectedType(OP_SEMICOLON);
		END_EXPECT
	END_IF

	AST_RETURN_NO_CREATE
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_model_entity_type()
{
	AST_DEC_TOK(model_entity_type)
	AST_SAV_TOK(model_entity_type)

    START_RULE

	SWITCH
        CASE(KW_PARAMETER)
        CASE(KW_VARIABLE)
        CASE(KW_ITER)
        CASE(DT_IDENT)
        CASE(KW_LINE)
		LAST
			FORCE_ERROR
				std::vector<int> possible_states;
                possible_states.push_back(KW_PARAMETER);
                possible_states.push_back(KW_VARIABLE);
                possible_states.push_back(KW_ITER);
                possible_states.push_back(DT_IDENT);
                possible_states.push_back(KW_LINE);
                syntaxErrorExpectedAnyofType(possible_states);
			UNFORCE_ERROR
	END_SWITCH

	AST_RETURN_NODE(ASTgeneric_tokenNode)(&t_model_entity_type);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_model_entities()
{
	AST_DEC_VEC(all_model_entities)

    START_RULE

	DO_RULE(model_entity) AST_SAV_VEC(all_model_entities) 

	FOR_ALL_WHILE
        ITEM_E(OP_COMMA)
            EAT
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

    START_RULE

	DO_RULE(type_name) AST_SAV_NODE(type_name) 

	FOR_ALL_IF
        ITEM_E(OP_EQUALS)
			DO_RULE(default) AST_SAV_NODE(pdefault) 
	END_IF

    IF(OP_DCOLON)
		DO_RULE(options) AST_SAV_NODE(options) 
	END_IF

	AST_RETURN_NODE(ASTmodel_entity_declNode)(astn_type_name, astn_options, astn_pdefault);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_default()
{
	AST_DEC_TOK(equal_type_op)
	AST_DEC_NODE(expression)

    START_RULE

	FOR_ALL_IF
        ITEM_E(OP_EQUALS)
			AST_SAV_TOK(equal_type_op)
            EAT
	ELSE
		FORCE_ERROR
			std::vector<int> possible_states;
            possible_states.push_back(OP_EQUALS);
            syntaxErrorExpectedAnyofType(possible_states);
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

    START_RULE

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

    START_RULE

	DO_RULE(term) AST_SAV_NODE(term1) 
	node1 = astn_term1;

	FOR_ALL_WHILE
        ITEM_S(OP_PLUS)
        ITEM_E(OP_MINUS)
			AST_SAV_TOK(bin_op)
            EAT

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

    START_RULE

	DO_RULE(factor) AST_SAV_NODE(factor1) 
	node1 = astn_factor1;

	FOR_ALL_WHILE
        ITEM_S(OP_MULT)
        ITEM_E(OP_DIV)
			AST_SAV_TOK(bin_op)
            EAT
				
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

    START_RULE

	DO_RULE(primary) AST_SAV_NODE(primary1) 

    IF(OP_RAISE)
		AST_SAV_TOK(raise_to)
        EAT

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
	
    START_RULE

	FOR_ALL_IF
        ITEM_S(OP_PLUS)
        ITEM_E(OP_MINUS)
			AST_SAV_TOK(un_op)
            EAT
			unary_op_enabled = true;
	END_IF

	FOR_ALL_IF
        ITEM_S(DT_REAL_VALUE)
        ITEM_S(DT_INTEGER_VALUE)
        ITEM_E(DT_EXPO_VALUE)
			DO_RULE(real_number)
        ELIF(OP_LCIRCLE)
            EAT
			DO_RULE(logical_expression)
            EXPECT_TOKEN_ELSE(OP_RCIRCLE)
                syntaxErrorExpectedType(OP_RCIRCLE);
			END_EXPECT
    ELIF(DT_IDENT)
        lookAhead(1);
        if (LAtoks.at(0).getType() == OP_LCIRCLE)
		{
			DO_RULE(function_name)
		}
		else
		{
			DO_RULE(full_type_name)
		}
    ELIF(OP_DOLLAR)
		DO_RULE(full_type_name)
	ELSE
		FORCE_ERROR
			std::vector<std::string> possible_states;
			possible_states.push_back("const number");
			possible_states.push_back("array reference");
			possible_states.push_back("function call");
			possible_states.push_back("expression");
            syntaxErrorExpectedAnyofString(possible_states);
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

    START_RULE

	SWITCH
        CASE(DT_REAL_VALUE)
        CASE(DT_INTEGER_VALUE)
        CASE(DT_EXPO_VALUE)
		LAST
			FORCE_ERROR
				std::vector<std::string> possible_states;
				possible_states.push_back("Real");
				possible_states.push_back("Integer");
                syntaxErrorExpectedAnyofString(possible_states);
			UNFORCE_ERROR
	END_SWITCH

	AST_RETURN_NODE(ASTNumberNode)(&t_number);	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_equation_type_operator()
{
	AST_DEC_TOK(eq_type_op)
	AST_SAV_TOK(eq_type_op)

    START_RULE

	SWITCH
        CASE(OP_EQUALS)
        CASE(OP_GTHAN)
        CASE(OP_LTHAN)
        CASE(OP_GEQUALS)
        CASE(OP_LEQUALS)
		LAST
			FORCE_ERROR
				std::vector<int> possible_states;
                possible_states.push_back(OP_EQUALS);
                possible_states.push_back(OP_GTHAN);
                possible_states.push_back(OP_LTHAN);
                possible_states.push_back(OP_GEQUALS);
                possible_states.push_back(OP_LEQUALS);
                syntaxErrorExpectedAnyofType(possible_states);
			UNFORCE_ERROR
	END_SWITCH

	AST_RETURN_NODE(ASTgeneric_tokenNode)(&t_eq_type_op);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_comparison_type_operator()
{
	AST_DEC_TOK(cmp_type_op)
	AST_SAV_TOK(cmp_type_op)

    START_RULE

	SWITCH
        CASE(OP_DEQUALS)
        CASE(OP_GTHAN)
        CASE(OP_LTHAN)
        CASE(OP_GEQUALS)
        CASE(OP_LEQUALS)
		LAST
			FORCE_ERROR
				std::vector<int> possible_states;
                possible_states.push_back(OP_DEQUALS);
                possible_states.push_back(OP_GTHAN);
                possible_states.push_back(OP_LTHAN);
                possible_states.push_back(OP_GEQUALS);
                possible_states.push_back(OP_LEQUALS);
                syntaxErrorExpectedAnyofType(possible_states);
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

    START_RULE

    IF(OP_DOLLAR)
		AST_SAV_TOK(dt)
        EAT
	END_IF

	AST_SAV_TOK(type_name)
    EXPECT_TOKEN_ELSE(DT_IDENT)
        syntaxErrorExpectedString("Identifier");
	END_EXPECT

	FOR_ALL_IF
        ITEM_E(OP_LSQUARE)
            EAT
			DO_RULE(array_indices) AST_SAV_NODE(array_indices) 
            EXPECT_TOKEN_ELSE(OP_RSQUARE)
                syntaxErrorMissing(OP_RSQUARE);
			END_EXPECT
	END_IF

    IF(OP_APOS)
		DO_RULE(derivative_sigle_var) AST_SAV_NODE(deriv)
	END_IF

	AST_RETURN_NODE(ASTNamedReferenceNode)(&t_dt, &t_type_name, astn_array_indices, astn_deriv);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_array_indices()
{
	AST_DEC_VEC(all_array_indices)

    START_RULE

	DO_RULE(array_index) AST_SAV_VEC(all_array_indices) 

	FOR_ALL_WHILE
        ITEM_E(OP_COMMA)
            EAT
			DO_RULE(array_index) AST_SAV_VEC(all_array_indices) 
	END_WHILE

	AST_RETURN_NODE(ASTArrayIndicesNode)(astvn_all_array_indices);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_array_index()
{
    START_RULE

    IF(DT_STRING)
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

    START_RULE
		
	DO_RULE(type_name) AST_SAV_VEC(full_type_name) 

	FOR_ALL_WHILE
        ITEM_E(OP_DOT)
            EAT
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

    START_RULE

    EAT
    EXPECT_TOKEN_ELSE(OP_LCIRCLE)
        syntaxErrorExpectedType(OP_LCIRCLE);
	END_EXPECT
		
	DO_RULE(expression) AST_SAV_VEC(functionArgs) 

	FOR_ALL_WHILE
        ITEM_E(OP_COMMA)
            EAT
			DO_RULE(expression) AST_SAV_VEC(functionArgs) 
	END_WHILE

    EXPECT_TOKEN_ELSE(OP_RCIRCLE)
        syntaxErrorExpectedType(OP_RCIRCLE);
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

    START_RULE

	DO_RULE(lterm) AST_SAV_NODE(lterm1) 
	node1 = astn_lterm1;

	FOR_ALL_WHILE
        ITEM_S(OP_AND)
        ITEM_E(OP_OR)
			AST_SAV_TOK(lbin_op)
            EAT

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

    START_RULE

    IF(OP_NOT)
		unary_enabled = true;
	AST_SAV_TOK(not)
        EAT
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

    START_RULE

	DO_RULE(expression) AST_SAV_NODE(expression1) 
	node1 = astn_expression1;

	FOR_ALL_WHILE
        ITEM_S(OP_DEQUALS)
        ITEM_S(OP_GTHAN)
        ITEM_S(OP_LTHAN)
        ITEM_S(OP_GEQUALS)
        ITEM_E(OP_LEQUALS)
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

    START_RULE

    EXPECT_TOKEN_ELSE(KW_FOR)
        syntaxErrorExpectedType(KW_FOR);
	END_EXPECT

    EXPECT_TOKEN_ELSE(OP_LCIRCLE)
        syntaxErrorExpectedType(OP_LCIRCLE);
	END_EXPECT

	AST_SAV_TOK(iter_name)
    EXPECT_TOKEN_ELSE(DT_IDENT)
        syntaxErrorExpectedString("Identifier");
	END_EXPECT

    EXPECT_TOKEN_ELSE(OP_EQUALS)
        syntaxErrorExpectedType(OP_EQUALS);
	END_EXPECT

	DO_RULE(expression) AST_SAV_NODE(start_expr) 
		 
    EXPECT_TOKEN_ELSE(OP_COMMA)
        syntaxErrorExpectedType(OP_COMMA);
	END_EXPECT

	DO_RULE(expression) AST_SAV_NODE(till_expr) 

    IF(OP_COMMA)
        EAT
		DO_RULE(expression) AST_SAV_NODE(incr_expr) 
	END_IF

    EXPECT_TOKEN_ELSE(OP_RCIRCLE)
        syntaxErrorMissing(OP_RCIRCLE);
	END_EXPECT

	DO_RULE(single_or_compound_statement) AST_SAV_NODE(for_body) 

	AST_RETURN_NODE(ASTfor_loopNode)(&t_iter_name, astn_start_expr, astn_till_expr, astn_incr_expr, astn_for_body);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_single_or_compound_statement()
{
	AST_DEC_VEC(all_statements)

    START_RULE

    EXPECT_TOKEN_ELSE(OP_LCURLY)
        syntaxErrorExpectedType(OP_LCURLY);
	END_EXPECT

	FOR_ALL_WHILE
        !ITEM_E(OP_RCURLY)
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONTEXT FIND
            if (context == CONTEXT_EQ_TYPE)
			{ 
				DO_RULE(eq_statement) AST_SAV_VEC(all_statements) 
			}
            else if (context == CONTEXT_NONEQ_TYPE)
			{
				DO_RULE(noneq_statement) AST_SAV_VEC(all_statements) 
			}
			else
			{
				//error
			}
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONTEXT FIND	
				
	END_WHILE

    EXPECT_TOKEN_ELSE(OP_RCURLY)
        syntaxErrorMissing(OP_RCURLY);
	END_EXPECT

	AST_RETURN_NODE(ASTstatement_blockNode)(astvn_all_statements);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode* parser::P_if_statement()
{
	AST_DEC_NODE(condition)
	AST_DEC_NODE(then_body)
	AST_DEC_NODE(else_body)

    START_RULE

    EXPECT_TOKEN_ELSE(KW_pIF)
        syntaxErrorExpectedType(KW_FOR);
	END_EXPECT

    EXPECT_TOKEN_ELSE(OP_LCIRCLE)
        syntaxErrorExpectedType(OP_LCIRCLE);
	END_EXPECT

	DO_RULE(logical_expression) AST_SAV_NODE(condition) 

    EXPECT_TOKEN_ELSE(OP_RCIRCLE)
        syntaxErrorMissing(OP_RCIRCLE);
	END_EXPECT

	DO_RULE(single_or_compound_statement) AST_SAV_NODE(then_body) 

    IF(KW_pELSE)
        EAT
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

    START_RULE

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

    START_RULE

    EXPECT_TOKEN_ELSE(OP_LTHAN)
        syntaxErrorMissing(OP_LTHAN);
	END_EXPECT

	AST_SAV_TOK(simType)

    EXPECT_TOKEN_ELSE(DT_IDENT)
        syntaxErrorMissing(DT_IDENT);
	END_EXPECT

	DO_RULE(real_number) AST_SAV_NODE(simStartt)

	DO_RULE(real_number) AST_SAV_NODE(simEndt)

	AST_SAV_TOK(simNumSteps)

    EXPECT_TOKEN_ELSE(DT_INTEGER_VALUE)
        syntaxErrorMissing(DT_INTEGER_VALUE);
	END_EXPECT

	DO_RULE(real_number) AST_SAV_NODE(simAbsTol)

	DO_RULE(real_number) AST_SAV_NODE(simRelTol)

    EXPECT_TOKEN_ELSE(OP_GTHAN)
        syntaxErrorMissing(OP_GTHAN);
	END_EXPECT

	AST_RETURN_NODE(ASTSimulationArgs)(&t_simType, astn_simStartt, astn_simEndt, &t_simNumSteps, astn_simAbsTol, astn_simRelTol);
}
