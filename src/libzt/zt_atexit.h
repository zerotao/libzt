/*!
 * Filename: zt_atexit.h
 * Description: Hook to extend the system atexit
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *      Copyright (C) 2010, Jason L. Shiffer.
 *      See file COPYING for details
 *
 * Notes:
 *
 */
#ifndef _ZT_ATEXIT_H_
#define _ZT_ATEXIT_H_


#ifdef __cplusplus
extern "C" {
/*}*/
#endif


void zt_atexit(void (*fn)(void * data), void * data);


#ifdef __cplusplus
}
#endif
#endif /* _ZT_ATEXIT_H_ */
