#include "Calculator.h"
//Init commit22
int main()
{
	std::cout << "Console calculator\n";
	Calculator calc;
	calc.EnterExpression();
	while (calc.getExpression() != "" || calc.getExpression() != "stop")
	{
		if (calc.getExpression() == "" || calc.getExpression() == "stop")
			break;
		try
		{
			calc.removeSpaces();
			std::cout << calc.getExpression() << "=" << calc.Solve() << std::endl;
		}
		catch (std::exception& error)
		{
			std::cout << error.what() << std::endl;
		}
		calc.EnterExpression();
	}

}

