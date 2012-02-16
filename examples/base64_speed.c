#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <zt.h>

static void
test_base64_speed(void) {
    srand(11);

    const size_t kBufSz = 4096;
    unsigned char iBuf[kBufSz];
    const size_t n = kBufSz;
    size_t       i;

    const long long kIterations = 2621440;
    const long long nGigs = (kIterations * kBufSz) / (1024 * 1024 * 1024);

    for (i=0; i<n; ++i) {
        iBuf[i] = rand() % 256;
    }

    {
        time_t s = time(NULL);
        int    i;

        for (i = 0; i < kIterations; ++i) {
            char obuff[kBufSz * 2];
            size_t oBufSz = kBufSz * 2;

            char **voidsux = (char **)&obuff;
            zt_base64_encode((void *)iBuf, n, (void **)&voidsux, &oBufSz);
        }

        time_t e = time(NULL);
        printf("zt64 Encoding :        %lld gigs in %ld seconds.\n", nGigs, (e - s));
    }

    {
        char oBuf[kBufSz * 2];
        size_t oBufSz = kBufSz * 2;

        char** voidsux = (char**)&oBuf;
        void **rptr = (void **)&voidsux;

        zt_base64_encode((void*)iBuf, n, rptr, &oBufSz);

        time_t s = time(NULL);
        int i;
        for (i=0; i < kIterations; ++i)
        {
            size_t tBufSz = kBufSz * 2;

            char** voidsux = (char**)&iBuf;
            zt_base64_decode(oBuf, oBufSz, (void**)&voidsux, &tBufSz);
        }
        time_t e = time(NULL);
        printf("zt64 Decoding:          %lld gigs in %ld seconds.\n", nGigs, (e - s));
    }
}

int
main(int argc, char **argv) {
    test_base64_speed();
    return 0;
}
