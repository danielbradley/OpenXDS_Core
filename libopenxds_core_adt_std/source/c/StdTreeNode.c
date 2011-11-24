/*
 *  Copyright (C) 2007 Daniel Robert Bradley. All rights reserved.
 */

#include "openxds.core.adt/IPIterator.h"
#include "openxds.core.adt.std/StdADTFactory.h"
#include "openxds.core.adt.std/StdPIterator.h"
#include "openxds.core.adt.std/StdTreeNode.h"

#include <openxds.core.adt/ISequence.h>
#include <openxds.core.base/CRuntime.h>

struct _TreeNode
{
	IPosition  super;
	TreeNode*  parent;
	ISequence* children;
	IObject*   element;
};

TreeNode* new_StdTreeNode( TreeNode* parent, IObject* anElement )
{
	TreeNode* self = CRuntime_calloc( 1, sizeof( TreeNode ) );
	self->super.super.free = (void*    (*)(       IObject*   )) free_StdTreeNode;
	self->super.free       = (void*    (*)(       IPosition* )) free_StdTreeNode;
	self->super.getElement = (const E* (*)( const IPosition* )) StdTreeNode_getElement;

	self->parent   = parent;
	self->children = StdADTFactory_createSequence();
	self->children->setFreeIObjects( self->children, TRUE );
	self->element  = anElement;
	return self;
}

void* free_StdTreeNode( TreeNode* self )
{
	if ( self->element ) self->element  = CRuntime_free( self->element );
	self->children = self->children->free( self->children );
	self->parent = NULL;
	return CRuntime_free( self );
}

void* freeAll_StdTreeNode( TreeNode* self )
{
	if ( self->element )
	{
		IObject* obj  = (IObject*) self->element;
		self->element = obj->free( obj );
	}
	self->children = self->children->freeAll( self->children );
	return CRuntime_free( self );
}

/*	IPosition */
const IObject* StdTreeNode_getElement( const TreeNode* self )
{
	return self->element;
}

TreeNode* StdTreeNode_getParent( TreeNode* self )
{
	return self->parent;
}

TreeNode* StdTreeNode_getChild( TreeNode* self, long i )
{
	return (TreeNode*) self->children->get( self->children, (int) i );
}

IObject* StdTreeNode_setElement( TreeNode* self, IObject* anElement )
{
	IObject* ret  = self->element;
	self->element = anElement;
	return ret;
}

TreeNode* StdTreeNode_addChild( TreeNode* self, IObject* anElement )
{
	int size = self->children->size( self->children );
	TreeNode* child = new_StdTreeNode( self, anElement );
	self->children->add( self->children, size, child );
	return child;
}

TreeNode* StdTreeNode_addNode( TreeNode* self, TreeNode* child )
{
	child->parent = self;
	self->children->insertLast( self->children, child );
	return child;
}

E* StdTreeNode_replace( TreeNode* self, E* e )
{
	E* ret = self->element;
	self->element = e;
	return ret;
}

E* StdTreeNode_extract( TreeNode* self )
{
	E* element = NULL;

	if ( self->children->isEmpty( self->children ) )
	{
		E* node = NULL;
		{
			if ( self->parent )
			{
				ISequence* seq = self->parent->children;
				IPIterator* it = seq->positions( seq );
				while ( it->hasNext( it ) )
				{
					const IPosition* p = it->next( it );
					const E* e = p->getElement( p );

					if ( e == self )
					{
						node = (TreeNode*) seq->remove( seq, p );
						break;
					}
				}
				it->free( it );
			}
			else
			{
				node = (TreeNode*) self;
			}
		}
		if ( node )
		{
			element = StdTreeNode_replace( node, NULL );
			free_StdTreeNode( node );
		}
	}
	return element;
}

TreeNode* StdTreeNode_extractWithChildren( TreeNode* self )
{
	TreeNode* node = NULL;
	{
		if ( self->parent )
		{
			ISequence* seq = self->parent->children;
			IPIterator* it = seq->positions( seq );
			while ( it->hasNext( it ) )
			{
				const IPosition* p = it->next( it );
				const E* e = p->getElement( p );
				if ( e == self )
				{
					node = (TreeNode*) seq->remove( seq, p );
					node->parent = NULL;
					break;
				}
			}
			it->free( it );
		} else {
			node = self;
		}
	}
	return node;
}

bool
StdTreeNode_isExternal( const TreeNode* self )
{
	return self->children->isEmpty( self->children );
}

bool
StdTreeNode_hasParent( const TreeNode* self )
{
	return (NULL != self->parent);
}

bool
StdTreeNode_hasChild( const TreeNode* self, long i )
{
	return (i < self->children->size( self->children ));
}

long
StdTreeNode_nrChildren( const TreeNode* self )
{
	return self->children->size( self->children );
}

IPIterator* StdTreeNode_children( const TreeNode* self )
{
	StdPIterator* it = new_StdPIterator();
	int max = self->children->size( self->children );
	int i;
	for ( i=0; i < max; i++ )
	{
		TreeNode* node = (TreeNode*) self->children->get( self->children, i );
		StdPIterator_addPosition( it, (IPosition*) node );
	}
	return (IPIterator*) it;
}

int
StdTreeNode_countDescendents( const TreeNode* self )
{
	int nr = 1;

	IPIterator* it = StdTreeNode_children( self );
	while ( it->hasNext( it ) )
	{
		TreeNode* node = (TreeNode*) it->next( it );
		nr += StdTreeNode_countDescendents( node );
	}
	it->free( it );
	
	return nr;
}




