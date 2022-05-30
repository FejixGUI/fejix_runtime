/**
 * @file myassert.h
 *
 * assert.h leads to the dark side of the Force. Do not use it in devtest. 
 */

#ifndef _MY_ASSERT_H_
#define _MY_ASSERT_H_

#include <stdio.h>

/// Fails if EXPR is false
#define massert(EXPR) _massert(__FILE__, __LINE__, (EXPR) ? 1 : 0);

void _massert(const char *file, int line, int value);

#endif // _MY_ASSERT_H_