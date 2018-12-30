#ifndef _CROSS_PLATFORM_H_
#define _CROSS_PLATFORM_H_

#if defined _WIN32 || defined __CYGWIN__
	#ifdef BUILDING_DLL
		#ifdef __GNUC__
			#define DLL_PUBLIC __attribute__ ((dllexport))
		#else
			#define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
		#endif
	#else
		#ifdef __GNUC__
			#define DLL_PUBLIC __attribute__ ((dllimport))
		#else
			#define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
		#endif
	#endif
		#define DLL_LOCAL
#else
		#if __GNUC__ >= 4
			#define DLL_PUBLIC __attribute__ ((visibility ("default")))
			#define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
		#else
			#define DLL_PUBLIC
			#define DLL_LOCAL
		#endif

#endif

#ifdef BUILDING_QUEUE_DLL
#define QUEUE_DLL_PUBLIC DLL_PUBLIC
#else
#define QUEUE_DLL_PUBLIC
#endif

#ifndef _WIN32

#define MAX_PATH 1024
// #define min(x, y) ((x) < (y) ? (x) : (y))
#ifndef __stdcall
	#define __stdcall  
#endif

#endif

#endif // _CROSS_PLATFORM_H_
