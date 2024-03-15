#include<iostream>
#include<math.h>
#include<tgmath.h>
#include<complex>
/**
* task: y = arth(x)
*/

struct row
{
	double value;
	int amount;
};

row arcth(double x, double epsilon)
{
	double buf, pow_x = x, k = 0, sum = 0;
	const double one = 1.0, two = 2.0;
	__asm
	{
		finit				; инициализация сопроцессора
		jmp For

		For:
		fld sum				; st(0) == sum
		fst buf
		fld one				; st(2) == sum | st(0) == 1
		fst buf
		fld two				; st(2) == sum | st(1) == 1 | st(0) == 2
		fst buf
		fld k				; st(3) == sum | st(2) == 1 | st(1) == 2						| st(0) == k
		fst buf
		fmul				; st(2) == sum | st(1) == 1 | st(0) == 2 * k
		fst buf
		fld one				; st(3) == sum | st(2) == 1 | st(1) == 2 * k					| st(0) == 1
		fst buf
		fadd				; st(2) == sum | st(1) == 1 | st(0) == 2 * k + 1
		fst buf
		fld pow_x			; st(3) == sum | st(2) == 1 | st(1) == 2 * k + 1				| st(0) == x^(2k + 1)
		fst buf
		fmul				; st(2) == sum | st(1) == 1 | st(0) == (2 * k + 1) * x^(2k + 1)
		fst buf
		fdiv				; st(1) == sum | st(0) == 1 / ((2 * k + 1) * x ^ (2k + 1))
		fst buf
		fcom epsilon			
		fstsw ax
		sahf
		fadd				; st(0) == sum + C_k
		fstp sum
		jc Exit_asm
		jmp Update_variables

		Update_variables:
			fld k
			fld one
			fadd
			fstp k
			fld pow_x			; st(0) == x^(2k + 1)
			fst buf
			fld x				; st(1) == x^(2k + 1)	| st(0) == x
			fst buf
			fld x				; st(2) == x^(2k + 1)	| st(1) == x | st(0) == x
			fst buf
			fmul				; st(1) == x^(2k + 1)	| st(0) == x^2
			fmul				; st(0) == x^(2k + 1) * x^2
			fstp pow_x
			jmp For

		Exit_asm:
	}
	row result;
	result.value = sum;
	result.amount = k;
	return result;
}

row cpparcth(double x, double epsilon)
{
	row result;
	double sum = 0, pow_x = x, C_k = 10;
	int k = 0;
	for (; C_k >= epsilon; ++k)
	{
		C_k = 1 / ((2 * k + 1) * (pow_x));
		pow_x *= x * x;
		sum += C_k;
	}
	result.value = sum;
	result.amount = k - 1;
	return result;
}

int main()
{
	double x, epsilon;
	std::cout << "Enter x:" << '\t';
	std::cin >> x;
	std::cout << "Enter precission:" << '\t';
	std::cin >> epsilon;
	row res_cpp, res_asm;
	res_asm = arcth(x, epsilon);
	res_cpp = cpparcth(x, epsilon);
	double result = 0.5 * log(abs((1 + x) / (1 - x)));
	std::cout << "Asm: arcth(" << x << ") = " << res_asm.value << '\n' << "Counted by the " << res_asm.amount << " member of the series" << '\n' << "With precission: " << result - res_asm.value << '\n';
	//std::cout << "Cpp: arcth(" << x << ") = " << res_cpp.value << " counted by the " << res_cpp.amount << " member of the series" << " with difference: " << result - res_cpp.value << '\n';
	return 0;
}