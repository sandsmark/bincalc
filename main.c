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

void printHelp(const char *progName)
{
    printf("usage: %s num [operator num2] \n", progName);

    printf(
            "operators:\n"
            "\t'&' 'a[nd]'\n"
            "\t'^' 'x[or]'\n"
            "\t'|' 'o[r]'\n"
            "\t'<' 'l[eftshift]'\n"
            "\t'>' 'r[ightshift]'\n"
            "\t'*' 'm[ultiply]'\n"
            "\t    't[imes]'\n"
            "\t'+' '-' '/'\n"
    );
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printHelp(argv[0]);
        return 1;
    }
    char *lastCharInNum;
    long long int num = strtoll(argv[1], &lastCharInNum, 0);

    if (lastCharInNum == argv[1]) {
        printHelp(argv[0]);
        return 1;
    }

    if (argc > 3) {
        const long long int num1 = num;

        const long long int num2 = strtoll(argv[3], &lastCharInNum, 0);
        if (lastCharInNum == argv[3]) {
            printHelp(argv[0]);
            return 1;
        }

        char op = ' ';
        switch(argv[2][0]) {
        case '&':
        case 'a':
            op = '&';
            num &= num2;
            break;
        case '^':
        case 'x':
            op = '^';
            num ^= num2;
            break;
        case 'o':
        case '|':
            op = '|';
            num |= num2;
            break;
        case '+':
            op = '+';
            num += num2;
            break;
        case '-':
            op = '-';
            num -= num2;
            break;
        case '/':
            op = '/';
            num /= num2;
            break;
        case '*':
        case 'm':
        case 't':
            op = '*';
            num *= num2;
            break;
        case 'l':
        case '<':
            op = '<';
            num <<= num2;
            break;
        case 'r':
        case '>':
            op = '>';
            num >>= num2;
            break;
        default:
            printf("invalid syntax\n");
            printHelp(argv[0]);
            return 1;
        }
        printf("0x%llx %c 0x%llx = 0x%llx\n", num1, op, num2, num);
        printf("0x%lld %c 0x%lld = %lld\n\n", num1, op, num2, num);
    } else {
        printf("0x%llx = %lld\n\n", num, num);
    }

    const size_t absNum = llabs(num);

    if (absNum < UCHAR_MAX - 1) {
        printBits(sizeof(unsigned char), &num);
    } else if (absNum < USHRT_MAX - 1) {
        printBits(sizeof(unsigned short), &num);
    } else if (absNum < UINT_MAX - 1) {
        printBits(sizeof(unsigned int), &num);
    } else if (absNum < ULONG_MAX - 1) {
        printBits(sizeof(unsigned long int), &num);
    } else if (absNum < ULLONG_MAX - 1) {
        printBits(sizeof(unsigned long int), &num);
    } else {
        printBits(sizeof(num), &num);
    }

    return 0;
}
