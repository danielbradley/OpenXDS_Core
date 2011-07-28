/*
 *  Copyright (C) 2004-2009 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENXDS_CORE_THREADS_MUTEX_PROTECTED_H
#define OPENXDS_CORE_THREADS_MUTEX_PROTECTED_H

#include "openxds.core.base.h"
#include "openxds.core.threads/Mutex.h"

void* Mutex_getState( Mutex* self );

#endif
