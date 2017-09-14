% #--------------------------------------------------------------------------
% # Program to solve a linear finite element problem in one-dimension.
% # Author: Arjun Ramesh
% # Date: 04/08/2016
% #
% # The code will solve the PDE:
% #
% #       Div(K1*grad(F)) + K2*F + K3*DX(F) = 0
% #
% #   here "F" is the field variable of interest which is solved and DX(F)
% #   is the first derivative of F on a one dimensional domain of unit length
% #   with boundary consitions
% #
% #       phi = 1        @ (x = 0)  [dirichlet condition]
% #       GRAD(phi) = -2 @ (x = 1)  [neumann condition]
% #       Defined under "Problem Geometry" part of code
% #
% #       Geometry:
% #         All description of the domain is described under "Problem Geometry"
% #         part of code
% #
% #   Linear piecewise continous basis functions will be used to solve this
% #   problem.
% #
% #--------------------------------------------------------------------------
% # Notations:
% #
% # "phi" is the linear basis function
% # "phi'" is the first derivative of the basis function
% # "<phi,i>" is the global basis function phi @ index i (hat function)
% # "<phi',i>" is the global derivative basis function phi @ index i
% #
% #--------------------------------------------------------------------------
% # Integration rules:
% #
% # The midpoint rule is the integration of choice as they give exact values for
% # linear polynomials.
% #
% # example:
% #           integral(f,[x1,x2]) = f((x2 - x1)/2) * (x2 - x1)
% #
% #--------------------------------------------------------------------------
% #
% #   Notes:
% #    Please read the attached hand written document before attempting to
% #    understand the code. Anyway, the code here is divided into functions for
% #    evaluating the residual and evaluating the integral. The weak form of
% #    the PDE is what is worked on directly in the residual. First convert the
% #    2nd order PDE to its weak form and then everything becomes clear in the
% #    residual section.
% #--------------------------------------------------------------------------
function main()
clear all;

% # Problem Geometry
%   # Parameters
global LENGTH
global TOTAL_NODES
global dx
global K1
global K2
global K3
LENGTH = 1;
TOTAL_NODES = 25;
dx = LENGTH/(TOTAL_NODES - 1);

K1 = 1;
K2 = 10;
K3 = 10;
%   # Define Boundaries
global DIRICHLET_BOUNDARY_VALUE
global NUEMANN_BOUNDARY_VALUE
DIRICHLET_BOUNDARY_VALUE = 1;
NUEMANN_BOUNDARY_VALUE = -2;

%# Solve
x0 = linspace(1,1,TOTAL_NODES-1);
x0 = x0';
[sol info] = fsolve(@residual,x0);

%# Plot
plot(linspace(0,1,TOTAL_NODES),[DIRICHLET_BOUNDARY_VALUE sol'])
end


% # Midpoint rule for term integral(<phi',j>*<phi',i>)
function integral = integral1(i,j,dx)
global TOTAL_NODES;

if((i==1) && (j == 1))
    integral = 1/dx;
    return;
end

if((i == TOTAL_NODES) && (j == TOTAL_NODES))
    integral = 1/dx;
    return;
end

if(i == j)
    integral = 2/dx;
    return;
end

if(abs(i-j) == 1)
    integral = -1/dx;
    return;
end
integral = 0;
end

% # Midpoint rule for term integral(<phi,j>*<phi,i>*xval(j))
function integral = integral2(i,j,dx)
global TOTAL_NODES;


if((i == 1) && (j == 1))
    integral = (0.5*0.5)*dx;
    return
end

if((i == TOTAL_NODES) && (j == TOTAL_NODES))
    integral = (0.5*0.5)*dx;
    return
end

if(i == j)
    integral = 0.5*0.5*dx + 0.5*0.5*dx;
    return
end

if((i-j) == 1)
    integral = 0.5*0.5*dx;
    return
end

if((i-j) == -1)
    integral = 0.5*0.5*dx;
    return
end

integral = 0;

end

% # Midpoint rule for term integral(<phi',j>*<phi,i>)
function integral = integral3(i,j,dx)
global TOTAL_NODES;

if((i == 1) && (j == 1))
    integral = 0.5*(-1/dx)*dx;
    return
end

if((i == TOTAL_NODES) && (j == TOTAL_NODES))
    integral = 0.5*(1/dx)*dx;
    return
end

if(i == j)
    integral = 0.5*(1/dx)*dx + 0.5*(-1/dx)*dx;
    return
end

if((i-j) == 1)
    integral = -0.5*(1/dx)*dx;
    return
end

if((i-j) == -1)
    integral = 0.5*(1/dx)*dx;
    return
end

integral = 0;

end

% # Residual function for system
function res = residual(x)

global LENGTH;
global TOTAL_NODES;
global dx;
global K1;
global K2;
global K3;
global K3;
global DIRICHLET_BOUNDARY_VALUE;
global NUEMANN_BOUNDARY_VALUE;

%   #LOOP DESCRIPTION
%   # The outer loop loops over multiplying the weak form with the basis function
%   # <phi,i>, if a dirichlet node is in "i", <phi,i> is removed from the loop, in
%   # this case this is at i=1 so i starts from 2. Integration is performed over the
%   # common intersection areas of the global basis functions, not necessarily over
%   # each element. The domain of integration over at each step will depend on the
%   # common integration areas of the intersceting basis functions, this is automatically
%   # done in the definition of the integral functions.
%
%   # The inner loop loops over the summation of Sigma(<alpha,j>*<phi,j>) over all
%   # the nodes, here <phi,1> is not eliminated like in the outer loop, just that the
%   # <alpha,1> is substituted as a known value. Be clear here the <alpha,j>'s area
%   # the unknown values to be solved for.
%   #
%   # Please understand the weak form first!!

%   # Main Loops
for i=2:TOTAL_NODES  %# Loops over total number of equations
    
    equation_sum = 0;
    for j=1:TOTAL_NODES %# Loops over summation over the domain (Sigma over j, look at weak form)
        
        %# Dirichlet boundary condition
        if(j == 1)
            equation_sum = equation_sum - K1*DIRICHLET_BOUNDARY_VALUE*integral1(i,j,dx) + K2*DIRICHLET_BOUNDARY_VALUE*integral2(i,j,dx) + K3*DIRICHLET_BOUNDARY_VALUE*integral3(i,j,dx);
            
            %# Nuemann Boundary condition (Contains the extra nuemann term in the end)
        elseif((i == TOTAL_NODES) && (j == TOTAL_NODES))
            equation_sum = equation_sum - K1*x(j - 1)*integral1(i,j,dx) + K2*x(j - 1)*integral2(i,j,dx) + K3*x(j - 1)*integral3(i,j,dx)  + K1*NUEMANN_BOUNDARY_VALUE;
            
            %# For all nodes in betwen boundaries (same format as "Dirichlet boundary condition" section instead of known "DIRICHLET_BOUNDARY_VALUE" unknown "x" is substituted)
        else
            equation_sum = equation_sum - K1*x(j - 1)*integral1(i,j,dx) + K2*x(j - 1)*integral2(i,j,dx) + K3*x(j - 1)*integral3(i,j,dx);
        end
    end
    %# residual assembly
    res(i-1) = equation_sum;
end
end






