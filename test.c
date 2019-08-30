#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "./apa.h"

int main() {
	struct u64 test, test2, *result, cmp;

	//          10000000000000000000
	test.msl  = 0;
	test.lsl  = 144123;
	test2.msl = 0;
	test2.lsl = 19;

	//          10000000000000000000
	cmp.msl   = 0;
	cmp.lsl   = 2738337;
	result = multiplier(&test, &test2);
	printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	assert(cmp.msl == result->msl);
	assert(cmp.lsl == result->lsl);
	free(result);

	//          10000000000000000000
	cmp.msl   = 0;
	cmp.lsl   = 144142;
	result = adder(&test, &test2);
	printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	assert(cmp.msl == result->msl);
	assert(cmp.lsl == result->lsl);
	free(result);

	//          10000000000000000000
	cmp.msl   = 0;
	cmp.lsl   = 144104;
	result = subtracter(&test, &test2);
	printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	assert(cmp.msl == result->msl);
	assert(cmp.lsl == result->lsl);
	free(result);

	///////////////////////////////////////////////

	//          10000000000000000000
	test.msl  = 1441;
	test.lsl  = 2335674573456241234;
	test2.msl = 112;
	test2.lsl = 3412349877877676769;

	//          10000000000000000000
	// cmp.msl   = 1619099588740133380782292;
	// cmp.lsl   = 2788755187141692946;
	// result = multiplier(&test, &test2);
	// printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	// assert(cmp.msl == result->msl);
	// assert(cmp.lsl == result->lsl);
	// free(result);

	//          10000000000000000000
	cmp.msl   = 1553;
	cmp.lsl   = 5748024451333918003;
	result = adder(&test, &test2);
	printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	assert(cmp.msl == result->msl);
	assert(cmp.lsl == result->lsl);
	free(result);

	//          10000000000000000000
	cmp.msl   = 1328;
	cmp.lsl   = 8923324695578564465;
	result = subtracter(&test, &test2);
	printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	assert(cmp.msl == result->msl);
	assert(cmp.lsl == result->lsl);
	free(result);

	///////////////////////////////////////////////

	//          10000000000000000000
	test.msl  = 4;
	test.lsl  = 2335674573456241234;
	test2.msl = 0;
	test2.lsl = 7712349877877676769;

	//          10000000000000000000
	// cmp.msl   = 32650753462646430761;
	// cmp.lsl   = 8988755187141692946;
	// result = multiplier(&test, &test2);
	// printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	// assert(cmp.msl == result->msl);
	// assert(cmp.lsl == result->lsl);
	// free(result);

	//          10000000000000000000
	cmp.msl   = 5;
	cmp.lsl   = 48024451333918003;
	result = adder(&test, &test2);
	printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	assert(cmp.msl == result->msl);
	assert(cmp.lsl == result->lsl);
	free(result);

	//          10000000000000000000
	cmp.msl   = 3;
	cmp.lsl   = 4623324695578564465;
	result = subtracter(&test, &test2);
	printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	assert(cmp.msl == result->msl);
	assert(cmp.lsl == result->lsl);
	free(result);

	///////////////////////////////////////////////

	//          10000000000000000000
	test.msl  = 0;
	test.lsl  = 8888888888888888888;
	test2.msl = 0;
	test2.lsl = 26359;

	// //          10000000000000000000
	// cmp.msl   = 23430;
	// cmp.lsl   = 2222222222222198792;
	// result = multiplier(&test, &test2);
	// printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	// assert(cmp.msl == result->msl);
	// assert(cmp.lsl == result->lsl);
	// free(result);

	//          10000000000000000000
	cmp.msl   = 0;
	cmp.lsl   = 8888888888888915247;
	result = adder(&test, &test2);
	printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	assert(cmp.msl == result->msl);
	assert(cmp.lsl == result->lsl);
	free(result);

	//          10000000000000000000
	cmp.msl   = 0;
	cmp.lsl   = 8888888888888862529;
	result = subtracter(&test, &test2);
	printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	assert(cmp.msl == result->msl);
	assert(cmp.lsl == result->lsl);
	free(result);

	///////////////////////////////////////////////

	//          10000000000000000000
	test.msl  = 1;
	test.lsl  = 8888888888888888888;
	test2.msl = 0;
	test2.lsl = 26359;

	// //          10000000000000000000
	// cmp.msl   = 49789;
	// cmp.lsl   = 2222222222222198792;
	// result = multiplier(&test, &test2);
	// printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	// assert(cmp.msl == result->msl);
	// assert(cmp.lsl == result->lsl);
	// free(result);

	//          10000000000000000000
	cmp.msl   = 1;
	cmp.lsl   = 8888888888888915247;
	result = adder(&test, &test2);
	printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	assert(cmp.msl == result->msl);
	assert(cmp.lsl == result->lsl);
	free(result);

	//          10000000000000000000
	cmp.msl   = 1;
	cmp.lsl   = 8888888888888862529;
	result = subtracter(&test, &test2);
	printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	assert(cmp.msl == result->msl);
	assert(cmp.lsl == result->lsl);
	free(result);

	///////////////////////////////////////////////

	//          10000000000000000000
	test.msl  = 1;
	test.lsl  = 8888888888888888888;
	test2.msl = 0;
	test2.lsl = 26359;

	// //          10000000000000000000
	// cmp.msl   = 49789;
	// cmp.lsl   = 2222222222222198792;
	// result = multiplier(&test, &test2);
	// printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	// assert(cmp.msl == result->msl);
	// assert(cmp.lsl == result->lsl);
	// free(result);

	//          10000000000000000000
	cmp.msl   = 1;
	cmp.lsl   = 8888888888888915247;
	result = adder(&test, &test2);
	printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	assert(cmp.msl == result->msl);
	assert(cmp.lsl == result->lsl);
	free(result);

	//          10000000000000000000
	cmp.msl   = 1;
	cmp.lsl   = 8888888888888862529;
	result = subtracter(&test, &test2);
	printf("%llu,%llu\n%llu,%llu\n", result->msl, result->lsl, cmp.msl, cmp.lsl);
	assert(cmp.msl == result->msl);
	assert(cmp.lsl == result->lsl);
	free(result);

	return 0;
}
