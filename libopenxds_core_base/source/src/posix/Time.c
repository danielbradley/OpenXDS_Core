/*
 *  Copyright (c) 2004-2010 Daniel Robert Bradley. All rights reserved.
 */

#include "openxds.core.base/CharString.h"
#include "openxds.core.base/CRuntime.h"
#include "openxds.core.base/Time.h"

#include <stdlib.h>

#ifndef __USE_POSIX
#define __USE_POSIX
#endif
#include <time.h>		/** times()    **/

struct _Time
{
	struct tm time;
	time_t    sinceEpoch;
	char*     textual;
};

Time* new_Time( unsigned long long secondsSinceEpoch )
{
	Time* self = CRuntime_calloc( 1, sizeof( Time ) );
	time_t t = (time_t) secondsSinceEpoch;
	localtime_r( &t, &self->time );
	self->sinceEpoch = t;
	
	self->textual = new_CharString_format_args( "%64s", "" );
	ctime_r( &t, self->textual );

	//	This gets rid of the annoying newline
	self->textual[CharString_getLength( self->textual ) - 1] = '\0';
	
	return self;
}

void free_Time( Time* self )
{
	free_CharString( self->textual );
	CRuntime_free( self );
}

Time* Time_copy( const Time* self )
{
	return new_Time( Time_getSecondsSinceEpoch( self ) );
}

const char* Time_getTextual( const Time* self )
{
	return self->textual;
}

unsigned long long Time_getSecondsSinceEpoch( const Time* self )
{
	return self->sinceEpoch;
}

Time* Time_Now()
{
	return new_Time( time( NULL ) );
}
