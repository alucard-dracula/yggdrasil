/*
 * Automatic configuration flags
 *
 * Copyright 2010 by Gray Watson
 *
 * This file is part of the dmalloc package.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose and without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies, and that the name of Gray Watson not be used in
 * advertising or publicity pertaining to distribution of the document
 * or software without specific, written prior permission.
 * 
 * Gray Watson makes no representations about the suitability of the
 * software described herein for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * The author may be contacted via http://dmalloc.com/
 *
 * $Id: conf.h,v 1.2 2011/10/31 04:47:36 xuyang Exp $
 */

#ifndef __CONF_H__
#define __CONF_H__

/*
 * NOTE: The following settings should not need to be tuned by hand.
 */

/*
 * Set to 1 if the first byte in a word is the "high" order byte.  Traditionally, Intel boxes
 * are "little" endian where the first byte is the "low" order byte.  Sparc and power-pc are
 * big endian.
 */
#define MD5_BIG_ENDIAN 0

#endif /* ! __CONF_H__ */
