/*
 *  Copyright (C) 2007 Daniel Robert Bradley. All rights reserved.
 */

#ifndef OPENXDS_CORE_ADT_STD_STDSEQUENCENODE_H
#define OPENXDS_CORE_ADT_STD_STDSEQUENCENODE_H

#include <openxds.core.adt/IPosition.h>

typedef struct _StdSequenceNode StdSequenceNode;

StdSequenceNode*  new_StdSequenceNode( E* anElement );
StdSequenceNode* free_StdSequenceNode( StdSequenceNode* self );

//	Implements openxds/core/adt/Position
const E* StdSequenceNode_getElement( const StdSequenceNode* self );

#endif
