#include <stdlib.h>

#include <zt.h>

static int
local_func_with_args(int argn, int defn, int * argc, char ** argv, zt_opt_def_t * def, zt_opt_error error) {
    return 0;
}

struct lfd {
    int data;
};

int
main(int argc, char ** argv) {
    int        bool_type        = 0;
    char     * str              = NULL;
    long       long_int         = 0;
    int        flag             = 0;
    int        flag2            = 0;
    struct lfd local_func_data  = {
        0
    };

    #define LONG_DESC "This is a really long string intended to overflow the screen and make things look all wack"
    struct zt_opt_def options[] = {
        { 'h',        "help",     NULL,   zt_opt_help_stdout,   "[options]",      "this help text"   },
        { 'b',        "bool",     "[true|false]",   zt_opt_bool_int,      &bool_type,       "boolean test"     },
        { 's',        "string",   "text", zt_opt_string,        &str,             LONG_DESC          },
        { 'f',        "func",     "arg",  local_func_with_args, &local_func_data, "generic function" },
        { 'l',        "long",     "int",  zt_opt_long,          &long_int,        "a long integer"   },
        { 'q',        ZT_OPT_NLO, NULL,   zt_opt_flag_int,      &flag,            "short only flag"  },
        { ZT_OPT_NSO, "quiet",    NULL,   zt_opt_flag_int,      &flag2,           "long only flag"   },
        { ZT_OPT_END() }
    };

    int               nargc = argc;
    char           ** pargv = argv;

    int               ret   = zt_opt_process_args(&nargc, pargv, options, NULL, NULL);
    if (ret < 0) {
        zt_log_printf(zt_log_err, "Error processing args");
        exit(1);
    }

    return 0;
}
