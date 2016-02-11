/**
 * @file vararg.h
 * @provides va_copy, va_start, va_arg, va_end
 * 
 * $Id: vararg.h 173 2007-07-11 21:42:09Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#ifndef _VARARG_H_
#define _VARARG_H_

/* GCC-specific varargs */
typedef	__builtin_va_list	va_list;

#define va_copy(dst, src)	__builtin_va_copy(dst, src)
#define va_start(last, va)	__builtin_stdarg_start(last, va)
#define va_arg(va, type)	__builtin_va_arg(va, type)
#define va_end(va)		__builtin_va_end(va)

#endif /* _VARARG_H_ */
