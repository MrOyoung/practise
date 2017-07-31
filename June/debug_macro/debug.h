#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifndef DEBUG_INCLUDE
#error "this file cannot be include by cfile!!!"
#endif

 
#if DEBUG_LEVEL > 2
#define PRINT_DEBUG printf("debug\n");
#else
#define PRINT_DEBUG
#endif

#endif
