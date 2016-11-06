//  Dumb and simple binary calculator
//  cc by-sa 3.0
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// from user295190, http://stackoverflow.com/a/3974138
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            if (j == 3) {
                printf(" ");
            }
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
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
        int num2 = strtol(argv[3], NULL, 0);
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


    /*printnum(num);*/
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
