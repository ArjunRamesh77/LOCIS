#ifndef solver_kinsol
#define solver_kinsol

struct problemdata{
//	char funcname[100];
	int has_bounds;
	realtype lb[NVAR];
	realtype ub[NVAR];
};

class solver_kinsol {
	problemdata problem_data;

public:
	void SetProblemdata(/*char funcname[], */int has_bounds 
		,realtype lb[], realtype ub[]);
	void PrintHasIneq();
	void Initialize(N_Vector u, N_Vector c, N_Vector s);
	void SetInitialGuess(realtype init_val[], N_Vector u);
	problemdata GetProblemData();
	//SetInitialGuess();
};

#endif