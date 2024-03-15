#include<iostream>
#include<string>
/*
* task: (4*c + d*c*a - 1)/(c - a/2); с вещественными числами
*/

double function_asm(double a, double c, double d)
{
	/*
	* 1) 4 * c
	* 2) d * c
	* 3) d * c * a
	* 4) 4 * c + d * c * a
	* 5) 4 * c + d * c * a - 1
	* 5) a / 2
	* 6) c - a / 2
	* 7) (4 * c + d * c * a - 1) / (c - a / 2)
	*/
	int dbz;
	double result_double, buf;
	const double four = 4, two = 2, one = 1, min_one = -1, zero = 0;
	__asm
	{
		finit		; инициализация сопроцессора

		fld qword ptr[four]	; st(0) == 4
		fld qword ptr[c]	; st(1) == 4	, st(0) == c
		fmul				; st(0) == 4 * c
		fld qword ptr[c]	; st(1) == 4 * c, st(0) == c
		fld d 		; st(2) == 4 * c, st(1) == c		, st(0) == d
		fmul		; st(1) == 4 * c, st(0) == c * d

		fld a		; st(2) == 4 * c, st(1) == d * c	, st(0) == a
		fmul		; st(1) == 4 * c, st(0) == d * c * a
		fadd		; st(0) == 4 * c + d * c * a
		fld min_one ; st(1) == 4 * c + d * c * a, st(0) == -1
		fadd		; st(0) == 4 * c + d * c * a - 1
		fld a		; st(1) == 4 * c + d * c * a - 1, st(0) == a
		fld two		; st(2) == 4 * c + d * c * a - 1, st(1) == a, st(0) == 2
		fdiv		; st(1) == 4 * c + d * c * a - 1, st(0) == a / 2
		
		fstp buf	; st(0) == 4 * c + d * c * a - 1
		fld c		; st(1) == 4 * c + d * c * a - 1, st(0) == c
		fld buf		; st(2) == 4 * c + d * c * a - 1, st(1) == c, st(0) == a / 2
		fsub		; st(1) == 4 * c + d * c * a - 1, st(0) == c - a / 2

		ftst
		fstsw ax	; проверяем делитель на соответствие 0
		sahf
		jz Division_by_zero

		fdiv		; st(0) == (4 * c + d * c * a - 1) / (c - a / 2)
		fst buf		; st(0) == (4 * c + d * c * a - 1) / (c - a / 2)
		jmp Write_result

		Division_by_zero:
			fstp buf			; st(0) == 4 * c + d * c * a - 1
			fstp buf			; st == empty
			fldz				; st(0) == 0
			mov eax, 0
			mov dbz, eax
			jmp Write_result

		Write_result:
			fstp result_double	; st == empty
	}
	if (!dbz)
	{
		std::cout << "Asm divizion by zero!" << '\n';
	}
	return result_double;
}

double function_cpp(double a, double c, double d)
{
	double first = (4 * c + d * c * a - 1);
	double second = (c - a / 2);
	if (!second)
	{
		std::cout << "Cpp division by zero!" << '\n';
		return 0;
	}
	return first / second;
}

bool continue_check()
{
	std::string str;
	int res;
	bool flag = true;
	std::cout << '\n' << "__________________________________" << '\n';
	std::cout << "To continue enter 1" << '\n';
	std::cout << "To quit enter 2" << '\n';
	while(flag)
	{
		std::cin >> str;
		try
		{
			res = std::stoi(str);
			if (res == 1 || res == 2)
			{
				flag = false;
			}
		}
		catch(std::invalid_argument e)
		{
			std::cout << "Incorrect input!" << '\n';
		}
		std::cin.ignore(-10000, '\n');
	}
	std::cout << "__________________________________" << '\n';
	return res - 2;
}

int main()
{
	do
	{
		double a, c, d;
		std::cout << "Enter a:" << '\t';
		std::cin >> a;
		std::cout << "Enter c:" << '\t';
		std::cin >> c;
		std::cout << "Enter d:" << '\t';
		std::cin >> d;
		std::cout << "Asm result:" << '\t' << function_asm(a, c, d) << '\n';
		std::cout << "Cpp result:" << '\t' << function_cpp(a, c, d) << '\n';
	} while (continue_check());
	return 0;
}