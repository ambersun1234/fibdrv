#define _POSIX_C_SOURCE 199309L

#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define CARRY 10000000000000000000ULL

struct u64 {
    unsigned long long lsl;
    unsigned long long msl;
};

long diff_in_ns(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec - t1.tv_nsec < 0) {
        diff.tv_sec = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec * 1000000000.0 + diff.tv_nsec);
}

uint32_t clz(uint32_t input)
{
    // use binary search method to check
    uint8_t count = 0;

    if ((input & 0xFFFF0000) == 0) {
        input <<= 16;
        count += 16;
    }  // 1111 1111 1111 1111
    if ((input & 0xFF000000) == 0) {
        input <<= 8;
        count += 8;
    }  // 1111 1111
    if ((input & 0xF0000000) == 0) {
        input <<= 4;
        count += 4;
    }  // 1111
    if ((input & 0xC0000000) == 0) {
        input <<= 2;
        count += 2;
    }  // 1100
    if ((input & 0x80000000) == 0) {
        input <<= 0;
        count += 1;
    }  // 1000
    return count;
}

struct u64 *adder(struct u64 *input1, struct u64 *input2)
{
    struct u64 *r = malloc(sizeof(struct u64));
    r->lsl = 0;
    r->msl = 0;

    r->msl = input1->msl + input2->msl;

    unsigned long long width = 8 * sizeof(unsigned long long), x = 1;
    // 8 bits * how many bytes
    unsigned long long mycarry = 0;

    for (unsigned long long i = 0; i < width; i++) {
        unsigned long long tmp1, tmp2;
        mycarry = 0;

        tmp1 = (input1->lsl >> i) & 0x1;
        tmp2 = (input2->lsl >> i) & 0x1;

        unsigned long long t1 = ((x << i) * tmp1);
        unsigned long long t2 = ((x << i) * tmp2);

        r->lsl += t1 + t2;
        if (r->lsl > CARRY) {
            mycarry += 1;
            r->lsl -= CARRY;
        }
        r->msl += mycarry;
    }
    r->msl += mycarry;
    return r;
}

struct u64 *subtracter(struct u64 *input1, struct u64 *input2)
{
    struct u64 *r = malloc(sizeof(struct u64));

    if (input1->lsl < input2->lsl) {
        unsigned long long mycarry = CARRY;
        input1->msl -= 1;
        r->lsl = mycarry + input1->lsl - input2->lsl;
        r->msl = input1->msl - input2->msl;
    } else {
        r->lsl = input1->lsl - input2->lsl;
        r->msl = input1->msl - input2->msl;
    }
    return r;
}

struct u64 *multiplier(struct u64 *input1, struct u64 *input2)
{
    struct u64 *r = malloc(sizeof(struct u64));
    r->lsl = 0;
    r->msl = 0;

    size_t width = 8 * sizeof(unsigned long long);
    // 8 bits * how many bytes
    for (size_t i = 0; i < width; i++) {
        if ((input2->lsl >> i) & 0x1) {
            r->msl += input1->msl << i;

            struct u64 tmp;

            tmp.lsl = (input1->lsl << i);
            tmp.msl = 0;
            r = adder(r, &tmp);
            // r->msl += input1->lsl << i;
        }
    }

    for (size_t i = 0; i < width; i++) {
        if ((input2->msl >> i) & 0x1) {
            // struct u64 tmp;
            // tmp.lsl = 0;
            // tmp.msl = input1->lsl << i;
            // r = adder(r, &tmp);
            r->msl += input1->lsl << i;
        }
    }
    return r;
}
