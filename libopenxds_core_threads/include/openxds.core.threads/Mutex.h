/*
 *  Copyright (C) 2004-2009 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENXDS_CORE_THREADS_MUTEX_H
#define OPENXDS_CORE_THREADS_MUTEX_H

#include <openxds.core/export.h>
#include <openxds.core.base.h>
#include <openxds.core.threads.h>

#ifdef __cplusplus
	namespace openxds {
		namespace core {
			namespace threads {
	extern "C" {
#endif

EXPORT	Mutex* new_Mutex();
EXPORT	void free_Mutex( Mutex* self );

EXPORT	bool Mutex_lock( Mutex* self );
EXPORT	bool Mutex_tryLock( Mutex* self );
EXPORT	bool Mutex_unlock( Mutex* self );

EXPORT	bool Mutex_isValid( Mutex* self );

#ifdef __cplusplus
}
};};};
#endif

#endif
