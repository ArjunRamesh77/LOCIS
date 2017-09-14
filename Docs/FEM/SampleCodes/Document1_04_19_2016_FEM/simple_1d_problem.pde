
TITLE 'Simple 1D problem'

COORDINATES
CARTESIAN1

VARIABLES
Phi           

DEFINITIONS
k1 = 1
k2 = 1
k3 = 1

EQUATIONS
Div(grad(phi))  + k1*phi + k2*DX(phi) + k3  = 0

BOUNDARIES
REGION 1
START(0,0)    POINT VALUE(Phi)=0  {dirichlet condition}
LINE TO(1,0)   POINT VALUE(Phi)=0  {nuemann condition}

PLOTS
ELEVATION(Phi) FROM (0,0) to (1,0)

END

 