/*
 *  Copyright (C) 2007 Daniel Robert Bradley. All rights reserved.
 */

#include "openxds.core.adt.std/StdSequenceNode.h"
#include "openxds.core.base/CRuntime.h"

struct _StdSequenceNode
{
	IPosition super;
	E*        e;
	int       r;
};

StdSequenceNode* new_StdSequenceNode( E* anElement )
{
	StdSequenceNode* self = CRuntime_calloc( 1, sizeof( StdSequenceNode ) );

	self->super.super.free = (void*      (*)( IObject* ))         free_StdSequenceNode;
	self->super.free       = (void*      (*)( IPosition* ))       free_StdSequenceNode;
	self->super.getElement = (const E*   (*)( const IPosition* )) StdSequenceNode_getElement;

	self->e = anElement;
	self->r = -1;

	return self;
}

StdSequenceNode* free_StdSequenceNode( StdSequenceNode* self )
{
	if ( self->e ) CRuntime_free( self->e );
	CRuntime_free( self );
}

const E* StdSequenceNode_getElement( const StdSequenceNode* self )
{
	return self->e;
}