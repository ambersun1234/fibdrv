#define _POSIX_C_SOURCE 199309L

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define FIB_DEV "/dev/fibonacci"
#define MYMAX 150

#define GETTIME(x) (long long) ((x).tv_sec * 1e9 + (x).tv_nsec)

int main()
{
    char buf[MYMAX];
    int offset = MYMAX;  // TODO: test something bigger than the limit
    int i = 0;
    struct timespec start, end;
    char out[500];
    srand(time(NULL));

    int fd = open(FIB_DEV, O_RDWR);
    FILE *fp = fopen("./time", "w");

    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }
    if (fp == NULL) {
        perror("Failed to open file");
        exit(1);
    }

    for (i = 0; i <= offset; i++) {
        lseek(fd, i, SEEK_SET);
        clock_gettime(CLOCK_MONOTONIC, &start);
        ssize_t t = write(fd, &buf, i);
        clock_gettime(CLOCK_MONOTONIC, &end);

        printf("%d %lld %ld %lld\n", i, GETTIME(start), t, GETTIME(end));

        sprintf(out, "%d %ld\n", i, t);
        fputs(out, fp);
    }

    fclose(fp);
    close(fd);
    return 0;
}
