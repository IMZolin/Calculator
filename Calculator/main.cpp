#include "Calculator.h"

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
			std::cout << calc.getExpression() << "=" << calc.Solve() << std::endl;
		}
		catch (std::exception& error)
		{
			std::cout << error.what() << std::endl;
		}
		calc.EnterExpression();
	}

}

