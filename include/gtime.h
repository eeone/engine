#ifndef __GTIME_H__
#define __GTIME_H__

#include "program.h"

#ifdef MACH
#include <mach/clock.h>
#include <mach/mach.h>
#endif
#ifdef __linux__
#include <time.h>
#endif

double get_time();

#endif
