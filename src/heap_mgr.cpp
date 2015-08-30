// HeapMgr
//
// This class manages a heap of homogeneous blocks of memory
// for fast allocation.
//
// The class does not at present perform dynamic reallocation upon
// exhaustion of its node pool.
//
#include <assert.h>
#include <stdlib.h>
#include "heap_mgr.h"

template <class c_t> HeapMgr <c_t>::HeapMgr()
{
    throw heapMgrException;
}

template <class c_t> HeapMgr <c_t>::HeapMgr( const int blockSize, const int reserve )
{
    _pool = ( void * ) new char[ blockSize * reserve ];
    _nodeMax = reserve;
    assert( _pool );
    // TODO throw exception if NULL == _pool
}

template <class c_t> HeapMgr <c_t>::~HeapMgr()
{
    if( _pool )
    {
        delete _pool;
    }
}

template <class c_t> void HeapMgr <c_t>::initNodeStack( const int nodeTot )
{
    // Allocate the stack itself
    _nodeStack = new c_t *[ nodeTot ];
    assert( _nodeStack );
    _nodeStackPtr = 0;

    // Build the free node stack pending sanity check
    if( _nodeStack )
    {
        int i = 0;
        while( i < nodeTot )
        {
            pushNodeStack( _nodeStack + i++ );
        }
    }
    else
    {
        throw heapMgrException;
    }
}

template <class c_t> void HeapMgr <c_t>::shutdownNodeStack()
{
    if( _nodeStack )
    {
        delete _nodeStack;
        _nodeStack = NULL;
    }
}

template <class c_t> void HeapMgr <c_t>::pushNode( c_t *pNode )
{
    if( _nodeStackPtr < _nodeMax )
        _nodeStack[ _nodeStackPtr++ ] = pNode;
    else
        throw( heapMgrException );
}

template <class c_t> c_t *HeapMgr <c_t>::popNode()
{
    c_t *pNode = NULL;
    if( _nodeStackPtr )
        pNode = _nodeStack[ --_nodeStackPtr ];
    return pNode;
}

template <class c_t> c_t *HeapMgr <c_t>::allocNode()
{
    c_t *pNode = popNode();
    if( !pNode )
        throw heapMgrException;
    return pNode;
}

template <class c_t> void HeapMgr <c_t>::freeNode( c_t *pNode )
{
    if( pNode )
    {
        pushNode( pNode );
    }
    else
    {
        throw( heapMgrException );
    }
}

