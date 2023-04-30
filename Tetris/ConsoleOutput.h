#pragma once
#include <iostream>
#include <windows.h>
#include <vector>

class ConsoleOutput
{
public:
	void print(std::vector<std::string>& frame);
	void print(std::string text);
	void setRow(std::string str);
	void end(HANDLE console, COORD position);
private:
	HANDLE _console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD _pos = { 0, 0 };
	
	const int8_t horizontalRatio = 2;

	std::string _row;
};