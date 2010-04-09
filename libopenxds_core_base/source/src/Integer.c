/*
 *  Copyright (c) 2008-2010 Daniel Robert Bradley. All rights reserved.
 */

#include "openxds.core.base/Integer.h"

#include <openxds.core/IObject.h>
#include <openxds.core.base/CRuntime.h>

struct _Integer
{
	IObject super;
	int     value;
};

Integer* new_Integer( int aValue )
{
	struct _Integer* self = CRuntime_calloc( 1, sizeof( struct _Integer ) );
	self->super.free = (void* (*)( IObject* )) free_Integer;
	self->value = aValue;
	
	return self;
}

void* free_Integer( Integer* self )
{
	return CRuntime_free( self );
}

void Integer_setValue( Integer* self, int aValue )
{
	self->value = aValue;
}

int Integer_getValue( const Integer* self )
{
	return self->value;
}
