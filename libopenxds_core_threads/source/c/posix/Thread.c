/*
 *  Copyright (C) 2004-2009 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openxds.core.base.h"
#include "openxds.core.threads/Thread.h"

#include <pthread.h>

struct _Thread
{
	void*(*callback)(void*);
	void* arg;
	pthread_t      state;
	pthread_attr_t attributes;
	void*          ret;
};

Thread* new_Thread( void*(*startRoutine)(void*), void* argument )
{
	Thread* self = CRuntime_calloc( 1, sizeof( Thread ) );
	self->callback = startRoutine;
	self->arg = argument;
	pthread_attr_init( &self->attributes );
	return self;
}

void free_Thread( Thread* self )
{
	CRuntime_free( self );
}

bool Thread_run( Thread* self )
{
	return !pthread_create( &self->state, &self->attributes, self->callback, self->arg );
}

bool Thread_wait( Thread* self )
{
	return !pthread_join( self->state, &self->ret );
}

