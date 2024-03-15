#include<iostream>

/*
* task: X={8-b^2/a, если a>b; -123, если a=b; (5a+7)/(b-1), если a<b;
*/

double function_asm(double a, double b)
{
	/*
		* if(a > b)
		* {
		*	x = (5 * a + 7) / (b - 1);
		* }
		* else if(a < b)
		* {
		*	x = 8 - b^2 / a;
		* }
		* else
		* {
		*	x = -123;
		* }
	*/
	int dbz;
	double x, buf;
	const double min_123 = -123.0, five = 5.0, seven = 7.0, eight = 8.0, one = 1.0;
	__asm
	{
		finit				; инициализация сопроцессора
		fld a				; st(0) == a
		fld b				; st(1) == a, st(0) == b
		fcom				; сравниваем st(1) и st(0)
		fstsw	ax
		sahf
		js Less
		jz Equal
		jmp Greater


		Less:
			fstp buf		; st(0) == a
			fstp buf		; st == empty
			fld eight		; st(0) == 8
			fld b			; st(1) == 8, st(0) == b
			fld b			; st(2) == 8, st(1) == b, st(0) == b
			fmul			; st(1) == 8, st(0) == b * b
			fst buf
			fld a			; st(2) == 8, st(1) == b * b, st(0) == a
			ftst			
			fstsw ax
			fst buf
			;jz Division_by_zero
			fdiv			; st(1) == 8, st(0) == b * b / a
			fst buf
			fsub			; st(0) == 8 - b * b / a
			fst buf
			jmp Write_result


		Equal:
			fld min_123		; st(0) == -123
			jmp Write_result


		Greater:
			fstp buf		; st(0) == a
			fstp buf		; st == empty
			fld	five		; st(0) == 5
			fld a			; st(1) == 5, st(0) == a
			fst buf
			fmul			; st(0) == 5 * a
			fst buf
			fld seven		; st(1) == 5 * a, st(0) == 7
			fadd			; st(0) == 5 * a + 7
			fst buf
			fld b			; st(1) == 5 * a + 7, st(0) == b
			fst buf
			fld one			; st(2) == 5 * a + 7, st(1) == b, st(0) == 1
			fst buf
			fsub			; st(1) == 5 * a - 7, st(0) == b - 1
			fst buf
			ftst
			fstsw ax
			sahf
			jz Division_by_zero
			fdiv			; st(0) == (5 * a - 7) / (b - 1)
			jmp Write_result

		Division_by_zero:
			fstp buf
			fstp buf
			fstp buf
			mov eax, 0
			mov dbz, eax
			fldz			; st(0) == 0
			jmp Write_result

		Write_result:
			fstp x			; st == empty
	}
	if (!dbz)
	{
		std::cout << "Asm division by zero!" << '\n';
	}
	return x;
}

double function_cpp(double a, double b)
{
	double x;
	if (a > b)
	{
		if (!(b - 1))
		{
			std::cout << "Cpp division by zero" << '\n';
			return 0;
		}
		x = (5 * a + 7) / (b - 1);

	}
	else if (a < b)
	{
		if (!a)
		{
			std::cout << "Cpp division by zero" << '\n';
			return 0;
		}
		x = 8.0 - b * b / a;
	}
	else
	{
		x = -123;
	}
	return x;
}

int main()
{
	double a, b;
	while (true)
	{
		std::cout << "Enter a:" << '\t';
		std::cin >> a;
		std::cout << "Enter b:" << '\t';
		std::cin >> b;
		std::cout << "Asm function:" << '\t' << function_asm(a, b) << '\n';
		std::cout << "Cpp function:" << '\t' << function_cpp(a, b) << '\n';
		std::cout << '\n' << "________________________" << '\n';
	}
	return 0;
}