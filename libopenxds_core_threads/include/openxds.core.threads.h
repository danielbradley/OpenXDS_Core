/*
 *  Copyright (c) 2008-2010 Daniel Robert Bradley. All rights reserved.
 */

#ifndef OPENXDS_CORE_THREADS_H
#define OPENXDS_CORE_THREADS_H

	#ifdef __cplusplus
namespace openxds {
	namespace core {
		namespace threads {
	#endif

	typedef struct _ConditionVariable ConditionVariable;
	typedef struct _Mutex             Mutex;
	typedef struct _Semaphore         Semaphore;
	typedef struct _Thread            Thread;
	
	#ifdef __cplusplus
};};};
	#endif
#endif
