#ifndef FILE_DEBUG_INCLUDED
#define FILE_DEBUG_INCLUDED

#ifndef DEBUG
#define DEBUG 0
#endif
// This macro acts just like printf
// Pinched from: https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
// Note: ## before __VA_ARGS__ omits the preceeding comma if there are no additional armuments. This ONLY WORKS IN GCC or clang with gcc compatability.
#define debug_print(fmt, ...) do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__); } while (0)

#endif