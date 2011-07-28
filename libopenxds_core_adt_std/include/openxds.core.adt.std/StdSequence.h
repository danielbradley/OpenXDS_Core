/*
 *  Copyright (C) 2009 Daniel Robert Bradley. All rights reserved.
 */

#ifndef OPENXDS_CORE_ADT_STD_STDSEQUENCE_H
#define OPENXDS_CORE_ADT_STD_STDSEQUENCE_H

#include "openxds.core.adt.std.h"

#include <openxds.core.adt/ISequence.h>

	#ifdef __cplusplus
namespace openxds {
	namespace core {
		namespace adt {
			namespace std {
	#endif


StdSequence*          new_StdSequence();
StdSequence*         free_StdSequence( StdSequence* self );

int StdSequence_size( const StdSequence* self );
int StdSequence_isEmpty( const StdSequence* self );
E* StdSequence_set( StdSequence* self, int r, E* e );
IPosition* StdSequence_insertFirst( StdSequence* self, E* e );
IPosition* StdSequence_insertLast( StdSequence* self, E* e );
E* StdSequence_removeFirst( StdSequence* self );
E* StdSequence_removeLast( StdSequence* self );
void StdSequence_add( StdSequence* self, int r, E* e );
E* StdSequence_removeFrom( StdSequence* self, int r );
const E* StdSequence_get( const StdSequence* self, int r );
int StdSequence_rankOf( const StdSequence* self, const IPosition* p );
const IPosition* StdSequence_atRank( const StdSequence* self, int r );
const IPosition* StdSequence_insertBefore( StdSequence* self, const IPosition* p, E* e );
const IPosition* StdSequence_insertAfter( StdSequence* self, const IPosition* p, E* e );
E* StdSequence_replace( StdSequence* self, const IPosition* p, E* e );
E* StdSequence_remove( StdSequence* self, const IPosition* p );
const IPosition* StdSequence_first( const StdSequence* self );
const IPosition* StdSequence_last( const StdSequence* self );
const E* StdSequence_getFirst( const StdSequence* self );
const E* StdSequence_getLast( const StdSequence* self );
const IPosition* StdSequence_previous( const StdSequence* self, const IPosition* p );
const IPosition* StdSequence_next( const StdSequence* self, const IPosition* p );
void StdSequence_addFirst( StdSequence* self, E* e );
void StdSequence_addLast( StdSequence* self, E* e );
IPIterator* StdSequence_positions( const StdSequence* self );


	#ifdef __cplusplus
};};};};
	#endif
#endif
