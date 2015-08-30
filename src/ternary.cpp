// ternary
//
// This is a sample implementation on a ternary (tri-nary) tree.
//
// Copyright (C) 2014 Gregory P. Hedger

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <memory.h>

template <typename KeyType> class T_Node
{
public:
    // constuctor
    T_Node()
    {
        clear();
    }
    // destructor
    ~T_Node() {}

    // getters/setters
    void setKey( KeyType key )
    {
        _key = key;
    }
    KeyType getKey()
    {
        return _key;
    }
    void setParent( T_Node *pParent )
    {
        _parent = pParent;
    }
    T_Node * getParent()
    {
        return _parent;
    }
    void setLK( T_Node *pNode )
    {
        _left = pNode;
    }
    T_Node * getLeft()
    {
        return _left;
    }
    void setEK( T_Node *pNode )
    {
        _center = pNode;
    }
    T_Node * getCenter()
    {
        return _center;
    }
    void setHK( T_Node *pNode )
    {
        _right = pNode;
    }
    T_Node * getRight()
    {
        return _right;
    }

//protected:
    // clear out node.
    void clear()
    {
        _parent = NULL;
        _left = NULL;
        _center = NULL;
        _right = NULL;
        setKey( 0 );
    }

    // member variables
    T_Node * _parent; // parent
    T_Node * _left; // left (less than)
    T_Node * _center; // center (equal)
    T_Node * _right; // right
    KeyType _key; // key
friend class TernaryTree;
};

typedef T_Node <char> TNode;

class TernaryTree
{
    public:
        TernaryTree() {};
        ~TernaryTree() {};

        // insertNode
        // insert a node into the tree
        //
        // @In:     key key of node establishing relationship in tree
        //          pNew pointer to new node being added
        //          pParent pointer to parent being evaluated
        // @Out:    Pointer to node
        void insertNode( int key, TNode *pNew, TNode *pParent )
        {
            recurse( key, pNew, pParent );
        };

        // deleteNode
        // delete a node and update linkage
        //
        // @In:     pDel pointer to node to be deleted.
        // @Out:    -
        void deleteNode( TNode *pDel )
        {
            if( pDel )
            {
                TNode *pParent = pDel->getParent();

                // Are we deleting the root?
                if( pParent )
                {
                    // We are not root node.  Find our relationship to parent;
                    // (do not make assumption based on key relationship; check link)
                    if( pParent->_left == pDel )
                    {
                        // delete lo kid
                        unlink( pDel, pParent, &pParent->_left );
                    }
                    else if( pParent->_right == pDel )
                    {
                        // delete hi kid
                        unlink( pDel, pParent, &pParent->_right );
                    }
                    else
                    {
                        // delete equal kid
                        unlink( pDel, pParent, &pParent->_center );
                    }
                }
                pDel->clear();
            }
        };

        // link
        // links new child to its parent
        //
        // @In:     pNew pointer to new node being added
        //          pParent pointer to parent being evaluated
        //          ppLink pointer to parent's link (_left, _center, _right)
        // @Out:    -
        void link( TNode *pNew, TNode *pParent, TNode **ppLink )
        {
            *ppLink = pNew;
            pNew->setParent( pParent );
        }

        // unlink
        // remove a node from tree linkage
        //
        // @In:     pDel node to delete
        //          pParent parent node
        //          ppParentLink pointer to parent's link (_left, _center, _right)
        void unlink( TNode *pDel, TNode *pParent, TNode **ppParentLink )
        {
            // Update parent link to new child; we need to choose the
            // child to link from the parent since we can have up to three
            // yet we have only one parent link.
            //
            //           /
            //          5
            //         /|\
            //        3 5 8
            //       /|\
            //      2 3 4
            //         /
            //        x
            //
            //

            // Re-parent
            // If child has a center child, prefer that as parent link's new child
            if( pDel->_center )
            {
                *ppParentLink = NULL;   //pDel->_center;
                // Note that the other two nodes are now orphaned.  We will re-add them.
                // Save off our links first
                TNode *pl = pDel->_left;
                TNode *pc = pDel->_center;
                TNode *pr = pDel->_right;

                recurse( pc->getKey(), pc, pParent );
                if( pl )
                    recurse( pl->getKey(), pl, pParent );
                if( pr )
                    recurse( pr->getKey(), pr, pParent );
            }
            else if( pDel->_left )
            {
                *ppParentLink = NULL;   //pDel->_left;
                // Note that the other two nodes are now orphaned.  We will re-add them.
                // Save off our links first (coding practice: treat recurse as opaque,
                // destructive process).
                TNode *pl = pDel->_left;
                TNode *pc = pDel->_center;
                TNode *pr = pDel->_right;

                recurse( pl->getKey(), pl, pParent );
                if( pc )
                    recurse( pc->getKey(), pc, pParent );
                if( pr )
                    recurse( pr->getKey(), pr, pParent );
            }
            else if( pDel->_right )
            {
                *ppParentLink = NULL;   //pDel->_right;
                // Note that the other two nodes are now orphaned.  We will re-add them.
                // Save off our links first
                TNode *pl = pDel->_left;
                TNode *pc = pDel->_center;
                TNode *pr = pDel->_right;

                recurse( pr->getKey(), pr, pParent );
                if( pl )
                    recurse( pl->getKey(), pl, pParent );
                if( pc )
                    recurse( pc->getKey(), pc, pParent );
            }


            // Update child links to point to new parent
            if( pDel->_center )
            {
                if( pDel->_left )
                {
                    pDel->_left = pParent;
                }
                if( pDel->_right )
                {
                    pDel->_right = pParent;
                }
                if( pDel->_center )
                {
                    pDel->_center = pParent;
                }
            }
        }

        // evaluateLeg
        // Determines whether we've found a "home" on this leg of the parent.
        // If so, it updates linkage.  If not, it continues recursion.
        //
        // @In:     key key of node establishing relationship in tree
        //          pNew pointer to new node being added
        //          pParent pointer to parent being evaluated
        //          ppLink pointer to parent leg pointer (l, r, c)
        // @Out:    Pointer to node
        TNode *evaluateLeg( int key, TNode *pNew, TNode *pParent, TNode **ppLink )
        {
            TNode *pRet = NULL;
            if( !*ppLink )
            {
                // parent has no leg; this is our home
                // update linkage
                link( pNew, pParent, ppLink );

                pNew->setKey( key );
                pRet = pNew;
            }
            else
            {
                pRet = recurse( key, pNew, *ppLink );
            }
            return pRet;
        }

        // recurse
        // This recurses through the tree, adding the new node
        // at the appropriate spot.
        //
        // @In:     key key of node establishing relationship in tree
        //          pNew pointer to new node being added
        //          pParent pointer to parent being evaluated
        // @Out:    Pointer to node
        TNode *recurse( int key, TNode *pNew, TNode *pParent )
        {
            TNode *pRet = NULL;
            if( !pParent )
            {
                pNew->setKey( key );
                // this is the first node; return
            }
            else
            {
                if( key < pParent->getKey() )
                {
                    // we are less than parent (left leg)
                    pRet = evaluateLeg( key, pNew, pParent, &pParent->_left );
                }
                else if( key > pParent->getKey() )
                {
                    // we are greater than parent (right leg)
                    pRet = evaluateLeg( key, pNew, pParent, &pParent->_right );
                }
                else
                {
                    // we are equal to parent (center leg)
                    pRet = evaluateLeg( key, pNew, pParent, &pParent->_center );
                }
            }
            return pRet;
        }
};

// LEG is used for printTraversal to tell what leg the current node is on.
enum LEG
{
    LEG_L   = 0,
    LEG_C,
    LEG_R,
};

// We're going to render into a buffer
const int screenWidth = 80;
const int screenHeight = 8;
static char _renderBuf [ screenWidth * screenHeight ];

// printTraversal
// Render and prints a rudimentary treelike representation to console.
// Note this is rather rough, added at the end of this demo; it could be
//  improved.
//
// @In:     pRoot pointer to root node
//          leg left, right, or center
//          htab starting htab
// @Out:    -
void printTraversal( TNode *pNode, LEG leg, int htab, int depth = 0 )
{
    int vtab = depth;

    // Initialize render buf if at top
    if( !depth )
    {
        printf("\n");
        memset( _renderBuf, ' ', sizeof( _renderBuf ) );
        htab = screenWidth >> 1;        // set initial htab to center of rendering buffer
    }


    // Caculate screen layout
    switch( leg )
    {
        case LEG_L:
            htab -= 16 - (depth << 2);
            _renderBuf[ htab - 1 + ( vtab * screenWidth ) ] = '>';
            break;
       case LEG_R:
            htab += 16 - (depth << 2);
            _renderBuf[ htab - 1 + ( vtab * screenWidth ) ] = '<';
            break;
        case LEG_C:
            _renderBuf[ htab - 1 + ( vtab * screenWidth ) ] = '^';
            break;
        default:
            break;
    }

    printf("htab: %d depth: %d value: %d\n", htab, depth, pNode->getKey());

    sprintf( &_renderBuf[ htab + ( vtab * screenWidth ) ], "%d",  pNode->getKey());
    if( pNode->getLeft() )
    {
        printTraversal( pNode->getLeft(), LEG_L, htab, depth + 1 );
    }
    if( pNode->getCenter() )
    {
        printTraversal( pNode->getCenter(), LEG_C, htab, depth + 1 );
    }
    if( pNode->getRight() )
    {
        printTraversal( pNode->getRight(), LEG_R, htab, depth + 1 );
    }

    // If we're done, print out the rendered tree
    if( !depth )
    {
        int i, j;
        for( j = 0; j < screenHeight; j++ )
        {
            for( i = 0; i < screenWidth; i++ )
            {
                char c = _renderBuf[ i +  ( j * screenWidth )];
                // Replace terminator with space
                c = c ? c : ' ';
                putchar( c );
            }
            printf( "\n" );         // Print LF every screenWidth columns
        }
    }
}

// printNodeArr
// Iterate through and print flat, canonical representation of array of nodes.
//
// @In: startIdx index of first node
//      endIdx index of last node
//      pNodeArr pointer to node array
// @Out: -
void printNodeArr( int startIdx, int endIdx, TNode *pNodeArr )
{
    assert( pNodeArr );
    if( pNodeArr )      // sanity
    {
        for( int i = startIdx; i < endIdx; i++ )
        {
            printf( "%.016lx : %.016lx\t@:%d\tl:%d\tc:%d\tr:%d\tp:%d\n",
                ( long ) &pNodeArr[i],
                ( long ) pNodeArr[i].getParent(),
                pNodeArr[i].getKey(),
                pNodeArr[i].getLeft() ? pNodeArr[i].getLeft()->getKey() : -1,
                pNodeArr[i].getCenter() ? pNodeArr[i].getCenter()->getKey() : -1,
                pNodeArr[i].getRight() ? pNodeArr[i].getRight()->getKey() : -1,
                pNodeArr[i].getParent() ? pNodeArr[i].getParent()->getKey() : -1 );
        }
    }
}

// main
// This is the main entry point and testbed for ternary tree.
//
// @In:     -
// @Out:    0 == success
int main()
{
    const int MAX_NODES = 16;
    TernaryTree t;
    TNode *root;
    static int keys[MAX_NODES] =
    {
        5,  4,  9,  5,  7,  2,  2,  6,
        3, 12,  6,  8,  1,  5,  8,  7
    };
    TNode node[ MAX_NODES ]; // allocate nodes on stack for test purposes.

    // Set up root node
    root = &node[ 0 ];
    root->setKey( 5 );
    t.recurse( 5, root, NULL );

    // Add half the nodes
    for( int i = 1; i < (MAX_NODES >> 1); i++ )
    {
        printf( "ABOUT TO ADD %d (%d)\n", keys[i], i );
        t.insertNode( keys[ i ], node + i, root );
    }

    // Print state of tree
    printf("\n");
    printNodeArr( 0, MAX_NODES, node );
    printTraversal( root, LEG_C, 0, 0 );

    // Add other half of the nodes
    for( int i = ( MAX_NODES >> 1 ); i < MAX_NODES; i++ )
    {
        printf( "ABOUT TO ADD %d (%d)\n", keys[i], i );
        t.insertNode( keys[ i ], node + i, root );
    }

    // Print state of tree
    printNodeArr( 0, MAX_NODES, node );
    printTraversal( root, LEG_C, 0, 0 );

    printf( "ABOUT TO DELETE %d (%d)\n", keys[2], 2 );
    // Try deleting a few nodes
    t.deleteNode(&node[2]);
    printTraversal( root, LEG_C, 0, 0 );

    // Print state of tree
    printNodeArr( 0, MAX_NODES, node );
    printTraversal( root, LEG_C, 0, 0 );

    printf( "ABOUT TO DELETE %d (%d)\n", keys[4], 4 );
    // Try deleting a few nodes
    t.deleteNode(&node[4]);

    // Print state of tree
    printNodeArr( 0, MAX_NODES, node );
    printTraversal( root, LEG_C, 0, 0 );

    return 0;
}

