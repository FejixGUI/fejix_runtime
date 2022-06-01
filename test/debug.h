/**
 * @file debug.h
 *
 * assert.h leads to the dark side of the Force. Do not use it in devtest. 
 */

#ifndef _MY_ASSERT_H_
#define _MY_ASSERT_H_

#include <stdio.h>

/// Fails if EXPR is false
#define _(RESULT) checkFj(__FILE__, __LINE__, (RESULT));

void checkFj(const char *file, int line, int value);

#endif // _MY_ASSERT_H_