#include <zt.h>
#include <string.h>

int
main(int argc, char * argv[]) {
    char    * ALPHA = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char    * ALPHA2 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    zt_hexdump_str(ALPHA, strlen(ALPHA), NULL, NULL);
    zt_hexdump_str(ALPHA2, strlen(ALPHA2), NULL, NULL);

#if 0
    {
        FILE    * fp;
        if ((fp = fopen(argv[0], "r")) != NULL) {
            zt_hexdump((int (*)(void*))fgetc, fp, NULL, NULL);
            fclose(fp);
        }
    }
#endif

    /* f = zt_hex_dump(ALPHA, strlen(ALPHA), 16), printf("%s\n", f), free(f); */
    /* f = zt_hex_dump(ALPHA, strlen(ALPHA), 32), printf("%s\n", f), free(f); */
    /* f = zt_hex_dump(ALPHA, strlen(ALPHA), 50), printf("%s\n", f), free(f); */
    /* f = zt_hex_dump(ALPHA, strlen(ALPHA), 64), printf("%s\n", f), free(f); */

    return 0;
}
