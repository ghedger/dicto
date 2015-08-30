#pragma once

#include <assert.h>

template <typename k_t, class n_c> class TemplNode
{
public:
    // constuctor
    TemplNode()
    {
        assert(0);  // Don't call
    }
    TemplNode( k_t key )
    {
        clear();
    }
    // destructor
    ~TemplNode() {}

    // getters/setters
    void setKey( k_t key ) { _key = key; }
    k_t getKey() { return _key; }
    void setParent( n_c *pParent ) { _parent = pParent; }
    n_c * getParent() { return _parent; }
    void setLeft( n_c *pNode ) { _l = pNode; }
    n_c * getLeft() { return _l; }
    void setRight( n_c *pNode ) { _r = pNode; }
    n_c * getRight() { return _r; }
    void setCenter( n_c *pNode ) { _c = pNode; }
    n_c * getCenter() { return _c; }
    void setTerminator() { _terminator = 1; }
    bool getTerminator() { return _terminator ? true : false; }
    void setUpper() { _upper = 1; }
    bool getUpper() { return _upper ? true : false; }

    // clear out node.
    void clear()
    {
        _parent = _l = _c = _r = NULL;
        _terminator = 0;
        _upper = 0;
    }

    // member variables
    n_c *       _parent;    // parent
    n_c *       _l;         // left (lo kid/less than)
    n_c *       _c;         // center (equal kid)
    n_c *       _r;         // right (hi kid/greater than)
    k_t         _key;       // key
    unsigned    _terminator: 1;
    unsigned    _upper: 1;
};
