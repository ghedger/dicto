#pragma once

#include <exception>

class HeapMgrException: public std::exception
{
  virtual const char* what() const throw()
  {
    return "HeapMgrException occurred...";
  }
};

// This HeapMgr is designed to manage a heap of homogeneous blocks of size x.
// It uses a node stack for lightning quick allocs and frees.
//
// It does not at present grow the pool.
template <class c_t> class HeapMgr
{
public:
	HeapMgr();
	HeapMgr( const int blockSize, const int reserve );
	~HeapMgr();
    c_t * allocNode();
    void freeNode( c_t *pNode );

protected:
    // Each node is a block
    void initNodeStack( const int nodeTot );
    void shutdownNodeStack();
    void pushNode( c_t *pNode );
    c_t * popNode();

    c_t *  _pool;
    c_t ** _nodeStack;
    int     _nodeStackPtr;
    int     _nodeMax;

    HeapMgrException heapMgrException;
};


