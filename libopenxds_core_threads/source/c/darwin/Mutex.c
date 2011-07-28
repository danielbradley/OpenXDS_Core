/*
 *  Copyright (C) 2004-2009 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openxds.core.base.h"
#include "openxds.core.threads/Mutex.protected.h"

#include <pthread.h>
#include <errno.h>

struct _Mutex
{
	pthread_mutex_t     state;
	pthread_mutexattr_t attributes;
	bool                valid;
	bool                locked;
};

Mutex* new_Mutex()
{
	Mutex* self = CRuntime_calloc( 1, sizeof( struct _Mutex ) );
	self->valid = !pthread_mutex_init( &self->state, NULL );// &self->attributes );
	self->locked = 0;
	return self;
}

void free_Mutex( Mutex* self )
{
	CRuntime_free( self );
}

bool Mutex_lock( Mutex* self )
{
	if ( !self->locked )
	{
		self->locked = 1;
		return !pthread_mutex_lock( &self->state );
	}
}

bool Mutex_tryLock( Mutex* self )
{
	bool success = 1;
	if ( EBUSY == pthread_mutex_trylock( &self->state ) )
	{
		success = 0;
	}
	return success;
}

bool Mutex_unlock( Mutex* self )
{
	return !pthread_mutex_unlock( &self->state );
}

bool Mutex_isValid( Mutex* self )
{
	return self->valid;
}

void* Mutex_getState( Mutex* self )
{
	return &self->state;
}
