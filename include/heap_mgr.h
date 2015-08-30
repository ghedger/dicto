/* Dicto
 * heap_mgr.h
 *
 * Copyright (C) 2015 Gregory P. Hedger
 * greg@hedgersoftware.com
 * 30329 112th Pl. SE
 * Auburn, WA 98092
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Also add information on how to contact you by electronic and paper mail.
 *
 */

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


