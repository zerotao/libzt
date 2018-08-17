/*!
 * Filename: zt_cstr.c
 * Description: C String Utilities
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *        Copyright (C) 2000-2010, Jason L. Shiffer.
 *        See file COPYING for details
 *
 * Notes:
 *
 */

#include "zt.h"
#include "zt_internal.h"

#ifdef HAVE_CONFIG_H
#include <libzt/zt_config.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include <ctype.h>

/*!
 * generate an index based on an index statement i, j and length len
 */
/*!
 * update s, i and j
 */
#define CONVERT(s, i, j) do {             \
        size_t   len;                     \
        zt_assert(s);                     \
        len = strlen(s);                  \
        if (len > 0) {                    \
            i = IDX(i, len);              \
            j = IDX(j, len);              \
            if (i > j) {                  \
                ssize_t   t = i;          \
                i     = j;                \
                j     = t;                \
            }                             \
            zt_assert(i >= 0 && j < (ssize_t)len); \
        } else {                          \
            i = 0;                        \
            j = 0;                        \
        }                                 \
} while (0)

/* #define IDXLEN(i, j) ((i) < (j) ? ((j) - (i)) + 1 : ((i) - (j)) + 1) */
static INLINE size_t
IDXLEN(ssize_t i, ssize_t j) {
    ssize_t   r;

    if (i < j) {
        r = (j - i) + 1;
    } else {
        r = (i - j) + 1;
    }
    zt_assert(r > 0);
    return (size_t)r;
}

/* #define BASE(i, len) ((i) >= (ssize_t)(len) ? (len - 1) : (i)) */
static INLINE ssize_t
BASE(ssize_t i, size_t len) {
    if ( i >= (ssize_t)len ) {
        return len - 1;
    }
    return i;
}

/* #define IDX(i, len) ((i) < 0 ? BASE((i) + (len), (len)) : BASE((i), (len))) */
static ssize_t
IDX(ssize_t i, size_t len) {
    ssize_t    r;

    if(i < 0) {
        r = BASE(i+len, len);
    } else {
        r = BASE(i, len);
    }

    return r;
}


/*!
 * return a newly allocated substring
 */
char *
zt_cstr_sub(const char *s, ssize_t i, ssize_t j) {
    char * new;
    char * p;

    CONVERT(s, i, j);

    new = zt_malloc(char, IDXLEN(i, j) + 1);

    p   = new;

    while (i <= j) {
        *p++ = s[i++];
    }
    *p  = '\0';
    return new;
}

/*!
 * return a newly allocated string containing n copies of s[i:j] plus NUL
 */
char *
zt_cstr_dup(const char *s, ssize_t i, ssize_t j, ssize_t n) {
    ssize_t   k;
    char    * new;
    char    * p;

    zt_assert(n >= 0);

    if (!s) {
        return NULL;
    }

    CONVERT(s, i, j);

    p = new = zt_malloc(char, (n * IDXLEN(i, j)) + 1);

    if (IDXLEN(i, j) > 0) {
        while (n-- > 0) {
            for (k = i; k <= j; k++) {
                *p++ = s[k];
            }
        }
    }

    *p = '\0';

    return new;
}

/*!
 * return a newly allocated string containing s1[i1:j1]s2[i2:j2]
 */
char *
zt_cstr_cat(const char *s1, ssize_t i1, ssize_t j1,
            const char *s2, ssize_t i2, ssize_t j2) {
    char * new;
    char * p;

    CONVERT(s1, i1, j1);
    CONVERT(s2, i2, j2);

    p = new = zt_malloc(char, IDXLEN(i1, j1) + IDXLEN(i2, j2) + 1);

    while (i1 <= j1) {
        *p++ = s1[i1++];
    }

    while (i2 <= j2) {
        *p++ = s2[i2++];
    }

    *p = '\0';
    return new;
}

/*!
 * return a newly allocated string containing s[i:j]s1[i1:j1]...sn[in:jn]
 */
char *
zt_cstr_catv(const char *s, ...) {
    char       * new;
    char       * p;
    const char * save = s;
    ssize_t      i;
    ssize_t      j;
    size_t       len  = 0;
    va_list      ap;

    va_start(ap, s);

    /* calculate the lentgh */
    while (s) {
        i    = va_arg(ap, int);
        j    = va_arg(ap, int);
        CONVERT(s, i, j);
        len += IDXLEN(i, j) + 1;
        s    = va_arg(ap, const char *);
    }

    va_end(ap);

    p = new = zt_malloc(char, len + 1);
    s = save;
    va_start(ap, s);

    while (s) {
        i = va_arg(ap, int);
        j = va_arg(ap, int);
        CONVERT(s, i, j);

        while (i <= j) {
            *p++ = s[i++];
        }
        s = va_arg(ap, const char *);
    }

    *p = '\0';
    return new;
}

/*!
 * returns a newly allocated string containing the reverse of s[i:j]
 */
char *
zt_cstr_reverse(const char *s, ssize_t i, ssize_t j) {
    char * new;
    char * p;

    CONVERT(s, i, j);

    p = new = zt_malloc(char, IDXLEN(i, j) + 1);

    while (i <= j) {
        *p++ = s[j--];
    }

    *p = '\0';
    return new;
}

/*!
 * returns a newly allocated string containing convert(s[i:j], from, to) where the chars in from are
 * converted to the corrisponding chars in to.
 * returns NULL on failure
 */
char *
zt_cstr_map(const char *s, ssize_t i, ssize_t j,
            const char *from, const char *to) {
    unsigned char map[256] = { 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,
                               10,  11,  12,  13,  14,  15,  16,  17,  18,  19,
                               20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
                               30,  31,  32,  33,  34,  35,  36,  37,  38,  39,
                               40,  41,  42,  43,  44,  45,  46,  47,  48,  49,
                               50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
                               60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
                               70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
                               80,  81,  82,  83,  84,  85,  86,  87,  88,  89,
                               90,  91,  92,  93,  94,  95,  96,  97,  98,  99,
                               100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                               110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
                               120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
                               130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
                               140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
                               150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
                               160, 161, 162, 163, 164, 165, 166, 167, 168, 169,
                               170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
                               180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
                               190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
                               200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
                               210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
                               220, 221, 222, 223, 224, 225 };

    if (from && to) {
        while (*from && *to) {
            map[(unsigned char)*from++] = *to++;
        }

        zt_assert(*from == 0 && *to == 0);
    }

    if (s) {
        char * new;
        char * p;

        CONVERT(s, i, j);
        p  = new = zt_malloc(char, IDXLEN(i, j) + 1);
        while (i <= j) {
            *p++ = map[(unsigned char)s[i++]];
        }
        *p = '\0';
        return new;
    }

    return NULL;
} /* zt_cstr_map */

/*!
 * returns the index into s that i corrisponds to.
 */
size_t
zt_cstr_pos(const char *s, ssize_t i) {
    size_t len;

    zt_assert(s);

    len = strlen(s);
    i   = (ssize_t)IDX(i, len);

    zt_assert(i >= 0 && (size_t)i <= len);
    return (size_t)i;
}

/*!
 * returns the length of the slice represented by [i:j]
 */
size_t
zt_cstr_len(const char *s, ssize_t i, ssize_t j) {
    CONVERT(s, i, j);
    return IDXLEN(i, j);
}

/*!
 * return -1, 0, 1 if s1[i1:j1] is lexically less then, equal to or
 * greater then s2[i2:j2]
 */
int
zt_cstr_cmp(const char *s1, ssize_t i1, ssize_t j1,
            const char *s2, ssize_t i2, ssize_t j2) {
    CONVERT(s1, i1, j1);
    CONVERT(s2, i2, j2);

    s1 += i1;
    s2 += i2;

    if (j1 - i1 < j2 - i2) {
        int cond = strncmp(s1, s2, IDXLEN(i1, j1));
        return cond == 0 ? -1 : cond;
    } else if (j1 - i1 > j2 - i2) {
        int cond = strncmp(s1, s2, IDXLEN(i2, j2));
        return cond == 0 ? +1 : cond;
    }

    return strncmp(s1, s2, j1 - i1);
}

/*!
 * locates the position of the first occurrence of c
 * in the string referenced by s[i:j]
 */
size_t
zt_cstr_chr(const char *s, ssize_t i, ssize_t j, int c) {
    zt_assert(s);

    CONVERT(s, i, j);
    for (; i <= j; i++) {
        if (s[i] == c) {
            return (size_t)i;
        }
    }

    return 0;
}
/*!
 * locates the last occurrence of c in the string referenced by s[i:j]
 */
size_t
zt_cstr_rchr(const char *s, ssize_t i, ssize_t j, int c) {
    zt_assert(s);

    CONVERT(s, i, j);
    for (; j >= i; j--) {
        if (s[j] == c) {
            return j;
        }
    }

    return 0;
}

/*!
 * locates the first occurrence of any char in set in the string
 * referenced by s[i:j]
 */
ssize_t
zt_cstr_upto(const char *s, ssize_t i, ssize_t j, const char *set) {
    zt_assert(set);

    CONVERT(s, i, j);

    for (; i <= j; i++) {
        if (strchr(set, s[i])) {
            return i;
        }
    }

    return -1;
}

/*!
 * locates the last occurrence of any char in set in the string referenced
 * by s[i:j]
 */
ssize_t
zt_cstr_rupto(const char *s, ssize_t i, ssize_t j, const char *set) {
    zt_assert(set);

    CONVERT(s, i, j);
    for (; j >= i; j--) {
        if (strchr(set, s[j])) {
            return j;
        }
    }

    return -1;
}

/*!
 * locates the first occurrence of the string str in the string referenced
 * by s[i:j]
 */
ssize_t
zt_cstr_find(const char *s, ssize_t i, ssize_t j, const char *str) {
    size_t    len;

    CONVERT(s, i, j);
    zt_assert(str);

    len = strlen(str);

    if (len == 0) {
        return -1;
    } else if (len == 1) {
        for (; i <= j; i++) {
            if (s[i] == *str) {
                return i;
            }
        }
    }

    for (; (ssize_t)(i + len) <= (j + 1); i++) {
        if (strncmp(&s[i], str, len) == 0) {
            return i;
        }
    }

    return -1;
}

/*!
 * locates the first occurrence of the string str (in reverse) in the string referenced
 * by s[i:j]
 */
ssize_t
zt_cstr_rfind(const char *s, ssize_t i, ssize_t j, const char *str) {
    size_t    len;
    ssize_t   offt;

    zt_assert(str);

    CONVERT(s, i, j);
    len = strlen(str);

    if (len == 0) {
        return -1;
    }

    if (len == 1) {
        for (; j >= i; j--) {
            if (s[j] == *str) {
                return j;
            }
        }
    }

    for (offt = j - len + 1; offt >= i; offt--) {
        if (strncmp(&s[offt], str, len) == 0) {
            return offt;
        }
    }

    return -1;
}

/*!
 * locates the first occurrence of any char in set in the string
 * referenced by s[i:j]
 */
ssize_t
zt_cstr_any(const char *s, ssize_t i, ssize_t j, const char *set) {
    zt_assert(s);
    zt_assert(set);

    CONVERT(s, i, j);

    zt_assert(i >= 0 && i <= j);

    for (; i <= j; i++) {
        if (s[i] != '\0' && strchr(set, s[i])) {
            return i;
        }
    }

    return -1;
}

/*!
 * locates the last occurrence of any char in set in the string
 * referenced by s[i]
 */
ssize_t
zt_cstr_rany(const char *s, ssize_t i, ssize_t j, const char *set) {
    ssize_t   orig;

    zt_assert(s);
    zt_assert(set);

    CONVERT(s, i, j);

    zt_assert(i >= 0 && i <= j);

    for (orig = i; j >= i; j--) {
        if (s[j] != '\0' && strchr(set, s[j])) {
            return j - orig;
        }
    }

    return -1;
}

/* strip the \n from the end of a string */
char*
zt_cstr_chomp(char *str) {
    zt_assert(str);
    {
        size_t    i = 0;

        i = strlen(str);
        if (str[i - 1] == '\n') {
            str[i - 1] = '\0';
        }
    }
    return str;
}

/* Strip white space characters from the front and back of the string */
char*
zt_cstr_strip(char *str) {
    zt_assert(str);
    {
        /* strip whitespace from the beginning of the string */
        size_t    len = 0;
        int       nl  = 0;

        len = strspn(str, WHITESPACE);
        memmove(str, &str[len], (strlen(str) - len) + 1); /* +1 captures \0 */

        /* strip whitespace from the end of the string */
        if (strchr(str, '\n') != NULL) {
            nl = 1;
        }
        len = zt_cstr_rspn(str, WHITESPACE "\n");
        len = strlen(str) - len;
        if (len) {
            if (nl) {
                str[len++] = '\n';
            }
            str[len] = '\0';
        }
    }
    return str;
}

size_t
zt_cstr_rspn(const char *s, const char *accept) {
    size_t    len = strlen(s);
    ssize_t   i   = 0;

    zt_assert(s);
    zt_assert(accept);

    for (i = len - 1; i > 0; i--) { /* -1 to skip \0 */
        if (strspn(&s[i], accept) == 0) {
            return (len - i) - 1;                          /* -1 for the first non matching char */
        }
    }
    return len;
}

size_t
zt_cstr_rcspn(const char *s, const char *reject) {
    size_t    len = strlen(s);
    ssize_t   i   = 0;

    zt_assert(s);
    zt_assert(reject);

    for (i = len - 1; i > 0; i--) { /* -1 to skip \0 */
        if (strcspn(&s[i], reject) == 0) {
            return i;                                      /* -1 for the first non matching char */
        }
    }
    return len;
}

char*
zt_cstr_basename(char *npath, size_t len, const char *path, const char *suffix) {
    ssize_t   start;
    ssize_t   end;

    zt_assert(npath);
    zt_assert(path);

    memset(npath, '\0', len);

    if ((start = zt_cstr_rfind(path, 0, -1, PATH_SEPERATOR)) != -1) {
        start = start + strlen(PATH_SEPERATOR);
    } else {
        start = 0;
    }

    if (suffix) {
        end = zt_cstr_rfind(path, start, -1, suffix);
        if (end > 0 && path[end] == '.') {
            end--;
        }
    } else {
        end = -1;
    }

    zt_cstr_copy(path, start, end, npath, len);
    return npath;
}


char *
zt_cstr_dirname(char *npath, size_t len, const char *path) {
    ssize_t    end    = -1;
    ssize_t    ps_len = 0;

    zt_assert(npath);
    zt_assert(path);

    memset(npath, '\0', len);

    /* if the end of the path is the PATH_SEPERATOR then skip over it */
    ps_len = strlen(PATH_SEPERATOR);
    if (strcmp(&path[strlen(path) - ps_len], PATH_SEPERATOR) == 0) {
        end = end - ps_len;
    }

    if ((end = zt_cstr_rfind(path, 0, end, PATH_SEPERATOR)) == -1) {
        /* there is no path seperator */
        zt_cstr_copy(".", 0, -1, npath, len);
        return npath;
    } else {
        end = end - 1;
    }

    zt_cstr_copy(path, 0, end, npath, len);
    return npath;
}


char *
zt_cstr_path_append(const char *path1, const char *path2) {
    char    * rpath = NULL;
    size_t    len1;
    size_t    len2;
    size_t    sep_len;
    ssize_t   x;
    ssize_t   y;

    len1    = strlen(path1);
    len2    = strlen(path2);
    sep_len = strlen(PATH_SEPERATOR);

    for (y = len1 - 1; y > 0 && strncmp(&path1[y], PATH_SEPERATOR, sep_len) == 0; y--) {
        ;
    }
    for (x = 0; x < (ssize_t)len2 && strncmp(&path2[x], PATH_SEPERATOR, sep_len) == 0; x++) {
        ;
    }

    rpath = zt_cstr_catv(path1, 0, y,
                         PATH_SEPERATOR, 0, -1,
                         path2, x, -1, NULL);
    return rpath;
}

bool
zt_cstr_abspath(const char * path) {

    int len = strlen(PATH_SEPERATOR);

    if ((strncmp(path, PATH_SEPERATOR, len) == 0) ||
        (path[0] == '.' && strncmp(&path[1], PATH_SEPERATOR, len) == 0)) {
        return true;
    }

    return false;
}

struct _str_state {
    char    * c;
    size_t    len;
    size_t    offt;
};

static int _read_str(void * ctx) {
    struct _str_state   * state = (struct _str_state *)ctx;
    int    c = 0;

    if (state->len == state->offt) {
        return EOF;
    }

    c = *(state->c + state->offt);
    state->offt += 1;
    return c;
}

int
zt_hexdump_default_printer(UNUSED void * ctx, size_t addr, char * hex, char * txt) {
    size_t    offt;
    size_t    base = 55;
    char      buffer[100+1];

    if (hex) {
        if (addr < 65535) {
            offt = snprintf(buffer, 100, "%.4zX   %.12s %.12s %.12s %.12s", addr, hex, hex+(4*3), hex+(8*3), hex+(12*3));
            base += 4;
        } else if (addr < 4294967295UL) {
            offt = snprintf(buffer, 100, "%.8zX   %.12s %.12s %.12s %.12s", addr, hex, hex+(4*3), hex+(8*3), hex+(12*3));
            base += 8;
        } else {
            offt = snprintf(buffer, 100, "%.16zX   %.12s %.12s %.12s %.12s", addr, hex, hex+(4*3), hex+(8*3), hex+(12*3));
            base += 16;
        }

        snprintf(buffer+offt, 100-offt, BLANK "| %s", INDENT_TO(base, 1, offt), txt);
        printf("%s\n", buffer);
    } else {
        printf("\n");
    }
    return 0;
}

size_t
zt_hexdump_str(char * data, size_t size, zt_hexdump_output output, void * odata) {
    struct _str_state     state = { data, size, 0 };

    return zt_hexdump(_read_str, &state, output, odata);
}

size_t
zt_hexdump(int _getchar(void *), void * data, zt_hexdump_output output, void * odata) {
    size_t    bytes = 0;
    size_t    hpos = 0, cpos = 0;
    int       c = 0;
    uint32_t  addr = 0L;
    size_t    _addr = 0L;
    char      _txt[16+1];
    char      _hex[(16*3)+3];

    if (output == NULL) {
        output = zt_hexdump_default_printer;
    }

    while ((c = _getchar(data)) != EOF) {

        if (addr % 16 == 0) {
            _addr = addr;
            memset(_txt, 0, sizeof(_txt));
            memset(_hex, 0, sizeof(_hex));
            hpos = 0;
            cpos = 0;
        }

        *(_hex+hpos++) = HEX_DIGITS[((c >> 4) & 0xF)];
        *(_hex+hpos++) = HEX_DIGITS[(c  & 0xF)];
        *(_hex+hpos++) = ' ';

        *(_txt+cpos++) = isprint(c) ? c : '.';

        if (addr % 16 == 15) {
            if (output(odata, _addr, _hex, _txt) == -1) {
                return -1;
            }
        }
        addr++;
        bytes++;
    }

    if ((addr % 16 != 15) && (addr % 16 != 0)) {
        if (output(odata, _addr, _hex, _txt) == -1 ) {
            return -1;
        }
    }

    output(odata, 0, NULL, NULL);
    return bytes;
}


/*!
 * converts binary data to a hex string it does not NULL terminate the
 * string
 */
size_t
zt_binary_to_hex(void *data, size_t dlen, char *hex, size_t hlen) {
    size_t n;
    char * dptr = hex;

    for (n = 0; (n < dlen) && ((n * 2) < hlen); n++, dptr += 2) {
        uint8_t c = ((uint8_t *)data)[n];
        if (hex != NULL) {
            dptr[0] = HEX_DIGITS[((c >> 4) & 0xF)];
            dptr[1] = HEX_DIGITS[(c & 0xF)];
        }
    }
    return n * 2;
}

static int8_t
hex_to_char(char hex) {
    uint8_t c = hex;

    if (c >= '0' && c <= '9') {
        c = c - 48;
    } else if (c >= 'A' && c <= 'F') {
        c = c - 55;  /* 65 - 10 */
    } else if (c >= 'a' && c <= 'f') {
        c = c - 87;  /* 97 - 10 */
    } else {
        return -1;
    }
    return c;
}

/*
 * convert a hex string to binary
 * hex - points to a (possibly) null terminated hex string
 * hlen - amount of hex to process
 * data - points to the location to store the converted data which
 *        should be 2 times the size of the hex values in hex
 * dlen = holds the length of data
 *
 * returns - 0 on error or the number of bytes in data on success
 * --
 * if called with a NULL data will return the number of bytes required
 * to convert all data in hex.
 */
size_t
zt_hex_to_binary(char *hex, size_t hlen, void *data, size_t dlen) {
    size_t n;
    size_t y = 0;

    if (data == NULL) {
        dlen = -1;
    }

    for (n = 0, y = 0; n < hlen && *hex != '\0' && y < dlen; n++) {
        int8_t c;
        int8_t c2;
        int8_t cc = 0;

        if ((c = hex_to_char(*hex++)) == -1) {
            errno = EINVAL;
            return 0;
        }

        if ((c2 = hex_to_char(*hex++)) == -1) {
            return 0;
        } else {
            n++;
        }

        cc = (c << 4) | (c2 & 0xF);

        if (data != NULL) {
            ((char *)data)[y] = (char)cc;
        }
        y++;
    }
    return y;
}

size_t
zt_cstr_copy(const char * from, ssize_t i, ssize_t j, char * to, size_t len) {
    size_t    flen;
    ssize_t    min;

    CONVERT(from, i, j);
    flen = IDXLEN(i, j);

    min  = MIN(flen, len);
    memcpy(to, &from[i], min);
    return min;
}

int
zt_cstr_split_free(zt_ptr_array *array) {
    return zt_ptr_array_free(array, 1);
}

int
zt_cstr_cut_free(zt_ptr_array *array) {
    return zt_ptr_array_free(array, 1);
}

zt_ptr_array *
zt_cstr_split(const char *str, const char *delim) {
    char         *str_copy;
    char         *endptr;
    char         *tok;
    zt_ptr_array *split_array;

    if (str == NULL || delim == NULL) {
        return NULL;
    }

    split_array = zt_ptr_array_init(NULL, (zt_ptr_array_free_cb)free);

    endptr      = NULL;

    str_copy    = strdup(str);

    for (tok = strtok_r(str_copy, delim, &endptr); tok != NULL; tok = strtok_r(NULL, delim, &endptr)) {
        if (zt_ptr_array_add(split_array, (void *)strdup(tok)) < 0) {
            zt_ptr_array_free(split_array, 1);
            free(str_copy);
            return NULL;
        }
    }

    free(str_copy);

    return split_array;
}

zt_ptr_array *
zt_cstr_cut(const char *str, const int delim, int keep_delim) {
    char         *str_copy;
    char         *cut_tok;
    zt_ptr_array *cuts;

    if ((cuts = zt_ptr_array_init(NULL, free)) == NULL) {
        return NULL;
    }

    cut_tok    = str_copy = strdup(str);
    keep_delim = keep_delim ? 1 : 0;

    while ((cut_tok = strchr(cut_tok, delim))) {
        cut_tok++;

        if (*cut_tok == '\0') {
            break;
        }

        if (zt_ptr_array_add(cuts, (void *)strdup((char *)(cut_tok - keep_delim))) < 0) {
            zt_ptr_array_free(cuts, 1);
            free(str_copy);
            return NULL;
        }
    }

    free(str_copy);
    return cuts;
}

zt_ptr_array *
zt_cstr_tok(const char *str, const int delim, int keep_delim UNUSED)
{
    char *str_copy;
    char *cut_tok;
    zt_ptr_array *cuts;

    if ((cuts = zt_ptr_array_init(NULL, free)) == NULL) {
        return NULL;
    }

    str_copy = strdup(str);

    if (str[strlen(str) - 1] == delim) {
        str_copy[ strlen(str) - 1 ] = '\0';
    }

    while((cut_tok = strrchr(str_copy, delim)) != str_copy)
    {
        if (cut_tok == NULL) {
            break;
        }

        if (zt_ptr_array_add(cuts, (void *)strdup(str_copy)) < 0) {
            zt_ptr_array_free(cuts, 1);
            free(str_copy);
            return NULL;
        }

        *cut_tok = '\0';
    }

    zt_ptr_array_add(cuts, (void *)strdup(str_copy));
    free(str_copy);

    return cuts;
}

static char * _itoa_integers = "9876543210123456789";

ssize_t zt_cstr_itoa(char *s, int value, size_t offset, size_t bufsiz)
{
    size_t    loc;
    ssize_t   result;

    result = loc = offset + zt_cstr_int_display_len(value);

    if ((s == NULL) || (loc > bufsiz)) {
        return -1;
    }

    if (value < 0) {
        s[offset] = '-';
    }

    s[loc] = 0;
    loc--;

    do {
        s[loc] = *(_itoa_integers + 9 + (value % 10));
        value /= 10;
        loc--;
    } while(value != 0);

    return result;
}

size_t zt_cstr_int_display_len(int value)
{
    int result;
    result = value >= 0 ? 1 : 2;

    while ((value >= 1000) || (value <= -1000)) {
        result += 3;
        value /= 1000;
    }

    if ((value >= 100) || (value <= -100)) {
        return result + 2;
    }

    if ((value >= 10) || (value <= -10)) {
        return result + 1;
    }

    return result;
}
