/*
 *  Copyright (C) 2004-2009 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENXDS_CORE_THREADS_CONDITIONVARIABLE_H
#define OPENXDS_CORE_THREADS_CONDITIONVARIABLE_H

#include "openxds.core.threads/Mutex.h"
#include <openxds.core.base.h>
#include <openxds.core.threads.h>

#ifdef __cplusplus
	namespace openxds {
		namespace core {
			namespace threads {
	extern "C" {
#endif

EXPORT	ConditionVariable* new_ConditionVariable();
EXPORT	void free_ConditionVariable( ConditionVariable* self );

EXPORT	bool ConditionVariable_wait( ConditionVariable* self, Mutex* aMutex );
EXPORT	bool ConditionVariable_timedWait( ConditionVariable* self, Mutex* aMutex, int seconds );
EXPORT	bool ConditionVariable_signal( ConditionVariable* self );
EXPORT	bool ConditionVariable_broadcast( ConditionVariable* self );

//EXPORT	IConditionVariable* ConditionVariable_copy( IConditionVariable* self );

#ifdef __cplusplus
}
};};};
#endif

#endif
