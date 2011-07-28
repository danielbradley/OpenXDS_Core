/*
 *  Copyright (C) 2004-2009 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENXDS_CORE_THREADS_THREAD_H
#define OPENXDS_CORE_THREADS_THREAD_H

#include <openxds.core/export.h>
#include <openxds.core.base.h>
#include <openxds.core.threads.h>

#ifdef __cplusplus
	namespace openxds {
		namespace core {
			namespace threads {
	extern "C" {
#endif

EXPORT	Thread* new_Thread( void*(*callback)(void*), void* aRunnable );
EXPORT	void free_Thread( Thread* self );

EXPORT	bool Thread_run( Thread* self );
EXPORT	bool Thread_wait( Thread* self );

//EXPORT  int  Thread_status( const IThread* self );

#ifdef __cplusplus
}
};};};
#endif

#endif
