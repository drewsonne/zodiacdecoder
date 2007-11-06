#ifndef __LOGGING_H__
#define __LOGGING_H__

#define DEBUG_PRINTF_MAX_STRING 512

// A simple printf style function for printing to the debug console.

void debugPrintf(const char *_Format, ...);
#endif