#ifndef __getch_h_2018_05_17_09_44
#define __getch_h_2018_05_17_09_44
#ifdef __cplusplus
extern "C"{
#endif
#ifdef _WIN32
#include <conio.h>
#elseif __SNESOS_SYSTEM
#define __ENABLE_GETCH
#include <os/terminal.h>
#else
int getch();
#endif
#ifdef __cplusplus
};
#endif

#endif
