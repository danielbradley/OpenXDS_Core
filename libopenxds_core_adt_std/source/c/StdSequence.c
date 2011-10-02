#include <openxds.core/IObject.h>
#include <openxds.core.adt/IPosition.h>
#include <openxds.core.adt.std/StdIterator.h>
#include <openxds.core.adt.std/StdSequence.h>
#include <openxds.core.adt.std/StdADTFactory.h>
#include <openxds.core.adt.std/StdPIterator.h>
#include <openxds.core.adt.std/StdSequenceNode.h>
#include <openxds.core.base/CRuntime.h>

#include <stdio.h>

static void    shuffleUp( StdSequence* self, int rank );
static void  shuffleDown( StdSequence* self, int rank );
static void       expand( StdSequence* self );
static bool    checkRank( const StdSequence* self, int index );
static bool checkRankAdd( const StdSequence* self, int index );
static bool       isFull( const StdSequence* self );

struct _StdSequence
{
	ISequence super;
	Node** V;
	int    N;
	int    f;
	int    b;
	bool   freeIObjects;
};

static int toIndex( const StdSequence* self, int rank )
{
	return (self->f + rank) % self->N;
}

static int toRank( const StdSequence* self, int index )
{
	return (self->N + index - self->f) % self->N;
}

static int increment( const StdSequence* self, int index )
{
	return (index + 1) % self->N;
}

static int decrement( const StdSequence* self, int index )
{
	return (self->N + index - 1) % self->N;
}

StdSequence* new_StdSequence()
{
	StdSequence* self = CRuntime_calloc( 1, sizeof( StdSequence ) );
	
	self->super.free        = (ISequence* (*)( ISequence*     )) free_StdSequence;
	self->super.freeAll     = (ISequence* (*)( ISequence*     )) freeAll_StdSequence;
	
	self->super.setFreeIObjects = (   void        (*)(       ISequence*, bool flag            )) StdSequence_setFreeIObjects;
	
	/* Deque */
	self->super.addFirst     = (      void        (*)(       ISequence*, E*                   )) StdSequence_addFirst;
	self->super.addLast      = (      void        (*)(       ISequence*, E*                   )) StdSequence_addLast;
	self->super.removeFirst  = (      E*          (*)(       ISequence*                       )) StdSequence_removeFirst;
	self->super.removeLast   = (      E*          (*)(       ISequence*                       )) StdSequence_removeLast;
	self->super.getFirst     = (const E*          (*)( const ISequence*                       )) StdSequence_getFirst;
	self->super.getLast      = (const E*          (*)( const ISequence*                       )) StdSequence_getLast;

	/* List */
	self->super.insertFirst  = (const IPosition*  (*)(       ISequence*, E*                   )) StdSequence_insertFirst;
	self->super.insertLast   = (const IPosition*  (*)(       ISequence*, E*                   )) StdSequence_insertLast;
	self->super.insertBefore = (const IPosition*  (*)(       ISequence*, const IPosition*, E* )) StdSequence_insertBefore;
	self->super.insertAfter  = (const IPosition*  (*)(       ISequence*, const IPosition*, E* )) StdSequence_insertAfter;
	self->super.replace      = (      E*          (*)(       ISequence*, const IPosition*, E* )) StdSequence_replace;
	self->super.remove       = (      E*          (*)(       ISequence*, const IPosition*     )) StdSequence_remove;
	self->super.first        = (const IPosition*  (*)( const ISequence*                       )) StdSequence_first;
	self->super.last         = (const IPosition*  (*)( const ISequence*                       )) StdSequence_last;
	self->super.prev         = (const IPosition*  (*)( const ISequence*, const IPosition*     )) StdSequence_previous;
	self->super.previous     = (const IPosition*  (*)( const ISequence*, const IPosition*     )) StdSequence_previous;
	self->super.next         = (const IPosition*  (*)( const ISequence*, const IPosition*     )) StdSequence_next;
	self->super.positions    = (      IPIterator* (*)( const ISequence*                       )) StdSequence_positions;
	self->super.elements     = (      IIterator*  (*)( const ISequence*                       )) StdSequence_elements;
	
	/* Vector */
	self->super.add          = (      void        (*)(       ISequence*, int, E*              )) StdSequence_add;
	self->super.set          = (      E*          (*)(       ISequence*, int, E*              )) StdSequence_set;
	self->super.removeFrom   = (      E*          (*)(       ISequence*, int                  )) StdSequence_removeFrom;
	self->super.get          = (const E*          (*)( const ISequence*, int                  )) StdSequence_get;
	
	/* Bridging */
	self->super.rankOf       = (      int         (*)( const ISequence*, const IPosition*     )) StdSequence_rankOf;
	self->super.atRank       = (const IPosition*  (*)( const ISequence*, int                  )) StdSequence_atRank;
	
	/* Common */
	self->super.size         = (      int         (*)( const ISequence*                       )) StdSequence_size;
	self->super.isEmpty      = (      bool        (*)( const ISequence*                       )) StdSequence_isEmpty;
	
	self->N = 1;
	self->V = CRuntime_calloc( self->N, sizeof( Node* ) );
	self->f = 0;
	self->b = 0;
	self->freeIObjects = 0;
	
	return self;
}

StdSequence* free_StdSequence( StdSequence* self )
{
	int i;
	int N = self->N;
	for ( i=0; i < N; i++ )
	{
		Node* v = self->V[i];
		if ( v )
		{
			if ( v->e )
			{
				if ( self->freeIObjects )
				{
					IObject* obj = (IObject*) v->e;
					v->e = obj->free( obj );
				}
				else
				{
					v->e = CRuntime_free( v->e );
				}
			}
			CRuntime_free( v );
		}
	}
	CRuntime_free( self->V );
	return CRuntime_free( self );
}

StdSequence* freeAll_StdSequence( StdSequence* self )
{
	int i;
	int N = self->N;
	for ( i=0; i < N; i++ )
	{
		Node* v = self->V[i];
		if ( v )
		{
			if ( v->e )
			{
				IObject* obj = (IObject*) v->e;
				v->e = obj->free( obj );
			}
			CRuntime_free( v );
		}
	}
	CRuntime_free( self->V );
	return CRuntime_free( self );
}

void StdSequence_setFreeIObjects( StdSequence* self, bool flag )
{
	self->freeIObjects = flag;
}

int StdSequence_size( const StdSequence* self )
{
	return (self->N - self->f + self->b) % self->N;
}

int StdSequence_isEmpty( const StdSequence* self )
{
	return (self->f == self->b);
}

/*
static int toRank( StdSequence* self, int index )
{

}
*/

E* StdSequence_set( StdSequence* self, int r, E* e )
{
	E* ret = NULL;

	if ( checkRank( self, r ) )
	{
		int   i = toIndex( self, r ); /* (self->f + r) % self->N; */
		ret = self->V[i]->e;
		self->V[i]->e = e;
	}
	
	return ret;
}

IPosition* StdSequence_insertFirst( StdSequence* self, E* e )
{
	if ( isFull( self ) ) expand( self );
	
	self->f = decrement( self, self->f ); /* (self->N + self->f - 1) % self->N; */
	
	self->V[self->f] = new_StdSequenceNode( e, self->f );
	
	return (IPosition*) self->V[self->f];
}

IPosition* StdSequence_insertLast( StdSequence* self, E* e )
{
	IPosition* ret = NULL;
	
	if ( isFull( self ) ) expand( self );
	
	self->V[self->b] = new_StdSequenceNode( e, self->b );
	ret = (IPosition*) self->V[self->b];

	self->b = increment( self, self->b ); /* (self->b + 1) % self->N; */
	
	return ret;
}

E* StdSequence_removeFirst( StdSequence* self )
{
	E* e = self->V[self->f]->e;
	self->V[self->f] = CRuntime_free( self->V[self->f] );
	
	self->f = increment( self, self->f ); /* (self->f + 1) % self->N; */

	return e;
}

E* StdSequence_removeLast( StdSequence* self )
{
	E* e = NULL;
	int size = StdSequence_size( self );
	if ( 0 < size )
	{
		int r = size - 1;
		int i = toIndex( self, r );
		
		e          = StdSequenceNode_replaceElement( self->V[i], NULL );
		self->V[i] = free_StdSequenceNode( self->V[i] );
		self->b    = i;
	}
	return e;
}

void StdSequence_add( StdSequence* self, int r, E* e )
{
	checkRankAdd( self, r );
	
	if ( isFull( self ) ) expand( self );
	
	if ( 0 == r )
	{
		StdSequence_insertFirst( self, e );
	}
	else
	{
		int i = toIndex( self, r );
		shuffleUp( self, r );
		self->V[i] = new_StdSequenceNode( e, i );
		self->b    = increment( self, self->b );
	}
}

E* StdSequence_removeFrom( StdSequence* self, int r )
{
	E* ret = NULL;
	if ( checkRank( self, r ) )
	{
		if ( 0 == r )
		{
			ret = StdSequence_removeFirst( self );
		}
		else
		{
			int i = toIndex( self, r );
			ret   =  StdSequenceNode_replaceElement( self->V[i], NULL );
			self->V[i] = free_StdSequenceNode( self->V[i] );
			shuffleDown( self, r );
			self->b = decrement( self, self->b );
		}
	}
	return ret;
}

const E* StdSequence_get( const StdSequence* self, int r )
{
	const E* ret = NULL;
	if ( checkRank( self, r ) )
	{
		int i = toIndex( self, r );
		ret   = StdSequenceNode_getElement( self->V[i] );
	}
	return ret;
}

int StdSequence_rankOf( const StdSequence* self, const IPosition* p )
{
	StdSequenceNode* n = (StdSequenceNode*) p;
	int i   = StdSequenceNode_getIndex( n ); 
	return toRank( self, i ); /* (self->N + n->i - self->f) % self->N; */
}

const IPosition* StdSequence_atRank( const StdSequence* self, int r )
{
	const IPosition* p = NULL;
	if ( checkRank( self, r ) )
	{
		int i = toIndex( self, r );
		p = (const IPosition*) self->V[i];
	}
	return p;
}

const IPosition* StdSequence_insertBefore( StdSequence* self, const IPosition* p, E* e )
{
	int r = StdSequence_rankOf( self, p );
	StdSequence_add( self, r, e );
	return StdSequence_atRank( self, r );
}

const IPosition* StdSequence_insertAfter( StdSequence* self, const IPosition* p, E* e )
{
	int r = StdSequence_rankOf( self, p );
	StdSequence_add( self, r + 1, e );
	return StdSequence_atRank( self, r + 1 );
}

E* StdSequence_replace( StdSequence* self, const IPosition* p, E* e )
{
	int r = StdSequence_rankOf( self, p );
	return StdSequence_set( self, r, e );
}

E* StdSequence_remove( StdSequence* self, const IPosition* p )
{
	int r = StdSequence_rankOf( self, p );
	return StdSequence_removeFrom( self, r );
}

const IPosition* StdSequence_first( const StdSequence* self )
{
	return StdSequence_atRank( self, 0 );
}

const IPosition* StdSequence_last( const StdSequence* self )
{
	int n = StdSequence_size( self );
	return StdSequence_atRank( self, n - 1 );
}

const E* StdSequence_getFirst( const StdSequence* self )
{
	return StdSequence_get( self, 0 );
}

const E* StdSequence_getLast( const StdSequence* self )
{
	int n = StdSequence_size( self );
	return StdSequence_get( self, n - 1 );
}

const IPosition* StdSequence_previous( const StdSequence* self, const IPosition* p )
{
	int r = StdSequence_rankOf( self, p );
	return StdSequence_atRank( self, r - 1 );
}

const IPosition* StdSequence_next( const StdSequence* self, const IPosition* p )
{
	int r = -1;
	if ( p ) r = StdSequence_rankOf( self, p );
	return StdSequence_atRank( self, r + 1 );
}

void StdSequence_addFirst( StdSequence* self, E* e )
{
	StdSequence_insertFirst( self, e );
}

void StdSequence_addLast( StdSequence* self, E* e )
{
	StdSequence_insertLast( self, e );
}

IPIterator* StdSequence_positions( const StdSequence* self )
{
	StdPIterator* positions = new_StdPIterator();
	
	int n = StdSequence_size( self );
	int r;
	const IPosition* p = NULL;
	for ( r=0; r < n; r++ )
	{
		p = StdSequence_atRank( self, r );
		StdPIterator_addPosition( positions, p );
	}
	return (IPIterator*) positions;
}

IIterator* StdSequence_elements( const StdSequence* self )
{
	StdIterator* elements = new_StdIterator();

	int n = StdSequence_size( self );
	int r;
	const IPosition* p = NULL;
	for ( r=0; r < n; r++ )
	{
		p = StdSequence_atRank( self, r );
		const void* e = p->getElement( p );
		StdIterator_addElement( elements, e );
	}

	return (IIterator*) elements;
}

void shuffleUp( StdSequence* self, int rank )
{
	int n = StdSequence_size( self );
	
	int r;
	for ( r=n-1; r >= rank; r-- )
	{
		int src  = toIndex( self, r ); /* (f + r) % N; */
		int dest = toIndex( self, r+1 ); /*(f + r + 1) % N; */
		
		self->V[dest] = self->V[src];
		self->V[dest]->i = dest;
		self->V[src] = NULL;
	}
}

void shuffleDown( StdSequence* self, int rank )
{
	int n = StdSequence_size( self );

	int r;
	for ( r=rank; r < n-1; r++ )
	{
		int dest = toIndex( self, r );
		int src  = toIndex( self, r+1 );
		
		self->V[dest] = self->V[src];
		self->V[dest]->i = dest;

		self->V[src] = NULL;
	}
}

void expand( StdSequence* self )
{
	int N = self->N;
	int N2 = N * 2;
	Node** V2 = CRuntime_calloc( N2, sizeof( Node* ) );

	int n = StdSequence_size( self );
	int r;
	for ( r=0; r < n; r++ )
	{
		int i = toIndex( self, r );
		V2[r] = self->V[i];
		V2[r]->i = r;
		self->V[i] = NULL;
	}

	CRuntime_free( self->V );
	self->V = V2;
	self->N = N2;
	self->f = 0;
	self->b = n;
}

bool checkRank( const StdSequence* self, int r )
{
	int n = StdSequence_size( self );

	if ( (r < 0) || (n <= r ) )
	{
/*		fprintf( stderr, "StdSequence::checkRank: invalid rank." ); */
/*		abort(); */
		return 0;
	} else {
		return 1;
	}
}

bool checkRankAdd( const StdSequence* self, int r )
{
	int n = StdSequence_size( self );

	if ( (r < 0) || (n < r ) )
	{
/*		fprintf( stderr, "StdSequence::checkRankAdd: invalid rank." ); */
/*		abort(); */
		return 0;
	} else {
		return 1;
	}
}

bool isFull( const StdSequence* self )
{
	int n = StdSequence_size( self );

	return (n == (self->N - 1));
}
