#include "Calculator.h"
void Calculator::EnterExpression()
{
	std::cout << "Expression: " << std::endl;
	std::getline(std::cin, expression);
}
Calculator::~Calculator()
{
	for (auto& dll : dlls)
		FreeLibrary(dll);
	for (auto& oper : operations)
		delete oper;
}
Calculator::Calculator()
{
	BaseOperations();
	//loadDll();
}
void Calculator::loadDll() {
	std::filesystem::path pluginsPath = std::filesystem::current_path().parent_path();
	pluginsPath /= "plugins";
	if (!std::filesystem::exists(pluginsPath))
		throw std::exception("Error: No files in file input");

	using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
	for (const auto& file : recursive_directory_iterator(pluginsPath)) {
		std::string filename = file.path().string();
		HMODULE dll = LoadLibraryA(filename.c_str());
		if (!dll)
			throw std::exception("Error: Files in directory is not dll format");
		dlls.push_back(dll);
		importFunctions(dll);
	}
}
void Calculator::BaseOperations()
{
	Operation* add = new Operation("+", 1, true, false, [](std::stack<double>& s) {
		if (s.size() < 2)
			throw std::exception("Error: There must be 2 arguments for addition\n");
		double x = s.top();
		s.pop();
		double y = s.top() + x;
		s.top() = y;
		});
	Operation* sub = new Operation("-", 1, true, false, [](std::stack<double>& s) {
		if (s.size() < 2)
			throw std::exception("Error: There must be 2 arguments for substract\n");
		double x = s.top();
		s.pop();
		double y = s.top() - x;
		s.top() = y;
		});
	Operation* mul = new Operation("*", 2, true, false, [](std::stack<double>& s) {
		if (s.size() < 2)
			throw std::exception("Error: There must be 2 arguments for multiplication\n");
		double x = s.top();
		s.pop();
		double y = s.top() * x;
		s.top() = y;
		});
	Operation* div = new Operation("/", 2, true, false, [](std::stack<double>& s) {
		if (s.size() < 2 || s.top() == 0)
			throw std::exception("Error: There must be 2 arguments for division\n");
		double x = s.top();
		s.pop();
		double y = s.top() / x;
		s.top() = y;
		});
	Operation* left_bracket = new Operation("(", 0, false, false, nullptr);
	Operation* right_bracket = new Operation(")", 0, false, false, nullptr);
	operations.push_back(add);
	operations.push_back(sub);
	operations.push_back(mul);
	operations.push_back(div);
	operations.push_back(left_bracket);
	operations.push_back(right_bracket);

}
void Calculator::importFunctions(HMODULE dll)
{
	char* func_name;
	(FARPROC&)func_name = GetProcAddress(dll, "name");
	bool* func_binary;
	(FARPROC&)func_binary = GetProcAddress(dll, "isBinary");
	bool* func_prefix;
	(FARPROC&)func_prefix = GetProcAddress(dll, "isPrefix");
	int* func_priority;
	(FARPROC&)func_priority = GetProcAddress(dll, "priority");

	std::cout << func_name << std::endl;
	std::function func_realization = [dll, func_binary](std::stack<double>& args) {
		if (args.size() == 0)
			throw std::exception("Error: The operation is performed on at least one operand\n");
		if (*func_binary && args.size() == 2) {
			double (*pFunction)(double, double);
			(FARPROC&)pFunction = GetProcAddress(dll, "function");
			double arg1 = args.top();
			args.pop();
			double arg2 = pFunction(args.top(), arg1);
			args.top() = arg2;
			return arg2;
		}
		else if (!*func_binary && args.size() == 1) {
			double (*pFunction)(double);
			(FARPROC&)pFunction = GetProcAddress(dll, "function");
			args.top() = pFunction(args.top());
			return args.top();
		}
	};
	Operation* new_func = new Operation(func_name, *func_priority, *func_binary, *func_prefix, func_realization);
	operations.push_back(new_func);
}
std::list<std::string> Calculator::getPolishNotation()
{
	loadDll();
	removeSpaces();
	for (size_t i = 0; i < expression.size(); i++)
	{
		if (isDigit(expression[i]) || expression[i] == '-' && (i == 0 || (expression.size() != 1 && isDigit(expression[i - 1])) || (expression.size() != 1 && expression[i - 1] == '(')))
		{
			std::string buf;
			buf += expression[i];
		}

	}
	return { "hello" };
}
double Calculator::SolvePolishNotation(std::list<std::string> polish)
{
	std::stack<double> polish_sol;
	return 0.0;
}
std::string Calculator::Solve()
{
	return "solved";
}
void Calculator::removeSpaces() {
	for (int i = 0; i < expression.length(); i++)
	{
		if (expression[i] == ' ')
		{
			expression.erase(expression.begin() + i);
			i--;
		}
	}
}
bool Calculator::isDigit(char const& c)
{
	return (c >= '0' && c <= '9') ? true : false;
}
bool Calculator::isLetter(char const& c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ? true : false;
}
bool Calculator::isOper(char const& c)
{
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') ? true : false;
}