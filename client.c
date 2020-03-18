#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define FIB_DEV "/dev/fibonacci"

int main()
{
    long long sz;

    char buf[100];
    char write_buf[] = "testing writing";
    char out[500];
    int offset = 150;  // TODO: test something bigger than the limit

    FILE *fp = fopen("kresult.txt", "w");

    int fd = open(FIB_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

    for (int i = 0; i <= offset; i++) {
        sz = write(fd, write_buf, strlen(write_buf));
        printf("Writing to " FIB_DEV ", returned the sequence %lld\n", sz);
    }

    for (int i = 0; i <= offset; i++) {
        lseek(fd, i, SEEK_SET);
        sz = read(fd, buf, i);
        printf("Reading from " FIB_DEV
               " at offset %d, returned the sequence "
               "%s.",
               i, buf);
        sprintf(out, "%d %s", i, buf);
        fputs(out, fp);
    }

    for (int i = offset; i >= 0; i--) {
        lseek(fd, i, SEEK_SET);
        sz = read(fd, buf, i);
        printf("Reading from " FIB_DEV
               " at offset %d, returned the sequence "
               "%s.",
               i, buf);
        sprintf(out, "%d %s", i, buf);
        fputs(out, fp);
    }

    close(fd);
    fclose(fp);
    return 0;
}
