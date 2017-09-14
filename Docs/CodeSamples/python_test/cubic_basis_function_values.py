import numpy as np

def main():
    x1 = 0.
    x2 = 1./3.
    x3 = 2./3.
    x4 = 1.
    
    print N1(x1), N1(x2), N1(x3), N1(x4)
    print N2(x1), N2(x2), N2(x3), N2(x4)
    print N3(x1), N3(x2), N3(x3), N3(x4)
    print N4(x1), N4(x2), N4(x3), N4(x4)    

    print dN1(x1), dN1(x2), dN1(x3), dN1(x4)
    print dN2(x1), dN2(x2), dN2(x3), dN2(x4)
    print dN3(x1), dN3(x2), dN3(x3), dN3(x4)
    print dN4(x1), dN4(x2), dN4(x3), dN4(x4)        
    return
    
    
def N1(x):
    return 1 - 3 * x**2 + 2 * x**3
    
    
def N2(x):
    return x - 2 * x**2 + x**3
    
def N3(x):
    return 3 * x**2 - 2 * x**3
    
def N4(x):
    return -x**2 + x**3    

def dN1(x):
    return  - 6 * x + 6 * x**2
    
    
def dN2(x):
    return 1 - 4 * x + 3* x**2
    
def dN3(x):
    return 6 * x - 6 * x**2
    
def dN4(x):
    return -2 * x + 3 * x**2        
main()