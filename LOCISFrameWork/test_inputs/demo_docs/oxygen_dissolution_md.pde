{ Fill in the following sections (removing comment marks ! if necessary),
  and delete those that are unused.}
TITLE 'Ficks'     { the problem identification }
COORDINATES cartesian1  { coordinate system, 1D,2D,3D, etc }
VARIABLES        { system variables }
  C_a              { choose your own names }
! SELECT         { method controls }
DEFINITIONS    { parameter definitions }
D_a
EQUATIONS        { PDE's, one for each variable }
C_a: D_a* div(grad(C_a))   = 0 { one possibility }

! CONSTRAINTS    { Integral constraints }

BOUNDARIES       { The domain definition }
  REGION 1       { For each material region }
	D_a = 0.0000214
    START(0,0)   POINT  VALUE(C_a)= 0.9375 { Walk the domain boundary }
      !LINE TO (0.001106741573,0) 
	LINE TO (0.001,0) 
	point contact(C_a) = 2.27326e-5*jump(C_a) 
	!point contact(C_a) = -jump(C_a) * 0.000000197/0.0000214
 REGION 2
	D_a = 0.000000197
!    START(0.001106741573,0)
    START(0.001,0)
!	LINE TO (0.0121741573,0) POINT VALUE(C_a)=0.00027
	LINE TO (0.012,0) POINT VALUE(C_a)=0.00027	



    START(0,0)   { Walk the domain boundary }

!    LINE TO (0.0121741573,0)
    LINE TO (0.012,0)

PLOTS
ELEVATION(C_a) FROM (0) to (0.012)

END
!HISTORIES
!HISTORY(Phi) AT (10,0.25) (10,0.5) (10,0.75)