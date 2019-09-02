#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>

#include "./apa.h"

void output(FILE *fp, struct u64 *r, struct u64 t, struct u64 t2, char op)
{
    char buffer[500];
    sprintf(buffer,
            "(%llu*18446744073709551616+%llu)%c(%llu*18446744073709551616+%llu)"
            " (%llu*18446744073709551616)+%llu\n",
            t.msl, t.lsl, op, t2.msl, t2.lsl, r->msl, r->lsl);
    fputs(buffer, fp);
    free(r);
}

int main()
{
    struct u64 test, test2, *result;
    FILE *fp = fopen("./test.txt", "w");

    test.msl = 0;
    test.lsl = 144123;
    test2.msl = 0;
    test2.lsl = 19;

    result = multiplier(&test, &test2);
    output(fp, result, test, test2, '*');

    test.msl = 0;
    test.lsl = 144123;
    test2.msl = 0;
    test2.lsl = 19;

    result = adder(&test, &test2);
    output(fp, result, test, test2, '+');

    test.msl = 0;
    test.lsl = 144123;
    test2.msl = 0;
    test2.lsl = 19;

    result = subtracter(&test, &test2);
    output(fp, result, test, test2, '-');

    test.msl = 1441;
    test.lsl = 2335674573456241234;
    test2.msl = 112;
    test2.lsl = 3412349877877676769;

    result = adder(&test, &test2);
    output(fp, result, test, test2, '+');

    test.msl = 1441;
    test.lsl = 2335674573456241234;
    test2.msl = 112;
    test2.lsl = 3412349877877676769;

    result = subtracter(&test, &test2);
    output(fp, result, test, test2, '-');

    test.msl = 4;
    test.lsl = 2335674573456241234;
    test2.msl = 0;
    test2.lsl = 7712349877877676769;

    result = adder(&test, &test2);
    output(fp, result, test, test2, '+');

    test.msl = 4;
    test.lsl = 2335674573456241234;
    test2.msl = 0;
    test2.lsl = 7712349877877676769;

    result = subtracter(&test, &test2);
    output(fp, result, test, test2, '-');

    test.msl = 0;
    test.lsl = 8888888888888888888;
    test2.msl = 0;
    test2.lsl = 26359;

    result = multiplier(&test, &test2);
    output(fp, result, test, test2, '*');

    test.msl = 0;
    test.lsl = 8888888888888888888;
    test2.msl = 0;
    test2.lsl = 26359;

    result = adder(&test, &test2);
    output(fp, result, test, test2, '+');

    test.msl = 0;
    test.lsl = 8888888888888888888;
    test2.msl = 0;
    test2.lsl = 26359;

    result = subtracter(&test, &test2);
    output(fp, result, test, test2, '-');

    test.msl = 1;
    test.lsl = 8888888888888888888;
    test2.msl = 0;
    test2.lsl = 26359;

    result = multiplier(&test, &test2);
    output(fp, result, test, test2, '*');

    test.msl = 1;
    test.lsl = 8888888888888888888;
    test2.msl = 0;
    test2.lsl = 26359;

    result = adder(&test, &test2);
    output(fp, result, test, test2, '+');

    test.msl = 1;
    test.lsl = 8888888888888888888;
    test2.msl = 0;
    test2.lsl = 26359;

    result = subtracter(&test, &test2);
    output(fp, result, test, test2, '-');

    test.msl = 1;
    test.lsl = 8888888888888888888;
    test2.msl = 0;
    test2.lsl = 26359;

    result = multiplier(&test, &test2);
    output(fp, result, test, test2, '*');

    test.msl = 1;
    test.lsl = 8888888888888888888;
    test2.msl = 0;
    test2.lsl = 26359;

    result = adder(&test, &test2);
    output(fp, result, test, test2, '+');

    test.msl = 1;
    test.lsl = 8888888888888888888;
    test2.msl = 0;
    test2.lsl = 26359;

    result = subtracter(&test, &test2);
    output(fp, result, test, test2, '-');

    test.msl = 0;
    test.lsl = 888888888888;
    test2.msl = 0;
    test2.lsl = 666666666666;

    result = multiplier(&test, &test2);
    output(fp, result, test, test2, '*');

    fclose(fp);

    return 0;
}
