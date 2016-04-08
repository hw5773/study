#include <stdio.h>

typedef	int 	(*calcFuncPtr) (int, int);
int 			plus(int, int);
int				minus(int, int);
int				multiple(int, int);
int				division(int, int);

int main()
{
	calcFuncPtr calc = NULL;
	int a = 0, b = 0;
	char op = 0;
	int result = 0;
	
	calcFuncPtr fp[4] = {plus, minus, multiple, division};
	int num = 0;

	scanf("%d %c %d", &a, &op, &b);

	switch (op)
	{
		case '+':
			calc = plus;
			num = 0;
			break;
		case '-':
			calc = minus;
			num = 1;
			break;
		case '*':
			calc = multiple;
			num = 2;
			break;
		case '/':
			calc = division;
			num = 3;
			break;
	}

	result = calc(a, b);

	printf("result using the function pointer: %d\n", result);

	result = fp[num](a, b);
	printf("result using the function pointer array: %d\n", result);

	return 0;
}

int plus(int first, int second)
{
	return first + second;
}

int minus(int first, int second)
{
	return first - second;
}

int multiple(int first, int second)
{
	return first * second;
}

int division(int first, int second)
{
	return first / second;
}
