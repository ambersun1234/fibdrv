#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <limits.h>

#include "./apa.h"

static struct u64 fibonacci(uint32_t input)
{
    unsigned int msb = clz(input);
    unsigned int mask = (1 << (31 - msb - 1));
    struct u64 current = {.msl = 0, .lsl = 1}, next = {.msl = 0, .lsl = 1};
    struct u64 mul = {.msl = 0, .lsl = 2}, zero = {.msl = 0, .lsl = 0};

    /* fast doubling formula
     * f(2k) = f(k)[2f(k + 1) - f(k)]
     * f(2k + 1) = f(k + 1)^2 + f(k)^2
     */

    if (input == 0)
        return zero;
    if (input >= 1 && input <= 2)
        return current;

    while (mask > 0) {
        if (mask & input) {
            // bit = 1: fast doubling then iterate 1
            struct u64 *t0, *t1, *t2, *temp, *temp2;
            // t0 = current * (2 * next - current);
            // t1 = next * next + current * current;

            temp = multiplier(&next, &mul);
            temp = subtracter(temp, &current);
            t0 = multiplier(&current, temp);

            t2 = &next;
            temp = multiplier(&next, t2);
            t2 = &current;
            temp2 = multiplier(&current, t2);
            t1 = adder(temp, temp2);

            current = *t0;
            next = *t1;

            // iterate 1
            temp = adder(&current, &next);
            current = next;
            next = *temp;
        } else {
            // bit = 0: fast doubling
            struct u64 *t0, *t1, *t2, *temp, *temp2;
            // t0 = current * (2 * next - current);
            // t1 = next * next + current * current;

            temp = multiplier(&next, &mul);
            temp = subtracter(temp, &current);
            t0 = multiplier(&current, temp);

            t2 = &next;
            temp = multiplier(&next, t2);
            t2 = &current;
            temp2 = multiplier(&current, t2);
            t1 = adder(temp, temp2);

            current = *t0;
            next = *t1;
        }
        mask >>= 1;
    }
    return current;
}

int main()
{
    struct u64 result;
    struct timespec start, end;
    uint32_t counter = 0, maxFib = 150;
    char buffer[500];

    FILE *fp = fopen("./result.txt", "w");

    for (counter = 0; counter <= maxFib; counter++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        result = fibonacci(counter);
        clock_gettime(CLOCK_MONOTONIC, &end);

        printf("%u %llu+%llu*%llu+%llu %ld\n", counter, result.lsl, result.msl,
               ULLONG_MAX, result.msl, diff_in_ns(start, end));
        sprintf(buffer, "%u %llu+%llu*%llu+%llu %ld\n", counter, result.lsl,
                result.msl, ULLONG_MAX, result.msl, diff_in_ns(start, end));
        fputs(buffer, fp);
    }
    fclose(fp);

    return 0;
}
