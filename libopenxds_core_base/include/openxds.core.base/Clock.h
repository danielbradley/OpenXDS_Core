/*
 *  Copyright (c) 2004-2010 Daniel Robert Bradley. All rights reserved.
 */

#ifndef OPENXDS_CORE_BASE_CLOCK_H
#define OPENXDS_CORE_BASE_CLOCK_H

#include "openxds.core/export.h"
#include "openxds.core.base.h"

	#ifdef __cplusplus
		namespace openxds {
			namespace core {
				namespace base {
					extern "C" {
	#endif


struct _IClock
{
	            IClock*                       (*free)(       IClock* self );
	              void                       (*start)(       IClock* self );
	              void                        (*stop)(       IClock* self );
	unsigned long long                 (*getInterval)( const IClock* self );
	               int                (*getTimeAsInt)();
	     unsigned long                 (*getUserTime)();
	     unsigned long               (*getSystemTime)();
	     unsigned long                   (*getUptime)();
	unsigned long long   (*getMicroSecondsSinceEpoch)();
	unsigned long long        (*getSecondsSinceEpoch)();
	     unsigned long          (*getClockCyclesUsed)();
};

EXPORT	IClock*            new_Clock();

EXPORT	int                Clock_GetTimeAsInt();
EXPORT	unsigned long      Clock_GetUserTime();
EXPORT	unsigned long      Clock_GetSystemTime();
EXPORT	unsigned long      Clock_GetUptime();

EXPORT	unsigned long long Clock_GetMicroSecondsSinceEpoch();
EXPORT	unsigned long long Clock_GetSecondsSinceEpoch();

EXPORT	unsigned long      Clock_GetClockCyclesUsed(); 

	#ifdef __cplusplus
		};};};};
	#endif
#endif
