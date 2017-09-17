#pragma once
#include<iostream>

void DAESIdaERRORFUN(int error_code, const char *module,
	const char *function, char *msg,
	void *user_data);

void DAESIdaOutputInfoHandler(void *s, int type);

void DAESIdaOutputErrorHandler(void *s, int type);

std::string IDAprd1(std::string x, int width);
std::string IDAprd(const double x, const int decDigits, const int width);
