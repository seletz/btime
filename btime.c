#define _DARWIN_FEATURE_64_BIT_INODE

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>


void usage() {
    printf("btime src dest\n");
}

int copy_btime(char *src, char *dst) {
    int ret = 0;
    struct stat st;
    struct timeval times[2];

    ret = stat(src, &st);
    if (ret) {
        perror("stat");
        goto done;
    }

    /* atime */
    times[0].tv_sec = st.st_birthtimespec.tv_sec;
    times[0].tv_usec = 0;

    /* mtime */
    times[1].tv_sec = st.st_birthtimespec.tv_sec;
    times[1].tv_usec = 0;

    ret = utimes(dst, times);
    if (ret) {
        perror("utimes");
        goto done;
    }


done:
    return ret;
}

int exists(char *path) {
    int ret = 0;
    struct stat st;
   ret = stat(path, &st);

   fprintf(stderr, "stat(%s) -> %d\n", path, ret);

   return ret ==0;
}

int main(int argc, char *argv[]) {
    int ret = 0;

    if (argc < 3) {
        usage();
        exit(10);
    }

    char *src = argv[1];
    char *dst = argv[2];

    if (!exists(src)) {
        perror("source file");
        usage();
        exit(10);
    }

    if (!exists(dst)) {
        perror("destination file");
        usage();
        exit(10);
    }

    ret = copy_btime(src, dst);
    if (ret) {
        fprintf(stderr, "copy btime failed: %d\n", ret);
        exit(11);
    }

    exit(0);
}
