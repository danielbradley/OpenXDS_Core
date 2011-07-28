/*
 *  Copyright (C) 2007-2009 Daniel Robert Bradley. All rights reserved.
 */

#include "openxds.core.adt.std/StdString.h"

#include <openxds.core.base/CRuntime.h>

#include <string.h>
#include <stdio.h>

static bool IsWhitespace( char c );

struct _StdString
{
	IString     super;
	char*       data;
	int         length;
};

static StdString* construct_StdString()
{
	StdString* self              = CRuntime_calloc( 1, sizeof( StdString ) );
	self->super.free             = (IString*    (*)(IString*))                                 free_StdString;
	self->super.copy             = (IString*    (*)(const IString*))                           StdString_copy;
	self->super.substring        = (IString*    (*)(const IString*, int, int))                 StdString_substring;
	self->super.removeWhitespace = (IString*    (*)(const IString*))                           StdString_removeWhitespace;
	self->super.between          = (IString*    (*)(const IString*, const char*, const char*)) StdString_between;
	self->super.token            = (IString*    (*)(const IString*, int, const char*))         StdString_token;

	self->super.basename         = (IString*    (*)(const IString*, char ifs))                 StdString_basename;
	self->super.dirname          = (IString*    (*)(const IString*, char ifs))                 StdString_dirname;

	self->super.charAt           = (char        (*)(const IString*, int))                      StdString_charAt;
	self->super.getChars         = (const char* (*)(const IString*))                           StdString_getChars;

	self->super.contains         = (bool        (*)(const IString*, const char*))              StdString_contains;
	self->super.startsWith       = (bool        (*)(const IString*, const char*))              StdString_startsWith;
	self->super.endsWith         = (bool        (*)(const IString*, const char*))              StdString_endsWith;
	self->super.matches          = (bool        (*)(const IString*, const char*))              StdString_matches;

	self->super.compare          = (int         (*)(const IString*, const char*))              StdString_compare;
	self->super.getLength        = (int         (*)(const IString*))                           StdString_getLength;
	self->super.indexOfNext      = (int         (*)(const IString*, int, const char*))         StdString_indexOfNext;
	return self;
}

StdString* new_StdString( const char* str )
{
	StdString* self = construct_StdString();
	self->length    = strlen( str );
	self->data      = CRuntime_malloc( (self->length + 1) * sizeof( char ) );
	strncpy( self->data, str, self->length + 1 );
	return self;
}

StdString* new_StdString_start_length( const char* str, int start, int length )
{
	int i;
	StdString* self = construct_StdString();
	self->length    = length;
	self->data      = CRuntime_malloc( (self->length + 1) * sizeof( char ) );
	
	for ( i=0; i < length; i++ )
	{
		if ( '\0' == str[i + start] )
		{
			self->length = i;
			break;
		}
		self->data[i] = str[i + start];
	}
	self->data[self->length] = '\0';
	return self;
}

StdString* free_StdString( StdString* self )
{
	self->data = CRuntime_free( self->data );
	return CRuntime_free( self );
}

StdString* StdString_copy( const StdString* self )
{
	return new_StdString_start_length( self->data, 0, self->length );
}

StdString* StdString_substring( const StdString* self, int start, int length )
{
	return new_StdString_start_length( self->data, start, length );
}

StdString* StdString_removeWhitespace( const StdString* self )
{
	int first;
	int last;
	int len;
	
	for ( first=0; first < self->length; first++ )
	{
		if ( !IsWhitespace( self->data[first] ) ) break;
	}
	
	for ( last = self->length - 1; last >= first; last-- )
	{
		if ( !IsWhitespace( self->data[last] ) ) break;
	}
	
	len = (last - first) + 1;
	return new_StdString_start_length( self->data, first, len );
}

StdString* StdString_between( const StdString* self, const char* prefix, const char* suffix )
{
	printf( "openxds::core::adt::std::StdString_between unimplemented\n" );
	abort();
}

StdString* StdString_token( const StdString* self, int start, const char* delimiter )
{
	printf( "openxds::core::adt::std::StdString_token unimplemented\n" );
	abort();
}

StdString* StdString_basename( const StdString* self, char ifs )
{
	printf( "openxds::core::adt::std::StdString_basename unimplemented\n" );
	abort();
}

StdString* StdString_dirname( const StdString* self, char ifs )
{
	printf( "openxds::core::adt::std::StdString_dirname unimplemented\n" );
	abort();
}

char StdString_charAt( const StdString* self, int i )
{
	if ( i < self->length )
	{
		return self->data[i];
	} else {
		return -1;
	}
}

const char* StdString_getChars( const StdString* self )
{
	return self->data;
}

bool StdString_contains( const StdString* self, const char* str )
{
	printf( "openxds::core::adt::std::StdString_contains unimplemented\n" );
	abort();
}

bool StdString_startsWith( const StdString* self, const char* str )
{
	printf( "openxds::core::adt::std::StdString_startsWith unimplemented\n" );
	abort();
}

bool StdString_endsWith( const StdString* self, const char* str )
{
	printf( "openxds::core::adt::std::StdString_endsWith unimplemented\n" );
	abort();
}

bool StdString_matches( const StdString* self, const char* str )
{
	printf( "openxds::core::adt::std::StdString_matches unimplemented\n" );
	abort();
}

int StdString_compare( const StdString* self, const char* str )
{
	return strcmp( self->data, str );
}

int StdString_getLength( const StdString* self )
{
	return self->length;
}

int StdString_indexOfNext( const StdString* self, int start, const char* delimiter )
{
	printf( "openxds::core::adt::std::StdString_indexOfNext unimplemented\n" );
	abort();
}


static bool IsWhitespace( char c )
{
	bool whitespace = 0;
	switch( c )
	{
		case ' ':
		case '\n':
		case '\t':
			whitespace = 1;
	}
	return whitespace;
}
