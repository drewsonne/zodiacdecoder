#include "logging.h"
#include "windows.h"
#include <stdio.h>
#include <stdarg.h>

void debugPrintf(const char *_Format, ...) {

   va_list argptr;
   va_start(argptr, _Format);
	
   char buffer[DEBUG_PRINTF_MAX_STRING];
   vsprintf_s(buffer, sizeof(buffer), _Format,argptr);
   va_end( argptr );
   OutputDebugString(buffer);

}