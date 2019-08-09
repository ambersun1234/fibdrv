#define _POSIX_C_SOURCE 199309L

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define FIB_DEV "/dev/fibonacci"
#define MYMAX 100

static long diff_in_ns(struct timespec t1, struct timespec t2)
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

int main()
{
    int fd;
    long long sz;

    ssize_t buf;
    char write_buf[] = "testing writing";
    int offset = MYMAX;  // TODO: test something bigger than the limit
    int arr[MYMAX + 1] = {0};
    int i = 0;
    struct timespec start, end;
    FILE *fp;
    srand(time(NULL));

    fd = open(FIB_DEV, O_RDWR);
    fp = fopen("./time", "w");

    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

    for (i = 0; i <= offset; i++) {
        int tmp;
        do {
            tmp = rand() % (MYMAX + 1);
        } while (arr[tmp] == 1);
        arr[tmp] = 1;

        lseek(fd, tmp, SEEK_SET);
        clock_gettime(CLOCK_MONOTONIC, &start);
        sz = read(fd, &buf, sizeof(ssize_t));
        clock_gettime(CLOCK_MONOTONIC, &end);
        printf("%lld * %lld * %lld\n",
               (long long) (start.tv_sec * 1e9 + start.tv_nsec), sz,
               (long long) (end.tv_sec * 1e9 + end.tv_nsec));

        long long u2k = sz - (long long) (start.tv_sec * 1e9 + start.tv_nsec);
        long long k2u = (long long) (end.tv_sec * 1e9 + end.tv_nsec) - sz;

        fprintf(fp, "%d %lld %lld\n", tmp, u2k, k2u);
    }

    close(fd);
    fclose(fp);
    return 0;
}
