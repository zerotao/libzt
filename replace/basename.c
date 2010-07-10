/* basename.c        Replacement version of basename
 * Copyright (C) 2000, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: basename.c,v 1.1 2002/11/10 23:36:59 jshiffer Exp $
 *
 * $Log: basename.c,v $
 * Revision 1.1  2002/11/10 23:36:59  jshiffer
 * Initial revision
 *
 * Revision 1.1.1.1  2002/01/21 01:03:15  jshiffer
 * Base Import
 *
 * Revision 1.1.1.1  2001/11/08 17:02:34  jshiffer
 * base import
 *
 * Revision 1.1.1.1  2001/07/30 14:32:20  jshiffer
 * base Import
 *
 * Revision 1.1.1.1  2001/07/03 01:46:23  jshiffer
 * Base import new deve server
 *
 * Revision 1.1.1.1  2001/02/12 03:54:30  jshiffer
 * Base Import libzt
 *
 *
 * History:  -jshiffer 12/20/00 11:49am: Created.
 */


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#if HAVE_STRING_H
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif

#if !HAVE_STRRCHR
#  ifndef strrchr
#    define strrchr rindex
#  endif
#endif

char*
basename( char *path )
{
    char *basename = strrchr(path, '/');

    return (basename ? ++basename : path);
}

