{ Fill in the following sections (removing comment marks ! if necessary),
  and delete those that are unused.}
TITLE 'Ficks'     { the problem identification }
COORDINATES cartesian1  { coordinate system, 1D,2D,3D, etc }
VARIABLES        { system variables }
  C_a              { choose your own names }
! SELECT         { method controls }
! DEFINITIONS    { parameter definitions }
INITIAL VALUES
C_a = 0
EQUATIONS        { PDE's, one for each variable }
C_a: div(grad(C_a)) - 0.001 * C_a - dt(C_a)  = 0 { one possibility }
! CONSTRAINTS    { Integral constraints }
BOUNDARIES       { The domain definition }
  REGION 1       { For each material region }
    START(0,0)   POINT  NATURAL(C_a)= 1.0 { Walk the domain boundary }
    LINE TO (1,0) POINT VALUE(C_a)=5.0 
    START(0,0)   { Walk the domain boundary }

    LINE TO (1,0)
TIME 0 TO 3    { if time dependent }
MONITORS         { show progress }
for cycle = 1
elevation(C_a)
PLOTS            { save result displays }
FOR T = 0 BY 0.01 TO 10
ELEVATION(C_a)  FROM (0,0) to (1,0)
END

!HISTORIES
!HISTORY(Phi) AT (10,0.25) (10,0.5) (10,0.75)