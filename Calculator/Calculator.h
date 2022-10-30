#pragma once
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <Windows.h>
#include <stack>
#include <list>
#include <algorithm>
#include "Operation.h"
const std::string plugins_dir = "plugins";

class Calculator
{
private:
	std::string expression;
	std::vector<HMODULE> dlls;
	std::vector<Operation*> operations;
public:
	Calculator();
	~Calculator();
	void loadDll();
	void BaseOperations();
	void importFunctions(HMODULE dll);
	std::list<std::string> getPolishNotation();
	double SolvePolishNotation(std::list<std::string> polish);
	std::string Solve();
	bool isDigit(char const& c);
	bool isLetter(char const& c);
	bool isNotOper(std::string c);
	void EnterExpression();
	void setExpression(std::string expr) { expression = expr; }
	std::string getExpression() { return this->expression; }
	void removeSpaces();
};

