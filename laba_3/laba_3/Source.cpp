#include<iostream>

/*
* task: X={8-b^2/a, если a>b; -123, если a=b; (5a+7)/(b-1), если a<b;
*/

int get_int(const char* message)
{
	int value;
	std::cout << message << '\n';
	std::cin >> value;
	return value;
}

int function_asm(int a, int b)
{
	int x;
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


	/*
		очерёдность для >
		1) 5 * a
		2) 5 * a + 7
		3) b - 1
		4) (5 * a + 7) / (b - 1)

		очерёдность для <
		1) b^2
		2) b^2 / a
		3) 8 - b^2/a

	*/
	int ofs, dbz; //ofs - выход за границы числа ;dbz - деление на 0

	__asm
	{
		mov eax, a		; eax = a | ebx = ? | ecx = ? | edx = ?
		mov ebx, b		; eax = a | ebx = b | ecx = ? | edx = ?
		cmp eax, ebx
		je first_equal
		jl first_less
		jg first_greater

		first_greater:
			mov eax, 5		; eax = 5 | ebx = ? | ecx = ? | edx = ?
			mov ebx, a		; eax = 5 | ebx = a | ecx = ? | edx = ?
			imul eax, ebx 	; eax = 5 * a | ebx = a | ecx = ? | edx = ?
			jo overflow_flag

			mov ebx, 7 		; eax = 5 * a | ebx = 7 | ecx = ? | edx = ?
			add eax, ebx 	; eax = 5 * a + 7 | ebx = a | ecx = ? | edx = ?
			jo overflow_flag

			push eax		; записываем в стек eax

			mov eax, b 		; eax = b	  | ebx = a | ecx = ? | edx = ?
			mov ebx, 1 		; eax = b	  | ebx = 1 | ecx = ? | edx = ?
			sub eax, ebx 	; eax = b - 1 | ebx = a | ecx = ? | edx = ?

			mov ebx, eax 	; eax = b - 1	  | ebx = b - 1 | ecx = ? | edx = ?
			pop eax  		; eax = 5 * a + 7 | ebx = b - 1 | ecx = ? | edx = ?
			cdq  			; eax = 5 * a + 7 | ebx = b - 1 | ecx = ? | edx = 0
			cmp ebx, 0
			je division_by_zero

			idiv ebx
			jmp write_result

		first_less:
			mov eax, b  	; eax = b	  | ebx = ? | ecx = ? | edx = ?
			mov ebx, b  	; eax = b	  | ebx = b | ecx = ? | edx = ?
			imul eax, ebx	; eax = b ^ 2 | ebx = a | ecx = ? | edx = ?
			jo overflow_flag

			cdq

			mov ebx, a  	; eax = b ^ 2					| ebx = a		  | ecx = ? | edx = 0
			cmp ebx, 0
			je division_by_zero
			idiv ebx 		; eax = b ^ 2 / a (целая часть) | ebx = a		  | ecx = ? | edx = b ^ 2 / a(остаток)

			mov ebx, eax  	; eax = b ^ 2 / a				| ebx = b ^ 2 / a | ecx = ? | edx = b ^ 2 / a(остаток)
			mov eax, 8 		; eax = 8					    | ebx = b ^ 2 / a | ecx = ? | edx = b ^ 2 / a(остаток)
			sub eax, ebx    ; eax = 8 - b ^ 2				| ebx = b ^ 2 / a | ecx = ? | edx = b ^ 2 / a(остаток)
			jmp write_result
		
		first_equal:
			mov eax, -123
			jmp write_result

		write_result:
			mov x, eax
			jmp exit_asm

		division_by_zero:
			mov eax, 0; eax = 0 | ebx = ? | ecx = ? | edx = ?
			mov x, eax
			mov dbz, 0; ofs = 0
			jmp exit_asm

		overflow_flag :
			mov eax, 0; eax = 0 | ebx = ? | ecx = ? | edx = ?
			mov x, eax
			mov ofs, 0; ofs = 0
			jmp exit_asm

		exit_asm:
			xor eax, eax	; eax = 0 | ebx = ? | ecx = ? | edx = ?
			xor ebx, ebx	; eax = 0 | ebx = 0 | ecx = ? | edx = ?
			xor ecx, ecx 	; eax = 0 | ebx = 0 | ecx = 0 | edx = ?
			xor edx, edx 	; eax = 0 | ebx = 0 | ecx = 0 | edx = 0
	}
	if (!ofs)
	{
		std::cout << "Asm out of size" << '\n';
	}
	else if (!dbz)
	{
		std::cout << "Asm divizion by zero" << '\n';
	}
	return x;
}

int function_cpp(int a, int b)
{
	int x;
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
		x = 8 - b * b / a;
	}
	else
	{
		x = -123;
	}
	return x;
}

int main()
{
	int a, b;
	a = get_int("Enter a:");
	b = get_int("Enter b:");
	std::cout << "Asm result:" << '\t' << function_asm(a, b) << '\n';
	std::cout << "Cpp result:" << '\t' << function_cpp(a, b) << '\n';
	return 0;
}