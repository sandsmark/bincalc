//  Dumb and simple binary calculator
// Copyright 2018 Martin Sandsmark
// This work is licensed under a Creative Commons Attribution Share-Alike 4.0 License.

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <gmp.h>
#include <string.h>

// from user295190, http://stackoverflow.com/a/3974138
void printBits(size_t const size, void const * const ptr, size_t offset, int negate)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    printf(" ");
    for (i=0; i < size * 8; i++) {
        if (!(i % 4) && i > 0) {
            printf(" |");
        }
        printf("%4lu", size*8 - i - 1 + offset);
    }

    printf("\n ");
    for (i=size-1;i>=0;i--) {
        if (i < size-1) {
            printf(" |");
        }

        for (j=7;j>=0;j--) {
            if (j == 3) {
                printf(" |");
            }

            byte = (b[i] >> j) & 1;
            byte ^= negate;

            if (byte) {
                printf("\033[02;32m");
            } else {
                printf("\033[01;33m");
            }
            printf("   %u\033[0m", byte);
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
    if (argc != 2 && argc != 4) {
        printHelp(argv[0]);
        return 1;
    }
    mpz_t num;

    if (mpz_init_set_str(num, argv[1], 0) < 0) {
        printHelp(argv[0]);
        return 1;
    }

    if (argc > 3) {
        mpz_t num2;

        if (mpz_init_set_str(num2, argv[3], 0) < 0) {
            printHelp(argv[0]);
            return 1;
        }

        mpz_t result;
        mpz_init(result);

        char op = ' ';
        switch(argv[2][0]) {
        case '&':
        case 'a':
            op = '&';
            mpz_and(result, num, num2);
            break;
        case '^':
        case 'x':
            op = '^';
            mpz_xor(result, num, num2);
            break;
        case 'o':
        case '|':
            op = '|';
            mpz_ior(result, num, num2);
            break;
        case '+':
            op = '+';
            mpz_add(result, num, num2);
            break;
        case '-':
            op = '-';
            mpz_sub(result, num, num2);
            break;
        case '/':
            op = '/';
            mpz_div(result, num, num2);
            break;
        case '*':
        case 'm':
        case 't':
            op = '*';
            mpz_mul(result, num, num2);
            break;
        case 'l':
        case '<':
            op = '<';
            mpz_mul_2exp(result, num, mpz_get_ui(num2));
            break;
        case 'r':
        case '>':
            op = '>';
            mpz_fdiv_q_2exp(result, num, mpz_get_ui(num2));
            break;
        default:
            printf("invalid syntax\n");
            printHelp(argv[0]);
            return 1;
        }

        gmp_printf("  %Zd %c %Zd   = %Zd\n", num, op, num2, result);
        gmp_printf("0x%Zx %c 0x%Zx = 0x%Zx\n", num, op, num2, result);

        mpz_clear(num);
        mpz_set(num, result);
        mpz_clear(num2);
        mpz_clear(result);
    } else {
        gmp_printf("0x%Zx=%Zd\n", num, num);
    }

    if (mpz_fits_slong_p(num)) {
        signed long int intNum = mpz_get_si(num);
        const size_t absNum = llabs(intNum);
        if (absNum < UCHAR_MAX - 1) {
            printBits(sizeof(unsigned char), &intNum, 0, 0);
        } else if (absNum < USHRT_MAX - 1) {
            printBits(sizeof(unsigned short), &intNum, 0, 0);
        } else if (absNum < UINT_MAX - 1) {
            printBits(sizeof(unsigned int), &intNum, 0, 0);
        } else if (absNum < ULONG_MAX - 1) {
            printBits(sizeof(unsigned long int), &intNum, 0, 0);
        } else {
            printBits(sizeof(num), &num, 0, 0);
        }

        if (absNum < UINT_MAX - 1) {
            int thirtytwobits = intNum;
            float fakeFloat;
            memcpy(&fakeFloat, &thirtytwobits, sizeof(float));
            printf("As 32bit float: ");
            switch(fpclassify(fakeFloat)) {
            case FP_NORMAL:
                printf("%f\n", fakeFloat);
                break;
            case FP_NAN:
                puts("nan");
                break;
            case FP_INFINITE:
                puts("infinite");
                break;
            case FP_ZERO:
                puts("zero");
                break;
            case FP_SUBNORMAL:
                puts("too small");
                break;
            default:
                puts("wtf");
                break;
            }
        }

        if (absNum < ULONG_MAX - 1) {
            int64_t sixtyfourbits = intNum;
            double fakeDouble;
            memcpy(&fakeDouble, &sixtyfourbits, sizeof(double));

            printf("As 64bit float: ");
            switch(fpclassify(fakeDouble)) {
            case FP_NORMAL:
                printf("%lf\n", fakeDouble);
                break;
            case FP_NAN:
                puts("nan");
                break;
            case FP_INFINITE:
                puts("infinite");
                break;
            case FP_ZERO:
                puts("zero");
                break;
            case FP_SUBNORMAL:
                puts("too small");
                break;
            default:
                puts("wtf");
                break;
            }
        }
    } else {
        size_t byteCount;
        unsigned char *bytes = mpz_export(NULL, &byteCount, -1, 1, 1, 0, num);
        printf("bytecount: %lu\n", byteCount);

        for (size_t i=0; i < byteCount; i+=4) {
            size_t size = i + 4 > byteCount ? (byteCount % 4) : 4;
            printBits(size, bytes + i, i * 8, mpz_sgn(num) < 0);
        }
        free(bytes);
    }

    return 0;
}
