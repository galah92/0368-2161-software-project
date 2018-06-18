#include <stdio.h>

char digits[] = "0123456789ABCDEF";

int get_number(char buffer[], int base)
{
	int digit, buff_len = 0;
	printf("Please enter a number in base %d:\n", base);
	while ((digit = getchar()) != EOF)
    {
        if (digit == '\n') continue;  // ignore newline chars
        if (digit < '0' || digit > digits[base - 1] || (digit > '9' && digit < 'A'))
        {
            printf("Invalid number!\n");
            return -1;
        }
        buffer[buff_len] = digit;
        buff_len++;
	}
	return buff_len;
}

void convert_and_print(char buffer[], int buff_len, int base_a, int base_b)
{
    int power = 1, i, as_int;
    unsigned int dec_num = 0;
    // convert to decimal number
    for (i = buff_len - 1; i >= 0; i--)
    {
        as_int = (int)buffer[i] - ((buffer[i] >= 'A') ? 'A' - 10 : '0');
        dec_num += as_int * power;
        power *= base_a;
    }
    // convert to num in base_b
    i = 99;
    if (!dec_num) buffer[--i] = 0;
    while (dec_num)
    {
        buffer[--i] = digits[dec_num % base_b];
        dec_num /= base_b;
    }
    // print from i to end of buffer
    printf("The result is : %.*s\n", 99 - i, buffer + i);
}

int main()
{
    char buffer[100];
	int base_a, base_b, buff_len;
    printf("Please enter the number's base:\n");
    if (scanf("%d", &base_a) != 1)
    {
        printf("An error occurred!\n");
        return 0;
    }
    if (base_a < 2 || base_a > 16)
    {
        printf("Invalid input base\n");
        return 0;
    }
    printf("Please enter the desired base:\n");
    if (scanf("%d", &base_b) != 1)
    {
        printf("An error occurred!\n");
        return 0;
    }
    if (base_b < 2 || base_b > 16)
    {
        printf("Invalid desired base\n");
        return 0;
    }
    buff_len = get_number(buffer, base_a);
    if (buff_len == -1) return 0;
	convert_and_print(buffer, buff_len, base_a, base_b);
	return 0;
}
