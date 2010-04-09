/*
 *  Copyright (c) 2004-2010 Daniel Robert Bradley. All rights reserved.
 */

#ifndef OPENXDS_CORE_BASE_ENVIRONMENT_H
#define OPENXDS_CORE_BASE_ENVIRONMENT_H

#include "openxds.core/export.h"
#include <stdarg.h>

#ifdef __cplusplus
namespace openxds {
	namespace core {
		namespace base {
extern "C"
{
#else
#ifndef BOOLEAN
#define BOOLEAN
	typedef int bool;
#endif
#endif

typedef struct _IEnvironment IEnvironment;

EXPORT	IEnvironment* new_Environment( const char* argv_0 );
EXPORT	IEnvironment* new_Environment_using( const char* argv_0, char fileSeparator );
EXPORT	void          free_Environment( IEnvironment* self );

EXPORT  char*       Environment_searchPathFor( const IEnvironment* self, const char* file );

EXPORT	const char* Environment_getExecutableName( const IEnvironment* self );
EXPORT	const char* Environment_getExecutableLocation( const IEnvironment* self );
EXPORT	const char* Environment_getDirectoryContainingExecutable( const IEnvironment* self );

EXPORT	const char* Environment_getPath( const IEnvironment* self );
//EXPORT	const char* Environment_getOrigin( const IEnvironment* self );

EXPORT	const char* Environment_GetEnvironmentValue( const char* variable );
EXPORT	void        Environment_SetEnvironmentVariable( const char* key, const char* value, int overwrite );

EXPORT  bool Environment_isLink( const char* path );
EXPORT  char* Environment_readLink( const char* path );
EXPORT	char Environment_getFileSeparator();

#ifdef __cplusplus
}
};};};
#endif
#endif
