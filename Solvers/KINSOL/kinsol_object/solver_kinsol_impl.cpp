#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <kinsol/kinsol.h>
#include <kinsol/kinsol_dense.h>
#include <nvector/nvector_serial.h>
#include <sundials/sundials_types.h>
#include <sundials/sundials_math.h>
#include <solver_kinsol.h>

#define NVAR 2
#define NEQ 3*NVAR
#define Ith(v,i) NV_Ith_S(v,i-1)
void solver_kinsol::SetProblemdata(/*char funcname[], */int has_bounds 
		,realtype lb[], realtype ub[]){
//		problem_data.funcname = funcname;
		problem_data.has_bounds = has_bounds;
		(problem_data.lb[0]) = lb[0];
		(problem_data.lb[1]) = lb[1];
		problem_data.ub[1] = ub[1];
		problem_data.ub[0] = ub[0];
	}

void solver_kinsol::PrintHasIneq() 
{printf("HASINEQ : %d\n", problem_data.has_bounds);}

void solver_kinsol::Initialize(N_Vector u, N_Vector c, N_Vector s){
	u = N_VNew_Serial(NEQ);
	s = N_VNew_Serial(NEQ);
	c = N_VNew_Serial(NEQ);
};

void solver_kinsol::SetInitialGuess(realtype init_val[],N_Vector u){
	realtype x1, x2; // initial guess
	realtype *udata;
	udata = NV_DATA_S(u);
	x1 = init_val[0];
	x2 = init_val[1];
	//x2 = lb[1];
	udata[0] = x1;
	udata[1] = x2;
	udata[2] = x1 - problem_data.lb[0];
	udata[3] = x1 - problem_data.ub[0];
	udata[4] = x2 - problem_data.lb[1];
	udata[5] = x2 - problem_data.ub[1];
};

problemdata solver_kinsol::GetProblemData(){
	return problem_data;
};