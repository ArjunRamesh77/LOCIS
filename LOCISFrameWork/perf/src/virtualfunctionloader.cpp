#include "virtualinstruction.h"
#include "math.h"

//singleton object
virtualFunctionLoader g_virtualFunctionLoader;

virtualFunctionLoader::virtualFunctionLoader()
{
    loadAllSISO();
    loadAllDISO();
}

void virtualFunctionLoader::loadAllSISO()
{
    int index = 0;

    //trigonometric
    fpSISOarr[index] = cos;
    index++;

    fpSISOarr[index] = sin;
    index++;

    fpSISOarr[index] = tan;
    index++;

    fpSISOarr[index] = acos;
    index++;

    fpSISOarr[index] = asin;
    index++;

    fpSISOarr[index] = atan;
    index++;

    fpSISOarr[index] = acos;
    index++;

    fpSISOarr[index] = cosh;
    index++;

    fpSISOarr[index] = sinh;
    index++;

    fpSISOarr[index] = tanh;
    index++;

    fpSISOarr[index] = acosh;
    index++;

    fpSISOarr[index] = asinh;
    index++;

    fpSISOarr[index] = atanh;
    index++;

    //Exponential and logarithmic
    fpSISOarr[index] = exp;
    index++;

    fpSISOarr[index] = log;
    index++;

    fpSISOarr[index] = log10;
    index++;

    //root
    fpSISOarr[index] = sqrt;
    index++;
}

void virtualFunctionLoader::loadAllDISO()
{
    int index = 0;

    fpDISOarr[index] = pow;
    index++;
}
