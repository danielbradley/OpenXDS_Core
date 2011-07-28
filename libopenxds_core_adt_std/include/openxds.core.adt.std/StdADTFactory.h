/*
 *  Copyright (C) 2007 Daniel Robert Bradley. All rights reserved.
 */

#ifndef OPENXDS_CORE_ADT_STD_STDADTFACTORY_H
#define OPENXDS_CORE_ADT_STD_STDADTFACTORY_H

#include <openxds.core/export.h>
#include <openxds.core/types.h>

#include <openxds.core.adt.h>
#include <openxds.core.adt.std.h>
#include <openxds.core.adt/ADTFactory.h>

	#ifdef __cplusplus
namespace openxds {
	namespace core {
		namespace adt {
			namespace std {
	#endif

StdADTFactory*   new_StdADTFactory();
StdADTFactory*  free_StdADTFactory();

IKey*               StdADTFactory_createKey( const char* key );

IValue*           StdADTFactory_createValue( E* value );
IValue*           StdADTFactory_createValue_ref( const E* value );
IValue*           StdADTFactory_createValue_IObject( IObject* value );

IEntry*           StdADTFactory_createEntry( const IKey* key, IValue* value );

IStack*           StdADTFactory_createStack();
IString*         StdADTFactory_createString( const char* str );
IList*             StdADTFactory_createList();
IMap*               StdADTFactory_createMap();
IDictionary* StdADTFactory_createDictionary();
IVector*         StdADTFactory_createVector();
ISequence*     StdADTFactory_createSequence();

	#ifdef __cplusplus
};};};};
	#endif
#endif
