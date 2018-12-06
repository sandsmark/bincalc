//  Dumb and simple binary calculator
// Copyright 2018 Martin Sandsmark
// This work is licensed under a Creative Commons Attribution Share-Alike 4.0 License.

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// from user295190, http://stackoverflow.com/a/3974138
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    printf(" ");
    for (i=0; i < size * 8; i++) {
        if (!(i % 4) && i > 0) {
            printf(" |");
        }
        printf("%3lu", size*8 - i - 1);
    }

    printf("\n ");
    for (i=size-1;i>=0;i--)
    {
        if (i < size-1) {
            printf(" |");
        }
        for (j=7;j>=0;j--)
        {
            if (j == 3) {
                printf(" |");
            }

            byte = (b[i] >> j) & 1;

            if (byte) {
                printf("\033[02;32m");
            } else {
                printf("\033[01;33m");
            }
            printf("  %u\033[0m", byte);
        }
    }
    puts("");
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("usage: %s num [& a ^ x o | + - / * < s > num2] \n", argv[0]);
        return 1;
    }
    long int num = strtol(argv[1], NULL, 0);

    if (argc > 3) {
        long int num2 = strtol(argv[3], NULL, 0);
        switch(argv[2][0]) {
        case '&':
        case 'a':
            num &= num2;
            break;
        case '^':
        case 'x':
            num ^= num2;
            break;
        case 'o':
        case '|':
            num |= num2;
            break;
        case '+':
            num += num2;
            break;
        case '-':
            num -= num2;
            break;
        case '/':
            num /= num2;
            break;
        case '*':
            num *= num2;
            break;
        case '<':
            num <<= num2;
            break;
        case 's':
        case '>':
            num >>= num2;
            break;
        default:
            printf("invalid syntax\n");
            return 1;
        }
    }
    printf("0x%lx = %ld:\n", num, num);

    if (num < UCHAR_MAX - 1) {
        printBits(sizeof(unsigned char), &num);
    } else if (num < USHRT_MAX - 1) {
        printBits(sizeof(unsigned short), &num);
    } else if (num < UINT_MAX - 1) {
        printBits(sizeof(unsigned int), &num);
    } else {
        printBits(sizeof(num), &num);
    }

    return 0;
}
