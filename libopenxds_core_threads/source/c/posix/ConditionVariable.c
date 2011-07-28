/*
 *  Copyright (C) 2004-2009 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openxds.core.base.h"
#include "openxds.core.threads/ConditionVariable.h"
#include "openxds.core.threads/Mutex.h"
#include "openxds.core.threads/Mutex.protected.h"

#include <pthread.h>
#include <sys/time.h>

struct _ConditionVariable
{
	pthread_cond_t     state;
	pthread_condattr_t attributes;
	bool                valid;
};

ConditionVariable* new_ConditionVariable()
{
	ConditionVariable* self = CRuntime_calloc( 1, sizeof( ConditionVariable ) );
	self->valid = !pthread_cond_init( &self->state, &self->attributes );
	return self;
}

void free_ConditionVariable( ConditionVariable* self )
{
	pthread_cond_destroy( &self->state );
	CRuntime_free( self );
}

bool ConditionVariable_signal( ConditionVariable* self )
{
	return !pthread_cond_signal( &self->state );
}

bool ConditionVariable_broadcast( ConditionVariable* self )
{
	return !pthread_cond_broadcast( &self->state );
}

bool ConditionVariable_wait( ConditionVariable* self, Mutex* aMutex )
{
	pthread_mutex_t* mutex_state = (pthread_mutex_t*) Mutex_getState( aMutex );
	return !pthread_cond_wait( &self->state, mutex_state );
}

bool ConditionVariable_timedWait( ConditionVariable* self, Mutex* aMutex, int seconds )
{
	struct timeval now;
	struct timespec timeout;
	struct timezone* tz = NULL;
	gettimeofday( &now, tz );
	
	timeout.tv_sec = now.tv_sec + seconds;
	timeout.tv_nsec = now.tv_usec * 1000;
	
	pthread_mutex_t* mutex_state = (pthread_mutex_t*) Mutex_getState( aMutex );
	return !pthread_cond_timedwait( &self->state, mutex_state, &timeout );
}
