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
	loadDll();
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
	Operation* add = new Operation("+", 1, true, false, [](std::stack<double>& args) {
		if (args.size() < 2)
			throw std::exception("Error: There must be 2 arguments for addition\n");
		double arg1 = args.top();
		args.pop();
		double arg2 = args.top() + arg1;
		args.top() = arg2;
		});
	Operation* sub = new Operation("-", 1, true, false, [](std::stack<double>& args) {
		if (args.size() < 2)
			throw std::exception("Error: There must be 2 arguments for substract\n");
		double arg1 = args.top();
		args.pop();
		double arg2 = args.top() - arg1;
		args.top() = arg2;
		});
	Operation* mul = new Operation("*", 2, true, false, [](std::stack<double>& args) {
		if (args.size() < 2)
			throw std::exception("Error: There must be 2 arguments for multiplication\n");
		double arg1 = args.top();
		args.pop();
		double arg2 = args.top() * arg1;
		args.top() = arg2;
		});
	Operation* div = new Operation("/", 2, true, false, [](std::stack<double>& args) {
		if (args.size() < 2)
			throw std::exception("Error: There must be 2 arguments for division\n");
		if (args.top() == 0)
			throw std::exception("Error: The second argument cannot be 0\n");
		double arg1 = args.top();
		args.pop();
		double arg2 = args.top() / arg1;
		args.top() = arg2;
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
		if (*func_binary && args.size() >= 2) {
			double (*pFunction)(double, double);
			(FARPROC&)pFunction = GetProcAddress(dll, "function");
			double arg1 = args.top();
			args.pop();
			double arg2 = pFunction(args.top(), arg1);
			args.top() = arg2;
			return arg2;
		}
		else if (!*func_binary && args.size() >= 1) {
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
	removeSpaces();
	std::stack<Operation*> oper_stack;
	std::list<std::string> polish_list;
	for (size_t i = 0; i < expression.size(); i++)
	{
		//1)Number
		if (isDigit(expression[i]) || expression[i] == '-' && (i == 0 || (expression.size() != 1 && isDigit(expression[i - 1])) || (expression.size() != 1 && expression[i - 1] == '(')))
		{
			std::string buf;
			size_t j = i;
			if (expression[i] == '-')
			{
				buf = "-";
				++j;
			}
			bool isPoint = false;
			while (isDigit(expression[j]) || expression[j] == '.')
			{
				if (expression[j] == '.')
				{
					if (isPoint)
					{
						throw std::exception("A number cannot have two separators (dots)");
					}
					isPoint = true;
				}
				buf += std::string(1, expression[j]);
				j++;
			}
			i += buf.size() - 1;
			polish_list.push_back(buf);

		}
		//2) ,
		else if (expression[i] == ',') 
		{
			while (!oper_stack.empty() && oper_stack.top()->getName() != "(") {
				polish_list.push_back(oper_stack.top()->getName());
				oper_stack.pop();
			}
		}
		//3) Operations
		else 
		{
			bool operExists = false;
			for (auto const& oper : operations) {
				bool letterMatch = true;
				for (int k = 0; k < oper->getName().size(); ++k) {
					if (oper->getName()[k] != expression[i + k]) {
						letterMatch = false;
						break;
					}
				}
				if (letterMatch) {
					operExists = true;
					
					i += oper->getName().size() - 1;
					if (oper_stack.empty() || oper->getName() == "(")
						oper_stack.push(oper);
					else {
						while (!oper_stack.empty() && oper_stack.top()->getPrior() >= oper->getPrior() && oper_stack.top()->getName() != "(") {
							if (oper->getPrefix() == true)
							polish_list.push_back(oper_stack.top()->getName());
							oper_stack.pop();
						}
						if (oper->getName() != ")") {
							oper_stack.push(oper);
						}
						else {
							oper_stack.pop();
						}
					}
					break;
				}
			}
			if (!operExists) {
				throw std::exception("Operation or symbol is not correct\n");
				exit(1);
			}
		}
	}
	//4)push operations from oper_stack to polish_list
	while (!oper_stack.empty())
	{
		if (oper_stack.top()->getName() == "(")
		{
			throw std::exception("Error: The bracket has been lost");
		}
		polish_list.push_back(oper_stack.top()->getName());
		oper_stack.pop();
	}
	return polish_list;
}
double Calculator::SolvePolishNotation(std::list<std::string> polish_list)
{
	std::stack<double> polish_sol;
	while (!polish_list.empty()) {
		if (isDigit(polish_list.front()[0]) || (polish_list.front()[0] == '-' && polish_list.front().size() > 1)) {
			polish_sol.push(std::stod(polish_list.front()));
			polish_list.pop_front();
		}
		else {
			for (auto const& item : operations) {
				if (item->getName() == polish_list.front()) {
					try {
						item->func(polish_sol);  
					}
					catch (std::exception& e) {
						std::cout << e.what();
						exit(1);
					}

					break;
				}
			}
			polish_list.pop_front();
		}
	}
	if (polish_sol.size() == 1)
		return polish_sol.top();
	else
		throw std::exception("Error: The operation didn't found\n");
}
std::string Calculator::Solve()
{
	std::list<std::string> polish = getPolishNotation();
	double result = SolvePolishNotation(polish);
	std::string str_result = std::to_string(result);
	return str_result;
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


