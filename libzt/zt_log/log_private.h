/*
 * private.h        logging private definitions
 *
 * Copyright (C) 2000-2002, 2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: log_private.h,v 1.2 2003/06/09 13:42:12 jshiffer Exp $
 *
 */

/* Private interface to log
 * You should NOT access this directly
 * unless you are extending the component
 */
#ifndef _LOG_PRIVATE_H_
#define _LOG_PRIVATE_H_

#include <stdlib.h>
#include <libzt/zt_log.h>

#ifdef __cplusplus
extern "C" {
#pragma }
#endif /* __cplusplus */

typedef struct log_level_desc_ty log_level_desc_ty;
struct log_level_desc_ty {
  log_level level;
  char *desc;
};
extern log_level_desc_ty log_level_desc[];

#define LOG_LEVEL_MAX 8+1

struct log_ty {
  struct log_vtbl_ty *vtbl;
  unsigned int opts;
  log_level level;
  char *file;
  char *function;
  int line;
  /* rest of opts */
};
  
typedef struct log_vtbl_ty log_vtbl_ty;
struct log_vtbl_ty {
  size_t size;
  /* virtual function pointers */
  void (* destructor)(log_ty *log);
  void (* print)(log_ty *log, log_level level, char *fmt, va_list ap);
};

log_ty *log_new (log_vtbl_ty *vptr, unsigned int opts);
char* log_gen_fmt (log_ty *log, char *fmt, log_level level, unsigned int opts);

#ifdef __cplusplus
}
#endif
#endif  /* _LOG_PRIVATE_H_ */
