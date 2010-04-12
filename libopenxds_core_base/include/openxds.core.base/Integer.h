/*
 *  Copyright (c) 2008-2010 Daniel Robert Bradley. All rights reserved.
 */

#ifndef OPENXDS_CORE_BASE_INTEGER_H
#define OPENXDS_CORE_BASE_INTEGER_H


#include "openxds.core/export.h"
#include "openxds.core/types.h"
#include "openxds.core.base.h"

	#ifdef __cplusplus
namespace openxds {
	namespace core {
		namespace base {
	#endif

EXPORT	Integer*      new_Integer( int aValue );
EXPORT  Integer*     free_Integer( Integer* self );
EXPORT  void     Integer_setValue( Integer* self, int aValue );
EXPORT  int      Integer_getValue( const Integer* self );

	#ifdef __cplusplus
};};};
	#endif
#endif
