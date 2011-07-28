#include <openxds.core/IObject.h>
#include <openxds.core.adt/IPosition.h>
#include <openxds.core.adt.std/StdSequence.h>
#include <openxds.core.adt.std/StdADTFactory.h>
#include <openxds.core.adt.std/StdPIterator.h>
#include <openxds.core.base/CRuntime.h>

typedef struct _Node Node;

static void    shuffleUp( StdSequence* self, int rank );
static void  shuffleDown( StdSequence* self, int rank );
static void       expand( StdSequence* self );
static void    checkRank( const StdSequence* self, int index );
static void checkRankAdd( const StdSequence* self, int index );
static bool       isFull( const StdSequence* self );

static const E* Node_getElement( const Node* self );

struct _Node
{
	IPosition super;
	E*  e;
	int i;
};

struct _StdSequence
{
	ISequence super;
	Node** V;
	int    N;
	int    f;
	int    b;
};

Node* new_Node( E* anElement, int anIndex )
{
	Node* self = CRuntime_calloc( 1, sizeof( Node ) );
	self->super.getElement = (const E* (*)( const IPosition* )) Node_getElement;
	self->e = anElement;
	self->i = anIndex;
	return self;
}

const E*
Node_getElement( const Node* self )
{
	return self->e;
}

StdSequence* new_StdSequence()
{
	StdSequence* self = CRuntime_calloc( 1, sizeof( StdSequence ) );
	
	self->super.free        = (ISequence* (*)( ISequence*     )) free_StdSequence;
	
	// Deque
	self->super.addFirst     = (      void        (*)(       ISequence*, E*                   )) StdSequence_addFirst;
	self->super.addLast      = (      void        (*)(       ISequence*, E*                   )) StdSequence_addLast;
	self->super.removeFirst  = (      E*          (*)(       ISequence*                       )) StdSequence_removeFirst;
	self->super.removeLast   = (      E*          (*)(       ISequence*                       )) StdSequence_removeLast;
	self->super.getFirst     = (const E*          (*)( const ISequence*                       )) StdSequence_getFirst;
	self->super.getLast      = (const E*          (*)( const ISequence*                       )) StdSequence_getLast;

	// List
	self->super.insertFirst  = (const IPosition*  (*)(       ISequence*, E*                   )) StdSequence_insertFirst;
	self->super.insertLast   = (const IPosition*  (*)(       ISequence*, E*                   )) StdSequence_insertLast;
	self->super.insertBefore = (const IPosition*  (*)(       ISequence*, const IPosition*, E* )) StdSequence_insertBefore;
	self->super.insertAfter  = (const IPosition*  (*)(       ISequence*, const IPosition*, E* )) StdSequence_insertAfter;
	self->super.replace      = (const E*          (*)(       ISequence*, const IPosition*, E* )) StdSequence_replace;
	self->super.remove       = (const E*          (*)(       ISequence*, const IPosition*     )) StdSequence_remove;
	self->super.first        = (const IPosition*  (*)( const ISequence*                       )) StdSequence_first;
	self->super.last         = (const IPosition*  (*)( const ISequence*                       )) StdSequence_last;
	self->super.prev         = (const IPosition*  (*)( const ISequence*, const IPosition*     )) StdSequence_previous;
	self->super.previous     = (const IPosition*  (*)( const ISequence*, const IPosition*     )) StdSequence_previous;
	self->super.next         = (const IPosition*  (*)( const ISequence*, const IPosition*     )) StdSequence_next;
	self->super.positions    = (      IPIterator* (*)( const ISequence*                      )) StdSequence_positions;
	
	// Vector
	self->super.add          = (      void        (*)(       ISequence*, int, E*              )) StdSequence_add;
	self->super.set          = (      E*          (*)(       ISequence*, int, E*              )) StdSequence_set;
	self->super.removeFrom   = (      E*          (*)(       ISequence*, int                  )) StdSequence_removeFrom;
	self->super.get          = (const E*          (*)( const ISequence*, int                  )) StdSequence_get;
	
	// Bridging
	self->super.rankOf       = (      int         (*)( const ISequence*, const IPosition*     )) StdSequence_rankOf;
	self->super.atRank       = (const IPosition*  (*)( const ISequence*, int                  )) StdSequence_atRank;
	
	// Common
	self->super.size         = (      int         (*)( const ISequence*                       )) StdSequence_size;
	self->super.isEmpty      = (      bool        (*)( const ISequence*                       )) StdSequence_isEmpty;
	
	self->N = 1;
	self->V = CRuntime_calloc( self->N, sizeof( Node* ) );
	self->f = 0;
	self->b = 0;
	
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
			if ( v->e ) v->e = CRuntime_free( v->e );
			CRuntime_free( v );
		}
	}
	CRuntime_free( self->V );
	CRuntime_free( self );
}

int StdSequence_size( const StdSequence* self )
{
	return (self->N - self->f + self->b) % self->N;
}

int StdSequence_isEmpty( const StdSequence* self )
{
	return (self->f == self->b);
}

E* StdSequence_set( StdSequence* self, int r, E* e )
{
	E* ret = NULL;
	checkRank( self, r );

	int   i = (self->f + r) % self->N;
	ret = self->V[i]->e;
	self->V[i]->e = e;
	
	return ret;
}

IPosition* StdSequence_insertFirst( StdSequence* self, E* e )
{
	if ( isFull( self ) ) expand( self );
	
	self->f = (self->N + self->f - 1) % self->N;
	
	self->V[self->f] = new_Node( e, self->f );
	
	return (IPosition*) self->V[self->f];
}

IPosition* StdSequence_insertLast( StdSequence* self, E* e )
{
	IPosition* ret = NULL;
	
	if ( isFull( self ) ) expand( self );
	
	self->V[self->b] = new_Node( e, self->b );
	ret = (IPosition*) self->V[self->b];

	self->b = (self->b + 1) % self->N;
	
	return ret;
}

E* StdSequence_removeFirst( StdSequence* self )
{
	E* e = self->V[self->f]->e;
	self->V[self->f] = CRuntime_free( self->V[self->f] );
	
	self->f = (self->f + 1) % self->N;

	return e;
}

E* StdSequence_removeLast( StdSequence* self )
{
	int r = StdSequence_size( self ) - 1;
	int i = (self->f + r) % self->N;
	
	E* e = self->V[i]->e;
	
	self->V[i] = CRuntime_free( self->V[i] );
	
	self->b = i;
	
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
		shuffleUp( self, r );
		
		Node* n = CRuntime_calloc( 1, sizeof( Node ) );
		n->e = e;
		n->i = (self->f + r) % self->N;

		self->V[n->i] = n;
		self->b = (self->b + 1) % self->N;
	}
}

E* StdSequence_removeFrom( StdSequence* self, int r )
{
	E* ret = NULL;
	checkRank( self, r );
	
	if ( 0 == r )
	{
		ret = StdSequence_removeFirst( self );
	}
	else
	{
		int i = (self->f + r) % self->N;
		E* e = self->V[i]->e;
		CRuntime_free( self->V[i] );
		
		shuffleDown( self, r );
		self->b = (self->N + self->b - 1) % self->N;
	}
	return ret;
}

const E* StdSequence_get( const StdSequence* self, int r )
{
	checkRank( self, r );
	
	int i = (self->f + r) % self->N;
	return self->V[i]->e;
}

int StdSequence_rankOf( const StdSequence* self, const IPosition* p )
{
	Node* n = (Node*) p;
	return (self->N + n->i - self->f) % self->N;
}

const IPosition* StdSequence_atRank( const StdSequence* self, int r )
{
	checkRank( self, r );

	int i = (self->f + r) % self->N;
	return (IPosition*) self->V[i];
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
	int i;
	const IPosition* p = NULL;
	for ( i=0; i < n; i++ )
	{
		p = StdSequence_next( self, p );
		StdPIterator_addPosition( positions, p );
	}
	return (IPIterator*) positions;
}

void shuffleUp( StdSequence* self, int rank )
{
	int n = StdSequence_size( self );
	int f = self->f;
	int N = self->N;
	
	int r;
	for ( r=n-1; r >= rank; r-- )
	{
		int src  = (f + r) % N;
		int dest = (f + r + 1) % N;
		
		self->V[dest] = self->V[src];
		self->V[dest]->i = dest;

		self->V[src] = NULL;
	}
}

void shuffleDown( StdSequence* self, int rank )
{
	int n = StdSequence_size( self );
	int f = self->f;
	int N = self->N;
	
	int r;
	for ( r=r; r < n-1; r++ )
	{
		int dest = (f + r) % N;
		int src  = (f + r + 1) % N;
		
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
		int i = (self->f + r) % N;
		V2[r] = self->V[i];
		self->V[i] = NULL;
	}

	CRuntime_free( self->V );
	self->V = V2;
	self->N = N2;
	self->f = 0;
	self->b = n;
}

void checkRank( const StdSequence* self, int r )
{
	int n = StdSequence_size( self );

	if ( (r < 0) || (n <= r ) ) abort();
}

void checkRankAdd( const StdSequence* self, int r )
{
	int n = StdSequence_size( self );

	if ( (r < 0) || (n < r ) ) abort();
}

bool isFull( const StdSequence* self )
{
	int n = StdSequence_size( self );

	return (n == (self->N - 1));
}
