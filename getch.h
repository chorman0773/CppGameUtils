#ifndef __getch_h_2018_05_17_09_44
#define __getch_h_2018_05_17_09_44
#ifdef __cplusplus
extern "C"{
#endif
#include <CompilerTraits.hpp>
#ifdef CPP_HAS_GETCH
#include CPP_GETCH_HEADER
#ifdef CPP_GETCH_DEPRECATED
#define getch _getch
#endif
#else
	int getch();
#endif
#ifdef __cplusplus
};
#endif

#endif