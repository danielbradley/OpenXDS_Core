/*
 *  Copyright (c) 2004-2010 Daniel Robert Bradley. All rights reserved.
 */

/*	posix */

#include "openxds.core.base/Clock.h"

#include <stdlib.h>
#include <time.h>			/** times()    **/
#include <sys/times.h>		/** struct tms **/
#include <sys/time.h>		/** struct timeval timezone **/

struct _Clock
{
	unsigned long long start;
	unsigned long long stop;
	unsigned long long interval;
};

Clock* new_Clock()
{
	Clock* self = CRuntime_calloc( 1, sizeof( Clock ) );
	return self;
}

void free_Clock( Clock* self )
{
	CRuntime_free( self );
}

void
Clock_start( Clock* self )
{
	self->start = Clock_GetMicroSecondsSinceEpoch();
}

void
Clock_stop( Clock* self )
{
	self->stop = Clock_GetMicroSecondsSinceEpoch();
	self->interval = self->stop - self->start;
}

unsigned long long
Clock_getInterval( const Clock* self )
{
	unsigned long long ret = 0;
	unsigned long long now = Clock_GetMicroSecondsSinceEpoch();

	if ( 0 == self->start )
	{
		return 0;
	}
	else if ( 0 != self->stop )
	{
		ret = self->interval;
	}
	else
	{
		ret = now - self->start;
	}
	return ret;
}


/*
 *  Class Methods
 */
int Clock_GetTimeAsInt()
{
	return time( NULL );
}

unsigned long long
Clock_GetMicroSecondsSinceEpoch()
{
	unsigned long seconds = 0;
	struct timeval tv;
	struct timezone tz;
	
	if ( 0 == gettimeofday( &tv, &tz ) )
	{
		seconds = tv.tv_sec * 1000;
		seconds += tv.tv_usec;
	}
	return seconds;
}

unsigned long long
Clock_GetSecondsSinceEpoch()
{
	unsigned long seconds = 0;
	struct timeval tv;
	struct timezone tz;
	
	if ( 0 == gettimeofday( &tv, &tz ) )
	{
		seconds = tv.tv_sec;
	}
	return seconds;
}

unsigned long Clock_GetClockCyclesUsed()
{
	return clock();
}

unsigned long Clock_GetUserTime()
{
	struct tms time;
	times( &time );
	return time.tms_utime;
}

unsigned long Clock_GetSystemTime()
{
	struct tms time;
	times( &time );
	return time.tms_stime;
}

unsigned long Clock_GetUptime()
{
	struct tms time;
	return times( &time );
}
