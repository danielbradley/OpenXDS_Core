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
extern "C"
{
#endif

/*
 *  Lifecycle functions
 */
EXPORT	Clock* new_Clock();
EXPORT	void free_Clock( Clock* self );

/*
 *  Functions
 */
EXPORT	void               Clock_start( Clock* self );
EXPORT	void               Clock_stop( Clock* self );
EXPORT	unsigned long long Clock_getInterval( const Clock* self );

/*
 *  Class Methods
 */
EXPORT	int           Clock_GetTimeAsInt();
EXPORT	unsigned long Clock_GetUserTime();
EXPORT	unsigned long Clock_GetSystemTime();
EXPORT	unsigned long Clock_GetUptime();

EXPORT	unsigned long long Clock_GetMicroSecondsSinceEpoch();
EXPORT	unsigned long long Clock_GetSecondsSinceEpoch();

EXPORT	unsigned long Clock_GetClockCyclesUsed(); 

#ifdef __cplusplus
}
};};};
#endif
#endif
