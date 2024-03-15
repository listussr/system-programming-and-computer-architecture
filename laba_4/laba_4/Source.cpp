#include<iostream>

/**
* 25) Найти сумму квадратов всех отрицательных элементов массива A={a[i]},
*	удовлетворяющих условию: a[i] <= d.
*/

int function_asm(int* array_int, int array_size, int d)
{
	int result_int;
	int ofs;
	__asm
	{
		mov eax, 0				; eax = 0 | ebx = ? | ecx = ? | edx = ?
		mov result_int, eax		; eax = 0 | ebx = ? | ecx = ? | edx = ?
		mov edx, d
		push eax

		mov ebx, array_int	; eax = 0 | ebx = array_int | ecx = ? | edx = ?
		mov ecx, 0			; eax = 0 | ebx = array_int | ecx = 0 | edx = ?

		For:
			cmp ecx, array_size
			je Write_result
			mov eax, [ebx + 4 * ecx]	; eax = *(array_int + ecx * 4) | ebx = array_int | ecx = i | edx = ?
			cmp eax, 0
			jl Is_Less_zero
			inc ecx						; eax = *(array_int + ecx * 4) | ebx = array_int | ecx = i + 1 | edx = ?
			jmp For

		Is_Less_zero:
			cmp eax, edx
			jle Is_Less_d
			inc ecx						; eax = *(array_int + ecx * 4) | ebx = array_int | ecx = i + 1 | edx = ?
			jmp For

		Is_Less_d:
			;push edx
			mov edx, eax				; eax = *(array_int + ecx * 4)					  | ebx = array_int | ecx = i + 1 | edx = *(array_int + ecx * 4)
			imul edx					; eax = (*(array_int + ecx * 4)) ^ 2			  | ebx = array_int | ecx = i + 1 | edx = *(array_int + ecx * 4)
			jo Overflow_flag
			mov edx, eax				; eax = (*(array_int + ecx * 4)) ^ 2			  | ebx = array_int | ecx = i	  | edx = (*(array_int + ecx * 4)) ^ 2
			pop eax						; eax = result_int								  | ebx = array_int | ecx = i	  | edx = (*(array_int + ecx * 4)) ^ 2
			add eax, edx				; eax = result_int + (*(array_int + ecx * 4)) ^ 2 | ebx = array_int | ecx = i	  | edx = (*(array_int + ecx * 4)) ^ 2
			;pop edx
			mov edx, d					; eax = result_int + (*(array_int + ecx * 4)) ^ 2 | ebx = array_int | ecx = i	  | edx = 1
			push eax					; eax = result_int + (*(array_int + ecx * 4)) ^ 2 | ebx = array_int | ecx = i	  | edx = (*(array_int + ecx * 4)) ^ 2
			inc ecx						; eax = result_int + (*(array_int + ecx * 4)) ^ 2 | ebx = array_int | ecx = i + 1 | edx = (*(array_int + ecx * 4)) ^ 2
			jmp For

		Overflow_flag:
			mov eax, 0					; eax = 0 | ebx = array_int | ecx = i + 1 | edx = *(array_int + ecx * 4)
			mov ofs, eax				
			jmp Write_result

		Write_result:
			pop eax						; eax = res_in_asm | ebx = array_int | ecx = i + 1 | edx = *(array_int + ecx * 4)
			mov result_int, eax			; eax = res_in_asm | ebx = array_int | ecx = i + 1 | edx = *(array_int + ecx * 4)
			jmp Exit_asm

		Exit_asm:
			xor eax, eax
			xor ebx, ebx
			xor ecx, ecx
			xor edx, edx
	}
	if (!ofs)
	{
		std::cout << "Asm out of size!" << '\n';
	}
	return result_int;
}

int function_cpp(int* array_int, int array_size, int d)
{
	int result = 0;
	for (int i = 0; i < array_size; ++i)
	{
		if (array_int[i] <= d && array_int[i] < 0)
		{
			result += array_int[i] * array_int[i];
		}
	}
	return result;
}

int main()
{
	int size, *arr, d;
	std::cout << "Enter size of array: ";
	std::cin >> size;
	arr = new int[size];
	for (int i = 0; i < size; ++i)
	{
		std::cout << "Enter array[" << i << "] : ";
		std::cin >> arr[i];
		std::cout << '\n';
	}
	std::cout << "Enter d: ";
	std::cin >> d;
	std::cout << '\n';
	std::cout << "Asm result:" << '\t' << function_asm(arr, size, d) << '\n';
	std::cout << "Cpp result:" << '\t' << function_cpp(arr, size, d) << '\n';
	return 0;
}