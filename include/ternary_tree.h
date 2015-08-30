#pragma once

#include <map>
#include <string>
#include <queue>
#include <deque>
#include <stack>
#include "templ_node.h"

using namespace std;

typedef unsigned char UCHAR;

//#define DEBUG
#define INFO

// This is the instantiable class from my TemplNode template
class TNode : public TemplNode <UCHAR, TNode>
{
    public:
        TNode() {};
        TNode(UCHAR key) : TemplNode <UCHAR, TNode> (key) { };
        ~TNode() {};
        void setKey( UCHAR key )
        {
            if( isupper( key ) )
            {
                setUpper();
            }
            _key = ( UCHAR ) tolower( key );
        }
};

// This class is the tree itself. It manages a ternary search tree of TNodes
// This is a dictionary and allows us quick word lookup
// with a yes/no response.
class TernaryTree
{
    public:
        TernaryTree() {};
        ~TernaryTree() {};

        // insertNode
        // insert a node into the tree
        //
        // @In: pWord pointer to null-terminated string
        // ppParent pointer to parent pointer
        // @Out: Node *
        TNode * insert( const char *pWord, TNode **ppNode = NULL );
       // find
        // find a word
        //
        // @In:     @pWord pointer to null-terminated string
        //          @pParent pointer to current parent node
        //          @ppTerminal pointer to terminal node pointer
        // @Out:    true == match found
        bool find( const char *pWord, TNode *pParent, TNode ** ppTerminal = NULL );

        // Perform an inexact, "fuzzy" lookup of a word
        //
        // @In:     @pWord pointer to null-terminated string
        //          @pParent pointer to current parent node
        // @Out:    true == match found
        //          @map key/value pair map with tiebroken score and word
        void fuzzyFind(
                const char *pWord,
                TNode *pParent,
                map< int, string > *pWords );
       // extrapolateAll
        // extrapolate all possibilities from an input strung.
        //
        // @In: pNode pointer to starting node
        // pVect vector of words
        // @Out: -
        // pVect filled with words from starting node
        bool extrapolateAll(
                TNode *pNode,
                map< int, string > *pWords,
                deque< UCHAR > *accum,
                const char *pStem,
                const char *pWord );

        // extrapolate
        // extrapolate from a word stem.
        //
        // I selected a deque as my accumulator data structure. A stack
        // requires a second stack or some other means of word reversal,
        // while a deque gives me both FIFO representation and FILO
        // functionality.
        //
        // @In:     pNode pointer to starting node
        //          pVect vector of words
        // @Out:    -
        //          pVect filled with words from starting node
        bool extrapolate(
                TNode *pRoot,
                TNode *pNode,
                map< int, string > *pWords,
                deque< UCHAR > *accum,
                const char *pStem,
                const char *pWord,
                int depth = 0 );

    protected:
        // allocNode
        // insert a node into the tree
        //
        // @In: key key of node to add
        // @Out: Pointer to node
        TNode *allocNode( char key );

        // Utility preprocessor macro for Levenshtein

        // getLevenshtein
        // This is an optimized Levenshtein string distance calculation.
        // It returns how "different" two strings are, effectively performing
        // a commutative subtraction operation.
        //
        // @In: s1 string #1
        // s2 string #2
        // @Out: Levenshtein difference
        int getLevenshtein( const char *s1, const char *s2 );
};
