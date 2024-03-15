#include<iostream>

/*
* task: (4*c + d*c*a - 1)/(c - a/2);
*/

int get_int(const char* message)
{
	int value;
	std::cout << message << '\n';
	std::cin >> value;
	return value;
}

int function_asm(int a, int c, int d)
{
	/*
	* очерёдность операций
		1) 4 * c
		2) d * c
		3) d * c * a
		4) 4 * c + d * c * a
		5) a/2
		6) c - a/2
		7) (4 * c + d * c * a) / (c - a/2)
	*/

	int ofs, dbz, result; //ofs - выход за границы числа ;dbz - деление на 0
	__asm
	{
		mov eax, c		; eax = c	  | ebx = ? | ecx = ? | edx = ?
		mov ebx, 4		; eax = c	  | ebx = 4 | ecx = ? | edx = ?
		imul eax, ebx	; eax = 4 * c | ebx = 4 | ecx = ? | edx = ?
		jo overflow_flag
		push eax		; добавляем в стек значение eax

		mov eax, d		; eax = d	  | ebx = 4 | ecx = ? | edx = ?
		mov ebx, c		; eax = d	  | ebx = c | ecx = ? | edx = ?
		imul eax, ebx	; eax = d * c | ebx = c | ecx = ? | edx = ?
		jo overflow_flag
		
		mov ebx, a		; eax = d * c	  | ebx = a | ecx = ? | edx = ?
		imul eax, ebx	; eax = d * c * a | ebx = a | ecx = ? | edx = ?
		jo overflow_flag

		pop ebx			; eax = d * c * a			| ebx = 4 * c | ecx = ? | edx = ?
		add eax, ebx	; eax = (d * c * a + 4 * c) | ebx = 4 * c | ecx = ? | edx = ?
		jo overflow_flag

		push eax		; добавляем в стек значение eax

		mov eax, a		; eax = a | ebx = 4 * c | ecx = ? | edx = ?
		cdq
		mov ebx, 2		; eax = a | ebx = 2 | ecx = ? | edx = ?
		idiv ebx		; eax = a / 2 (целая часть) | ebx = 2 | ecx = ? | edx = a / 2 (остаток)

		mov ebx, eax	; eax = a / 2 (целая часть) | ebx = a / 2 (целая часть) | ecx = ? | edx = a / 2 (остаток)
		mov eax, c		; eax = c | ebx = a / 2 (целая часть) | ecx = ? | edx = a / 2 (остаток)
		sub eax, ebx	; eax = c - a / 2 | ebx = a / 2 (целая часть) | ecx = ? | edx = a / 2 (остаток)

		mov eax, c		; eax = c		  | ebx = a / 2 (целая часть) | ecx = ? | edx = a / 2 (остаток)
		sub eax, ebx	; eax = c - a / 2 | ebx = a / 2 (целая часть) | ecx = ? | edx = a / 2 (остаток)
		jo overflow_flag

		mov ebx, eax	; eax = c - a / 2			| ebx = c - a / 2 | ecx = ? | edx = a / 2 (остаток)
		pop eax			; eax = (d * c * a + 4 * c) | ebx = c - a / 2 | ecx = ? | edx = a / 2 (остаток)
		cdq
		cmp ebx, 0
		je division_by_zero
		
		idiv ebx		; eax = (d * c * a + 4 * c) / (c - a / 2) | ebx = c - a / 2 | ecx = ? | edx = a / 2 (остаток)
		jo overflow_flag
		
		jmp write_result

		write_result:
			mov result, eax
			jmp exit_asm

		division_by_zero:	
			mov eax, 0; eax = 0 | ebx = ? | ecx = ? | edx = ?
			mov result, eax
			mov dbz, 0; ofs = 0
			jmp exit_asm

		overflow_flag:
			mov eax, 0			; eax = 0 | ebx = ? | ecx = ? | edx = ?
			mov result, eax 
			mov ofs, 0 ; ofs = 0
			jmp exit_asm

		exit_asm:
			xor eax, eax ; eax = 0 | ebx = ? | ecx = ? | edx = ?
			xor ebx, ebx ; eax = 0 | ebx = 0 | ecx = ? | edx = ?
			xor ecx, ecx ; eax = 0 | ebx = 0 | ecx = 0 | edx = ?
			xor edx, edx ; eax = 0 | ebx = 0 | ecx = 0 | edx = 0
	}
	if (!ofs)
	{
		std::cout << "Asm out of size" << '\n';
	}
	else if (!dbz)
	{
		std::cout << "Asm division by zero" << '\n';
	}
	return result;
}

int function_cpp(int a, int c, int d)
{
	int first = (4 * c + d * c * a);
	int second = (c - a / 2);
	if (second)
	{
		return first / second;
	}
	else 
	{
		std::cout << "Cpp division by zero" << '\n';
		return 0;
	}
}

int main()
{
	int a, c, d;
	a = get_int("Enter a:");
	c = get_int("Enter c:");
	d = get_int("Enter d:");
	int result_asm = function_asm(a, c, d);
	int result_cpp = function_cpp(a, c, d);
	std::cout << "Result asm:" << '\t' << result_asm << '\n';
	std::cout << "Result cpp:" << '\t' << result_cpp << '\n';
	return 0;
}