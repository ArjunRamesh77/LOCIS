#include "parser.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Fem one D line declaration
ASTNode* parser::P_basis_type()
{
	AST_DEC_TOK(basis_type)

    START_RULE

	EXPECT_TOKEN_ELSE(BASIS)
        syntaxErrorExpectedType(BASIS);
	END_EXPECT

	EXPECT_TOKEN_ELSE(EQUALS)
        syntaxErrorExpectedType(EQUALS);
	END_EXPECT

	AST_SAV_TOK(basis_type)

	EXPECT_TOKEN_ELSE(STRING)
        syntaxErrorExpectedType(STRING);
	END_EXPECT

	AST_RETURN_NODE(ASTBasisTypeNode)(&t_basis_type);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Fem one D line declaration
ASTNode* parser::P_segment_def()
{
    START_RULE

	EXPECT_TOKEN_ELSE(SEGMENT)
        syntaxErrorExpectedType(SEGMENT);
	END_EXPECT

	EXPECT_TOKEN_ELSE(EQUALS)
        syntaxErrorExpectedType(EQUALS);
	END_EXPECT

	DO_RULE(segment_attributes)

	AST_RETURN_NO_CREATE
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// defines the attributes for a line segment
ASTNode* parser::P_segment_attributes()
{
	AST_DEC_VEC(all_segment_attributes)

    START_RULE

	EXPECT_TOKEN_ELSE(LCURLY)
        syntaxErrorExpectedType(LCURLY);
	END_EXPECT

	DO_RULE(segment_attribute) AST_SAV_VEC(all_segment_attributes)

	FOR_ALL_WHILE
		ITEM_E(COMMA)
            EAT
			DO_RULE(segment_attribute) AST_SAV_VEC(all_segment_attributes)		
	END_WHILE

	EXPECT_TOKEN_ELSE(RCURLY)
        syntaxErrorExpectedType(RCURLY);
	END_EXPECT

	AST_RETURN_NODE(ASTSegmentAttributesNode)(astvn_all_segment_attributes);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// segment attributes
ASTNode* parser::P_segment_attribute()
{
    START_RULE

	SWITCH
		CASE_RULE(ID, segment_id_type)
		CASE_RULE(NUMEL, segment_numel)
		CASE_RULE(LENGTH, segment_length)
		CASE_RULE(pDOMAIN, domain_option)
		LAST
			FORCE_ERROR
				std::vector<int> possible_states;
				possible_states.push_back(ID);
				possible_states.push_back(NUMEL);
				possible_states.push_back(LENGTH);
				possible_states.push_back(pDOMAIN);
                syntaxErrorExpectedAnyofType(possible_states);
			UNFORCE_ERROR
	END_SWITCH

	AST_RETURN_NO_CREATE
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// segment attribute id
ASTNode* parser::P_segment_id_type()
{
	AST_DEC_TOK(id)

    START_RULE

	EXPECT_TOKEN_ELSE(ID)
        syntaxErrorExpectedType(ID);
	END_EXPECT

	EXPECT_TOKEN_ELSE(EQUALS)
        syntaxErrorExpectedType(EQUALS);
	END_EXPECT

	AST_SAV_TOK(id)

	EXPECT_TOKEN_ELSE(STRING)
        syntaxErrorExpectedType(STRING);
	END_EXPECT

	AST_RETURN_NODE(ASTSegmentIdTypeNode)(&t_id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// segment attribute id
ASTNode* parser::P_segment_numel()
{
	AST_DEC_TOK(Tag)
    START_RULE

	AST_SAV_TOK(Tag)
	EXPECT_TOKEN_ELSE(NUMEL)
        syntaxErrorExpectedType(NUMEL);
	END_EXPECT

	EXPECT_TOKEN_ELSE(EQUALS)
        syntaxErrorExpectedType(EQUALS);
	END_EXPECT

	DO_RULE(expression)

	AST_RETURN_NODE(ASTNumelNode)(ASTtempNode, &t_Tag);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// segment attribute id
ASTNode* parser::P_segment_length()
{
	AST_DEC_TOK(SegLen)
    START_RULE

	AST_SAV_TOK(SegLen)
	EXPECT_TOKEN_ELSE(LENGTH)
        syntaxErrorExpectedType(LENGTH);
	END_EXPECT

	EXPECT_TOKEN_ELSE(EQUALS)
        syntaxErrorExpectedType(EQUALS);
	END_EXPECT

	DO_RULE(expression)

	AST_RETURN_NODE(ASTLengthNode)(ASTtempNode, &t_SegLen);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// domain option node for declaring PDE variables
ASTNode* parser::P_domain_option()
{
	AST_DEC_TOK(Tag)
	AST_DEC_VEC(domains)

    START_RULE

	AST_SAV_TOK(Tag)
	EXPECT_TOKEN_ELSE(pDOMAIN)
        syntaxErrorExpectedType(pDOMAIN);
	END_EXPECT

	EXPECT_TOKEN_ELSE(EQUALS)
        syntaxErrorExpectedType(EQUALS);
	END_EXPECT

	EXPECT_TOKEN_ELSE(LCURLY)
        syntaxErrorExpectedType(LCURLY);
	END_EXPECT

	DO_RULE(type_name) AST_SAV_VEC(domains)
	
	FOR_ALL_WHILE
		ITEM_E(COMMA)
        EAT
			DO_RULE(type_name) AST_SAV_VEC(domains)
	END_WHILE

	EXPECT_TOKEN_ELSE(RCURLY)
        syntaxErrorExpectedType(RCURLY);
	END_EXPECT

	AST_RETURN_NODE(ASTdomainOptionNode)(astvn_domains, &t_Tag);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FEM one D equation
ASTNode* parser::P_fem1d_equation()
{
	AST_DEC_NODE(domain)
	AST_DEC_NODE(fem_eqn)
	AST_DEC_NODE(leftb)
	AST_DEC_NODE(rightb)
	AST_DEC_NODE(fem_boundary_left_eqn)
	AST_DEC_NODE(fem_boundary_right_eqn)

    START_RULE

	EXPECT_TOKEN_ELSE(PDE)
        syntaxErrorExpectedType(PDE);
	END_EXPECT

	EXPECT_TOKEN_ELSE(COMMA)
        syntaxErrorExpectedType(COMMA);
	END_EXPECT

	DO_RULE(full_type_name) AST_SAV_NODE(domain)

	EXPECT_TOKEN_ELSE(COMMA)
        syntaxErrorExpectedType(COMMA);
	END_EXPECT

	DO_RULE(boundary_type) AST_SAV_NODE(leftb)

	EXPECT_TOKEN_ELSE(COMMA)
        syntaxErrorExpectedType(COMMA);
	END_EXPECT

	DO_RULE(boundary_type) AST_SAV_NODE(rightb)

	EXPECT_TOKEN_ELSE(COLON)
        syntaxErrorExpectedType(COLON);
	END_EXPECT

	DO_RULE(equation) AST_SAV_NODE(fem_eqn)

	EXPECT_TOKEN_ELSE(COMMA)
        syntaxErrorExpectedType(COMMA);
	END_EXPECT

	DO_RULE(expression) AST_SAV_NODE(fem_boundary_left_eqn)

	EXPECT_TOKEN_ELSE(COMMA)
        syntaxErrorExpectedType(COMMA);
	END_EXPECT

	DO_RULE(expression) AST_SAV_NODE(fem_boundary_right_eqn)

	EXPECT_TOKEN_ELSE(SEMICOLON)
        syntaxErrorExpectedType(SEMICOLON);
	END_EXPECT

	AST_RETURN_NODE(ASTFEMoneDEquation)(astn_domain, astn_fem_eqn, astn_fem_boundary_left_eqn, astn_fem_boundary_right_eqn, astn_leftb, astn_rightb);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FEM one D equation
ASTNode* parser::P_string_index()
{
	AST_DEC_TOK(string_val)
	AST_SAV_TOK(string_val)

    START_RULE

	EXPECT_TOKEN_ELSE(STRING)
        syntaxErrorExpectedType(STRING);
	END_EXPECT

	AST_RETURN_NODE(ASTstringIndex)(&t_string_val);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FEM pdevar
ASTNode* parser::P_pdevar_def()
{
    START_RULE

	EXPECT_TOKEN_ELSE(PDEVAR)
        syntaxErrorExpectedType(PDEVAR);
	END_EXPECT

	EXPECT_TOKEN_ELSE(EQUALS)
        syntaxErrorExpectedType(EQUALS);
	END_EXPECT

	DO_RULE(pdevar_attributes)

	AST_RETURN_NO_CREATE
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// defines the attributes for a pdevar
ASTNode* parser::P_pdevar_attributes()
{
	AST_DEC_VEC(all_segment_attributes)

    START_RULE

	EXPECT_TOKEN_ELSE(LCURLY)
        syntaxErrorExpectedType(LCURLY);
	END_EXPECT

	DO_RULE(segment_attribute) AST_SAV_VEC(all_segment_attributes)

	FOR_ALL_WHILE
		ITEM_E(COMMA)
            EAT
			DO_RULE(segment_attribute) AST_SAV_VEC(all_segment_attributes)		
	END_WHILE

	EXPECT_TOKEN_ELSE(RCURLY)
        syntaxErrorExpectedType(RCURLY);
	END_EXPECT

	AST_RETURN_NODE(ASTPdevarAttributesNode)(astvn_all_segment_attributes);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// gives the derivative information for a variable
ASTNode* parser::P_derivative_sigle_var()
{
	std::vector<token> astvtn_numquotes;

    START_RULE

	FOR_ALL_WHILE
		ITEM_E(APOS)
            astvtn_numquotes.push_back(curTok);
            EAT
	END_WHILE

	AST_RETURN_NODE(ASTDerivativeSingleVar)(astvtn_numquotes);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Boundary condition types for FEM oneD equation
ASTNode* parser::P_boundary_type()
{
	AST_DEC_TOK(boundary_type)
	AST_SAV_TOK(boundary_type)

    START_RULE

	SWITCH
		CASE(VALUE)
		CASE(FLUX)
		LAST
			FORCE_ERROR
				std::vector<int> possible_states;
				possible_states.push_back(VALUE);
				possible_states.push_back(FLUX);
                syntaxErrorExpectedAnyofType(possible_states);
			UNFORCE_ERROR
	END_SWITCH

	AST_RETURN_NODE(ASTBoundaryConditions)(&t_boundary_type);
}
