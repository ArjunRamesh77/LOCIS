# README #

This is the primary repository for the LOCIS project. An object oriented DSL for modeling and simulation.

```
Example:
# CSTR with energy balance
# Reaction A -> B
# Adapted from http://apmonitor.com/che436/index.php/Main/CaseStudyCSTR
model cstr 
{
  parameter T_c = 270;
  parameter q = 10;
  parameter V = 100;
  parameter rho = 1000;
  parameter C_p = 0.239;
  parameter del_H = 5.0E4;
  parameter E_R = 8750;
  parameter k_o = 7.2E10;
  parameter U_a = 0.5E4;
  variable Ca_in;
  variable T_in;
  variable Ca;
  variable T;
  
  guess
  {
    Ca = 0.9;
    T = 305;
  }
  
  init
  {
    Ca = 0.9;
    T = 305;
  }
  
  equation
  {
    # Mole Balance
    V * $Ca = q * (Ca_in - Ca) - k_o * V * EXP(-E_R/T) * Ca^2;
    # Energy Balance
    rho * C_p * V * $T = q * rho * C_p * (T_in - T) + V * del_H * k_o 
    * EXP(-E_R/T) * Ca + U_a * (T_c - T);
  }
}

odel tank_farm <DYNAMIC 0.0 100.0 100 1.0E-5 1.0E-5>
{
  parameter NUM_TANKS = 2;
  cstr Tanks[NUM_TANKS];
  iter i;
  
  fix
  {
    Tanks[1].Ca_in = 0.2;
    Tanks[1].T_in = 350;
  }
  
  equation
  {
    for(i = 1, NUM_TANKS - 1)
    {
      Tanks[i].Ca = Tanks[i+1].Ca_in;
      Tanks[i].T = Tanks[i+1].T_in;
    }
  }
}
```
