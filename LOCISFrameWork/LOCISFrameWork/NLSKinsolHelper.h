#pragma once
#include <iostream>
void NLSKinsolIHFUN(const char *module, const char *function, char *msg, void *user_data);

void NLSKinsolERRORFUN(int error_code, const char *module,
	const char *function, char *msg,
	void *user_data);

void NLSKinsolOutputInfoHandler(void *s, int type);

void NLSKinsolOutputErrorHandler(void *s, int type);

std::string KINprd1(std::string x, int width);

std::string KINprd(const double x, const int decDigits, const int width);